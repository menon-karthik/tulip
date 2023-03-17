#include "svZeroD_distalResistance.h"

using namespace std;

svZeroD_distalResistance::svZeroD_distalResistance(std::string input_target_file, std::string input_perfusion_volumes_file) {
  this-> target_file = input_target_file;
  this->perfusion_volumes_file = input_perfusion_volumes_file;
  if (perfusion_volumes_file != "None") {
    std::cout<<"Using perfusion data"<<std::endl;
    this->use_perfusion = true;
    this->readPerfusionFile(perfusion_volumes_file);
  }
}

void svZeroD_distalResistance::setupModel(LPNSolverInterface& interface){
  
//// Load shared library and get interface functions.
//auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
//this->interface.load_library(interface_lib);
//this->interface.initialize(model_path);
  this->nUnknowns = this->interface.system_size_;
  
//// Save initial state
//this->init_state_y.resize(this->nUnknowns);
//this->init_state_ydot.resize(this->nUnknowns);
//this->interface.return_y(init_state_y);
//this->interface.return_ydot(init_state_ydot);

  // String inputs should be <path_to_flow_targets> and <path_to_perfusion_file> (optional)
//if ((string_inputs.size() < 1) || (string_inputs.size() > 2)) {
//  throw std::runtime_error("ERROR: String inputs should be <path_to_flow_targets> and <path_to_perfusion_file> (optional)")
//} else {
//  std::string target_file = string_inputs[0];
//  if (string_inputs.size() == 2) {
//    std::string perfusion_volumes_file = string_inputs[1];
//    std::cout<<"Using perfusion data"<<std::endl;
//    this->use_perfusion = true;
//    this->readPerfusionFile(perfusion_volumes_file);
//  }
//}

//if (perfusion_volumes_file != "None") {
//  std::cout<<"Using perfusion data"<<std::endl;
//  this->use_perfusion = true;
//  this->readPerfusionFile(perfusion_volumes_file);
//}

  // Number of blocks and number of each type
  this->num_blocks = this->interface.block_names_.size();
  this->n_corBC_l = 0;
  this->n_corBC_r = 0;
  std::string block_name, branch_name;
  for (int i = 0; i < this->num_blocks; i++) {
    block_name = this->interface.block_names_[i];
    if (block_name.substr(0,6) == "BC_lca") {
      if (!this->use_perfusion) {
        this->n_corBC_l++;
        this->names_corBC_l.push_back(block_name);
      } else {
        branch_name = block_name.substr(3,block_name.size()-3);
        if (this->perfusion_data.at(branch_name) > 0.0) {
          this->n_corBC_l++;
          this->names_corBC_l.push_back(block_name);
          std::cout<<"Perfusion data available for "<<block_name<<std::endl;
        } // perfusion_data[branch_name] > 0.0
      } // use_perfusion
    } else if (block_name.substr(0,6) == "BC_rca") {
      if (!this->use_perfusion) {
        this->n_corBC_r++;
        this->names_corBC_r.push_back(block_name);
      } else {
        branch_name = block_name.substr(3,block_name.size()-3);
        if (this->perfusion_data.at(branch_name) > 0.0) {
          this->n_corBC_r++;
          this->names_corBC_r.push_back(block_name);
          std::cout<<"Perfusion data available for "<<block_name<<std::endl;
        } // if perfusion_data[branch_name] > 0.0
      } // if use_perfusion
    } // if block_name.substr(0,6) == "BC_*" 
  } //  i = 0; i < this->num_blocks
       
  std::cout<<"Number of left and right coronaries: "<<this->n_corBC_l<<" , "<<this->n_corBC_r<<std::endl; 
 
  // Write a file with the order of blocks to specify order of parameters
  FILE* fp;
  fp = fopen("distalR_tuning_branches.txt","w");
  std::ofstream branch_file("distalR_tuning_branches.txt");
  for (int i = 0; i < this->n_corBC_l; i++) {
    block_name = this->names_corBC_l[i];
    branch_file << block_name <<"\n";
    //fprintf(fp,"%15s\n",block_name);
  }
  for (int i = 0; i < this->n_corBC_r; i++) {
    block_name = this->names_corBC_r[i];
    branch_file << block_name <<"\n";
    //fprintf(fp,"%15s\n",block_name);
  }
  fclose(fp);

  this->coronary_params.resize(6);
//this->rcr_params.resize(3);
//this->heart_params.resize(27);
//this->vessel_params.resize(4);
  this->R_total_inv_base = 0.0;
  // Initialize parameter vectors and read baseline block params
  // Baseline parameters (named *_base) are set in the svZeroD config file
  //
  // Read left coronary parameters
  this->Ra_l_base.reserve(this->n_corBC_l);
  this->Ram_l_base.reserve(this->n_corBC_l);
  this->Rv_l_base.reserve(this->n_corBC_l);
  this->Ca_l_base.reserve(this->n_corBC_l);
  this->Cim_l_base.reserve(this->n_corBC_l);
  this->iml_base.reserve(this->n_corBC_l);
  for (int i = 0; i < this->n_corBC_l; i++) {
    block_name = this->names_corBC_l[i];
    this->interface.read_block_params(block_name, this->coronary_params);
    this->Ra_l_base[i] = this->coronary_params[0];
    this->Ram_l_base[i] = this->coronary_params[1];
    this->Rv_l_base[i] = this->coronary_params[2];
    this->Ca_l_base[i] = this->coronary_params[3];
    this->Cim_l_base[i] = this->coronary_params[4];
    this->iml_base[i] = this->coronary_params[5];
    this->R_total_inv_base += 1.0/(this->Ra_l_base[i]+this->Ram_l_base[i]+this->Rv_l_base[i]);
  }
  
  // Read right coronary parameters
  this->Ra_r_base.reserve(this->n_corBC_r);
  this->Ram_r_base.reserve(this->n_corBC_r);
  this->Rv_r_base.reserve(this->n_corBC_r);
  this->Ca_r_base.reserve(this->n_corBC_r);
  this->Cim_r_base.reserve(this->n_corBC_r);
  this->imr_base.reserve(this->n_corBC_r);
  for (int i = 0; i < this->n_corBC_r; i++) {
    block_name = this->names_corBC_r[i];
    this->interface.read_block_params(block_name, this->coronary_params);
    this->Ra_r_base[i] = this->coronary_params[0];
    this->Ram_r_base[i] = this->coronary_params[1];
    this->Rv_r_base[i] = this->coronary_params[2];
    this->Ca_r_base[i] = this->coronary_params[3];
    this->Cim_r_base[i] = this->coronary_params[4];
    this->imr_base[i] = this->coronary_params[5];
    this->R_total_inv_base += 1.0/(this->Ra_r_base[i]+this->Ram_r_base[i]+this->Rv_r_base[i]);
  }
  std::cout<<"Total baseline coronary resistance = "<<(1.0/this->R_total_inv_base)<<std::endl;
  
  // Read RCR parameters
//this->Rp_rcr_base.reserve(this->nRCR);
//this->Rd_rcr_base.reserve(this->nRCR);
//this->C_rcr_base.reserve(this->nRCR);
//for (int i = 0; i < this->nRCR; i++) {
//  block_name = "BC_RCR" + to_string(i+1);
//  this->interface.read_block_params(block_name, this->rcr_params);
//  this->Rp_rcr_base[i] = this->rcr_params[0];
//  this->C_rcr_base[i] = this->rcr_params[1];
//  this->Rd_rcr_base[i] = this->rcr_params[2];
//}
  
//// Read closed-loop heart parameters
//this->interface.read_block_params("CLH", this->heart_params);
//this->Rrv_base = this->heart_params[9];
//this->Rlv_base = this->heart_params[13];
//this->Rpd_base = this->heart_params[16];
  
  // Read aorta parameters
//this->interface.read_block_params("aorta", this->vessel_params);

//if (this->vessel_params[1] == 0) { // If C = 0
//  this->useRigidSurrogate = true;
//}
  
  // Save solution IDs corresponding to important quantities
  this->Q_lca_ids.resize(this->n_corBC_l, -1);
  this->Q_rca_ids.resize(this->n_corBC_r, -1);
  this->Q_lca_ids_names.resize(this->n_corBC_l);
  this->Q_rca_ids_names.resize(this->n_corBC_r);
//this->Q_rcr_ids.resize(this->nRCR, -1);
  int ct_lca = 0, ct_rca = 0;
  //int ct_rcr = 0;
  string var_name;
  bool lca_flag, rca_flag, flow_flag, perfusion_flag;
  //bool rcr_flag;
  
  // Iterate through the names of variables in the 0D system
  for (int i = 0; i < this->interface.system_size_; i++) {
    var_name = this->interface.variable_names_[i];
    flow_flag = (var_name.substr(0,4) == "flow"); // Is this a flow variable?

    // Find last occurence of ":" in variable name. 
    // The rest of var_name is either the exit block name or the main block name (if it is an internal variable).
    std::size_t blk_name_start = var_name.rfind(":");
    if (blk_name_start == std::string::npos) {
      std::cout<<"Error: Invalid variable name format for:"<<var_name<<std::endl;
      throw std::runtime_error("Error: Invalid variable name format.");
    }
    // Search for specific patterns in variable name
    lca_flag = !var_name.compare(blk_name_start+1,6,"BC_lca"); //string.compare() returns 0 for exact matches
    rca_flag = !var_name.compare(blk_name_start+1,6,"BC_rca");
//  rcr_flag = !var_name.compare(blk_name_start+1,6,"BC_RCR");
//
    if (this->use_perfusion && (lca_flag || rca_flag)) {
      branch_name = var_name.substr(blk_name_start+4,var_name.length()-1);
      if (this->perfusion_data.at(branch_name) > 0.0) {
        perfusion_flag = true;
      } else {
        perfusion_flag = false;
      } // perfusion_data[branch_name] > 0.0
    } else {
      perfusion_flag = true; // include all branches
    } // use_perfusion

    if (flow_flag && lca_flag && perfusion_flag) {
      this->Q_lca_ids[ct_lca] = i;
      this->Q_lca_ids_names[ct_lca] = var_name.substr(blk_name_start+4,var_name.length()-1);
      std::cout<<"Found Q_lca_ids for "<<Q_lca_ids_names[ct_lca]<<std::endl;
      ct_lca++;
    } else if (flow_flag && rca_flag && perfusion_flag) {
      this->Q_rca_ids[ct_rca] = i;
      this->Q_rca_ids_names[ct_rca] = var_name.substr(blk_name_start+4,var_name.length()-1);
      std::cout<<"Found Q_rca_ids for "<<Q_rca_ids_names[ct_rca]<<std::endl;
      ct_rca++;
    }
  }
  //std::cout << "[cmLPN_svZeroD] Q_lca_ids.size() this->n_corBC_l: " << Q_lca_ids.size() << ", "<< this->n_corBC_l << std::endl;
  
  // Check to make sure all variables ids have been assigned
  for (int i = 0; i < this->n_corBC_l; i++) {
    if (this->Q_lca_ids[i] < 0) {
      std::cout << "Q_lca index: "<< i << std::endl;
      std::cout << "Error: Did not find all solution IDs for variables named BC_lca" << std::endl;
      throw std::runtime_error("Error: Did not find all solution IDs for variables named BC_lca");
    }
  }
  for (int i = 0; i < this->n_corBC_r; i++) {
    if (this->Q_rca_ids[i] < 0) {
      std::cout << "Q_rca index: "<< i << std::endl;
      std::cout << "Error: Did not find all solution IDs for variables named BC_rca" << std::endl;
      throw std::runtime_error("Error: Did not find all solution IDs for variables named BC_rca");
    } 
  }
//for (int i = 0; i < this->nRCR; i++) {
//  if (this->Q_rcr_ids[i] < 0) {
//    throw std::runtime_error("Error: Did not find all solution IDs for variables named BC_RCR");
//  }
//}
//if (Q_aorta_id < 0) {
//  std::cout << "[svZeroD_distalResistance] Variable IDs: " << Q_aorta_id << std::endl;
//  throw std::runtime_error("Error: Did not find all solution IDs for variables.");
//}
//if ((Q_aorta_id < 0) || (P_aorta_id < 0) || (Q_LV_id < 0) || (V_LV_id < 0) || (Q_LA_id < 0) || (Q_RV_id < 0) || (P_pul_id < 0) || (P_RV_id < 0) || (P_RA_id < 0)) {
//  std::cout << "[cmLPN_svZeroD] Variable IDs: " << Q_aorta_id << ", " << P_aorta_id << ", " << Q_LV_id << ", " << V_LV_id << ", " << Q_LA_id << ", " << Q_RV_id << ", " << P_pul_id << ", " << P_RV_id << ", " << P_RA_id << std::endl;
//  throw std::runtime_error("Error: Did not find all solution IDs for variables.");
//}


  // Read targets and discard targets without perfusion data if required
  this->readTargetsFromFile(target_file);
  if (this->use_perfusion) {
    std::vector<double> all_targets = this->target_flows;
    std::vector<std::string> all_outlet_names = this->outlet_names;
    this->target_flows.clear();
    this->outlet_names.clear();
    this->total_target_flow = 0.0;
    int ct_targets = 0;
    std::cout<<"Target flows:"<<std::endl;
    for (int i = 0; i < all_outlet_names.size(); i++) {
      if (this->perfusion_data.at(all_outlet_names[i]) > 0.0) {
        this->target_flows.push_back(all_targets[i]);
        this->outlet_names.push_back(all_outlet_names[i]);
        this->total_target_flow += all_targets[i];
        std::cout<<ct_targets<<" "<<all_outlet_names[i]<<" "<<all_targets[i]<<std::endl;
        ct_targets++;
      }
    }
  } // use_perfusion
  
  if (this->target_flows.size() != (this->Q_lca_ids.size()+this->Q_rca_ids.size())) {
    std::cout << "Error: Number of target flows does not match number of saved flow state IDs" << std::endl;
    throw std::runtime_error("Error: Number of target flows does not match number of saved flow state IDs");
  }
  // Rearrange targets to match the order of names in Q_lca/rca_ids
  //Add target_flows and outlet_names and Q_lca/rca_ids_names and R_total_inv_base to header file
  int idx;
  std::vector<double> targets_copy = this->target_flows;
  for (int i = 0; i < this->Q_lca_ids.size(); i++) {
    std::cout<<"Branch name: "<<this->Q_lca_ids_names[i]<<std::endl;
    auto itr = find(this->outlet_names.begin(), this->outlet_names.end(), this->Q_lca_ids_names[i]);
    if(itr != this->outlet_names.end()) {
      idx = std::distance(this->outlet_names.begin(), itr);
      std::cout<<"Found in targets at idx: "<<idx<<std::endl;
      this->target_flows[i] = targets_copy[idx];
    } else {
      throw std::runtime_error("Error: Could not find "+this->Q_lca_ids_names[i]+" in outlet_names.");
    }
  }
  for (int i = 0; i < this->Q_rca_ids.size(); i++) {
    std::cout<<"Branch name: "<<this->Q_rca_ids_names[i]<<std::endl;
    auto itr = find(this->outlet_names.begin(), this->outlet_names.end(), this->Q_rca_ids_names[i]);
    if(itr != this->outlet_names.end()) {
      idx = std::distance(this->outlet_names.begin(), itr);
      std::cout<<"Found in targets at idx: "<<idx<<std::endl;
      this->target_flows[this->Q_lca_ids.size()+i] = targets_copy[idx];
    } else {
      throw std::runtime_error("Error: Could not find "+this->Q_rca_ids_names[i]+" in outlet_names.");
    }
  }

  // Save target flow fractions
  std::cout<<"Total target flow = "<<this->total_target_flow<<std::endl;
  for (int i = 0; i < this->target_flows.size(); i++) {
    this->target_flow_fracs.push_back(this->target_flows[i]/this->total_target_flow);
  }
}

// ==========================
// READ TARGET DATA FROM FILE
// ==========================
void svZeroD_distalResistance::readTargetsFromFile(string targetFileName)
{
  std::ifstream read_file;
  read_file.open(targetFileName.c_str());
  
  std::string buffer;
  std::vector<std::string> tokens;
  if(!read_file.is_open())
    throw cmException("ERROR: Cannot open targets file");
  if(read_file.eof())
    throw cmException("ERROR: No targets found!");
  
  //std::vector<double> targets;
  //std::vector<std::string> outlet_names;
  this->total_target_flow = 0.0;
  while(std::getline(read_file,buffer))
  {
    schSplit(buffer,tokens," ");
    this->target_flows.push_back(atof(tokens[1].c_str()));
    this->outlet_names.push_back(tokens[0].c_str());
    this->total_target_flow += atof(tokens[1].c_str());
  }
  
  read_file.close();
}

// ==========================
// READ PERFUSION DATA FROM FILE
// ==========================
void svZeroD_distalResistance::readPerfusionFile(string perfusionFileName)
{
  std::ifstream read_file;
  read_file.open(perfusionFileName.c_str());
  
  std::string buffer;
  std::vector<std::string> tokens;
  if(!read_file.is_open())
    throw cmException("ERROR: Cannot open perfusion file");
  if(read_file.eof())
    throw cmException("ERROR: No perfusion data found!");
  
  int line_ct = 0;
  while(std::getline(read_file,buffer))
  {
    if (line_ct > 0) { //skip header
      schSplit(buffer,tokens," ");
      //this->target_flows.push_back(atof(tokens[1].c_str()));
      //this->outlet_names.push_back(tokens[0].c_str());
      this->perfusion_data.insert({tokens[0].c_str(),atof(tokens[1].c_str())});
    }
    line_ct++;
  }
  
  read_file.close();
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int svZeroD_distalResistance::getParameterTotal(){
  return (this->n_corBC_l+this->n_corBC_r);
}

// ===================================
// GET NUMBER OF PARAMETERS (UNKNOWNS)
// ===================================
int svZeroD_distalResistance::getStateTotal(){
  return this->nUnknowns; 
}

//int cmLPN_svZeroD::getAuxStateTotal(){
//  return 50;
//}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int svZeroD_distalResistance::getResultTotal(){
  return (this->n_corBC_l+this->n_corBC_r);  
}

// ==================
// GET PARAMETER NAME
// ==================
string svZeroD_distalResistance::getParamName(int index) {
  if (index < this->n_corBC_l) {
    return this->Q_lca_ids_names[index];
  } else if ((index >= this->n_corBC_l) && (index < this->n_corBC_r)) {
    return this->Q_rca_ids_names[index-this->n_corBC_l];
  } else {
    throw std::runtime_error("ERROR: Invalid index in svZeroD_distalResistance::getParamName(index)")
  }
}

// ==================
// GET PARAMETER NAME
// ==================
string svZeroD_distalResistance::getResultName(int index) {
  if (index < this->n_corBC_l) {
    return this->Q_lca_ids_names[index];
  } else if ((index >= this->n_corBC_l) && (index < this->n_corBC_r)) {
    return this->Q_rca_ids_names[index-this->n_corBC_l];
  } else {
    throw std::runtime_error("ERROR: Invalid index in svZeroD_distalResistance::getResultName(index)")
  }
}


//void svZeroD_distalResistance::getRScaling(){
double svZeroD_distalResistance::getRScaling(){
  return this->R_scaling;  
  //std::cout<<"hi"<<std::endl;
}

//// ====================
//// GET MODEL PARAMETERS
//// ====================
void svZeroD_distalResistance::getDefaultParams(stdVec& default_params){
      
  default_params.resize(getParameterTotal());

  for (int i = 0; i < getParameterTotal(); i++) {
    default_params[i] = 1.0;
  }

}

// ====================
// GET PARAMETER RANGES
// ====================
void svZeroD_distalResistance::getParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());
  for (int i = 0; i < getParameterTotal(); i++) {
    limits[2*i] = 0.5;
    limits[2*i+1] = 2.0;
  }

//int currParam = 0;
//for(size_t loopA=0;loopA<frozenParamIDX.size();loopA++){
//	currParam = frozenParamIDX[loopA];
//  // Assign the new lower and upper bounds to the center
//  limits[currParam*2]     = frozenParamVAL[loopA];
//  limits[currParam*2 + 1] = frozenParamVAL[loopA];
//}
}

void svZeroD_distalResistance::printResults(int totalResults, double* Xn) {
  printf("RESULT PRINTOUT\n");
  for(int loopA = 0; loopA < totalResults; loopA++) {
    string s = getResultName(loopA);
    printf("%20s : %f\n",s.c_str(),Xn[loopA]);
  }
}

//// ==================
//// SAVE ALL DATA FILE
//// ==================
//void writeAllDataFile(int totalSteps,int totalStates,int totAuxStates,double** outVals,double** auxOutVals){
//  FILE* outFile;
//  outFile = fopen("AllData","w");
//  for(int loopA=0;loopA<totalSteps;loopA++){
//    // Print State Variables
//    for(int loopB=0;loopB<totalStates;loopB++){
//      fprintf(outFile,"%e ",outVals[loopB][loopA]);
//    }
//    // Print Aux Variables
//    for(int loopB=0;loopB<totAuxStates;loopB++){
//      fprintf(outFile,"%e ",auxOutVals[loopB][loopA]);
//    }
//    fprintf(outFile,"\n");
//  }
//  // CLOSE THE FILE
//  fclose(outFile);
//}

// ==========================================
// UPDATE PARAMETERS OF THE ZEROD MODEL
// ==========================================
void svZeroD_distalResistance::setModelParams(LPNSolverInterface& interface, const stdVec& params) {

  // Scale each resistance so the total coronary resistance remains the same
  double R_total_inv = 0.0;
  double outlet_R;
  for (int i = 0; i < this->n_corBC_l; i++) {
    outlet_R = (this->Ra_l_base[i]+this->Ram_l_base[i]+this->Rv_l_base[i])*params[i];
    R_total_inv += 1.0/outlet_R;
  }
  for (int i = 0; i < this->n_corBC_r; i++) {
    outlet_R = (this->Ra_r_base[i]+this->Ram_r_base[i]+this->Rv_r_base[i])*params[n_corBC_l+i];
    R_total_inv += 1.0/outlet_R;
  }
  this->R_scaling = R_total_inv/this->R_total_inv_base;
  //std::cout<<"[solveCoronaryLPN] R scaling: "<<R_scaling<<std::endl;

  std::string block_name;
  
  R_total_inv = 0.0;
  // Update the model parameters 
  for (int i = 0; i < this->n_corBC_l; i++) {
    //std::cout<<"params["<<i<<"]: "<<params[i]<<std::endl;
    block_name = this->names_corBC_l[i];
    this->coronary_params[0] = this->Ra_l_base[i]*params[i]*this->R_scaling; //Ra
    this->coronary_params[1] = this->Ram_l_base[i]*params[i]*this->R_scaling; //Ram
    this->coronary_params[2] = this->Rv_l_base[i]*params[i]*this->R_scaling; //Rv
    this->coronary_params[3] = this->Ca_l_base[i]; //Ca
    this->coronary_params[4] = this->Cim_l_base[i]; //Cim
    this->coronary_params[5] = this->iml_base[i]; //iml
    this->interface.update_block_params(block_name, this->coronary_params);
    R_total_inv += 1.0/(this->coronary_params[0] + this->coronary_params[1] + this->coronary_params[2]);
  }
  
  for (int i = 0; i < this->n_corBC_r; i++) {
    //std::cout<<"params["<<n_corBC_l+i<<"]: "<<params[n_corBC_l+i]<<std::endl;
    block_name = this->names_corBC_r[i];
    this->coronary_params[0] = this->Ra_r_base[i]*params[n_corBC_l+i]*this->R_scaling; //Ra
    this->coronary_params[1] = this->Ram_r_base[i]*params[n_corBC_l+i]*this->R_scaling; //Ram
    this->coronary_params[2] = this->Rv_r_base[i]*params[n_corBC_l+i]*this->R_scaling; //Rv
    this->coronary_params[3] = this->Ca_r_base[i]; //Ca
    this->coronary_params[4] = this->Cim_r_base[i]; //Cim
    this->coronary_params[5] = this->imr_base[i]; //imr
    this->interface.update_block_params(block_name, this->coronary_params);
    R_total_inv += 1.0/(this->coronary_params[0] + this->coronary_params[1] + this->coronary_params[2]);
  }
  //std::cout<<"[solveCoronaryLPN] Total assigned coronary resistance = "<<(1.0/R_total_inv)<<std::endl;
}

// ==========================================
// POSTPROCESS ZEROD SIMULATION
// ==========================================
void svZeroD_distalResistance::postProcess(LPNSolverInterface& interface, const stdVec& t, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) {
  double mean_branch_flow;
  double total_flow = 0.0;
  for(int i = 0; i < n_corBC_l; i++){    
    mean_branch_flow = 0.0;
    for (int t = totOutputSteps - totalStepsOnSingleCycle - 1; t < totOutputSteps; t++) {
      mean_branch_flow += outVals[this->Q_lca_ids[i]][t];
    }
    mean_branch_flow = mean_branch_flow/(double)(totalStepsOnSingleCycle - 1);
    results.push_back(mean_branch_flow);
    total_flow += mean_branch_flow;
  }
  for(int i = 0; i < n_corBC_r; i++){    
    mean_branch_flow = 0.0;
    for (int t = totOutputSteps - totalStepsOnSingleCycle - 1; t < totOutputSteps; t++) {
      mean_branch_flow += outVals[this->Q_rca_ids[i]][t];
    }
    mean_branch_flow = mean_branch_flow/(double)(totalStepsOnSingleCycle - 1);
    results.push_back(mean_branch_flow);
    total_flow += mean_branch_flow;
  }
  //std::cout<<"Total coronary flow = "<<total_flow<<std::endl;
  
  for(int i = 0; i < results.size(); i++) {
    results[i] = results[i]/total_flow;
  }
}

  
////int svZeroD_distalResistance::solveCoronaryLPN(double* params, double* results){
//int svZeroD_distalResistance::solveCoronaryLPN(std::vector<double> params, std::vector<double>& results){

//  //std::cout << "[solveCoronaryLPN] START " << std::endl;
//  // Time parameters
//  //double totalTime = numCycles * cycleTime;
//  int totalStepsOnSingleCycle = this->interface.pts_per_cycle_;
//  int numCycles = this->interface.num_cycles_;
//  int totOutputSteps = this->interface.num_output_steps_;

//  int totalStates = getStateTotal();
//  int totalResults = getResultTotal();

//  // State Variables
////double** outVals = NULL;
////outVals = new double*[totalStates];
////for(int loopA=0;loopA<totalStates;loopA++){
////  outVals[loopA] = new double[totOutputSteps];
////}

//  //std::cout << "[solveCoronaryLPN] 1 " << std::endl;
//  std::vector<std::vector<double>> outVals(totalStates, std::vector<double> (totOutputSteps, 0));

//  // Scale each resistance so the total coronary resistance remains the same
//  double R_total_inv = 0.0;
//  double outlet_R;
//  for (int i = 0; i < this->n_corBC_l; i++) {
//    outlet_R = (this->Ra_l_base[i]+this->Ram_l_base[i]+this->Rv_l_base[i])*params[i];
//    R_total_inv += 1.0/outlet_R;
//  }
//  for (int i = 0; i < this->n_corBC_r; i++) {
//    outlet_R = (this->Ra_r_base[i]+this->Ram_r_base[i]+this->Rv_r_base[i])*params[n_corBC_l+i];
//    R_total_inv += 1.0/outlet_R;
//  }
//  this->R_scaling = R_total_inv/this->R_total_inv_base;
//  //std::cout<<"[solveCoronaryLPN] R scaling: "<<R_scaling<<std::endl;

//  std::string block_name;
//  
//  R_total_inv = 0.0;
//  // Update the model parameters 
//  for (int i = 0; i < this->n_corBC_l; i++) {
//    //std::cout<<"params["<<i<<"]: "<<params[i]<<std::endl;
//    block_name = this->names_corBC_l[i];
//    this->coronary_params[0] = this->Ra_l_base[i]*params[i]*this->R_scaling; //Ra
//    this->coronary_params[1] = this->Ram_l_base[i]*params[i]*this->R_scaling; //Ram
//    this->coronary_params[2] = this->Rv_l_base[i]*params[i]*this->R_scaling; //Rv
//    this->coronary_params[3] = this->Ca_l_base[i]; //Ca
//    this->coronary_params[4] = this->Cim_l_base[i]; //Cim
//    this->coronary_params[5] = this->iml_base[i]; //iml
//    this->interface.update_block_params(block_name, this->coronary_params);
//    R_total_inv += 1.0/(this->coronary_params[0] + this->coronary_params[1] + this->coronary_params[2]);
//  }
//  
//  for (int i = 0; i < this->n_corBC_r; i++) {
//    //std::cout<<"params["<<n_corBC_l+i<<"]: "<<params[n_corBC_l+i]<<std::endl;
//    block_name = this->names_corBC_r[i];
//    this->coronary_params[0] = this->Ra_r_base[i]*params[n_corBC_l+i]*this->R_scaling; //Ra
//    this->coronary_params[1] = this->Ram_r_base[i]*params[n_corBC_l+i]*this->R_scaling; //Ram
//    this->coronary_params[2] = this->Rv_r_base[i]*params[n_corBC_l+i]*this->R_scaling; //Rv
//    this->coronary_params[3] = this->Ca_r_base[i]; //Ca
//    this->coronary_params[4] = this->Cim_r_base[i]; //Cim
//    this->coronary_params[5] = this->imr_base[i]; //imr
//    this->interface.update_block_params(block_name, this->coronary_params);
//    R_total_inv += 1.0/(this->coronary_params[0] + this->coronary_params[1] + this->coronary_params[2]);
//  }
//  //std::cout<<"[solveCoronaryLPN] Total assigned coronary resistance = "<<(1.0/R_total_inv)<<std::endl;
//  
//  // Set up solution and time vectors, and run simulation
//  std::vector<double> solutions(interface.system_size_*interface.num_output_steps_);
//  std::vector<double> times(interface.num_output_steps_);
//  int error_code = 0;
//  interface.update_state(init_state_y, init_state_ydot);
//  this->interface.run_simulation(0.0, times, solutions, error_code);
//  //std::cout << "[solveCoronaryLPN] error_code: " << error_code << std::endl;

//  // Parse the solution vector
//  int state, step;
//  double t[interface.num_output_steps_];
//  for (step = 0; step < totOutputSteps; step++) {
//    t[step] = times[step];
//    //std::cout<<"times: "<<times[step]<<std::endl;
//  }
//  int sol_idx;
//  for (state = 0; state < totalStates; state++) {
//    for (step = 0; step < totOutputSteps; step++) {
//      sol_idx = step*totalStates + state;
//      outVals[state][step] = solutions[sol_idx];
//    }
//  }
//  //std::cout << "[solveCoronaryLPN] after parsing solution" << std::endl;

//  double mean_branch_flow;
//  double total_flow = 0.0;
//  for(int i = 0; i < n_corBC_l; i++){    
//    mean_branch_flow = 0.0;
//    for (int t = totOutputSteps - totalStepsOnSingleCycle - 1; t < totOutputSteps; t++) {
//      mean_branch_flow += outVals[this->Q_lca_ids[i]][t];
//    }
//    mean_branch_flow = mean_branch_flow/(double)(totalStepsOnSingleCycle - 1);
//    results.push_back(mean_branch_flow);
//    total_flow += mean_branch_flow;
//  }
//  for(int i = 0; i < n_corBC_r; i++){    
//    mean_branch_flow = 0.0;
//    for (int t = totOutputSteps - totalStepsOnSingleCycle - 1; t < totOutputSteps; t++) {
//      mean_branch_flow += outVals[this->Q_rca_ids[i]][t];
//    }
//    mean_branch_flow = mean_branch_flow/(double)(totalStepsOnSingleCycle - 1);
//    results.push_back(mean_branch_flow);
//    total_flow += mean_branch_flow;
//  }
//  //std::cout<<"Total coronary flow = "<<total_flow<<std::endl;
//  
//  for(int i = 0; i < results.size(); i++) {
//    results[i] = results[i]/total_flow;
//  }

////if(error_code != 0) {
////  for(int loopA=0;loopA<totalStates;loopA++){
////    delete [] outVals[loopA];
////  }
////  delete [] outVals;
////  return 1;
////}

//  // FREE MEMORY
////  for(int loopA=0;loopA<totalStates;loopA++){
////    delete [] outVals[loopA];
////  }
////delete [] outVals;

//  //std::cout << "[solveCoronaryLPN] END " << std::endl;
//  return error_code;
//}

// =========================
// EVAL MODEL ERROR FUNCTION
// =========================
//double svZeroD_distalResistance::evalModelError(std::vector<double> paramsVals, std::vector<double>& outputs, std::vector<int>& errorCode) {
double svZeroD_distalResistance::evalModelError(std::vector<double>& results) {
  //std::cout << "[evalModelError] START " << std::endl;

  int totalParams = getParameterTotal();
  int resultTotal   = getResultTotal();
  
//std::vector<double> results;

//// Solve coronary model
//int error = 0;
//try{
//  error = solveCoronaryLPN(paramsVals,results);
//}catch(...){
//  error = 1;
//}
//errorCode.push_back(error);
 
  //std::cout << "[evalModelError] 2 " << std::endl;
  
  double loss = 0.0;
  double sq_pct_error = 0.0, pct_error = 0.0;
  for(int i=0; i < resultTotal; i++){
    //loss += (results[i] - this->target_flows[i])*(results[i] - this->target_flows[i])/(this->target_flows[i]*this->target_flows[i]);
    //sq_pct_error = (results[i] - this->target_flows[i])*(results[i] - this->target_flows[i])/(this->target_flows[i]*this->target_flows[i]);
    sq_pct_error = (results[i] - this->target_flow_fracs[i])*(results[i] - this->target_flow_fracs[i])/(this->target_flow_fracs[i]*this->target_flow_fracs[i]);
    loss += sq_pct_error;
    pct_error += sqrt(sq_pct_error);
    //std::cout<<"[evalModelError] results, targets: "<<results[i]<<" "<<this->target_flow_fracs[i]<<std::endl;
  }
  loss = loss/double(resultTotal); // mean of square pct error
  pct_error = 100.0*pct_error/double(resultTotal); // mean pct error
  std::cout<<"[evalModelError] Mean pct error: "<<pct_error<<std::endl;


  //std::cout << "[evalModelError] END " << std::endl;
  return loss;
}
