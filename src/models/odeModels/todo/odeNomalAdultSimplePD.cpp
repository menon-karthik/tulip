# include "odeNormalAdultSimplePD.h"

// Constants for Variable and Auxiliary outputs
const int ipAuxRAPressure = 5;
const int ipAuxRVPressure = 7;
const int ipAuxLVPressure = 8;
const int ipPAAPressure = 5;
const int ipPAVPressure = 6;
const int ipAOPressure = 9;
const int ipRVVolume = 2;
const int ipLVVolume = 3;
const int ipAOFlow = 10;

// Constants for Result Quantities
// Results v-0001
const int ip_0001_HR   = 0;
const int ip_0001_SBP  = 1;
const int ip_0001_DBP  = 2;
const int ip_0001_mBP  = 3;
const int ip_0001_RAP  = 4;
const int ip_0001_sPAP = 5;
const int ip_0001_dPAP = 6;
const int ip_0001_mPAP = 7;
const int ip_0001_PWP  = 8;
const int ip_0001_CO   = 9;
const int ip_0001_PVR  = 10;
const int ip_0001_SVR  = 11;
const int ip_0001_CVP  = 12;
const int ip_0001_LVEF = 13;
const int ip_0001_RVEF = 14;

// Results v-0002
const int ip_0002_heart_rate2                = 0;
const int ip_0002_systolic_bp_2              = 1;
const int ip_0002_diastolic_bp_2             = 2;
const int ip_0002_cardiac_output             = 3;
const int ip_0002_systemic_vascular_resistan = 4;
const int ip_0002_pulmonary_vascular_resista = 5;
const int ip_0002_cvp                        = 6;
const int ip_0002_right_ventricle_diastole   = 7;
const int ip_0002_right_ventricle_systole    = 8;
const int ip_0002_rvedp                      = 9;
const int ip_0002_aov_mean_pg                = 10;
const int ip_0002_aov_peak_pg                = 11;
const int ip_0002_mv_decel_time              = 12;
const int ip_0002_mv_e_a_ratio               = 13;
const int ip_0002_pv_at                      = 14;
const int ip_0002_pv_max_pg                  = 15;
const int ip_0002_ra_pressure                = 16;
const int ip_0002_ra_vol_a4c                 = 17;
const int ip_0002_la_vol_a4c                 = 18;
const int ip_0002_lv_esv                     = 19;
const int ip_0002_lv_vol                     = 20;
const int ip_0002_lvef                       = 21;
const int ip_0002_lvot_max_flow              = 22;
const int ip_0002_pap_diastolic              = 23;
const int ip_0002_pap_systolic               = 24;
const int ip_0002_wedge_pressure             = 25;

using namespace std;

odeNormalAdultSimplePD::odeNormalAdultSimplePD(int cycles, int stepsPerCycle){
  this->data = NULL;
  this->numCycles = cycles;
  this->totalStepsOnSingleCycle = stepsPerCycle;
}

int odeNormalAdultSimplePD::getParameterTotal(){
  return 49;
}

int odeNormalAdultSimplePD::getStateTotal(){
  return 12; 
}

int odeNormalAdultSimplePD::getAuxStateTotal(){
  return 18; 	
}

int odeNormalAdultSimplePD::getResultTotal(){
  return 15;  
}

string odeNormalAdultSimplePD::getParamName(int index){
  switch(index){
    case 0:
      return string("V_ra_ini");
      break;
    case 1:
      return string("V_la_ini");
      break;
    case 2:
      return string("V_rv_ini");
      break;
    case 3:
      return string("V_lv_ini");
      break;
    case 4:
      return string("Q_ra_rv_ini");
      break;
    case 5:
      return string("P_pa_a_ini");
      break;
    case 6:
      return string("P_pa_v_ini");
      break;
    case 7:
      return string("Q_rv_pa_ini");
      break;
    case 8:
      return string("Q_la_lv_ini");
      break;
    case 9:
      return string("P_ao_ini");
      break;
    case 10:
      return string("Q_lv_ao_ini");
      break;
    case 11:
      return string("P_sys_ini");
      break;
    case 12:
      return string("HR");
      break;
    case 13:
      return string("tsas");
      break;
    case 14:
      return string("tpws");
      break;
    case 15:
      return string("tsvs");
      break;
    case 16:
      return string("K_pas_ra_1");
      break;
    case 17:
      return string("K_pas_ra_2");
      break;
    case 18:
      return string("Emax_ra");
      break;
    case 19:
      return string("Vra0");
      break;
    case 20:
      return string("K_pas_la_1");
      break;
    case 21:
      return string("K_pas_la_2");
      break;
    case 22:
      return string("Emax_la");
      break;
    case 23:
      return string("Vla0");
      break;
    case 24:
      return string("K_pas_rv_1");
      break;
    case 25:
      return string("K_pas_rv_2");
      break;
    case 26:
      return string("Emax_rv");
      break;
    case 27:
      return string("Vrv0");
      break;
    case 28:
      return string("K_pas_lv_1");
      break;
    case 29:
      return string("K_pas_lv_2");
      break;
    case 30:
      return string("Emax_lv");
      break;
    case 31:
      return string("Vlv0");
      break;
    case 32:
      return string("L_ra_rv");
      break;
    case 33:
      return string("R_ra_rv");
      break;
    case 34:
      return string("L_rv_pa");
      break;
    case 35:
      return string("R_rv_pa");
      break;
    case 36:
      return string("L_la_lv");
      break;
    case 37:
      return string("R_la_lv");
      break;
    case 38:
      return string("L_lv_ao");
      break;
    case 39:
      return string("R_lv_ao");
      break;
    case 40:
      return string("C_ao");
      break;
    case 41:
      return string("C_pa_a");
      break;
    case 42:
      return string("C_pa_v");
      break;
    case 43:
      return string("R_pa_a");
      break;
    case 44:
      return string("R_pa_c");
      break;
    case 45:
      return string("R_pa_v");
      break;
    case 46:
      return string("C_sys");
      break;
    case 47:
      return string("R_sys_a");
      break;
    case 48:
      return string("R_sys_v");
      break;
  }
}

string odeNormalAdultSimplePD::getResultName(int index){
  switch(index){
    case 0:
      return string("HR");
      break;
    case 1:
      return string("SBP");
      break;
    case 2:
      return string("DBP");
      break;
    case 3:
      return string("mBP");
      break;
    case 4:
      return string("RAP");
      break;
    case 5:
      return string("sPAP");
      break;
    case 6:
      return string("dPAP");
      break;
    case 7:
      return string("mPAP");
      break;
    case 8:
      return string("PWP");
      break;
    case 9:
      return string("CO");
      break;
    case 10:
      return string("PVR");
      break;
    case 11:
      return string("SVR");
      break;
    case 12:
      return string("CVP");
      break;
    case 13:
      return string("LVEF");
      break;
    case 14:
      return string("RVEF");
      break;
  }
}

void odeNormalAdultSimplePD::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("Not Implemented!");
}

void odeNormalAdultSimplePD::getDefaultParams(stdVec& params){

  // Resize Parameter Array
  params.resize(getParameterTotal());

  // NOTE: CGS Units: Pressures in Barye, Flowrates in mL/s

  // --- Initial State Values
  params[0] = 0.0; // V_ra
  params[1] = 0.0; // V_la
  params[2] = 0.0; // V_rv
  params[3] = 0.0; // V_lv
  params[4] = 0.0; // Q_ra_rv
  params[5] = 70.0 * convertmmHgToBarye; // P_pa_a
  params[6] = 70.0 * convertmmHgToBarye; // P_pa_v
  params[7] = 0.0; // Q_rv_pa
  params[8] = 0.0; // Q_la_lv
  params[9] = 100.0 * convertmmHgToBarye; // P_ao
  params[10] = 0.0; // Q_lv_ao
  params[11] = 0.0; // P_sys

  // --- Heart Cycle Parameters
  params[12+0] = 78.0; // HR - Heart Rate (beats per minute)

  // --- Atrial and ventricular activation duration
  params[12+1] = 0.2; // tsas - Atrial relative activation duration
  params[12+2] = 9.5; // tpws - Atrial relative activation time shift
  params[12+3] = 0.4; // tsvs - Ventricular relative activation duration

  // --- Atrial model parameters
  params[12+4] = 5.0; // K_pas_ra_1 - Atrial passive curve slope, right atrium
  params[12+5] = 0.006; // K_pas_ra_2 - Atrial passive curve exponent factor, right atrium
  params[12+6] = 0.1; // Emax_ra - Atrial active curve slope, right atrium
  params[12+7] = 0.0; // Vra0 - Unstressed right atrial volume
  params[12+8] = 5.0; // K_pas_la_1 - Atrial passive curve slope, left atrium
  params[12+9] = 0.0065; // K_pas_la_2 - Atrial passive curve exponent factor, left atrium
  params[12+10] = 0.2; // Emax_la - Atrial active curve slope, left atrium
  params[12+11] = 0.0; // Vla0 - Unstressed left atrial volume

  // --- Ventricular Model Parameters
  params[12+12] = 5.0; // K_pas_rv_1 - Ventricular passive curve slope, right atrium
  params[12+13] = 0.003; // K_pas_rv_2 - Ventricular passive curve exponent factor, right atrium
  params[12+14] = 0.5; // Emax_rv - Ventricular active curve slope, right atrium
  params[12+15] = 0.0; // Vrv0 - Unstressed right atrial volume
  params[12+16] = 2.0; // K_pas_lv_1 - Ventricular passive curve slope, left atrium
  params[12+17] = 0.003; // K_pas_lv_2 - Ventricular passive curve exponent factor, left atrium
  params[12+18] = 4.0; // Emax_lv - Ventricular active curve slope, left atrium
  params[12+19] = 20.0; // Vlv0 - Unstressed left atrial volume

  // --- Atrial and Ventricular Inductances and Resistances
  params[12+20] = 0.1;   // L_ra_rv - Inductance of right atrium
  params[12+21] = 10.0;  // R_ra_rv - Resistance of right atrium
  params[12+22] = 0.1;  // L_rv_pa - Inductance of right ventricle
  params[12+23] = 15.0;  // R_rv_pa - Resistance of right ventricle
  params[12+24] = 0.1;   // L_la_lv - Inductance of left atrium
  params[12+25] = 20.0;  // R_la_lv - Resistance of left atrium
  params[12+26] = 0.1;   // L_lv_ao - Inductance of left ventricle
  params[12+27] = 30.0; // R_lv_ao - Resistance of left ventricle

  // --- Aortic Arch
  params[12+28] = 1000.0e-6; // C_ao - Aortic capacitance

  // --- Pulmonary Resistance and Capacitance
  params[12+29] = 2000.0e-6; // C_pa_a - Pulmonary capacitance 1
  params[12+30] = 2000.0e-6; // C_pa_v - Pulmonary capacitance 2

  params[12+31] = 50.0; // R_pa_a - Pulmonary resistance 1
  params[12+32] = 50.0; // R_pa_c - Pulmonary resistance 2
  params[12+33] = 50.0; // R_pa_v - Pulmonary resistance 3

  // --- Systemic Resistance and Capacitance
  params[12+34] = 400.0e-6; // C_sys - Systemic Capacitance
  params[12+35] = 400.0; // R_sys_a - Systemic Resistance - Arteries
  params[12+36] = 1200.0; // R_sys_v - Systemic Resistance - Veins
}

void odeNormalAdultSimplePD::getDefaultParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());
  
  // Set Limits for initial conditions
  limits[0] = 0.0; limits[1] = 0.0; // V_ra
  limits[2] = 0.0; limits[3] = 0.0; // V_la
  limits[4] = 0.0; limits[5] = 0.0; // V_rv
  limits[6] = 0.0; limits[7] = 0.0; // V_lv
  limits[8] = 0.0; limits[9] = 0.0; // Q_ra_rv
  limits[10] = 15.0 * convertmmHgToBarye; limits[11] = 25.0 * convertmmHgToBarye; // P_pa_a
  limits[12] = 15.0 * convertmmHgToBarye; limits[13] = 25.0 * convertmmHgToBarye; // P_pa_v
  limits[14] = 0.0; limits[15] = 0.0; // Q_rv_pa
  limits[16] = 0.0; limits[17] = 0.0; // Q_la_lv
  limits[18] = 80.0 * convertmmHgToBarye; limits[19] = 150.0 * convertmmHgToBarye; // P_ao
  limits[20] = 0.0; limits[21] = 0.0; // Q_lv_ao
  limits[22] = 60.0 * convertmmHgToBarye; limits[23] = 120.0 * convertmmHgToBarye; // P_sys

  // --- Heart Cycle Parameters
  limits[24] = 40.0; limits[25] = 100.0;  // HR - Heart Rate

  // --- Atrial and ventricular activation duration and shift
  limits[26] = 0.3; limits[27] = 0.3; // tsas - Atrial relative activation duration
  limits[28] = 9.5; limits[29] = 9.5; // tpws - Atrial relative activation time shift
  limits[30] = 0.2; limits[31] = 0.4; // tsvs - Ventricular relative activation duration

  // --- Atrial Model Parameters
  limits[32] = 0.1; limits[33] = 10.0;   // K_pas_ra_1 - Atrial passive curve slope, right atrium
  limits[34] = 0.0001; limits[35] = 0.06;   // K_pas_ra_2 - Atrial passive curve exponent factor, right atrium
  limits[36] = 1.0; limits[37] = 5.0; // Emax_ra - Atrial active curve slope, right atrium
  limits[38] = 0.0; limits[39] = 50.0;     // Vra0 - Unstressed right atrial volume
  limits[40] = 0.1; limits[41] = 10.0;      // K_pas_la_1 - Atrial passive curve slope, left atrium
  limits[42] = 0.0001; limits[43] = 0.06;   // K_pas_la_2 - Atrial passive curve exponent factor, left atrium
  limits[44] = 1.0; limits[45] = 5.0;  // Emax_la - Atrial active curve slope, left atrium
  limits[46] = 0.0; limits[47] = 50.0;  // Vla0 - Unstressed left atrial volume

  // --- Ventricular Model Parameters
  limits[48] = 0.1; limits[49] = 20.0;  // K_pas_rv_1 - Ventricular passive curve slope, right ventricle
  limits[50] = 0.0001; limits[51] = 0.01;  // K_pas_rv_2 - Ventricular passive curve exponent factor, right ventricle
  limits[52] = 1.0; limits[53] = 5.0;  // Emax_rv - Ventricular active curve slope, right ventricle
  limits[54] = 0.0; limits[55] = 50.0;  // Vrv0 - Unstressed right atrial volume
  limits[56] = 0.1; limits[57] = 20.0;  // K_pas_lv_1 - Ventricular passive curve slope, left ventricle
  limits[58] = 0.0001; limits[59] = 0.01;  // K_pas_lv_2 - Ventricular passive curve exponent factor, left ventricle
  limits[60] = 1.0; limits[61] = 5.0;  // Emax_lv - Ventricular active curve slope, left ventricle
  limits[62] = 0.0; limits[63] = 5.0;  // Vlv0 - Unstressed left atrial volume

  // --- Atrial and Ventricular Inductances and Resistances
  limits[64] = 0.1; limits[65] = 0.1;  // L_ra_rv - Inductance of right atrium
  limits[66] = 5.0; limits[67] = 20.0; // R_ra_rv - Resistance of right atrium
  limits[68] = 0.1; limits[69] = 0.1;  // L_rv_pa - Inductance of right ventricle
  limits[70] = 15.0; limits[71] = 35.0; // R_rv_pa - Resistance of right ventricle
  limits[72] = 0.1; limits[73] = 0.1;  // L_la_lv - Inductance of left atrium
  limits[74] = 25.0; limits[75] = 45.0; // R_la_lv - Resistance of left atrium
  limits[76] = 0.1; limits[77] = 0.1;  // L_lv_ao - Inductance of left ventricle
  limits[78] = 45.0; limits[79] = 65.0; // R_lv_ao - Resistance of left ventricle

  // --- Aortic Arch
  limits[80] = 1.0e-5; limits[81] = 0.001; // C_ao - Aortic capacitance

  // --- Pulmonary Resistance and Capacitance
  limits[82] = 100.0e-6; limits[83] = 0.01; // C_pa_a - Pulmonary capacitance 1
  limits[84] = 100.0e-6; limits[85] = 0.01; // C_pa_v - Pulmonary capacitance 2
  limits[86] = 1.0; limits[87] = 1000.0;  // R_pa_a - Pulmonary resistance 1
  limits[88] = 1.0; limits[89] = 1000.0;  // R_pa_c - Pulmonary resistance 2
  limits[90] = 1.0; limits[91] = 1000.0;  // R_pa_v - Pulmonary resistance 3

  // --- Systemic Resistance and Capacitance
  limits[92] = 100.0e-6; limits[93] = 0.05; // C_sys - Systemic capacitance
  limits[94] = 100.0; limits[95] = 800.0; // R_sys_a - Systemic Resistance - Arteries
  limits[96] = 500.0; limits[97] = 2500.0; // R_sys_v - Systemic Resistance - Veins

}

void odeNormalAdultSimplePD::getParameterLimits(stdVec& limits){
  // Get Default Parameters
  getDefaultParameterLimits(limits);
  // Change the limits bases on the Fixed Parameter List
  int currParam = 0;
  for(size_t loopA=0;loopA<frozenParamIDX.size();loopA++){
    currParam = frozenParamIDX[loopA];
    // Assign the new lower and upper bounds to the center
    limits[currParam*2]     = frozenParamVAL[loopA];
    limits[currParam*2 + 1] = frozenParamVAL[loopA];
  }
}

void odeNormalAdultSimplePD::lpnODE(double tn, stdVec Xn, stdVec params, stdVec& Xn1, stdVec& out, stdVec& Ind){

  // Assign the Parameters
  double HR         = params[0]; // Heart Rate
  // Atrial and ventricular duration and shift
  double tsas       = params[1]; // Atrial relative activation duration
  double tpws       = params[2]; // Atrial relative activation time shift
  double tsvs       = params[3]; // Ventricular relative activation duration
  // Atrial Model Parameters
  double K_pas_ra_1 = params[4];
  double K_pas_ra_2 = params[5];
  double Emax_ra    = params[6];
  double Vra0       = params[7];
  double K_pas_la_1 = params[8];
  double K_pas_la_2 = params[9];
  double Emax_la    = params[10];
  double Vla0       = params[11];
  // Ventricular Model Parameters
  double K_pas_rv_1 = params[12];
  double K_pas_rv_2 = params[13];
  double Emax_rv    = params[14];
  double Vrv0       = params[15];  
  double K_pas_lv_1 = params[16];
  double K_pas_lv_2 = params[17];
  double Emax_lv    = params[18];
  double Vlv0       = params[19];
  // Atrial and Ventricular Inductances and Resistances
  double L_ra_rv    = params[20];
  double R_ra_rv    = params[21];
  double L_rv_pa    = params[22];
  double R_rv_pa    = params[23];
  double L_la_lv    = params[24];
  double R_la_lv    = params[25];
  double L_lv_ao    = params[26];
  double R_lv_ao    = params[27];
  // Aortic Arch
  double C_ao       = params[28];
  // Pulmonary Resistance and Capacitance
  double C_pa_a     = params[29];
  double C_pa_v     = params[30];
  double R_pa_a     = params[31];  
  double R_pa_c     = params[32];  
  double R_pa_v     = params[33];  

  // Systemic Resistance and Capacitance
  double C_sys      = params[34];
  double R_sys_a    = params[35];
  double R_sys_v    = params[36];

  // Assign state variables
  double V_ra    = Xn[0];
  double V_la    = Xn[1];
  double V_rv    = Xn[2];
  double V_lv    = Xn[3];
  double Q_ra_rv = Xn[4];
  double P_pa_a  = Xn[5];
  double P_pa_v  = Xn[6];
  double Q_rv_pa = Xn[7];
  double Q_la_lv = Xn[8];
  double P_ao    = Xn[9];
  double Q_lv_ao = Xn[10];
  double P_sys   = Xn[11];

  // SET OPTIONS
  bool printMessages = false;
  int totalStates = Xn.size();

  // Set Time integration parameters
  double tc = 60.0/HR;
  double tsa = tc * tsas;
  double tsv = tc * tsvs;
  double tpw = tc/(double)tpws;
  double tcr = fmod(tn,tc);

  if(printMessages){
    printf("--- TIME CYCLE\n");
    printf("Current Time: %f\n",tn);
  }

  // ====================================================
  // PART I - ATRIAL ACTIVATION AND VENTRICULAR ELASTANCE
  // ====================================================

  // Heart function
  double tmv = tcr; // Ventricle time
  double tma = fmod(tn+tsa-tpw,tc); // Atrium time
  
  // Ventricle activation
  double fAV = 0.0;
  if(tmv<tsvs){
    fAV = (1.0-cos(2.0*M_PI*tmv/(double)tsvs))/2.0;
  }else{
    fAV = 0.0;
  }
  
  // Atrium activation
  double fAA = 0.0;
  if(tma<tsas){
    fAA = (1.0-cos(2.0*M_PI*tma/(double)tsas))/2.0; 
  }else{
    fAA = 0.0;
  }

  // ATRIA
  // Compute exponential atrial passive pressures curves
  double P_pas_ra = K_pas_ra_1*(exp((V_ra-Vra0)*K_pas_ra_2)-1.0);
  double P_pas_la = K_pas_la_1*(exp((V_la-Vla0)*K_pas_la_2)-1.0);
  // Compute linear atrial active pressure curves
  double P_act_ra = Emax_ra*(V_ra-Vra0);
  double P_act_la = Emax_la*(V_la-Vla0);
  // Blend with activation function
  double P_ra = (P_pas_ra + fAA * (P_act_ra - P_pas_ra)) * convertmmHgToBarye;
  double P_la = (P_pas_la + fAA * (P_act_la - P_pas_la)) * convertmmHgToBarye;

  // VENTRICLES
  // Passive Curve - Exponential
  double P_pas_rv = K_pas_rv_1*(exp((V_rv-Vrv0)*K_pas_rv_2)-1.0);
  double P_pas_lv = K_pas_lv_1*(exp((V_lv-Vlv0)*K_pas_lv_2)-1.0);
  // Active Curve - Linear
  double P_act_rv = Emax_rv*(V_rv-Vrv0);
  double P_act_lv = Emax_lv*(V_lv-Vlv0);
  // Use Activation to blend between active and passive Curves
  double P_rv = (P_pas_rv + fAV * (P_act_rv - P_pas_rv)) * convertmmHgToBarye;
  double P_lv = (P_pas_lv + fAV * (P_act_lv - P_pas_lv)) * convertmmHgToBarye;

  // ========================
  // PART II - HEART CHAMBERS
  // ========================

  // Initialize variables for valves 
  // 1.0 - Valve Open; 0.0 - Valve Closed
  for(int loopA=0;loopA<totalStates;loopA++){
    Ind[loopA] = 1.0;
  }

  // Check if RA-RV Valve is open
  if(P_rv >= P_ra){
    Ind[4]=0.0;
  }

  // Right Atrium Equation
  double Q_ra_rv_p = 0.0;
  if(Ind[4]>0.0){
    Q_ra_rv_p = (1.0/L_ra_rv)*(P_ra - P_rv - R_ra_rv * Q_ra_rv);	
  }
  if(printMessages){
    printf("Q_ra_rv_p: %f\n",Q_ra_rv_p);
  }

  // Check if RV-PA Valve is open
  if(P_pa_a + R_pa_a * Q_rv_pa >= P_rv){
    Ind[7]=0.0;
  }

  // Right Ventricle
  double Q_rv_pa_p = 0.0;
  if(Ind[7]>0.0){
    // CHECK !!!
    Q_rv_pa_p = (1.0/L_rv_pa)*(P_rv - (P_pa_a + R_pa_a * Q_rv_pa) - R_rv_pa * Q_rv_pa);
  }
  if(printMessages){
    printf("Q_rv_pa_p: %f\n",Q_rv_pa_p);
  }

  // Pulmonary Circulation: Two Capacitance Equations
  double Q_pul_a = (P_rv - Q_rv_pa * R_rv_pa - P_pa_a)/R_pa_a;
  double Q_pul_c = (P_pa_a - P_pa_v)/R_pa_c;
  double Q_pul_v = (P_pa_v - P_la)/R_pa_v;
  // Equations
  double P_pa_a_p = ( 1.0 / C_pa_a )*( Q_pul_a * Ind[7] - Q_pul_c );
  double P_pa_v_p = ( 1.0 / C_pa_v )*( Q_pul_c - Q_pul_v );

  if(printMessages){
    printf("Q_pul_a: %f\n",Q_pul_a);
    printf("Q_pul_c: %f\n",Q_pul_c);
    printf("Q_pul_v: %f\n",Q_pul_v);
    printf("P_pa_a_p: %f\n",P_pa_a_p);
    printf("P_pa_v_p: %f\n",P_pa_v_p);
  }

  // Check if LA-LV Valve is open
  if(P_lv >= P_la){
    Ind[8]=0.0;
  }

  // Left Atrium
  double Q_la_lv_p = 0.0;
  if(Ind[8]>0.0){
    Q_la_lv_p = (1.0/L_la_lv)*( P_la - P_lv - R_la_lv * Q_la_lv );
  }
  if(printMessages){
    printf("Q_la_lv_p: %f\n",Q_la_lv_p);
  }

  // Check if LV-AO Valve is open
  if(P_ao >= P_lv){
    Ind[10]=0.0;
  }

  // Left Ventricle
  double Q_lv_ao_p = 0.0;
  if(Ind[10]>0.0){
    Q_lv_ao_p = (1.0/L_lv_ao)*(P_lv - P_ao - R_lv_ao * Q_lv_ao);
  }
  if(printMessages){
    printf("Q_lv_ao_p: %f\n",Q_lv_ao_p);
  }

  // Flow in VEINS
  double Q_sys_a = ( P_ao - P_sys ) / R_sys_a;
  if(printMessages){
    printf("Q_sys_a: %f\n",Q_sys_a);
  }

  // ====================================================
  // COMPUTE THE VARIATION IN VOLUME FOR ALL THE CHAMBERS
  // ====================================================

  double V_ra_p = 0.0;
  double V_la_p = 0.0;
  double V_rv_p = 0.0;
  double V_lv_p = 0.0; 

  // Add Systemic Resistance
  double Q_sys_v = ( P_sys - P_ra ) / R_sys_v;
  
  // Variation of Volume in Atria and Ventricles
  V_ra_p = Q_sys_v - Q_ra_rv * Ind[4];
  V_rv_p = Q_ra_rv * Ind[4] - Q_rv_pa * Ind[7];
  V_la_p = Q_pul_v - Q_la_lv * Ind[8];
  V_lv_p = Q_la_lv * Ind[8] - Q_lv_ao * Ind[10];

  if(printMessages){
  	printf("Q_sys_v: %f\n",Q_sys_v);
    printf("V_ra_p: %f\n",V_ra_p);
    printf("V_la_p: %f\n",V_la_p);
    printf("V_rv_p: %f\n",V_rv_p);
    printf("V_lv_p: %f\n",V_lv_p);
  }

  // ======================
  // PART III - AORTIC ARCH
  // ======================

  // Aortic arch capacitance
  double P_ao_p = (1.0/C_ao) * ( Q_lv_ao - Q_sys_a );

  if(printMessages){
    printf("P_ao_p: %f\n",P_ao_p);
  }

  // Systemic Capacitance
  double P_sys_p = (1.0/C_sys) * ( Q_sys_a - Q_sys_v );  

  // Store the derivatives
  Xn1[0] = V_ra_p;
  Xn1[1] = V_la_p;
  Xn1[2] = V_rv_p;
  Xn1[3] = V_lv_p;
  Xn1[4] = Q_ra_rv_p;
  Xn1[5] = P_pa_a_p;
  Xn1[6] = P_pa_v_p;
  Xn1[7] = Q_rv_pa_p;
  Xn1[8] = Q_la_lv_p;
  Xn1[9] = P_ao_p;
  Xn1[10] = Q_lv_ao_p;
  Xn1[11] = P_sys_p;

  // Get Auxiliary Results
  out[0] = tn; // Current Time
  out[1] = tcr; // Relative Cycle Time
  out[2] = fAA; // Atrial Activation Function
  out[3] = fAV; // Right Ventricle Elastance
  out[4] = 0.0;
  out[5] = P_ra; // Right Atrial Pressure
  out[6] = P_la; // Left Atrial Pressure
  out[7] = P_rv; // Right Ventricular Pressure
  out[8] = P_lv; // Left Ventricular Pressure
  out[9] = Q_pul_a; // Pulmonary Arterial flow rate
  out[10] = Q_pul_c; // Pulmonary Capillary flow rate
  out[11] = Q_pul_v; // Pulmonary Venous flow rate
  out[12] = Q_sys_a; // Systemic flow Rate - Arteries
  out[13] = Q_sys_v; // Systemic flow Rate - Veins
  out[14] = Ind[4]; // RA-RV valve status
  out[15] = Ind[7]; // RV-PA valve status
  out[16] = Ind[8]; // LA-LV valve status
  out[17] = Ind[10]; // LV-AO valve status

  // Stop every iteration when printing messages
  if(printMessages){
    getchar();	
  }
}

int odeNormalAdultSimplePD::PerformRK4Steps(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = 0;
  bool printIC = false;
  bool isValidDouble = true;

  // Get Total number of States
  int totalStates = getStateTotal();
  int totAuxStates = getAuxStateTotal();

  // Allocate Temporary Variables
  stdVec k1;
  k1.resize(totalStates);
  stdVec k2;
  k2.resize(totalStates);
  stdVec k3;
  k3.resize(totalStates);
  stdVec k4;
  k4.resize(totalStates);
  stdVec k1AuxOut;
  k1AuxOut.resize(totAuxStates);
  stdVec k2AuxOut;
  k2AuxOut.resize(totAuxStates);
  stdVec k3AuxOut;
  k3AuxOut.resize(totAuxStates);
  stdVec k4AuxOut;
  k4AuxOut.resize(totAuxStates);
  stdVec Xk2;
  Xk2.resize(totalStates);
  stdVec Xk3;
  Xk3.resize(totalStates);
  stdVec Xk4;
  Xk4.resize(totalStates);
  stdVec Ind;
  Ind.resize(totalStates);

  // Initialize State Vectors and Copy Initial Conditions
  stdVec Xn;
  Xn.resize(totalStates);
  stdVec Xn1;
  Xn1.resize(totalStates);

  // Initialize Variables
  for(int loopA=0;loopA<totalStates;loopA++){
    k1[loopA] = 0.0;
    k2[loopA] = 0.0;
    k3[loopA] = 0.0;
    k4[loopA] = 0.0;

    Xk2[loopA] = 0.0;
    Xk3[loopA] = 0.0;
    Xk4[loopA] = 0.0;
    Ind[loopA] = 1.0;
    Xn[loopA] = 0.0;
    Xn1[loopA] = 0.0;
  }
  for(int loopA=0;loopA<totAuxStates;loopA++){
    k1AuxOut[loopA] = 0.0;
    k2AuxOut[loopA] = 0.0;
    k3AuxOut[loopA] = 0.0;
    k4AuxOut[loopA] = 0.0;
  }

  // Set Initial Conditions
  for(int loopA=0;loopA<getStateTotal();loopA++){
    Xn[loopA] = iniVals[loopA];
  }

  // =========
  // TIME LOOP
  // =========
  for(int loopA=0;loopA<totalSteps;loopA++){

    // Increment Time Step
    stepId++;

    // Eval K1
    lpnODE(currTime,Xn,params,k1,k1AuxOut,Ind);

    // Eval K2
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk2[loopB] = Xn[loopB] + ((1.0/3.0)*timeStep) * k1[loopB];
    }
    lpnODE(currTime + (1.0/3.0) * timeStep,Xk2,params,k2,k2AuxOut,Ind);
    
    // Eval K3
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk3[loopB] = Xn[loopB] - (1.0/3.0)*timeStep * k1[loopB] + (1.0*timeStep) * k2[loopB];
    }
    lpnODE(currTime + (2.0/3.0) * timeStep,Xk3,params,k3,k3AuxOut,Ind);
    
    // Eval K4
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk4[loopB] = Xn[loopB] + timeStep*k1[loopB] - timeStep*k2[loopB] + timeStep * k3[loopB];
    }
    lpnODE(currTime + timeStep,Xk4,params,k4,k4AuxOut,Ind);

    // Eval Xn1
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn1[loopB] = Xn[loopB] + (1.0/8.0)*timeStep*(k1[loopB] + 3.0 * k2[loopB] + 3.0 * k3[loopB] + k4[loopB]);
    }

    // CHECK IF SOLUTION IS VALID
    if((stepId % 1) == 0){
      for(int loopB=0;loopB<totalStates;loopB++){
        isValidDouble = (Xn1[loopB] != Xn1[loopB]);
        if(isValidDouble){
          // cout << "FOUND SOMETHING THAT IS NOT VALID AT " << loopB << "\n";
          return 1;
        }
      }
    }

    // Update Xn
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn[loopB] = Xn1[loopB] * Ind[loopB];
    }    

    // Copy back k1AuxOut for each time step.
    for(int loopB=0;loopB<totAuxStates;loopB++){
      auxOutVals[loopB][stepId] = k4AuxOut[loopB];
    }

    // Copy The solution back for each time step
    for(int loopB=0;loopB<totalStates;loopB++){
      outVals[loopB][stepId] = Xn1[loopB];
    }  

    // Update Current Time
    currTime += timeStep;
  }
  
  // RETURN OK
  return 0;
}

bool odeNormalAdultSimplePD::areParamsInsideLimits(stdVec params){
  // GET LIMITS
  int par_num = getParameterTotal();
  double currParam = 0.0;
  stdVec limits;
  getParameterLimits(limits);
  
  bool result = true;
  for(int loopA=0;loopA<par_num;loopA++){
    currParam = params[loopA];
    result = ((currParam>=limits[loopA*2 + 0]) && (currParam<=limits[loopA*2 + 1]));
    if(!result){
      printf("PROBLEM %d\n",loopA);
      return result;
    }
  }

  // Return
  return result;
}


void odeNormalAdultSimplePD::writeAllDataToFile(string fileName,int totalSteps,int totalStates,stdMat outVals,int totAuxStates,stdMat auxOutVals){
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  for(int loopA=0;loopA<totalSteps;loopA++){
    // Print State Variables
    for(int loopB=0;loopB<totalStates;loopB++){
      fprintf(outFile,"%e ",outVals[loopB][loopA]);
    }
    // Print Aux Variables
    for(int loopB=0;loopB<totAuxStates;loopB++){
      fprintf(outFile,"%e ",auxOutVals[loopB][loopA]);
    }
    fprintf(outFile,"\n");
  }
  // CLOSE THE FILE
  fclose(outFile);
}

bool odeNormalAdultSimplePD::isParamsSignCorrect(stdVec params){
  // GET LIMITS
  int par_num = params.size();
  double currParam = 0.0;
  stdVec limits;
  getParameterLimits(limits);
  
  bool result = true;
  for(int loopA=0;loopA<par_num;loopA++){
  	// Get Current Parameter
    currParam = params[loopA];
    // Check Sign w.r.t. limits
    if(isParamPositive(loopA,limits)||isParamNegative(loopA,limits)){
      result = result && (isParamPositive(loopA,limits) == (currParam>=0.0)) && (isParamNegative(loopA,limits) == (currParam<0.0));	
    }    
  }
  // RETURN
  return result;
}

int odeNormalAdultSimplePD::solveLPN(lpnOptions options, stdVec params, stdVec& results){

  // Set Time integration quantities
  double heartRate = params[getStateTotal()];
  double cycleTime = 60.0/(double)heartRate;
  int totalSteps = numCycles*totalStepsOnSingleCycle;
  double totalTime = totalSteps*cycleTime;
  double timeStep = cycleTime/(double)totalStepsOnSingleCycle;

  // CHECK PARAMS INSIDE LIMITS
  if(options.checkLimits){
    if(!areParamsInsideLimits(params)){
      printf("ERROR: VIOLATED PARAMETER LIMITS.\n");
      return 1;
    }
  }

  // CHECK PARAMETER SIGN
  if(options.checkSign){
    if(!isParamsSignCorrect(params)){
      printf("ERROR: VIOLATED PARAMETER SIGN.\n");
      return 1;
    }
  }

  // TIME STEP ARRAY
  stdVec t;
  t.resize(totalSteps);
  t[0] = 0.0;
  for(int loopA=1;loopA<totalSteps;loopA++){
    t[loopA] = t[loopA-1] + timeStep;
  }

  // GET TOTALS
  int totalParams = getParameterTotal();
  int totalStates = getStateTotal();
  int totAuxStates = getAuxStateTotal();
  int totalResults = getResultTotal();

  // Separate Initial Conditions and parameters
  // Note: the first "totalStates" parameters are initial conditions
  stdVec iniVals;
  iniVals.resize(totalStates);
  stdVec trueParams;
  trueParams.resize(totalParams-totalStates);
  for(int loopA=0;loopA<totalParams;loopA++){
    if(loopA<totalStates){
      iniVals[loopA] = params[loopA];
    }else{
      trueParams[loopA-totalStates] = params[loopA];
    }
  }

  // State Variables
  // double outVals2[totalStates][totalSteps];
  stdMat outVals;
  outVals.resize(totalStates);
  for(int loopA=0;loopA<totalStates;loopA++){
    outVals[loopA].resize(totalSteps);
  }

  // Auxiliary Outputs
  stdMat auxOutVals;
  auxOutVals.resize(totAuxStates);
  for(int loopA=0;loopA<totAuxStates;loopA++){
    auxOutVals[loopA].resize(totalSteps);
  }

  // Solve In Time
  int error = PerformRK4Steps(totalSteps,timeStep,iniVals,trueParams,outVals,auxOutVals);
  if(error !=0 ){
    throw cmException("RK4 Time Integration not Converged!");
  }
  
  // WRITE ALL DATA FILE
  if(options.flushData){
    string allDataFileName("allData.dat");
    writeAllDataToFile(allDataFileName,totalSteps,totalStates,outVals,totAuxStates,auxOutVals);    
  }

  // ===============================
  // COMPUTE THE RESULTS OF INTEREST
  // ===============================
  results.clear();

  // DETERMINE START AND END OF LAST HEART CYCLE
  int startLastCycle = (numCycles-1) * totalStepsOnSingleCycle;
  int stopLastCycle = numCycles * totalStepsOnSingleCycle;
  double output[totalSteps];

  // MIN AND MAX AO PRESSURE
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = outVals[ipAOPressure][loopA];
  }
  double maxAOPress  = getMax(startLastCycle, stopLastCycle, output);
  double minAOPress  = getMin(startLastCycle, stopLastCycle, output);
  double avAOPress  = getMean(startLastCycle, stopLastCycle, output);

  // AVERAGE RA PRESSURE
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = auxOutVals[ipAuxRAPressure][loopA];
  }
  double avRAPress  = getMean(startLastCycle, stopLastCycle, output);

  // RV PRESSURE
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = auxOutVals[ipAuxRVPressure][loopA];
  }
  double minRVPress  = getMin(startLastCycle, stopLastCycle, output);
  double maxRVPress  = getMax(startLastCycle, stopLastCycle, output);

  // SYSTOLIC, DIASTOLIC AND AVERAGE PA PRESSURES
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = outVals[ipPAAPressure][loopA];
  }
  double maxPAPress  = getMax(startLastCycle, stopLastCycle, output);
  double minPAPress  = getMin(startLastCycle, stopLastCycle, output);
  double avPAPress   = getMean(startLastCycle, stopLastCycle, output);

  // AVERAGE PCWP PRESSURE
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = auxOutVals[ipPAVPressure][loopA];
  }
  double avPCWPress  = getMean(startLastCycle, stopLastCycle, output);

  // CARDIAC OUTPUT
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = outVals[ipAOFlow][loopA];
  }
  double CO =  getMean(startLastCycle, stopLastCycle, output);

  // MAXIMUM AND MINIMUM VENTRICULAR VOLUMES
  double minRVVolume = outVals[ipRVVolume][startLastCycle];
  double maxRVVolume = outVals[ipRVVolume][startLastCycle];
  double minLVVolume = outVals[ipLVVolume][startLastCycle];
  double maxLVVolume = outVals[ipLVVolume][startLastCycle];
  for(int loopA=startLastCycle+1;loopA<stopLastCycle;loopA++){
    if(outVals[ipRVVolume][loopA] < minRVVolume){
      minRVVolume = outVals[ipRVVolume][loopA];
    }
    if(outVals[ipRVVolume][loopA] > maxRVVolume){
      maxRVVolume = outVals[ipRVVolume][loopA];
    }
    if(outVals[ipLVVolume][loopA] < minLVVolume){
      minLVVolume = outVals[ipLVVolume][loopA];
    }
    if(outVals[ipLVVolume][loopA] > maxLVVolume){
      maxLVVolume = outVals[ipLVVolume][loopA];
    }
  }

  // Assign Results Based on Model Version
  results.clear(); 
  if((modelVersion >= 0001)&&(modelVersion < 0002)){
    results.push_back(heartRate); // HR
    results.push_back(maxAOPress); // SBP
    results.push_back(minAOPress); // DBP
    results.push_back(avAOPress); // mBP
    results.push_back(avRAPress); // RAP
    results.push_back(maxPAPress); // sPAP
    results.push_back(minPAPress); // dPAP
    results.push_back(avPAPress); // mPAP
    results.push_back(avPCWPress); // PWP
    results.push_back(CO); // CO
    results.push_back(params[43]+params[44]+params[45]); // PVR
    results.push_back(params[47]+params[48]); // SVR
    results.push_back(avRAPress); // CVP
    results.push_back(((maxLVVolume - minLVVolume)/maxLVVolume)*100.0); // LVEF 
    results.push_back(((maxRVVolume - minRVVolume)/maxRVVolume)*100.0); // RVEF
  }else if(modelVersion >= 0002){
    // Add To Results: TO COMPLETE !!!
    results.push_back(heartRate); // ip_0002_heart_rate2
    results.push_back(maxAOPress); // ip_0002_systolic_bp_2
    results.push_back(minAOPress); // ip_0002_diastolic_bp_2
    results.push_back(CO); // ip_0002_cardiac_output
    results.push_back(params[47]+params[48]); // ip_0002_systemic_vascular_resistan
    results.push_back(params[43]+params[44]+params[45]); // ip_0002_pulmonary_vascular_resista
    results.push_back(avRAPress); // ip_0002_cvp
    results.push_back(minRVPress); // ip_0002_right_ventricle_diastole
    results.push_back(maxRVPress); // ip_0002_right_ventricle_systole
    results.push_back(0.0); // ip_0002_rvedp
    results.push_back(0.0); // ip_0002_aov_mean_pg
    results.push_back(0.0); // ip_0002_aov_peak_pg
    results.push_back(0.0); // ip_0002_mv_decel_time
    results.push_back(0.0); // ip_0002_mv_e_a_ratio
    results.push_back(0.0); // ip_0002_pv_at
    results.push_back(0.0); // ip_0002_pv_max_pg
    results.push_back(avRAPress); // ip_0002_ra_pressure
    results.push_back(0.0); // ip_0002_ra_vol_a4c
    results.push_back(0.0); // ip_0002_la_vol_a4c
    results.push_back(0.0); // ip_0002_lv_esv
    results.push_back(0.0); // ip_0002_lv_vol
    results.push_back(0.0); // ip_0002_lvef
    results.push_back(0.0); // sip_0002_lvot_max_flow
    results.push_back(minPAPress); // ip_0002_pap_diastolic
    results.push_back(maxPAPress); // ip_0002_pap_systolic
    results.push_back(avPCWPress); // ip_0002_wedge_pressure  
  }

  // Solution Successful
  return 0;
}

void odeNormalAdultSimplePD::setModelResults(stdVec outputs,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths){

  // CHECK INSTALLED VERSION
  if((modelVersion >= 0001)&&(modelVersion < 0002)){

    // KEYS
    keys.clear();
    keys.push_back(string("HR"));
    keys.push_back(string("SBP"));
    keys.push_back(string("DBP"));
    keys.push_back(string("mBP"));
    keys.push_back(string("RAP"));
    keys.push_back(string("sPAP"));
    keys.push_back(string("dPAP"));
    keys.push_back(string("mPAP"));
    keys.push_back(string("PWP"));
    keys.push_back(string("CO"));
    keys.push_back(string("PVR"));
    keys.push_back(string("SVR"));
    keys.push_back(string("CVP"));
    keys.push_back(string("LVEF"));
    keys.push_back(string("RVEF"));

    // COMPUTED VALUES
    computedValues.clear();
    computedValues.push_back(outputs[ip_0001_HR]);
    computedValues.push_back(outputs[ip_0001_SBP]/1333.22);
    computedValues.push_back(outputs[ip_0001_DBP]/1333.22);
    computedValues.push_back(outputs[ip_0001_mBP]/1333.22);
    computedValues.push_back(outputs[ip_0001_RAP]/1333.22);
    computedValues.push_back(outputs[ip_0001_sPAP]/1333.22);
    computedValues.push_back(outputs[ip_0001_dPAP]/1333.22);
    computedValues.push_back(outputs[ip_0001_mPAP]/1333.22);
    computedValues.push_back(outputs[ip_0001_PWP]/1333.22);
    computedValues.push_back(outputs[ip_0001_CO]*(60.0/1000.0));
    computedValues.push_back(outputs[ip_0001_PVR]);
    computedValues.push_back(outputs[ip_0001_SVR]);
    computedValues.push_back(outputs[ip_0001_CVP]/1333.22);
    computedValues.push_back(outputs[ip_0001_LVEF]);
    computedValues.push_back(outputs[ip_0001_RVEF]);

    // STANDARD DEVIATIONS
    stdFactors.clear();
    stdFactors.push_back(3.0); // HR
    stdFactors.push_back(1.5); // SBP
    stdFactors.push_back(1.5); // DBP
    stdFactors.push_back(1.5); // mBP
    stdFactors.push_back(0.5); // RAP
    stdFactors.push_back(1.0); // sPAP
    stdFactors.push_back(1.0); // dPAP
    stdFactors.push_back(1.0); // mPAP
    stdFactors.push_back(1.0); // PWP
    stdFactors.push_back(0.2); // CO
    stdFactors.push_back(5.0); // PVR
    stdFactors.push_back(50.0); // SVR
    stdFactors.push_back(0.5); // CVP
    stdFactors.push_back(2.0); // LVEF 
    stdFactors.push_back(2.0); // RVEF
  
    // WEIGHTS
    weigths.clear();
    for(int loopA=0;loopA<computedValues.size();loopA++){
      weigths.push_back(1.0);
    }

  }else if(modelVersion >= 0002){

    // KEYS
    keys.clear();
    keys.push_back(string("heart_rate2"));
    keys.push_back(string("systolic_bp_2"));
    keys.push_back(string("diastolic_bp_2"));    
    keys.push_back(string("cardiac_output"));
    keys.push_back(string("systemic_vascular_resistan"));
    keys.push_back(string("pulmonary_vascular_resista"));
    keys.push_back(string("cvp"));
    keys.push_back(string("right_ventricle_diastole"));
    keys.push_back(string("right_ventricle_systole"));
    keys.push_back(string("rvedp"));
    keys.push_back(string("aov_mean_pg"));
    keys.push_back(string("aov_peak_pg"));
    keys.push_back(string("mv_decel_time"));
    keys.push_back(string("mv_e_a_ratio"));
    keys.push_back(string("pv_at"));
    keys.push_back(string("pv_max_pg"));
    keys.push_back(string("ra_pressure"));
    keys.push_back(string("ra_vol_a4c"));
    keys.push_back(string("la_vol_a4c"));
    keys.push_back(string("lv_esv"));
    keys.push_back(string("lv_vol"));
    keys.push_back(string("lvef"));
    keys.push_back(string("lvot_max_flow"));
    // Pulmonary Qtys
    keys.push_back(string("pap_diastolic"));
    keys.push_back(string("pap_systolic"));
    keys.push_back(string("wedge_pressure"));

    // COMPUTED VALUES
    computedValues.clear();
    computedValues.push_back(outputs[ip_0002_heart_rate2]);
    computedValues.push_back(outputs[ip_0002_systolic_bp_2]/1333.22);
    computedValues.push_back(outputs[ip_0002_diastolic_bp_2]/1333.22);    
    computedValues.push_back(outputs[ip_0002_cardiac_output]*(60.0/1000.0));
    computedValues.push_back(outputs[ip_0002_systemic_vascular_resistan]);
    computedValues.push_back(outputs[ip_0002_pulmonary_vascular_resista]);
    computedValues.push_back(outputs[ip_0002_cvp]/1333.22);
    computedValues.push_back(outputs[ip_0002_right_ventricle_diastole]/1333.22);
    computedValues.push_back(outputs[ip_0002_right_ventricle_systole]/1333.22);
    computedValues.push_back(outputs[ip_0002_rvedp]/1333.22);
    computedValues.push_back(outputs[ip_0002_aov_mean_pg]/1333.22);
    computedValues.push_back(outputs[ip_0002_aov_peak_pg]/1333.22);
    computedValues.push_back(outputs[ip_0002_mv_decel_time]);
    computedValues.push_back(outputs[ip_0002_mv_e_a_ratio]);
    computedValues.push_back(outputs[ip_0002_pv_at]);
    computedValues.push_back(outputs[ip_0002_pv_max_pg]/1333.22);
    computedValues.push_back(outputs[ip_0002_ra_pressure]/1333.22);
    computedValues.push_back(outputs[ip_0002_ra_vol_a4c]);
    computedValues.push_back(outputs[ip_0002_la_vol_a4c]);
    computedValues.push_back(outputs[ip_0002_lv_esv]);
    computedValues.push_back(outputs[ip_0002_lv_vol]);
    computedValues.push_back(outputs[ip_0002_lvef]);
    computedValues.push_back(outputs[ip_0002_lvot_max_flow]);
    // Pulmonary Qtys
    computedValues.push_back(outputs[ip_0002_pap_diastolic]/1333.22);
    computedValues.push_back(outputs[ip_0002_pap_systolic]/1333.22);
    computedValues.push_back(outputs[ip_0002_wedge_pressure]/1333.22);

    // STANDARD DEVIATIONS
    stdFactors.clear();
    stdFactors.push_back(3.0); // heart_rate2
    stdFactors.push_back(1.5); // systolic_bp_2
    stdFactors.push_back(1.5); // diastolic_bp_2    
    stdFactors.push_back(0.2); // cardiac_output
    stdFactors.push_back(50.0); // systemic_vascular_resistan
    stdFactors.push_back(5.0); // pulmonary_vascular_resista
    stdFactors.push_back(0.5); // cvp
    stdFactors.push_back(1.0); // right_ventricle_diastole
    stdFactors.push_back(1.0); // right_ventricle_systole
    stdFactors.push_back(1.0); // rvedp
    stdFactors.push_back(0.5); // aov_mean_pg
    stdFactors.push_back(0.5); // aov_peak_pg
    stdFactors.push_back(6.0); // mv_decel_time
    stdFactors.push_back(0.2); // mv_e_a_ratio
    stdFactors.push_back(6.0); // pv_at
    stdFactors.push_back(0.5); // pv_max_pg
    stdFactors.push_back(0.5); // ra_pressure
    stdFactors.push_back(3.0); // ra_vol_a4c
    stdFactors.push_back(3.0); // la_vol_a4c
    stdFactors.push_back(10.0); // lv_esv
    stdFactors.push_back(20.0); // lv_vol_a4c
    stdFactors.push_back(2.0); // lvef
    stdFactors.push_back(1.0); // lvot_max_flow
    // Pulmonary
    stdFactors.push_back(1.0); // pap_diastolic
    stdFactors.push_back(1.0); // pap_systolic
    stdFactors.push_back(1.0); // wedge_pressure
  
    // WEIGHTS
    weigths.clear();
    for(int loopA=0;loopA<computedValues.size();loopA++){
      weigths.push_back(1.0);
    }
  }
}

double odeNormalAdultSimplePD::evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCode) {
  
  // Set Options
  lpnOptions options;
  options.writeDebugData = false;
  options.flushData = false;
  options.checkLimits = false;
  options.checkSign = false;

  // Solve LPN Model
  int locErrorCode = solveLPN(options,inputs,outputs);

  // Store Error Code
  errorCode.push_back(locErrorCode); 

  // SET RESULTS FOR VARIOUS VERSIONS
  stdStringVec keys;
  stdVec computedValues;
  stdVec stdFactors;
  stdVec weigths;
  setModelResults(outputs,keys,computedValues,stdFactors,weigths);
 
  // Print and compare
  double result = 0.0;
  if(data != NULL){
    
    // Print Info
    if(options.flushData){
      data->printAndCompare(datasetColumn,keys,computedValues,weigths);  
    }
  
    // Evaluate Objective Function
    result = data->evalLogLikelihood(datasetColumn,keys,computedValues,stdFactors,weigths);
    // result = data->evalOBJ(datasetColumn,keys,computedValues,weigths);
  }

  // Return 
  return result;
}
