#include "svZeroD_ClosedLoopCoronary_GSI.h"

using namespace std;

svZeroD_ClosedLoopCoronary_GSI::svZeroD_ClosedLoopCoronary_GSI(){
}

void svZeroD_ClosedLoopCoronary_GSI::setupModel(LPNSolverInterface& interface){
  
  this->nUnknowns = interface.system_size_;
  this->use_CCO = false; 

  // Number of blocks and number of each type
  this->num_blocks = interface.block_names_.size();
  this->n_corBC_l = 0; // Number of left coronary BC blocks
  this->n_corBC_r = 0; // Number of right coronary BC blocks
  this->n_cor3d_l = 0; // Number of left coronary 3D segmented outlets
  this->n_cor3d_r = 0; // Number of right coronary 3D segmented outlets
  this->n_corBC = 0;
  this->n_RCR = 0;
  std::string block_name;
  for (int i = 0; i < this->num_blocks; i++) {
    block_name = interface.block_names_[i];
   
    if (block_name.substr(0,6) == "BC_lca") { // Left coronary BC block
      this->n_corBC_l++;
      this->names_corBC_l.push_back(block_name);
      std::cout<<"n_corBC_l: "<<block_name<<std::endl;
    
    } else if (block_name.substr(0,3) == "lca") { // Left coronary 3D segmented outlet
      // Search for string "cco" in block name
      std::size_t search_cco = block_name.find("cco",3);
      if (search_cco == std::string::npos) { // Does the block correspond to a non-CCO vessel?
        this->n_cor3d_l++;
        std::cout<<"n_cor3d_l: "<<block_name<<std::endl;
      } //search_cco

    } else if (block_name.substr(0,6) == "BC_rca") { // Right coronary BC block
      this->n_corBC_r++;
      this->names_corBC_r.push_back(block_name);
      std::cout<<"n_corBC_r: "<<block_name<<std::endl;
    
    } else if (block_name.substr(0,3) == "rca") { // Right coronary 3D segmented outlet
      // Search for string "cco" in block name
      std::size_t search_cco = block_name.find("cco",3);
      if (search_cco == std::string::npos) { // Does the block correspond to a non-CCO vessel?
        this->n_cor3d_r++;
        std::cout<<"n_cor3d_r: "<<block_name<<std::endl;
      } //search_cco
    
    } else if (block_name.substr(0,6) == "BC_RCR") {
      this->n_RCR++;
    } 
    
  } // for (int i = 0; i < this->num_blocks; i++)
  this->n_corBC = this->n_corBC_l + this->n_corBC_r; 

  std::cout<<"\nNumber of left and right coronary BC blocks: "<<this->n_corBC_l<<" , "<<this->n_corBC_r<<std::endl;
  std::cout<<"Number of left and right coronary 3D outlets: "<<this->n_cor3d_l<<" , "<<this->n_cor3d_r<<std::endl;
  std::cout<<"Number of RCR BC blocks: "<<this->n_RCR<<std::endl;

  if (this->n_corBC_l == this->n_cor3d_l) {
    if (this->n_corBC_r == this->n_cor3d_r) {
      this->use_CCO = false;
    } else {
      throw std::runtime_error("ERROR: n_corBC_l == n_cor3d_l but n_corBC_r != n_cor3d_r");
    }
  } else if (this->n_corBC_l > this->n_cor3d_l) {
    this->use_CCO = true;
    std::cout<<"Using CCO."<<std::endl;
  } else {
    throw std::runtime_error("ERROR: n_corBC_l < this->n_cor3d_l");
  }
        
  // Initialize parameter vectors and read baseline block params
  this->coronary_params.resize(6);
  this->rcr_params.resize(3);
  this->heart_params.resize(27);
  this->vessel_params.resize(4);
 
  // Baseline parameters (named *_base) are set in the svZeroD config file
  //
  // Read left coronary parameters
  this->Ra_l_base.reserve(this->n_corBC_l);
  this->Ram_l_base.reserve(this->n_corBC_l);
  this->Rv_l_base.reserve(this->n_corBC_l);
  this->Ca_l_base.reserve(this->n_corBC_l);
  this->Cim_l_base.reserve(this->n_corBC_l);
  for (int i = 0; i < this->n_corBC_l; i++) {
    block_name = this->names_corBC_l[i];
    interface.read_block_params(block_name, this->coronary_params);
    this->Ra_l_base[i] = this->coronary_params[0];
    this->Ram_l_base[i] = this->coronary_params[1];
    this->Rv_l_base[i] = this->coronary_params[2];
    this->Ca_l_base[i] = this->coronary_params[3];
    this->Cim_l_base[i] = this->coronary_params[4];
  }
  
  // Read right coronary parameters
  this->Ra_r_base.reserve(this->n_corBC_r);
  this->Ram_r_base.reserve(this->n_corBC_r);
  this->Rv_r_base.reserve(this->n_corBC_r);
  this->Ca_r_base.reserve(this->n_corBC_r);
  this->Cim_r_base.reserve(this->n_corBC_r);
  for (int i = 0; i < this->n_corBC_r; i++) {
    block_name = this->names_corBC_r[i];
    interface.read_block_params(block_name, this->coronary_params);
    this->Ra_r_base[i] = this->coronary_params[0];
    this->Ram_r_base[i] = this->coronary_params[1];
    this->Rv_r_base[i] = this->coronary_params[2];
    this->Ca_r_base[i] = this->coronary_params[3];
    this->Cim_r_base[i] = this->coronary_params[4];
  }
  
  // Read RCR parameters
  this->Rp_rcr_base.reserve(this->n_RCR);
  this->Rd_rcr_base.reserve(this->n_RCR);
  this->C_rcr_base.reserve(this->n_RCR);
  for (int i = 0; i < this->n_RCR; i++) {
    block_name = "BC_RCR" + to_string(i+1);
    interface.read_block_params(block_name, this->rcr_params);
    this->Rp_rcr_base[i] = this->rcr_params[0];
    this->C_rcr_base[i] = this->rcr_params[1];
    this->Rd_rcr_base[i] = this->rcr_params[2];
  }
  
  // Read closed-loop heart parameters
  interface.read_block_params("CLH", this->heart_params);
  this->Rrv_base = this->heart_params[9];
  this->Rlv_base = this->heart_params[13];
  this->Rpd_base = this->heart_params[16];
  
  // Read aorta parameters
  interface.read_block_params("aorta", this->vessel_params);

  if (this->vessel_params[1] == 0) { // If C = 0
    this->useRigidSurrogate = true;
    this->useRigidSurrogate = false;
  }
  
  // Save solution IDs corresponding to important quantities
  // TODO: Q_lca_ids[0] and Q_rca_ids[0] should correspond to lca1/rca1?
  this->Q_lca_ids.resize(this->n_cor3d_l, -1);
  this->Q_rca_ids.resize(this->n_cor3d_r, -1);
  this->Q_rcr_ids.resize(this->n_RCR, -1);
  int ct_lca = 0, ct_rca = 0, ct_rcr = 0;
  string var_name;
  bool lca_flag, rca_flag, rcr_flag, flow_flag;

  // Iterate through the names of variables in the 0D system
  for (int i = 0; i < interface.system_size_; i++) {
    var_name = interface.variable_names_[i];
    flow_flag = (var_name.substr(0,4) == "flow"); // Is this a flow variable?
    
    // Find last occurence of ":" in variable name. 
    // The rest of var_name is either the exit block name or the main block name (if it is an internal variable).
    std::size_t blk_name_start = var_name.rfind(":");
    if (blk_name_start == std::string::npos) {
      throw std::runtime_error("Error: Invalid variable name format.");
    }
    // Outlet block should be a CCO tree root, RCR BC, or coronary BC that is not connected to a CCO vessel
    auto str_len = var_name.size();
    if (str_len < 7) {
      std::cout << "\nVariable name: "<< var_name << std::endl;
      std::cout << "Error: Variable name is too short." << std::endl;
      throw std::runtime_error("Error: Variable name is too short.");
    } //str_len
    auto cco_root_flag = !var_name.compare(str_len-6,6,"_cco_0"); // Is the outlet a CCO root?
    auto cco_lca_flag = !var_name.compare(blk_name_start+1,3,"lca"); // Is the outlet a lca?
    auto lca_bc_flag = !var_name.compare(blk_name_start+1,6,"BC_lca"); // Is the outlet a BC_lca?
    lca_flag = false;
    if (cco_root_flag && cco_lca_flag) { // For outlets with CCO trees
      lca_flag = true;
    } else if (lca_bc_flag) { // For outlets without CCO trees
      // Search for string "cco" in exit block name
      std::size_t search_cco = var_name.find("cco",blk_name_start+1);
      if (search_cco == std::string::npos) { // Does the outlet correspond to a non-CCO block?
        lca_flag = true;
      } //search_cco
    } 
    auto cco_rca_flag = !var_name.compare(blk_name_start+1,3,"rca"); // Is the outlet a rca?
    auto rca_bc_flag = !var_name.compare(blk_name_start+1,6,"BC_rca"); // Is the outlet a BC_rca?
    rca_flag = false;
    if (cco_root_flag && cco_rca_flag) { // For outlets with CCO trees
      rca_flag = true;
    } else if (rca_bc_flag) { // For outlets without CCO trees
      // Search for string "cco" in exit block name
      std::size_t search_cco = var_name.find("cco",blk_name_start+1);
      if (search_cco == std::string::npos) { // Does the outlet correspond to a non-CCO block?
        rca_flag = true;
      } // search_cco
    }
    rcr_flag = !var_name.compare(blk_name_start+1,6,"BC_RCR");

    if (flow_flag && lca_flag) {
      this->Q_lca_ids[ct_lca] = i;
      ct_lca++;
      std::cout<<"Q_rca: "<<ct_lca<<" "<<i<<" "<<var_name<<std::endl;
    } else if (flow_flag && rca_flag) {
      this->Q_rca_ids[ct_rca] = i;
      ct_rca++;
      std::cout<<"Q_rca: "<<ct_rca<<" "<<i<<" "<<var_name<<std::endl;
    } else if (flow_flag && rcr_flag) {
      this->Q_rcr_ids[ct_rcr] = i;
      ct_rcr++;
    } else if (var_name == "flow:J_heart_outlet:aorta") {
      this->Q_aorta_id = i;
    } else if (var_name == "pressure:J_heart_outlet:aorta") {
      this->P_aorta_id = i;
    } else if (var_name == "Q_LV:CLH") {
      this->Q_LV_id = i;
    } else if (var_name == "V_LV:CLH") {
      this->V_LV_id = i;
    } else if (var_name == "Q_LA:CLH") {
      this->Q_LA_id = i;
    } else if (var_name == "Q_RV:CLH") {
      this->Q_RV_id = i;
    } else if (var_name == "P_pul:CLH") {
      this->P_pul_id = i;
    } else if (var_name == "P_RV:CLH") {
      this->P_RV_id = i;
    } else if (var_name == "pressure:J_heart_inlet:CLH") {
      this->P_RA_id = i;
    }
  }
  //std::cout << "[svZeroD_ClosedLoopCoronary_GSI] Q_lca_ids.size() this->n_cor3d_l: " << Q_lca_ids.size() << ", "<< this->n_cor3d_l << std::endl;
  
  // Check to make sure all variables ids have been assigned
  for (int i = 0; i < this->Q_lca_ids.size(); i++) { 
    if (this->Q_lca_ids[i] < 0) {
      std::cout << "Q_lca index: "<< i << std::endl;
      std::cout << "Error: Did not find all solution IDs for variables named BC_lca" << std::endl;
      throw std::runtime_error("Error: Did not find all solution IDs for variables named BC_lca");
    }
  }
  for (int i = 0; i < this->Q_rca_ids.size(); i++) {
    if (this->Q_rca_ids[i] < 0) {
      std::cout << "Q_rca index: "<< i << std::endl;
      std::cout << "Error: Did not find all solution IDs for variables named BC_rca" << std::endl;
      throw std::runtime_error("Error: Did not find all solution IDs for variables named BC_rca");
    } 
  }
  for (int i = 0; i < this->Q_rcr_ids.size(); i++) {
    if (this->Q_rcr_ids[i] < 0) {
      std::cout << "Q_rcr index: "<< i << std::endl;
      std::cout << "Error: Did not find all solution IDs for variables named BC_RCR" << std::endl;
      throw std::runtime_error("Error: Did not find all solution IDs for variables named BC_RCR");
    }
  }
  if ((Q_aorta_id < 0) || (P_aorta_id < 0) || (Q_LV_id < 0) || (V_LV_id < 0) || (Q_LA_id < 0) || (Q_RV_id < 0) || (P_pul_id < 0) || (P_RV_id < 0) || (P_RA_id < 0)) {
    //std::cout << "[svZeroD_ClosedLoopCoronary_GSI] Variable IDs: " << Q_aorta_id << ", " << P_aorta_id << ", " << Q_LV_id << ", " << V_LV_id << ", " << Q_LA_id << ", " << Q_RV_id << ", " << P_pul_id << ", " << P_RV_id << ", " << P_RA_id << std::endl;
    throw std::runtime_error("Error: Did not find all solution IDs for variables.");
  }

}
// ========================
// GET NUMBER OF PARAMETERS
// ========================
int svZeroD_ClosedLoopCoronary_GSI::getParameterTotal(){
  //return 38;
  //return 37;
  //return 36;
  return 35;
}

// ===================================
// GET NUMBER OF UNKNOWNS
// ===================================
int svZeroD_ClosedLoopCoronary_GSI::getStateTotal(){
  return this->nUnknowns; 
}

int svZeroD_ClosedLoopCoronary_GSI::getAuxStateTotal(){
  return 50;
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int svZeroD_ClosedLoopCoronary_GSI::getResultTotal(){
  return 29;  
}

// ==================
// GET PARAMETER NAME
// ==================
string svZeroD_ClosedLoopCoronary_GSI::getParamName(int index){
  string result;
  switch(index){
    case 0: {          
      result = "Tsa";
      break; }
    case 1: {      
      result = "tpwave";
      break; }
    case 2: {      
      result = "Erv";
      break; }
    case 3: {      
      result = "Elv";
      break; }
    case 4:  {      
      result = "iml";
      break; }
    case 5: {      
      result = "Lrv_a";
      break; }
    case 6: {      
      result = "Rrv_a";
      break; }
    case 7: {      
      result = "Lra_v";
      break; }
    case 8: {      
      result = "Rra_v";
      break; }
    case 9: {      
      result = "Lla_v";
      break; }    
    case 10: {      
      result = "Rla_v";
      break; }    
    case 11: {      
      result = "Rlv_ao";
      break; }    
    case 12: {      
      result = "Llv_a";
      break; }    
    case 13: {      
      result = "Vrv_u";
      break; }    
    case 14: {      
      result = "Vlv_u";
      break; }    
    case 15: {      
      result = "Rpd";
      break; }
    case 16: {
	    result = "Cp";
      break; }    
    case 17: {
	    result = "Cpa";
      break; }
  //case 18: {
  //        result = "R_inlet";
  //  break; }
    case 18: {
            result = "Kxp_ra";
      break; }
    case 19: {
            result = "Kxv_ra";
      break; }
    case 20: {
            result = "Emax_ra";
      break; }
    case 21: {
            result = "Vaso_ra";
      break; }
    case 22: {
            result = "Kxp_la";
      break; }
    case 23: {
            result = "Kxv_la";
      break; }
    case 24: {
            result = "Emax_la";
      break; }
    case 25: {
            result = "Vaso_la";
      break; }
    case 26: {
            result = "Ram_cor";
      break; }
    case 27: {
            result = "Rv_cor";
      break; }
    case 28: {
            result = "Cam_l";
      break; }
    case 29: {
            result = "Ca_l";
      break; }
    case 30: {
            result = "Cam_r";
      break; }
    case 31: {
            result = "Ca_r";
      break; }
    case 32: {
            result = "Rrcr";
      break; }
    case 33: {
            result = "Crcr";
      break; }
  //case 34: {
  //        result = "R_prox_factor";
  //  break; }
    case 34: {
            result = "imr";
      break; }
  //case 36: {
  //        result = "init_volume_scaling";
  //  break; }
//  case 18: {
//          result = "R_inlet";
//    break; }
//  case 19: {
//          result = "Kxp_ra";
//    break; }
//  case 20: {
//          result = "Kxv_ra";
//    break; }
//  case 21: {
//          result = "Emax_ra";
//    break; }
//  case 22: {
//          result = "Vaso_ra";
//    break; }
//  case 23: {
//          result = "Kxp_la";
//    break; }
//  case 24: {
//          result = "Kxv_la";
//    break; }
//  case 25: {
//          result = "Emax_la";
//    break; }
//  case 26: {
//          result = "Vaso_la";
//    break; }
//  case 27: {
//          result = "Ram_cor";
//    break; }
//  case 28: {
//          result = "Rv_cor";
//    break; }
//  case 29: {
//          result = "Cam_l";
//    break; }
//  case 30: {
//          result = "Ca_l";
//    break; }
//  case 31: {
//          result = "Cam_r";
//    break; }
//  case 32: {
//          result = "Ca_r";
//    break; }
//  case 33: {
//          result = "Rrcr";
//    break; }
//  case 34: {
//          result = "Crcr";
//    break; }
//  case 35: {
//          result = "R_prox_factor";
//    break; }
//  case 36: {
//          result = "imr";
//    break; }
//  case 37: {
//          result = "init_volume_scaling";
//    break; }
  }
  return result;
}

// ===============
// GET RESULT NAME
// ===============
string svZeroD_ClosedLoopCoronary_GSI::getResultName(int index){
  string result;
  switch(index){
    case 0: {      
      result = "Pao-min";
      break; }
    case 1: {      
      result = "Pao-min_conv";
      break; }
    case 2: {      
      result = "Pao-max";
      break; }
    case 3: {      
      result = "Pao-max_conv";
      break; }
    case 4: {      
      result = "Pao-mean";
      break; }
    case 5: {      
      result = "Pao-mean_conv";
      break; }
    case 6: {      
      result = "Aor-Cor-split";
      break; }
    case 7: {      
      result = "ABSQinlet";
      break; }
    case 8: {      
      result = "ABSQinlet_conv";
      break; }
    case 9: {
       result = "Qsystole_perc";
       break; }
    case 10: {      
      result = "Ppul-mean";
      break; }
    case 11: {      
      result = "EF-LV";
      break; }
    case 12: {
      result = "ESV";
      break; }
    case 13: {
      result = "EDV";
      break; }
    case 14: {      
      result = "Qla-ratio";
      break; }
    case 15: {      
      result = "mit-valve-time";
      break; }
    case 16: {      
      result = "aor-valve-time";
      break; }
    case 17: {      
      result = "pul-valve-time";
      break; }
    case 18: {      
      result = "Pra-mean";
      break; }
    case 19: {      
      result = "l-cor-max-ratio";
      break; }
    case 20: {      
      result = "l-cor-tot-ratio";
      break; }
    case 21: {      
      result = "l-third-FF";
      break; }   
    case 22: {      
      result = "l-half-FF";
      break; }  
    case 23: {
      result = "l-grad-ok";
      break; } 
    case 24: {      
      result = "r-cor-max-ratio";
      break; }   
    case 25: {      
      result = "r-cor-tot-ratio";
      break; }       
    case 26: {      
      result = "r-third-FF";
      break; }    
    case 27: {      
      result = "r-half-FF";
      break; }
    case 28: {
      result = "r-grad-ok";
      break; } 
  }
  return result;
}

// ====================
// GET DEFAULT MODEL PARAMETERS
// ====================
void svZeroD_ClosedLoopCoronary_GSI::getDefaultParams(stdVec& zp){
      
      zp.resize(getParameterTotal());

      zp[0] = 0.4;  // Tsa
      zp[1] = 8.00; // tpwave

      zp[2] = 0.25;  // Erv   
      zp[3] = 1.25;  // Elv
      zp[4] = 5.0;  // iml

      zp[5] = 0.55; // Lrv_a
      zp[6] = 1.0; // Rrv_a

      zp[7] = 1.0;  // Lra_v
      zp[8] = 5.0;  // Rra_v

      zp[9] = 0.002;// Lla_v
      zp[10] = 5.0; // Rla_v

      zp[11] = 1.0;  // Rlv_ao
      zp[12] = 1.0;   // Llv_a

      zp[13] = 0.0; // Vrv_u
      zp[14] = 0.0; // Vlv_u

      zp[15] = 100.0; // Rpd
      zp[16] = 1.0;   // Cp
      zp[17] = 0.9;   // Cpa
      //zp[18] = 15.0; // R_inlet

      zp[18] = 4.0;   // Kxp_ra
      zp[19] = 0.005; // Kxv_ra
      zp[20] = 0.2;   // Emax_ra
      zp[21] = 0.0;   // Vaso_ra

      zp[22] = 8.0;   // Kxp_la
      zp[23] = 0.008;// Kxv_la
      zp[24] = 0.3;   // Emax_la
      zp[25] = 0.0;   // Vaso_la

      zp[26] = 1.0;  // Ram_cor
      zp[27] = 1.0;   // Rv_cor
      zp[28] = 0.25;   // Cam_l
      zp[39] = 1.0;   // Ca_l
      zp[30] = 0.25;  // Cam_r
      zp[31] = 1.0;  // Ca_r
      zp[32] = 1.00;  // Rrcr
      zp[33] = 1.0;  // Crcr
      //zp[34] = 0.11;  // Rprox_factor
      zp[34] = 0.75;  // imr
      //zp[36] = 1.0; // init_volume_scaling
//    zp[18] = 15.0; // R_inlet

//    zp[19] = 4.0;   // Kxp_ra
//    zp[20] = 0.005; // Kxv_ra
//    zp[21] = 0.2;   // Emax_ra
//    zp[22] = 0.0;   // Vaso_ra

//    zp[23] = 8.0;   // Kxp_la
//    zp[24] = 0.008;// Kxv_la
//    zp[25] = 0.3;   // Emax_la
//    zp[26] = 0.0;   // Vaso_la

//    zp[27] = 1.0;  // Ram_cor
//    zp[28] = 1.0;   // Rv_cor
//    zp[29] = 0.25;   // Cam_l
//    zp[30] = 1.0;   // Ca_l
//    zp[31] = 0.25;  // Cam_r
//    zp[32] = 1.0;  // Ca_r
//    zp[33] = 1.00;  // Rrcr
//    zp[34] = 1.0;  // Crcr
//    zp[35] = 0.11;  // Rprox_factor
//    zp[36] = 0.75;  // imr
//    zp[37] = 1.0; // init_volume_scaling
}

// ====================
// GET PARAMETER RANGES
// ====================
void svZeroD_ClosedLoopCoronary_GSI::getParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());

  // Constrained ranges
  limits[0]=0.4000;   limits[1]=0.4500; // Tsa
  limits[2]=8.0000;   limits[3]=9.000; // tpwave
  limits[4]=1.0000;   limits[5]=3.00; // Erv
  limits[6]=1.5000;   limits[7]=5.00; // Elv
  limits[8]=0.2000;   limits[9]=1.00; // iml
  limits[10]=0.1;     limits[11]=0.5;   // Lrv_a
  limits[12]=0.5;     limits[13]=1.5; // Rrv_a
  //limits[14]=0.001;   limits[15]=0.5;  // Lra_v
  limits[14]=0.01;   limits[15]=0.5;  // Lra_v
  limits[16]=5.0;     limits[17]=12.0; // Rra_v
  //limits[18]=0.001;   limits[19]=0.5;  // Lla_v
  limits[18]=0.01;   limits[19]=0.5;  // Lla_v
  limits[20]=5.0;     limits[21]=12.0; // Rla_v
  limits[22]=0.5;     limits[23]=1.5; // Rlv_ao
  limits[24]=0.1;     limits[25]=0.5;   // Llv_a
  limits[26]=-10.0;   limits[27]=10.0; // Vrv_u
  limits[28]=-10.0;   limits[29]=10.0; // Vlv_u
  limits[30]=0.5000;  limits[31]=1.5; // Rpd
  limits[32]=0.900;   limits[33]=1.1;   //Cp
  limits[34]=0.08;    limits[35]=1.0;   //Cpa
  //limits[36]=0.0;     limits[37]=0.0; // R_inlet
  //limits[36]=0.001;   limits[37]=10.00; //Kxp_ra
  limits[36]=1.0;   limits[37]=10.00; //Kxp_ra
  limits[38]=0.003;   limits[39]=0.005; //Kxv_ra
  limits[40]=0.2;     limits[41]=0.3;  //Emax_ra
  limits[42]=-5.00;   limits[43]=5.00; //Vaso_ra
  //limits[44]=0.0001;  limits[45]=10.00; //Kxp_la
  limits[44]=1.0;  limits[45]=10.00; //Kxp_la
  limits[46]=0.0075;  limits[47]=0.0085; //Kxv_la
  limits[48]=0.29;    limits[49]=0.310; //Emax_la
  limits[50]=-5.00;   limits[51]=10.00; //Vaso_la
  //limits[52]=0.0100;  limits[53]=10.0;  //Ram_cor
  limits[52]=0.1;  limits[53]=2.0;  //Ram_cor
  //limits[54]=0.0001;  limits[55]=10.0;  //Rv_cor
  limits[54]=0.5;  limits[55]=10.0;  //Rv_cor
  //limits[56]=0.0001;  limits[57]=10.0;  //Cam_l
  //limits[56]=1.0;  limits[57]=10.0;  //Cam_l
  limits[56]=0.5;  limits[57]=10.0;  //Cam_l
  //limits[58]=0.0001;  limits[59]=10.0;  //Ca_l
  limits[58]=0.5;  limits[59]=10.0;  //Ca_l
  //limits[60]=0.0001;  limits[61]=10.0;  //Cam_r
  limits[60]=0.05;  limits[61]=10.0;  //Cam_r
  //limits[62]=0.0001;  limits[63]=10.0;  //Ca_r
  limits[62]=0.5;  limits[63]=10.0;  //Ca_r
  //limits[64]=0.1000;  limits[65]=10.0;  //Rrcr
  limits[64]=0.5;  limits[65]=10.0;  //Rrcr
  //limits[66]=0.0100;  limits[67]=1.1;  //Crcr
  limits[66]=0.1;  limits[67]=2.0;  //Crcr
  //limits[68]=0.1;     limits[69]=0.2;   // Rprox_factor
  //limits[70]=0.2000;  limits[71]=1.000; // imr
  limits[68]=0.2000;  limits[69]=1.000; // imr
  //limits[72]=0.3;     limits[73]=1.5;   // init_volume_scaling
  //limits[72]=0.1;     limits[73]=1.5;   // init_volume_scaling
//limits[0]=0.4000;   limits[1]=0.4500; // Tsa
//limits[2]=8.0000;   limits[3]=9.000; // tpwave
//limits[4]=1.0000;   limits[5]=3.00; // Erv
//limits[6]=1.5000;   limits[7]=5.00; // Elv
//limits[8]=0.2000;   limits[9]=1.00; // iml
//limits[10]=0.1;     limits[11]=0.5;   // Lrv_a
//limits[12]=0.5;     limits[13]=1.5; // Rrv_a
////limits[14]=0.001;   limits[15]=0.5;  // Lra_v
//limits[14]=0.01;   limits[15]=0.5;  // Lra_v
//limits[16]=5.0;     limits[17]=12.0; // Rra_v
////limits[18]=0.001;   limits[19]=0.5;  // Lla_v
//limits[18]=0.01;   limits[19]=0.5;  // Lla_v
//limits[20]=5.0;     limits[21]=12.0; // Rla_v
//limits[22]=0.5;     limits[23]=1.5; // Rlv_ao
//limits[24]=0.1;     limits[25]=0.5;   // Llv_a
//limits[26]=-10.0;   limits[27]=10.0; // Vrv_u
//limits[28]=-10.0;   limits[29]=10.0; // Vlv_u
//limits[30]=0.5000;  limits[31]=1.5; // Rpd
//limits[32]=0.900;   limits[33]=1.1;   //Cp
//limits[34]=0.08;    limits[35]=1.0;   //Cpa
//limits[36]=0.0;     limits[37]=1.0; // R_inlet
////limits[38]=0.001;   limits[39]=10.00; //Kxp_ra
//limits[38]=1.0;   limits[39]=10.00; //Kxp_ra
//limits[40]=0.003;   limits[41]=0.005; //Kxv_ra
//limits[42]=0.2;     limits[43]=0.3;  //Emax_ra
//limits[44]=-5.00;   limits[45]=5.00; //Vaso_ra
////limits[46]=0.0001;  limits[47]=10.00; //Kxp_la
//limits[46]=1.0;  limits[47]=10.00; //Kxp_la
//limits[48]=0.0075;  limits[49]=0.0085; //Kxv_la
//limits[50]=0.29;    limits[51]=0.310; //Emax_la
//limits[52]=-5.00;   limits[53]=10.00; //Vaso_la
////limits[54]=0.0100;  limits[55]=10.0;  //Ram_cor
//limits[54]=0.100;  limits[55]=2.0;  //Ram_cor
////limits[56]=0.0001;  limits[57]=10.0;  //Rv_cor
//limits[56]=0.5;  limits[57]=10.0;  //Rv_cor
////limits[58]=0.0001;  limits[59]=10.0;  //Cam_l
//limits[58]=0.5;  limits[59]=10.0;  //Cam_l
////limits[60]=0.0001;  limits[61]=10.0;  //Ca_l
//limits[60]=0.5;  limits[61]=10.0;  //Ca_l
////limits[62]=0.0001;  limits[63]=10.0;  //Cam_r
//limits[62]=0.05;  limits[63]=10.0;  //Cam_r
////limits[64]=0.0001;  limits[65]=10.0;  //Ca_r
//limits[64]=0.5;  limits[65]=10.0;  //Ca_r
////limits[66]=0.1000;  limits[67]=10.0;  //Rrcr
//limits[66]=0.5;  limits[67]=10.0;  //Rrcr
////limits[68]=0.0100;  limits[69]=1.1;  //Crcr
//limits[68]=0.100;  limits[69]=2.0;  //Crcr
//limits[70]=0.1;     limits[71]=0.2;   // Rprox_factor
//limits[72]=0.2000;  limits[73]=1.000; // imr
////limits[74]=0.3;     limits[75]=1.5;   // init_volume_scaling
//limits[74]=0.1;     limits[75]=1.5;   // init_volume_scaling
}

// ====================
// GET DEFAULT RANGES
// ====================
void svZeroD_ClosedLoopCoronary_GSI::getDefaultParameterLimits(stdVec& limits) {
  std::cout<<"ERROR: svZeroD_ClosedLoopCoronary_GSI::getDefaultParameterLimits not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  std::runtime_error("ERROR: svZeroD_ClosedLoopCoronary_GSI::getDefaultParameterLimits not implemented.");
}

// ====================
// GET PRIOR MAPPING
// ====================
void svZeroD_ClosedLoopCoronary_GSI::getPriorMapping(int priorModelType,int* prPtr) {
  std::cout<<"ERROR: svZeroD_ClosedLoopCoronary_GSI::getPriorMapping not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  std::runtime_error("ERROR: svZeroD_ClosedLoopCoronary_GSI::getPriorMapping not implemented.");
}

// ==========================================
// UPDATE PARAMETERS OF THE ZEROD MODEL
// ==========================================
void svZeroD_ClosedLoopCoronary_GSI::setModelParams(LPNSolverInterface& interface, const stdVec& params) {
  std::string block_name;
  
  // Update the model parameters 
  for (int i = 0; i < this->n_corBC_l; i++) {
    block_name = this->names_corBC_l[i];
//  this->coronary_params[0] = this->Ra_l_base[i]*params[27]; //Ra
//  this->coronary_params[1] = this->Ram_l_base[i]*params[27]; //Ram
//  this->coronary_params[2] = this->Rv_l_base[i]*params[28]; //Rv
//  this->coronary_params[3] = this->Ca_l_base[i]*params[30]; //Ca
//  this->coronary_params[4] = this->Cim_l_base[i]*params[29]; //Cim
    this->coronary_params[0] = this->Ra_l_base[i]*params[26]; //Ra
    this->coronary_params[1] = this->Ram_l_base[i]*params[26]; //Ram
    this->coronary_params[2] = this->Rv_l_base[i]*params[27]; //Rv
    this->coronary_params[3] = this->Ca_l_base[i]*params[29]; //Ca
    this->coronary_params[4] = this->Cim_l_base[i]*params[28]; //Cim
    this->coronary_params[5] = params[4]; //iml
    interface.update_block_params(block_name, this->coronary_params);
  }
  
  for (int i = 0; i < this->n_corBC_r; i++) {
    block_name = this->names_corBC_r[i];
//  this->coronary_params[0] = this->Ra_r_base[i]*params[27]; //Ra
//  this->coronary_params[1] = this->Ram_r_base[i]*params[27]; //Ram
//  this->coronary_params[2] = this->Rv_r_base[i]*params[28]; //Rv
//  this->coronary_params[3] = this->Ca_r_base[i]*params[32]; //Ca
//  this->coronary_params[4] = this->Cim_r_base[i]*params[31]; //Cim
//  this->coronary_params[5] = params[36]; //imr
    this->coronary_params[0] = this->Ra_r_base[i]*params[26]; //Ra
    this->coronary_params[1] = this->Ram_r_base[i]*params[26]; //Ram
    this->coronary_params[2] = this->Rv_r_base[i]*params[27]; //Rv
    this->coronary_params[3] = this->Ca_r_base[i]*params[31]; //Ca
    this->coronary_params[4] = this->Cim_r_base[i]*params[30]; //Cim
    this->coronary_params[5] = params[34]; //imr
    interface.update_block_params(block_name, this->coronary_params);
  }
  
  for (int i = 0; i < this->n_RCR; i++) {
    block_name = "BC_RCR" + to_string(i+1);
//  this->rcr_params[0] = this->Rp_rcr_base[i]*params[33]; //Rp
//  this->rcr_params[1] = this->C_rcr_base[i]*params[34]; //C
//  this->rcr_params[2] = this->Rd_rcr_base[i]*params[33]; //Rd
    this->rcr_params[0] = this->Rp_rcr_base[i]*params[32]; //Rp
    this->rcr_params[1] = this->C_rcr_base[i]*params[33]; //C
    this->rcr_params[2] = this->Rd_rcr_base[i]*params[32]; //Rd
    interface.update_block_params(block_name, this->rcr_params);
  }
  
  this->heart_params[0]  = params[0]; //Tsa_s
  this->heart_params[1]  = params[1]; //tpwave_s
  this->heart_params[2]  = params[2]; //Erv_s
  this->heart_params[3]  = params[3]; //Elv_s
  this->heart_params[4]  = params[4]; //iml
  //this->heart_params[5]  = params[36]; //imr
  this->heart_params[5]  = params[34]; //imr
  this->heart_params[6]  = params[7]/pConv; //Lra_v
  this->heart_params[7]  = params[8]/pConv; //Rra_v
  this->heart_params[8]  = params[5]/pConv; //Lrv_a
  //heart_params[9]  = this->params_heart[9]*params[6]/pConv; //Rrv_a
  this->heart_params[9]  = this->Rrv_base*params[6]/pConv; //Rrv_a
//this->heart_params[9]  = this->Rrv_base; //Rrv_a
  this->heart_params[10] = params[9]/pConv; //Lla_v
  this->heart_params[11] = params[10]/pConv; //Rla_v
  this->heart_params[12] = params[12]/pConv; //Llv_a
  //heart_params[13] = this->params_heart[13]*params[11]/pConv; //Rlv_ao
  this->heart_params[13] = this->Rlv_base*params[11]/pConv; //Rlv_ao
//this->heart_params[13] = this->Rlv_base; //Rlv_ao
  this->heart_params[14] = params[13]; //Vrv_u
  this->heart_params[15] = params[14]; //Vlv_u
  //heart_params[16] = this->params_heart[16]*params[15]/pConv; //Rpd
  this->heart_params[16] = this->Rpd_base*params[15]/pConv; //Rpd
//this->heart_params[16] = this->Rpd_base; //Rpd
  this->heart_params[17] = params[16]; //Cp
  this->heart_params[18] = params[17]; //Cpa
//this->heart_params[19] = params[19]; //Kxp_ra
//this->heart_params[20] = params[20]; //Kxv_ra
//this->heart_params[21] = params[23]; //Kxp_la
//this->heart_params[22] = params[24]; //Kxv_la
//this->heart_params[23] = params[21]; //Emax_ra
//this->heart_params[24] = params[25]; //Emax_la
//this->heart_params[25] = params[22]; //Vaso_ra
//this->heart_params[26] = params[26]; //Vaso_la
  this->heart_params[19] = params[18]; //Kxp_ra
  this->heart_params[20] = params[19]; //Kxv_ra
  this->heart_params[21] = params[22]; //Kxp_la
  this->heart_params[22] = params[23]; //Kxv_la
  this->heart_params[23] = params[20]; //Emax_ra
  this->heart_params[24] = params[24]; //Emax_la
  this->heart_params[25] = params[21]; //Vaso_ra
  this->heart_params[26] = params[25]; //Vaso_la
  interface.update_block_params("CLH", this->heart_params);
}

// ==========================================
// POSTPROCESS ZEROD SIMULATION
// ==========================================
void svZeroD_ClosedLoopCoronary_GSI::postProcess(LPNSolverInterface& interface, const stdVec& t, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) {
  // Time parameters
  int totalStepsOnSingleCycle = interface.pts_per_cycle_;
  int numCycles = interface.num_cycles_;
  int totOutputSteps = interface.num_output_steps_;
  // IDs of main lca/rca
  int lca_main_id = 0;
  int rca_main_id = 0;
  double max_flow = 0.0;
    
  // SUM RCR FLUX
  double temp = 0.0;
  double Q_rcr = 0.0;
  for(int loopA=0;loopA<n_RCR;loopA++){
    temp = cmUtils::trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,outVals[this->Q_rcr_ids[loopA]]);
    Q_rcr += temp;
  }

  // SUM LEFT CORONARY FLUX
  double Q_lcor = 0.0;
  max_flow = 0.0;
  for(int loopA=0;loopA<n_cor3d_l;loopA++){    
    temp = cmUtils::trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,outVals[this->Q_lca_ids[loopA]]);
    Q_lcor = Q_lcor + temp;
//  if (std::fabs(temp) > max_flow) { //TODO
//    max_flow = std::fabs(temp);
//    lca_main_id = loopA;
//  }
  }

  // INTEGRATE MAIN FLOW
  double lmain_flow = cmUtils::trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,outVals[this->Q_lca_ids[lca_main_id]]);

  // SUM RIGHT CORONARY FLUX
  double Q_rcor = 0.0;
  max_flow = 0.0;
  for(int loopA=0;loopA<n_cor3d_r;loopA++){    
    temp = cmUtils::trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,outVals[this->Q_rca_ids[loopA]]);
    Q_rcor = Q_rcor + temp;
//  if (std::fabs(temp) > max_flow) { // TODO
//    max_flow = std::fabs(temp);
//    rca_main_id = loopA;
//  }
  }

  // SUM RIGHT CORONARY FLUX
  double rmain_flow = cmUtils::trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,outVals[this->Q_rca_ids[rca_main_id]]);

  // FIND THE END OF SYSTOLE
  double small_number = 1e-4;
  int systole_end = totOutputSteps-totalStepsOnSingleCycle/2 - 1;
  for(int count = totOutputSteps-totalStepsOnSingleCycle-1; count < totOutputSteps-1; count++) {
     if(outVals[this->Q_LV_id][count-1] > small_number && outVals[this->Q_LV_id][count] > small_number && outVals[this->Q_LV_id][count+1] < small_number ) {
        systole_end = count;
 //       printf("SYSTOLE END FOUND TO BE: %d\n",systole_end);
        break;
     }
  }

  // FIND THE START OF SYSTOLE
  int systole_start = totOutputSteps - totalStepsOnSingleCycle - 1;
  for(int count = totOutputSteps - totalStepsOnSingleCycle - 1; count < totOutputSteps - 1; count++) {
     if(outVals[this->Q_LV_id][count] < small_number && outVals[this->Q_LV_id][count+1] > small_number && outVals[this->Q_LV_id][count+2] > small_number) {
        systole_start = count;
	//std::cout << "Found systole start" << std::endl;
        break;
     }
  }

  // FIND WHEN AORTIC VALVE OPENS
  int ao_open = systole_start;

  // FIND WHEN MITRAL VALVE OPENS    
  int mit_open = totOutputSteps - totalStepsOnSingleCycle - 1;
  for(int count = totOutputSteps - totalStepsOnSingleCycle - 1; count < totOutputSteps-1; count++) {
     if(outVals[this->Q_LA_id][count] < small_number && outVals[this->Q_LA_id][count+1] > small_number && outVals[this->Q_LA_id][count+2] > small_number) {
        mit_open = count;
        break;
     }
  }

  // COUNT PERCENTAGE OF
  // Find midpoint of duration when these valves are open 
  int mit_half = round((mit_open+totOutputSteps)/(double)2.0);
  int aor_half = round((ao_open+systole_end)/(double)2.0);
    
  // CALCULATE MAX AND TOTAL CORONARY FLOW DURING SYSTOLE
  double l_cor_qmax_s = cmUtils::getMax(systole_start, systole_end,outVals[this->Q_lca_ids[lca_main_id]]);
  double l_cor_qtot_s = cmUtils::trapz(systole_start, systole_end,t,outVals[this->Q_lca_ids[lca_main_id]]);
  double r_cor_qmax_s = cmUtils::getMax(systole_start, systole_end,outVals[this->Q_rca_ids[rca_main_id]]);
  double r_cor_qtot_s = cmUtils::trapz(systole_start, systole_end,t,outVals[this->Q_rca_ids[rca_main_id]]);
    
  // CALCULATE MAX AND TOTAL CORONARY FLOW DURING DIASTOLE
  int sys_buffer = totalStepsOnSingleCycle/10;
  double l_cor_qmax_d = max(cmUtils::getMax(systole_end + sys_buffer, totOutputSteps,outVals[this->Q_lca_ids[lca_main_id]]),cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_start,outVals[this->Q_lca_ids[lca_main_id]]));
  double l_cor_qtot_d = cmUtils::trapz(systole_end, totOutputSteps,t,outVals[this->Q_lca_ids[lca_main_id]]) + cmUtils::trapz(totOutputSteps - totalStepsOnSingleCycle - 1, systole_start,t,outVals[this->Q_lca_ids[lca_main_id]]);  
  double r_cor_qmax_d = max(cmUtils::getMax(systole_end + sys_buffer, totOutputSteps,outVals[this->Q_rca_ids[rca_main_id]]),cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_start,outVals[this->Q_rca_ids[rca_main_id]]));
  double r_cor_qtot_d = cmUtils::trapz(systole_end, totOutputSteps,t,outVals[this->Q_rca_ids[rca_main_id]]) + cmUtils::trapz(totOutputSteps - totalStepsOnSingleCycle - 1, systole_start,t,outVals[this->Q_rca_ids[rca_main_id]]);

  // FIND HOW MANY PEAKS AND VALLEYS IN THE CORONARY WAVEFORMS
  int l_grad_check[5];
  int r_grad_check[5];
  for(int i = 0; i < 5; i++)
  {
    l_grad_check[i] = -1;
    r_grad_check[i] = -1;
  }
  double l_last = (outVals[this->Q_lca_ids[lca_main_id]][totOutputSteps - totalStepsOnSingleCycle - 1]
    - outVals[this->Q_lca_ids[lca_main_id]][totOutputSteps - totalStepsOnSingleCycle - 2])/
    (t[1] - t[0]);
  double r_last = (outVals[this->Q_rca_ids[rca_main_id]][totOutputSteps - totalStepsOnSingleCycle - 1]
    - outVals[this->Q_rca_ids[rca_main_id]][totOutputSteps - totalStepsOnSingleCycle - 2])/
    (t[1] - t[0]);
  double l_grad, r_grad;
  for(int i = totOutputSteps - totalStepsOnSingleCycle; i < totOutputSteps; i++)
  {
    l_grad = (outVals[this->Q_lca_ids[lca_main_id]][i] - outVals[this->Q_lca_ids[lca_main_id]][i - 1])/(t[1] - t[0]);
    r_grad = (outVals[this->Q_rca_ids[rca_main_id]][i] - outVals[this->Q_rca_ids[rca_main_id]][i - 1])/(t[1] - t[0]);

    // Checking the gradients on the left side
    if(l_grad > 0 && l_last <= 0 && l_grad_check[0] == -1) //valley
    {
      l_grad_check[0] = i - (numCycles - 1)*totalStepsOnSingleCycle;
      if(l_grad_check[1] < 0)
      {
        l_grad_check[4] = 1;  //Starts with a valley?
      }
    }
    else if(l_grad < 0 && l_last >= 0 && l_grad_check[1] == -1) //peak
    {
      l_grad_check[1] = i - (numCycles - 1)*totalStepsOnSingleCycle;
    }
    else if(l_grad > 0 && l_last <= 0 && l_grad_check[2] == -1 && l_grad_check[0] > 0) //valley
    {
      l_grad_check[2] = i - (numCycles - 1)*totalStepsOnSingleCycle;
    }
    else if(l_grad < 0 && l_last >= 0 && l_grad_check[3] == -1 && l_grad_check[1] > 0
      && (i - (numCycles - 1)*totalStepsOnSingleCycle - l_grad_check[2]) > totalStepsOnSingleCycle/10) //peak
    {
      l_grad_check[3] = i - (numCycles - 1)*totalStepsOnSingleCycle;
    }

    // Check the gradients on the right side
    if(r_grad > 0 && r_last <= 0 && r_grad_check[0] == -1)
    {
      r_grad_check[0] = i - (numCycles - 1)*totalStepsOnSingleCycle;
      if(r_grad_check[1] < 0)
      {
        r_grad_check[4] = 1;
      }
    }
    else if(r_grad < 0 && r_last >= 0 && r_grad_check[1] == -1)
    {
      r_grad_check[1] = i - (numCycles - 1)*totalStepsOnSingleCycle;
    }
    else if(r_grad > 0 && r_last <= 0 && r_grad_check[2] == -1 && r_grad_check[0] > 0)
    {
      r_grad_check[2] = i - (numCycles - 1)*totalStepsOnSingleCycle;
    }
    else if(r_grad < 0 && r_last >= 0 && r_grad_check[3] == -1 && r_grad_check[1] > 0
      && (i - (numCycles - 1)*totalStepsOnSingleCycle - r_grad_check[2]) > totalStepsOnSingleCycle/10)
    {
      r_grad_check[3] = i - (numCycles - 1)*totalStepsOnSingleCycle;
    }

    // Setting the last variables for next timestep
    l_last = l_grad;
    r_last = r_grad;
  }

  // Tally up the good scores
  double l_grad_ok = 0.0;
  double r_grad_ok = 0.0;
  for(int i = 0; i < 5; i++)
  {
    if(l_grad_check[i] > 0)
    {
      l_grad_ok = l_grad_ok + 1.0;
    }
    if(r_grad_check[i] > 0)
    {
      r_grad_ok = r_grad_ok + 1.0;
    }
  }
    
  // CALCULATE RATIOS (DIASTOLE TO SYSTOLE)
  double r_cor_max_ratio = r_cor_qmax_d/r_cor_qmax_s;
  double r_cor_tot_ratio = r_cor_qtot_d/r_cor_qtot_s;
  double l_cor_max_ratio = l_cor_qmax_d/l_cor_qmax_s;
  double l_cor_tot_ratio = l_cor_qtot_d/l_cor_qtot_s;

  // CALCULATE 1/3 FF AND 1/2 FF
  double r_third_FF, r_half_FF, l_third_FF, l_half_FF;
  int thirdcyc = round(totalStepsOnSingleCycle/3);
  int halfcyc = round(totalStepsOnSingleCycle/2);
  if(systole_end+thirdcyc-1 < totOutputSteps) {
     r_third_FF = cmUtils::trapz(systole_end-1, systole_end+thirdcyc,t,outVals[this->Q_rca_ids[rca_main_id]])/rmain_flow;
     l_third_FF = cmUtils::trapz(systole_end-1, systole_end+thirdcyc,t,outVals[this->Q_lca_ids[lca_main_id]])/lmain_flow;
  } else {
     r_third_FF = 0;
     l_third_FF = 0;
  }

  if(systole_end+halfcyc-1 < totOutputSteps) {
     r_half_FF = cmUtils::trapz(systole_end-1, systole_end+halfcyc,t,outVals[this->Q_rca_ids[rca_main_id]])/rmain_flow;
     l_half_FF = cmUtils::trapz(systole_end-1, systole_end+halfcyc,t,outVals[this->Q_lca_ids[lca_main_id]])/lmain_flow;
  } else {
     r_half_FF = 0;
     l_half_FF = 0;
  }

  // COMPUTE QUANTITIES
  double Qinlet = cmUtils::trapz(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps,t,outVals[Q_aorta_id]);
  double Qsystole = cmUtils::trapz(ao_open, systole_end, t, outVals[Q_aorta_id]);
  double systole_perc = Qsystole/Qinlet;
  double Aor_Cor_split = ((Q_lcor+Q_rcor)/(Q_lcor+Q_rcor+Q_rcr))*100.0;
  double Pao_max = cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_aorta_id]);
  double Pao_min = cmUtils::getMin(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_aorta_id]);
  double Pao_mean = cmUtils::getMean(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_aorta_id]);
  double Ppul_max = cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_pul_id]);
  double Ppul_min = cmUtils::getMin(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_pul_id]);
  double Ppul_mean = cmUtils::getMean(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_pul_id]);
  double ESV = cmUtils::getMin(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[V_LV_id]);
  double EDV = cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[V_LV_id]);
  double EF_LV = (EDV - ESV)/EDV;
  double Prv_Pra = cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end, outVals[P_RV_id]) - cmUtils::getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end, outVals[P_RA_id]);
  double Ppul_Prv = cmUtils::getMax(systole_end-1, systole_start, outVals[P_RV_id]) - cmUtils::getMax(systole_end-1, systole_start, outVals[P_pul_id]);
  double mit_valve[totalStepsOnSingleCycle] = {0};
  double ao_valve[totalStepsOnSingleCycle] = {0};
  double pul_valve[totalStepsOnSingleCycle] = {0};
  int step_ct = 0;
  for(auto step = totOutputSteps-totalStepsOnSingleCycle-1; step < totOutputSteps-1; step++) {
      if(outVals[this->Q_LV_id][step] > small_number && outVals[this->Q_LV_id][step+1] > small_number) {
	  ao_valve[step_ct] = 1.0;
      }
      if(outVals[this->Q_LA_id][step] > small_number && outVals[this->Q_LA_id][step+1] > small_number) {
	  mit_valve[step_ct] = 1.0;
      }
      if(outVals[this->Q_RV_id][step] > small_number && outVals[this->Q_RV_id][step+1] > small_number) {
	  pul_valve[step_ct] = 1.0;
      }
      step_ct++;
  }
  double mit_valve_time = cmUtils::getSum(0, totalStepsOnSingleCycle, mit_valve)/(double)totalStepsOnSingleCycle;
  double aor_valve_time = cmUtils::getSum(0, totalStepsOnSingleCycle, ao_valve)/(double)totalStepsOnSingleCycle;
  double pul_valve_time = cmUtils::getSum(0, totalStepsOnSingleCycle, pul_valve)/(double)totalStepsOnSingleCycle;
  double Qla_ratio = cmUtils::getMax(mit_open-1,mit_half,outVals[Q_LA_id])/cmUtils::getMax(mit_half-1,totOutputSteps,outVals[Q_LA_id]);
  double Pra_mean = cmUtils::getMean(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[P_RA_id]);
  double LR_split = (Q_lcor/(Q_lcor + Q_rcor))*100;
  if(r_cor_max_ratio < 0 || l_cor_max_ratio < 0 || r_cor_tot_ratio < 0 || l_cor_tot_ratio < 0) {
      r_cor_max_ratio = 9001.0;
  }

  // COMPUTE CONVERGENCE QUANTITIES (for past 2 cycles)
  double Qinlet1 = cmUtils::trapz(totOutputSteps - 2*totalStepsOnSingleCycle - 1, totOutputSteps - totalStepsOnSingleCycle,t,outVals[Q_aorta_id]);
  double Qinlet_diff = fabs(Qinlet - Qinlet1);
  double Pao_max1 = cmUtils::getMax(totOutputSteps - 2*totalStepsOnSingleCycle - 1, totOutputSteps - totalStepsOnSingleCycle, outVals[P_aorta_id]);
  double Pao_max_diff = fabs(Pao_max1 - Pao_max);
  double Pao_min1 = cmUtils::getMin(totOutputSteps - 2*totalStepsOnSingleCycle - 1, totOutputSteps - totalStepsOnSingleCycle, outVals[P_aorta_id]);
  double Pao_min_diff = fabs(Pao_min1 - Pao_min);
  double Pao_mean1 = cmUtils::getMean(totOutputSteps - 2*totalStepsOnSingleCycle - 1, totOutputSteps - totalStepsOnSingleCycle, outVals[P_aorta_id]);
  double Pao_mean_diff = fabs(Pao_mean1 - Pao_mean);
  
  // WRITE RESULT
  results[0] = Pao_min;
  results[1] = Pao_min_diff;
  results[2] = Pao_max;
  results[3] = Pao_max_diff;
  results[4] = Pao_mean;
  results[5] = Pao_mean_diff;
  results[6] = Aor_Cor_split;
  results[7] = fabs(Qinlet);
  results[8] = Qinlet_diff;
  results[9] = systole_perc;
  results[10] = Ppul_mean;
  results[11] = EF_LV;
  results[12] = ESV;
  results[13] = EDV;
  results[14] = Qla_ratio;
  results[15] = mit_valve_time;
  results[16] = aor_valve_time;
  results[17] = pul_valve_time;
  results[18] = Pra_mean;
  results[19] = l_cor_max_ratio;
  results[20] = l_cor_tot_ratio;
  results[21] = l_third_FF;
  results[22] = l_half_FF;
  results[23] = l_grad_ok;
  results[24] = r_cor_max_ratio;
  results[25] = r_cor_tot_ratio;
  results[26] = r_third_FF;
  results[27] = r_half_FF;
  results[28] = r_grad_ok;

  // Check for nans
  for (int i = 0; i < this->getResultTotal(); i++) {
    if (isnan(results[i]) || isinf(results[i])) {
      const std::string error_msg = "ERROR: nan/inf in result "+this->getResultName(i)+" with index "+std::to_string(i);
      std::cout<<error_msg<<std::endl;
      results[i] = 10000.0;
      //throw std::runtime_error(error_msg);
    }
  }

}

// =========================
// KEY/NAME FOR EACH TARGET QUANTITY
// =========================
void svZeroD_ClosedLoopCoronary_GSI::getResultKeys(vector<string>& keys) {
  keys.push_back(string("PaoMin"));
  keys.push_back(string("PaoMin_conv"));
  keys.push_back(string("PaoMax"));
  keys.push_back(string("PaoMax_conv"));
  keys.push_back(string("PaoMean"));
  keys.push_back(string("PaoMean_conv"));
  keys.push_back(string("AorCorSplit"));
  keys.push_back(string("AbsQin"));
  keys.push_back(string("AbsQin_conv"));
  keys.push_back(string("Qsystole_perc"));
  keys.push_back(string("PpulMean"));
  keys.push_back(string("EFLV"));
  keys.push_back(string("ESV"));
  keys.push_back(string("EDV"));
  keys.push_back(string("QlaRatio"));
  keys.push_back(string("mitValveTime"));
  keys.push_back(string("aorValveTime"));
  keys.push_back(string("pulValveTime"));
  keys.push_back(string("PraMean"));
  keys.push_back(string("LCorMaxRatio"));
  keys.push_back(string("LCorTotRatio"));
  keys.push_back(string("LThirdFF"));
  keys.push_back(string("LHalfFF"));
  keys.push_back(string("LGradOK"));
  keys.push_back(string("RCorMaxRatio"));
  keys.push_back(string("RCorTotRatio"));
  keys.push_back(string("RThirdFF"));
  keys.push_back(string("RHalfFF"));
  keys.push_back(string("RGradOK"));
}

// =========================
// STANDARD DEVIATION OF EACH TARGET MEASUREMENT
// =========================
void svZeroD_ClosedLoopCoronary_GSI::getDataStd(stdVec& stdFactors) {
  stdFactors.push_back(8.1); // PaoMin
  stdFactors.push_back(8.1); // PaoMin_diff
  stdFactors.push_back(12.6); // PaoMax
  stdFactors.push_back(12.6); // PaoMax_diff
  stdFactors.push_back(9.6); // PaoMean
  stdFactors.push_back(9.6); // PaoMean_diff
  stdFactors.push_back(0.4); // AorCorSplit
  stdFactors.push_back(9.07); // AbsQin
  stdFactors.push_back(9.07); // AbsQin_diff
  stdFactors.push_back(0.5); // Qsystole_perc
  stdFactors.push_back(3.3); // PpulMean
  stdFactors.push_back(0.065); // EFLV
  stdFactors.push_back(4.0); // ESV
  stdFactors.push_back(10.0); // EDV
  stdFactors.push_back(0.236); // QlaRatio
  stdFactors.push_back(0.084); // mitValveTime
  stdFactors.push_back(0.051); // aorValveTime
  stdFactors.push_back(0.051); // pulValveTime
  stdFactors.push_back(1.2); // PraMean
  stdFactors.push_back(0.8); // LCorMaxRatio
  stdFactors.push_back(2.5337); // LCorTotRatio
  stdFactors.push_back(0.02); // LThirdFF
  stdFactors.push_back(0.03); // LHalfFF
  stdFactors.push_back(1.00); // LGradOK
  stdFactors.push_back(0.3); // RCorMaxRatio
  stdFactors.push_back(1.0816); // RCorTotRatio
  stdFactors.push_back(0.07); // RThirdFF
  stdFactors.push_back(0.07); // RHalfFF
  stdFactors.push_back(1.00); // RGradOK
}

// =========================
// INVERSE WEIGHT OF EACH TARGET QUANTITY IN LOG LIKELIHOOD
// =========================
void svZeroD_ClosedLoopCoronary_GSI::getResultWeights(stdVec& weights) {
  weights.push_back(0.25);  // PaoMin
  weights.push_back(0.25);  // PaoMin_diff
  weights.push_back(0.25);  // PaoMax
  weights.push_back(0.25);  // PaoMax_diff
  weights.push_back(1.0); // PaoMean
  weights.push_back(0.25); // PaoMean_diff
  weights.push_back(1.0);  // AorCorSplit
  weights.push_back(0.5);  // AbsQin
  weights.push_back(0.25);  // AbsQin_diff
  if(this->useRigidSurrogate)
    weights.push_back(999999999.0);  // Qsystole_perc
  else
    weights.push_back(1.0);
  weights.push_back(2.0); // PpulMean
  weights.push_back(1.0);  // EFLV
  weights.push_back(0.5); // ESV
  weights.push_back(0.5); // EDV
  weights.push_back(2.0);  // QlaRatio
  weights.push_back(2.0);  // mitValveTime
  weights.push_back(2.0);  // aorValveTime
  weights.push_back(2.0);  // pulValveTime
  weights.push_back(1.0); // PraMean
  weights.push_back(0.5); // LCorMaxRatio
  weights.push_back(0.5); // LCorTotRatio
  weights.push_back(1.0); // LThirdFF
  weights.push_back(1.0); // LHalfFF
  weights.push_back(1.0); // LGradOK
  weights.push_back(0.5); // RCorMaxRatio
  weights.push_back(0.5); // RCorTotRatio
  weights.push_back(1.0); // RThirdFF
  weights.push_back(1.0); // RHalfFF
  weights.push_back(1.0); // RGradOK
}

//// ====================
//// RETURN PARAMETER SPECIFIED BY STRING SPECIFIER
//// ====================
void svZeroD_ClosedLoopCoronary_GSI::getSpecifiedParameter(string& specifier, double& return_db_param, int& return_int_param) {
  std::cout<<"ERROR: svZeroD_ClosedLoopCoronary_GSI::getSpecifiedParameter not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  throw std::runtime_error("ERROR: svZeroD_ClosedLoopCoronary_GSI::getSpecifiedParameter not implemented.");
}

//// ====================
//// CUSTOM ERROR EVALUATION
//// ====================
double svZeroD_ClosedLoopCoronary_GSI::evalModelError(std::vector<double>& results) {
  std::cout<<"ERROR: svZeroD_ClosedLoopCoronary_GSI::evalModelError not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  throw std::runtime_error("ERROR: svZeroD_ClosedLoopCoronary_GSI::evalModelError not implemented.");
  return 0.0;
}
//// ====================
//// CUSTOM DATA
//// ====================
daData* svZeroD_ClosedLoopCoronary_GSI::createCustomData() {
  std::cout<<"ERROR: svZeroD_ClosedLoopCoronary_GSI::createCustomData not implemented."<<std::endl;
  std::cout<<"Execution should be terminated but might not if this is in a try-catch block."<<std::endl;
  throw std::runtime_error("ERROR: svZeroD_ClosedLoopCoronary_GSI::createCustomData not implemented.");
  return NULL;
}
