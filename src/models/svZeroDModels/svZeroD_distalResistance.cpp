#include "svZeroD_distalResistance.h"

using namespace std;

svZeroD_distalResistance::svZeroD_distalResistance(std::string target_file, std::string perfusion_volumes_file, std::string covariance_file, bool scaled) {
  // Read target flows
  this->readTargetsFromFile(target_file);

  // Read perfusion_volumes.dat if provided
  if (perfusion_volumes_file != "None") {
    std::cout<<"Using perfusion data"<<std::endl;
    this->use_perfusion = true;
    this->readPerfusionFile(perfusion_volumes_file);
  }

  if (covariance_file != "None") {
    this->useCovariance = true;
    this->readCovarianceFile(covariance_file);
  }
  this->scaled = scaled;
}

// ==========================
// SET UP MODEL-SPECIFIC PARAMETERS FOR OPTIMIZATION
// ==========================
void svZeroD_distalResistance::setupModel(LPNSolverInterface& interface){
  
  // Load shared library and get interface functions.
  this->nUnknowns = interface.system_size_;
  
  // Number of blocks and number of each type
  this->num_blocks = interface.block_names_.size();
  this->n_corBC_l = 0;
  this->n_corBC_r = 0;
  std::string block_name, branch_name;
  for (int i = 0; i < this->num_blocks; i++) {
    block_name = interface.block_names_[i];
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
  }
  fclose(fp);

  this->coronary_params.resize(6);
  //this->coronary_params.resize(5);
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
    interface.read_block_params(block_name, this->coronary_params);
    this->Ra_l_base[i] = this->coronary_params[0];
    this->Ram_l_base[i] = this->coronary_params[1];
    this->Rv_l_base[i] = this->coronary_params[2];
    this->Ca_l_base[i] = this->coronary_params[3];
    this->Cim_l_base[i] = this->coronary_params[4];
    this->iml_base[i] = this->coronary_params[5];
    this->R_total_inv_base += 1.0/(this->Ra_l_base[i]+this->Ram_l_base[i]+this->Rv_l_base[i]);
  }

////TODO:TEST
//double temp;
//temp = this->Ra_l_base[0];
//this->Ra_l_base[0] = this->Ra_l_base[2];
//this->Ra_l_base[2] = temp;
//temp = this->Ram_l_base[0];
//this->Ram_l_base[0] = this->Ram_l_base[2];
//this->Ram_l_base[2] = temp;
//temp = this->Rv_l_base[0];
//this->Rv_l_base[0] = this->Rv_l_base[2];
//this->Rv_l_base[2] = temp;
//temp = this->Ra_l_base[3];
//this->Ra_l_base[3] = this->Ra_l_base[4];
//this->Ra_l_base[4] = temp;
//temp = this->Ram_l_base[3];
//this->Ram_l_base[3] = this->Ram_l_base[4];
//this->Ram_l_base[4] = temp;
//temp = this->Rv_l_base[3];
//this->Rv_l_base[3] = this->Rv_l_base[4];
//this->Rv_l_base[4] = temp;
  
  // Read right coronary parameters
  this->Ra_r_base.reserve(this->n_corBC_r);
  this->Ram_r_base.reserve(this->n_corBC_r);
  this->Rv_r_base.reserve(this->n_corBC_r);
  this->Ca_r_base.reserve(this->n_corBC_r);
  this->Cim_r_base.reserve(this->n_corBC_r);
  this->imr_base.reserve(this->n_corBC_r);
  for (int i = 0; i < this->n_corBC_r; i++) {
    block_name = this->names_corBC_r[i];
    interface.read_block_params(block_name, this->coronary_params);
    this->Ra_r_base[i] = this->coronary_params[0];
    this->Ram_r_base[i] = this->coronary_params[1];
    this->Rv_r_base[i] = this->coronary_params[2];
    this->Ca_r_base[i] = this->coronary_params[3];
    this->Cim_r_base[i] = this->coronary_params[4];
    this->imr_base[i] = this->coronary_params[5];
    this->R_total_inv_base += 1.0/(this->Ra_r_base[i]+this->Ram_r_base[i]+this->Rv_r_base[i]);
  }
  std::cout<<"Total baseline coronary resistance = "<<(1.0/this->R_total_inv_base)<<std::endl;
  
  // Save solution IDs corresponding to important quantities
  this->Q_lca_ids.resize(this->n_corBC_l, -1);
  this->Q_rca_ids.resize(this->n_corBC_r, -1);
  this->Q_lca_ids_names.resize(this->n_corBC_l);
  this->Q_rca_ids_names.resize(this->n_corBC_r);
  int ct_lca = 0, ct_rca = 0;
  string var_name;
  bool lca_flag, rca_flag, flow_flag, perfusion_flag;
  
  // Iterate through the names of variables in the 0D system
  for (int i = 0; i < interface.system_size_; i++) {
    var_name = interface.variable_names_[i];
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

  // Discard targets without perfusion data if required
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
  // Add target_flows and outlet_names and Q_lca/rca_ids_names and R_total_inv_base to header file
  int idx;
  std::vector<double> targets_copy = this->target_flows;
  std::vector<std::vector<double>> cov_inv_copy = this->cov_inv;
  for (int i = 0; i < this->Q_lca_ids.size(); i++) {
    std::cout<<"Branch name: "<<this->Q_lca_ids_names[i]<<std::endl;
    auto itr = find(this->outlet_names.begin(), this->outlet_names.end(), this->Q_lca_ids_names[i]);
    if(itr != this->outlet_names.end()) {
      idx = std::distance(this->outlet_names.begin(), itr);
      std::cout<<"Found in targets at idx: "<<idx<<std::endl;
      this->target_flows[i] = targets_copy[idx];
      std::cout<<"Rearrange: "<<i<<" "<<idx<<std::endl;
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
      std::cout<<"Rearrange: "<<this->Q_lca_ids.size()+i<<" "<<idx<<std::endl;
    } else {
      throw std::runtime_error("Error: Could not find "+this->Q_rca_ids_names[i]+" in outlet_names.");
    }
  }
  
  // Save target flow fractions, result keys, weights and standard deviation
  double perfusion_std = 20.0;
  std::cout<<"Total target flow = "<<this->total_target_flow<<std::endl;
  for (int i = 0; i < this->target_flows.size(); i++) {
    // Flow fractions
    this->target_flow_fracs.push_back(this->target_flows[i]/this->total_target_flow);
    // Result names/keys
    this->result_keys.push_back(this->getResultName(i));
    // Result weights
    this->result_weights.push_back(1.0);
    if (!this->useCovariance){
      // Standard deviations of target flows
      this->data_std.push_back(this->target_flow_fracs[i]*perfusion_std/100.0);
      std::cout<<"Std: "<<i<<" "<<this->target_flow_fracs[i]*perfusion_std/100.0<<std::endl;
    }
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
  
  this->total_target_flow = 0.0;
  while(std::getline(read_file,buffer))
  {
    cmUtils::schSplit(buffer,tokens," ");
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
      cmUtils::schSplit(buffer,tokens," ");
      this->perfusion_data.insert({tokens[0].c_str(),atof(tokens[1].c_str())});
    }
    line_ct++;
  }
  
  read_file.close();
}

// ==========================
// READ COVARIANCE DATA FROM FILE
// ==========================
void svZeroD_distalResistance::readCovarianceFile(string covarianceFileName)
{
  std::ifstream read_file;
  read_file.open(covarianceFileName.c_str());
  
  std::string buffer;
  std::vector<std::string> tokens;
  if(!read_file.is_open())
    throw cmException("ERROR: Cannot open covariance file");
  if(read_file.eof())
    throw cmException("ERROR: No covariance data found!");
 
  int dim;
  int line_ct = 0;
  int dim_ct = 0;
  while(std::getline(read_file,buffer))
  {
    if (line_ct == 0) {
      cmUtils::schSplit(buffer,tokens," ");
      dim = atoi(tokens[0].c_str());
      this->cov_inv.resize(dim);
      std::cout<<"dim="<<dim<<std::endl;
    } else if (line_ct == 1) {
      cmUtils::schSplit(buffer,tokens," ");
      this->cov_det = atof(tokens[0].c_str());
      std::cout<<"det="<<this->cov_det<<std::endl;
    } else if (line_ct > 1) { //skip header
      cmUtils::schSplit(buffer,tokens," ");
      this->cov_inv[dim_ct].resize(dim);
      for (int j=0; j<dim; j++) {
        this->cov_inv[dim_ct][j] = atof(tokens[j].c_str());
        std::cout<<dim_ct<<" "<<j<<" "<<this->cov_inv[dim_ct][j]<<std::endl;
      }
      dim_ct++;
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

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int svZeroD_distalResistance::getResultTotal(){
  return (this->n_corBC_l+this->n_corBC_r);  
}

// ===================================
// IS THIS MODEL SCALED?
// ===================================
bool svZeroD_distalResistance::isScaled(){
  return this->scaled; 
}

// ==================
// GET PARAMETER NAME
// ==================
string svZeroD_distalResistance::getParamName(int index) {
  if (index < this->n_corBC_l) {
    return this->Q_lca_ids_names[index];
  } else if ((index >= this->n_corBC_l) && (index < (this->n_corBC_l+this->n_corBC_r))) {
    return this->Q_rca_ids_names[index-this->n_corBC_l];
  } else {
    const std::string err_msg = "ERROR in svZeroD_distalResistance::getParamName: Invalid index: "
            +std::to_string(index)+" ; Max index: "+std::to_string(this->n_corBC_l+this->n_corBC_r);
    throw std::runtime_error(err_msg);
  }
}

// ==================
// GET PARAMETER NAME
// ==================
string svZeroD_distalResistance::getResultName(int index) {
  if (index < this->n_corBC_l) {
    return this->Q_lca_ids_names[index];
  } else if ((index >= this->n_corBC_l) && (index < (this->n_corBC_l+this->n_corBC_r))) {
    return this->Q_rca_ids_names[index-this->n_corBC_l];
  } else {
    const std::string err_msg = "ERROR in svZeroD_distalResistance::getResultName: Invalid index: "
            +std::to_string(index)+" ; Max index: "+std::to_string(this->n_corBC_l+this->n_corBC_r);
    throw std::runtime_error(err_msg);
  }
}

// ====================
// RETURN PARAMETER SPECIFIED BY STRING SPECIFIER
// ====================
void svZeroD_distalResistance::getSpecifiedParameter(string& specifier, double& return_db_param, int& return_int_param, std::vector<double>& return_vector) {
  if (specifier == "RScaling") {
    return_db_param = this->R_scaling;
  } else if (specifier == "RScaling_history") {
    return_vector = this->R_scaling_history;
  } else {
    throw std::runtime_error("ERROR: Invalid specifier in svZeroD_distalResistance::getSpecifiedParameter.");
  }
}

// ====================
// GET MODEL PARAMETERS
// ====================
void svZeroD_distalResistance::getDefaultParams(stdVec& default_params){
      
  default_params.resize(getParameterTotal());

  for (int i = 0; i < getParameterTotal(); i++) {
    default_params[i] = 1.0;
  }

}

// ====================
// GET PARAMETER RANGES
// ====================
void svZeroD_distalResistance::getParameterLimits(stdVec& limits, bool reverse_scaling){

  limits.resize(2*getParameterTotal());
  for (int i = 0; i < getParameterTotal(); i++) {
    limits[2*i] = 0.5;
    limits[2*i+1] = 2.0;
    //limits[2*i] = 0.25;
    //limits[2*i+1] = 4.0;
    //limits[2*i] = 0.1;
    //limits[2*i+1] = 1.9;
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
void svZeroD_distalResistance::setModelParams(LPNSolverInterface& interface, stdVec params) {

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

  this->R_scaling_history.push_back(this->R_scaling);
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
    interface.update_block_params(block_name, this->coronary_params);
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
    interface.update_block_params(block_name, this->coronary_params);
    R_total_inv += 1.0/(this->coronary_params[0] + this->coronary_params[1] + this->coronary_params[2]);
  }
  //std::cout<<"[solveCoronaryLPN] Total assigned coronary resistance = "<<(1.0/R_total_inv)<<std::endl;
}

// ==========================================
// POSTPROCESS ZEROD SIMULATION
// ==========================================
void svZeroD_distalResistance::postProcess(LPNSolverInterface& interface, const stdVec& t, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) {
  int totOutputSteps = interface.num_output_steps_;
  int totalStepsOnSingleCycle = interface.pts_per_cycle_;
  
  double mean_branch_flow;
  double total_flow = 0.0;
  for(int i = 0; i < n_corBC_l; i++){    
    mean_branch_flow = 0.0;
    for (int t = totOutputSteps - totalStepsOnSingleCycle - 1; t < totOutputSteps; t++) {
      mean_branch_flow += outVals[this->Q_lca_ids[i]][t];
    }
    mean_branch_flow = mean_branch_flow/(double)(totalStepsOnSingleCycle - 1);
    results[i] = mean_branch_flow;
    total_flow += mean_branch_flow;
  }
  for(int i = 0; i < n_corBC_r; i++){    
    mean_branch_flow = 0.0;
    for (int t = totOutputSteps - totalStepsOnSingleCycle - 1; t < totOutputSteps; t++) {
      mean_branch_flow += outVals[this->Q_rca_ids[i]][t];
    }
    mean_branch_flow = mean_branch_flow/(double)(totalStepsOnSingleCycle - 1);
    results[n_corBC_l+i] = mean_branch_flow;
    total_flow += mean_branch_flow;
  }
  //std::cout<<"Total coronary flow = "<<total_flow<<std::endl;
  
  for(int i = 0; i < results.size(); i++) {
    results[i] = results[i]/total_flow;
    //std::cout<<i<<" "<<results[i]<<std::endl;
  }
}


// =========================
// EVAL MODEL ERROR FUNCTION
// =========================
double svZeroD_distalResistance::evalModelError(std::vector<double>& results) {

  int resultTotal   = getResultTotal();

  // Compute mean squared percentage error and mean percentage error of flow fractions
  double loss = 0.0;
  double sq_pct_error = 0.0, pct_error = 0.0;
  for(int i=0; i < resultTotal; i++){
    sq_pct_error = (results[i] - this->target_flow_fracs[i])*(results[i] - this->target_flow_fracs[i])/(this->target_flow_fracs[i]*this->target_flow_fracs[i]);
    loss += sq_pct_error;
    pct_error += sqrt(sq_pct_error);
    //std::cout<<"[evalModelError] results, targets: "<<results[i]<<" "<<this->target_flow_fracs[i]<<std::endl;
  }
  loss = loss/double(resultTotal); // mean of square pct error
  pct_error = 100.0*pct_error/double(resultTotal); // mean pct error
  std::cout<<"[evalModelError] Mean pct error: "<<pct_error<<std::endl;


  return loss;
}

// ====================
// GET DEFAULT RANGES
// ====================
void svZeroD_distalResistance::getDefaultParameterLimits(stdVec& limits) {
  std::cout<<"ERROR: svZeroD_distalResistance::getDefaultParameterLimits not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  std::runtime_error("ERROR: svZeroD_distalResistance::getDefaultParameterLimits not implemented.");
}

// ====================
// GET PRIOR MAPPING
// ====================
void svZeroD_distalResistance::getPriorMapping(int priorModelType,int* prPtr) {
  std::cout<<"ERROR: svZeroD_distalResistance::getPriorMapping not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  std::runtime_error("ERROR: svZeroD_distalResistance::getPriorMapping not implemented.");
}

// ====================
// GET NUMBER OF AUXILIARY STATES
// ====================
int svZeroD_distalResistance::getAuxStateTotal(){
  std::cout<<"ERROR: svZeroD_distalResistance::getAuxStateTotal not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  std::runtime_error("ERROR: svZeroD_distalResistance::getAuxStateTotal not implemented.");
  return 0;
}

// =========================
// KEY/NAME FOR EACH TARGET QUANTITY
// =========================
void svZeroD_distalResistance::getResultKeys(vector<string>& keys) {
  keys = this->result_keys;
}

// =========================
// STANDARD DEVIATION OF EACH TARGET MEASUREMENT
// =========================
void svZeroD_distalResistance::getDataStd(stdVec& stdFactors) {
  stdFactors = this->data_std;
}

// =========================
// INVERSE WEIGHT OF EACH TARGET QUANTITY IN LOG LIKELIHOOD
// =========================
void svZeroD_distalResistance::getResultWeights(stdVec& weights) {
  weights = this->result_weights;
}

// =========================
// CUSTOM DATA OBJECT AFTER SCALING TARGET FLOWS 
// AND REMOVING BRANCHES WITHOUT PERFUSION DATA
// =========================
daData* svZeroD_distalResistance::createCustomData() {
  bool useSingleColumn = true;
  int columnID = 0; // column N-1 after the column with keys (not relevant here if not reading file)
  daData* custom_data = new daData_multiple_Table(useSingleColumn,columnID);
  for (int i = 0; i < this->getResultTotal(); i++) {
    std::cout<<"[createCustomData] key: "<<this->result_keys[i]<<" ; Flow frac: "<<this->target_flow_fracs[i]<<std::endl;
    custom_data->addKeyValue(this->result_keys[i], this->target_flow_fracs[i]);
  }
  if (this->useCovariance == true) {
    custom_data->addCovariance(this->cov_inv, this->cov_det);
  }
  return custom_data;
}
