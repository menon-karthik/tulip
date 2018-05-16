# include "odeNormalAdultSimplePD.h"

using namespace std;

odeNormalAdultSimplePD::odeNormalAdultSimplePD(int modelVersion){
  this->modelVersion = modelVersion;
}

odeNormalAdultSimplePD::~odeNormalAdultSimplePD(){
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

int odeNormalAdultSimplePD::getHRIndex(){
  return 12;
}

string odeNormalAdultSimplePD::getParamName(int index){
  string res;
  switch(index){
    case 0:
      res = string("V_ra_ini");
      break;
    case 1:
      res = string("V_la_ini");
      break;
    case 2:
      res = string("V_rv_ini");
      break;
    case 3:
      res = string("V_lv_ini");
      break;
    case 4:
      res = string("Q_ra_rv_ini");
      break;
    case 5:
      res = string("P_pa_a_ini");
      break;
    case 6:
      res = string("P_pa_v_ini");
      break;
    case 7:
      res = string("Q_rv_pa_ini");
      break;
    case 8:
      res = string("Q_la_lv_ini");
      break;
    case 9:
      res = string("P_ao_ini");
      break;
    case 10:
      res = string("Q_lv_ao_ini");
      break;
    case 11:
      res = string("P_sys_ini");
      break;
    case 12:
      res = string("HR");
      break;
    case 13:
      res = string("tsas");
      break;
    case 14:
      res = string("tpws");
      break;
    case 15:
      res = string("tsvs");
      break;
    case 16:
      res = string("K_pas_ra_1");
      break;
    case 17:
      res = string("K_pas_ra_2");
      break;
    case 18:
      res = string("Emax_ra");
      break;
    case 19:
      res = string("Vra0");
      break;
    case 20:
      res = string("K_pas_la_1");
      break;
    case 21:
      res = string("K_pas_la_2");
      break;
    case 22:
      res = string("Emax_la");
      break;
    case 23:
      res = string("Vla0");
      break;
    case 24:
      res = string("K_pas_rv_1");
      break;
    case 25:
      res = string("K_pas_rv_2");
      break;
    case 26:
      res = string("Emax_rv");
      break;
    case 27:
      res = string("Vrv0");
      break;
    case 28:
      res = string("K_pas_lv_1");
      break;
    case 29:
      res = string("K_pas_lv_2");
      break;
    case 30:
      res = string("Emax_lv");
      break;
    case 31:
      res = string("Vlv0");
      break;
    case 32:
      res = string("L_ra_rv");
      break;
    case 33:
      res = string("R_ra_rv");
      break;
    case 34:
      res = string("L_rv_pa");
      break;
    case 35:
      res = string("R_rv_pa");
      break;
    case 36:
      res = string("L_la_lv");
      break;
    case 37:
      res = string("R_la_lv");
      break;
    case 38:
      res = string("L_lv_ao");
      break;
    case 39:
      res = string("R_lv_ao");
      break;
    case 40:
      res = string("C_ao");
      break;
    case 41:
      res = string("C_pa_a");
      break;
    case 42:
      res = string("C_pa_v");
      break;
    case 43:
      res = string("R_pa_a");
      break;
    case 44:
      res = string("R_pa_c");
      break;
    case 45:
      res = string("R_pa_v");
      break;
    case 46:
      res = string("C_sys");
      break;
    case 47:
      res = string("R_sys_a");
      break;
    case 48:
      res = string("R_sys_v");
      break;
  }
  return res;
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

void odeNormalAdultSimplePD::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, 
                                       stdVec& DXk, stdVec& auxOut, stdVec& Ind){

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
  double V_ra    = Xk[0];
  double V_la    = Xk[1];
  double V_rv    = Xk[2];
  double V_lv    = Xk[3];
  double Q_ra_rv = Xk[4];
  double P_pa_a  = Xk[5];
  double P_pa_v  = Xk[6];
  double Q_rv_pa = Xk[7];
  double Q_la_lv = Xk[8];
  double P_ao    = Xk[9];
  double Q_lv_ao = Xk[10];
  double P_sys   = Xk[11];

  // SET OPTIONS
  bool printMessages = false;
  int totalStates = Xk.size();

  // Set Time integration parameters
  double tc = 60.0/HR;
  double tsa = tc * tsas;
  double tsv = tc * tsvs;
  double tpw = tc/(double)tpws;
  double tcr = fmod(t,tc);

  if(printMessages){
    printf("--- TIME CYCLE\n");
    printf("Current Time: %f\n",t);
  }

  // ====================================================
  // PART I - ATRIAL ACTIVATION AND VENTRICULAR ELASTANCE
  // ====================================================

  // Heart function
  double tmv = tcr; // Ventricle time
  double tma = fmod(t+tsa-tpw,tc); // Atrium time
  
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
  DXk[0] = V_ra_p;
  DXk[1] = V_la_p;
  DXk[2] = V_rv_p;
  DXk[3] = V_lv_p;
  DXk[4] = Q_ra_rv_p;
  DXk[5] = P_pa_a_p;
  DXk[6] = P_pa_v_p;
  DXk[7] = Q_rv_pa_p;
  DXk[8] = Q_la_lv_p;
  DXk[9] = P_ao_p;
  DXk[10] = Q_lv_ao_p;
  DXk[11] = P_sys_p;

  // Get Auxiliary Results
  auxOut[0] = t; // Current Time
  auxOut[1] = tcr; // Relative Cycle Time
  auxOut[2] = fAA; // Atrial Activation Function
  auxOut[3] = fAV; // Right Ventricle Elastance
  auxOut[4] = 0.0;
  auxOut[5] = P_ra; // Right Atrial Pressure
  auxOut[6] = P_la; // Left Atrial Pressure
  auxOut[7] = P_rv; // Right Ventricular Pressure
  auxOut[8] = P_lv; // Left Ventricular Pressure
  auxOut[9] = Q_pul_a; // Pulmonary Arterial flow rate
  auxOut[10] = Q_pul_c; // Pulmonary Capillary flow rate
  auxOut[11] = Q_pul_v; // Pulmonary Venous flow rate
  auxOut[12] = Q_sys_a; // Systemic flow Rate - Arteries
  auxOut[13] = Q_sys_v; // Systemic flow Rate - Veins
  auxOut[14] = Ind[4]; // RA-RV valve status
  auxOut[15] = Ind[7]; // RV-PA valve status
  auxOut[16] = Ind[8]; // LA-LV valve status
  auxOut[17] = Ind[10]; // LV-AO valve status

  // Stop every iteration when printing messages
  if(printMessages){
    getchar();	
  }
}

void odeNormalAdultSimplePD::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, 
                                         const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, 
                                         stdVec& results){

  // DETERMINE START AND END OF LAST HEART CYCLE
  double heartRate      = 60.0/(totalStepsOnSingleCycle*timeStep);
  int    numCycles      = totalSteps/totalStepsOnSingleCycle;
  int    startLastCycle = (numCycles-1) * totalStepsOnSingleCycle;
  int    stopLastCycle  = numCycles * totalStepsOnSingleCycle;
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
}

void odeNormalAdultSimplePD::getResultKeys(stdStringVec& keys){
  
  // CHECK VERSION
  if((modelVersion >= 0001)&&(modelVersion < 0002)){
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
  }else if(modelVersion >= 0002){
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
  }
}

void odeNormalAdultSimplePD::getFinalOutputs(const stdVec& outputs, stdVec& outs){
  if((modelVersion >= 0001)&&(modelVersion < 0002)){
    outs.clear();
    outs.push_back(outputs[ip_0001_HR]);
    outs.push_back(outputs[ip_0001_SBP]/1333.22);
    outs.push_back(outputs[ip_0001_DBP]/1333.22);
    outs.push_back(outputs[ip_0001_mBP]/1333.22);
    outs.push_back(outputs[ip_0001_RAP]/1333.22);
    outs.push_back(outputs[ip_0001_sPAP]/1333.22);
    outs.push_back(outputs[ip_0001_dPAP]/1333.22);
    outs.push_back(outputs[ip_0001_mPAP]/1333.22);
    outs.push_back(outputs[ip_0001_PWP]/1333.22);
    outs.push_back(outputs[ip_0001_CO]*(60.0/1000.0));
    outs.push_back(outputs[ip_0001_PVR]);
    outs.push_back(outputs[ip_0001_SVR]);
    outs.push_back(outputs[ip_0001_CVP]/1333.22);
    outs.push_back(outputs[ip_0001_LVEF]);
    outs.push_back(outputs[ip_0001_RVEF]);
  }else if(modelVersion >= 0002){
    outs.clear();
    outs.push_back(outputs[ip_0002_heart_rate2]);
    outs.push_back(outputs[ip_0002_systolic_bp_2]/1333.22);
    outs.push_back(outputs[ip_0002_diastolic_bp_2]/1333.22);    
    outs.push_back(outputs[ip_0002_cardiac_output]*(60.0/1000.0));
    outs.push_back(outputs[ip_0002_systemic_vascular_resistan]);
    outs.push_back(outputs[ip_0002_pulmonary_vascular_resista]);
    outs.push_back(outputs[ip_0002_cvp]/1333.22);
    outs.push_back(outputs[ip_0002_right_ventricle_diastole]/1333.22);
    outs.push_back(outputs[ip_0002_right_ventricle_systole]/1333.22);
    outs.push_back(outputs[ip_0002_rvedp]/1333.22);
    outs.push_back(outputs[ip_0002_aov_mean_pg]/1333.22);
    outs.push_back(outputs[ip_0002_aov_peak_pg]/1333.22);
    outs.push_back(outputs[ip_0002_mv_decel_time]);
    outs.push_back(outputs[ip_0002_mv_e_a_ratio]);
    outs.push_back(outputs[ip_0002_pv_at]);
    outs.push_back(outputs[ip_0002_pv_max_pg]/1333.22);
    outs.push_back(outputs[ip_0002_ra_pressure]/1333.22);
    outs.push_back(outputs[ip_0002_ra_vol_a4c]);
    outs.push_back(outputs[ip_0002_la_vol_a4c]);
    outs.push_back(outputs[ip_0002_lv_esv]);
    outs.push_back(outputs[ip_0002_lv_vol]);
    outs.push_back(outputs[ip_0002_lvef]);
    outs.push_back(outputs[ip_0002_lvot_max_flow]);
    // Pulmonary Qtys
    outs.push_back(outputs[ip_0002_pap_diastolic]/1333.22);
    outs.push_back(outputs[ip_0002_pap_systolic]/1333.22);
    outs.push_back(outputs[ip_0002_wedge_pressure]/1333.22);
  }
}

void odeNormalAdultSimplePD::getDataSTD(stdVec& stds){
  if((modelVersion >= 0001)&&(modelVersion < 0002)){
    stds.clear();
    stds.push_back(3.0); // HR
    stds.push_back(1.5); // SBP
    stds.push_back(1.5); // DBP
    stds.push_back(1.5); // mBP
    stds.push_back(0.5); // RAP
    stds.push_back(1.0); // sPAP
    stds.push_back(1.0); // dPAP
    stds.push_back(1.0); // mPAP
    stds.push_back(1.0); // PWP
    stds.push_back(0.2); // CO
    stds.push_back(5.0); // PVR
    stds.push_back(50.0); // SVR
    stds.push_back(0.5); // CVP
    stds.push_back(2.0); // LVEF 
    stds.push_back(2.0); // RVEF
  }else if(modelVersion >= 0002){
    stds.clear();
    stds.push_back(3.0); // heart_rate2
    stds.push_back(1.5); // systolic_bp_2
    stds.push_back(1.5); // diastolic_bp_2    
    stds.push_back(0.2); // cardiac_output
    stds.push_back(50.0); // systemic_vascular_resistan
    stds.push_back(5.0); // pulmonary_vascular_resista
    stds.push_back(0.5); // cvp
    stds.push_back(1.0); // right_ventricle_diastole
    stds.push_back(1.0); // right_ventricle_systole
    stds.push_back(1.0); // rvedp
    stds.push_back(0.5); // aov_mean_pg
    stds.push_back(0.5); // aov_peak_pg
    stds.push_back(6.0); // mv_decel_time
    stds.push_back(0.2); // mv_e_a_ratio
    stds.push_back(6.0); // pv_at
    stds.push_back(0.5); // pv_max_pg
    stds.push_back(0.5); // ra_pressure
    stds.push_back(3.0); // ra_vol_a4c
    stds.push_back(3.0); // la_vol_a4c
    stds.push_back(10.0); // lv_esv
    stds.push_back(20.0); // lv_vol_a4c
    stds.push_back(2.0); // lvef
    stds.push_back(1.0); // lvot_max_flow
    // Pulmonary
    stds.push_back(1.0); // pap_diastolic
    stds.push_back(1.0); // pap_systolic
    stds.push_back(1.0); // wedge_pressure
  }
}

void odeNormalAdultSimplePD::getResultWeigths(stdVec& weights){
  if((modelVersion >= 0001)&&(modelVersion < 0002)){  
    weights.clear();
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
  }else if(modelVersion >= 0002){
    weights.clear();
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
    weights.push_back(1.0);
  }
}

