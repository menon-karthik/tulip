# include "odeNormalAdult.h"

using namespace std;

odeNormalAdult::odeNormalAdult(){
}

odeNormalAdult::~odeNormalAdult(){
}

int odeNormalAdult::getParameterTotal(){
  return 89;
}

int odeNormalAdult::getStateTotal(){
  return 28; 
}

int odeNormalAdult::getAuxStateTotal(){
  return 22; 	
}

int odeNormalAdult::getResultTotal(){
  return 5;  
}

string odeNormalAdult::getResultName(int resID){
  throw cmException("ERROR: odeNormalAdult::getResultName Not Implemented.");
}

string odeNormalAdult::getParamName(int index){
  string result;
  switch(index){
    case 0: {          
      result = "Tsa";
      break; }
    case 1: {      
      result = "tpwave";
      break; }
  }
  return result;
}

void odeNormalAdult::getDefaultParams(stdVec& params){

  // Resize Parameter Array
  params.resize(getParameterTotal());

  // NOTE: CGS Units: Pressures in Barye, Flowrates in mL/s

  // Initial State Values
  params[0] = 0.0; // V_ra
  params[1] = 0.0; // V_la
  params[2] = 0.0; // V_rv
  params[3] = 0.0; // V_lv
  params[4] = 0.0; // Q_ra_rv
  params[5] = 70.0 * convertmmHgToBarye; // P_pa
  params[6] = 0.0; // Q_rv_pa
  params[7] = 0.0; // Q_la_lv
  params[8] = 100.0 * convertmmHgToBarye; // P_ao
  params[9] = 0.0; // Q_lv_ao
  params[10] = 50.0 * convertmmHgToBarye; // P_ub
  params[11] = 0.0; // Q_uba
  params[12] = 10.0 * convertmmHgToBarye; // P_svc
  params[13] = 0.0; // Q_thao
  params[14] = 120.0 * convertmmHgToBarye; // P_thao
  params[15] = 120.0 * convertmmHgToBarye; // P_abao
  params[16] = 0.0; // Q_abao
  params[17] = 50.0 * convertmmHgToBarye; // P_i
  params[18] = 50.0 * convertmmHgToBarye; // P_lega
  params[19] = 50.0 * convertmmHgToBarye; // P_abivc
  params[20] = 50.0 * convertmmHgToBarye; // P_legv
  params[21] = 0.0; // Q_lega
  params[22] = 10.0 * convertmmHgToBarye; // P_thivc
  params[23] = 0.0; // Q_lla
  params[24] = 120.0 * convertmmHgToBarye; // P_ll
  params[25] = 0.0; // Q_ka
  params[26] = 120.0 * convertmmHgToBarye; // P_k
  params[27] = 0.0; // Q_ia
  // --- Heart Cycle Parameters
  params[28+0] = 78.0; // HR - Heart Rate (beats per minute)
  // --- Atrial Model Parameters
  params[28+1] = 0.4; // tsas - Atrial relative activation duration
  params[28+2] = 9.5; // tpws - Atrial relative activation time shift
  params[28+3] = 4.0; // K_pas_ra_1 - Atrial passive curve slope, right atrium
  params[28+4] = 0.006; // K_pas_ra_2 - Atrial passive curve exponent factor, right atrium
  params[28+5] = 0.1; // Emax_ra - Atrial active curve slope, right atrium
  params[28+6] = 0.0; // Vra0 - Unstressed right atrial volume
  params[28+7] = 8.0; // K_pas_la_1 - Atrial passive curve slope, left atrium
  params[28+8] = 0.0065; // K_pas_la_2 - Atrial passive curve exponent factor, left atrium
  params[28+9] = 0.5; // Emax_la - Atrial active curve slope, left atrium
  params[28+10] = 0.0; // Vla0 - Unstressed left atrial volume
  // --- Ventricular Model Parameters
  params[28+11] = 0.75 * 0.8; // Ervs - Right Ventricular Elastance Scaling Factor
  params[28+12] = 1.3 * 0.8; // Elvs - Left Ventricular Elastance Scaling Factor
  params[28+13] = 6.0; // Elvps - Left Ventricular Elastance Derivative Scaling Factor
  params[28+14] = 0.0; // Vrv0 - Unstressed right ventricular volume
  params[28+15] = 0.0; // Vlv0 - Unstressed left ventricular volume [ml]
  // --- Atrial and Ventricular Inductances and Resistances
  params[28+16] = 0.1;   // L_ra_rv - Inductance of right atrium
  params[28+17] = 20.0;  // R_ra_rv - Resistance of right atrium
  params[28+18] = 0.1;  // L_rv_pa - Inductance of right ventricle
  params[28+19] = 30.0;  // R_rv_pa - Resistance of right ventricle
  params[28+20] = 0.1;   // L_la_lv - Inductance of left atrium
  params[28+21] = 50.0;  // R_la_lv - Resistance of left atrium
  params[28+22] = 0.1;   // L_lv_ao - Inductance of left ventricle
  params[28+23] = 20.0; // R_lv_ao - Resistance of left ventricle
  // --- Pulmonary Resistance and Capacitance
  params[28+24] = 5.0 / convertmmHgToBarye; // C_pa - Pulmonary capacitance
  params[28+25] = 50.0; // R_pa - Pulmonary resistance
  // --- Aortic Arch
  params[28+26] = 0.5 / convertmmHgToBarye; // C_ao - Aortic capacitance
  // --- Upped Body Circulation Parameters
  params[28+27] = 0.1; // L_uba - Upper body arterial inductance
  params[28+28] = 200.0; // R_uba - Upper body arterial resistance
  params[28+29] = 100.0e-6; // C_ub - Upper body compliance
  params[28+30] = 700.0; // R_ubv - Upper body venous resistance
  // --- Superior Vena Cava
  params[28+31] = 400.0e-6; // C_svc - Compliance of SVC
  params[28+32] = 700.0; // R_svc - SVC Resistance
  // --- Thoracic Aorta
  params[28+33] = 1.0; // L_thao - Inductance of thoracic aorta
  params[28+34] = 15.0; // R_thao - Resistance of thoracic aorta
  params[28+35] = 300.0e-6;//300.0e-6; // C_thao - Capacitance of thoracic aorta
  // --- Abdominal Aorta
  params[28+36] = 50.0; // R_abao - Resistance of abdominal aorta
  params[28+37] = 1.0; // L_abao - Inductance of abdominal aorta
  params[28+38] = 100.0e-6;//75.0e-6; // C_abao - Capacitance of abdominal aorta
  // --- Legs
  params[28+39] = 0.1; // L_lega - Inductance of leg arteries
  params[28+40] = 100.0; // R_lega - Resistance of leg arteries
  params[28+41] = 450.0e-6; // C_lega - Capacitance of leg arteries
  params[28+42] = 1000.0; // R_legc - Resistance of leg capillary circulation
  params[28+43] = 6000.0e-6; // C_legv - Capacitance of leg venous circulation
  params[28+44] = 500.0; // R_legv - Resistance of leg venous circulation
  // --- Abdominal IVC - Inferior Vena Cava
  params[28+45] = 1500.0e-6; // C_abivc - Capacitance of abdominal IVC
  params[28+46] = 50.0;//0.83; // R_abivc - Resistance of abdominal IVC
  // --- Thoracic IVC
  params[28+47] = 900.0e-6; // C_thivc - Capacitance of thoracic IVC
  params[28+48] = 700.0;//0.63; // R_thivc - Resistance of thoracic IVC
  // --- Liver
  params[28+49] = 10.0; // L_lla - Inductance of Liver arterial circulation
  params[28+50] = 200.0;//7000.0; // R_lla - Resistance of Liver arterial circulation
  params[28+51] = 1890.0e-6; // C_ll - Capacitance of Liver circulation
  params[28+52] = 2000.0; // R_llv - Resistance of Liver venous circulation
  // --- Kidneys  
  params[28+53] = 10.0; // L_ka - Inductance of arterial circulation in the Kidneys
  params[28+54] = 200.0;//5100.0; // R_ka - Resistance of arterial circulation in the Kidneys
  params[28+55] = 7200.0e-6; // C_k - Capacitance of circulation in the Kidneys
  params[28+56] = 2000.0; // R_kv - Resistance of venous circulation in the Kidneys
  // --- Intestine
  params[28+57] = 10.0; // L_ia - Inductance of arterial circulation in the Intestine
  params[28+58] = 200.0;//46000.0; // R_ia - Resistance of arterial circulation in the Intestine
  params[28+59] = 1130.0e-6; // C_i - Capacitance of circulation in the Intestine
  params[28+60] = 2000.0; // R_iv - Resistance of venous circulation in the Intestine
}

void odeNormalAdult::getParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());
  
  // Set Limits for initial conditions
  limits[0] = 0.0; limits[1] = 0.0; // V_ra
  limits[2] = 0.0; limits[3] = 0.0; // V_la
  limits[4] = 0.0; limits[5] = 0.0; // V_rv
  limits[6] = 0.0; limits[7] = 0.0; // V_lv
  limits[8] = 0.0; limits[9] = 0.0; // Q_ra_rv
  limits[10] = 0.0; limits[11] = 0.0; // P_pa
  limits[12] = 0.0; limits[13] = 0.0; // Q_rv_pa
  limits[14] = 0.0; limits[15] = 0.0; // Q_la_lv
  limits[16] = 0.0; limits[17] = 0.0; // P_ao
  limits[18] = 0.0; limits[19] = 0.0; // Q_lv_ao
  limits[20] = 0.0; limits[21] = 0.0; // P_ub
  limits[22] = 0.0; limits[23] = 0.0; // Q_uba
  limits[24] = 0.0; limits[25] = 0.0; // P_svc
  limits[26] = 0.0; limits[27] = 0.0; // Q_thao
  limits[28] = 0.0; limits[29] = 0.0; // P_thao
  limits[30] = 0.0; limits[31] = 0.0; // P_abao
  limits[32] = 0.0; limits[33] = 0.0; // Q_abao
  limits[34] = 0.0; limits[35] = 0.0; // P_i
  limits[36] = 0.0; limits[37] = 0.0; // P_lega
  limits[38] = 0.0; limits[39] = 0.0; // P_abivc
  limits[40] = 0.0; limits[41] = 0.0; // P_legv
  limits[42] = 0.0; limits[43] = 0.0; // Q_lega
  limits[44] = 0.0; limits[45] = 0.0; // P_thivc
  limits[46] = 0.0; limits[47] = 0.0; // Q_lla
  limits[48] = 0.0; limits[49] = 0.0; // P_ll
  limits[50] = 0.0; limits[51] = 0.0; // Q_ka
  limits[52] = 0.0; limits[53] = 0.0; // P_k
  limits[54] = 0.0; limits[55] = 0.0; // Q_ia

  // --- Heart Cycle Parameters
  limits[56] = 0.0; limits[57] = 0.0;  // HR - Heart Rate
  // --- Atrial Model Parameters
  limits[58] = 0.0; limits[59] = 0.0;  // tsas - Atrial relative activation duration
  limits[60] = 0.0; limits[61] = 0.0;  // tpws - Atrial relative activation time shift
  limits[62] = 0.0; limits[63] = 0.0;  // K_pas_ra_1 - Atrial passive curve slope, right atrium
  limits[64] = 0.0; limits[65] = 0.0;  // K_pas_ra_2 - Atrial passive curve exponent factor, right atrium
  limits[66] = 0.0; limits[67] = 0.0;  // Emax_ra - Atrial active curve slope, right atrium
  limits[68] = 0.0; limits[69] = 0.0;  // Vra0 - Unstressed right atrial volume
  limits[70] = 0.0; limits[71] = 0.0;  // K_pas_la_1 - Atrial passive curve slope, left atrium
  limits[72] = 0.0; limits[73] = 0.0;  // K_pas_la_2 - Atrial passive curve exponent factor, left atrium
  limits[74] = 0.0; limits[75] = 0.0;   // Emax_la - Atrial active curve slope, left atrium
  limits[76] = 0.0; limits[77] = 0.0;  // Vla0 - Unstressed left atrial volume
  // --- Ventricular Model Parameters
  limits[78] = 0.0; limits[79] = 0.0;  // Ervs - Right Ventricular Elastance Scaling Factor
  limits[80] = 0.0; limits[81] = 0.0;  // Elvs - Left Ventricular Elastance Scaling Factor
  limits[82] = 0.0; limits[83] = 0.0;  // Elvps - Left Ventricular Elastance Derivative Scaling Factor
  limits[84] = 0.0; limits[85] = 0.0;  // Vrv0 - Unstressed right ventricular volume
  limits[86] = 0.0; limits[87] = 0.0;  // Vlv0 - Unstressed left ventricular volume
  // --- Atrial and Ventricular Inductances and Resistances
  limits[88] = 0.0; limits[89] = 0.0;  // L_ra_rv - Inductance of right atrium
  limits[90] = 0.0; limits[91] = 0.0;  // R_ra_rv - Resistance of right atrium
  limits[92] = 0.0; limits[93] = 0.0;  // L_rv_pa - Inductance of right ventricle
  limits[94] = 0.0; limits[95] = 0.0;  // R_rv_pa - Resistance of right ventricle
  limits[96] = 0.0; limits[97] = 0.0;  // L_la_lv - Inductance of left atrium
  limits[98] = 0.0; limits[99] = 0.0;  // R_la_lv - Resistance of left atrium
  limits[100] = 0.0; limits[101] = 0.0;  // L_lv_ao - Inductance of left ventricle
  limits[102] = 0.0; limits[103] = 0.0;  // R_lv_ao - Resistance of left ventricle
  // --- Pulmonary Resistance and Capacitance
  limits[104] = 0.0; limits[105] = 0.0;  // C_pa - Pulmonary capacitance
  limits[106] = 0.0; limits[107] = 0.0;  // R_pa - Pulmonary resistance
  // --- Aortic Arch
  limits[108] = 0.0; limits[109] = 0.0;  // C_ao - Aortic capacitance
  // --- Upped Body Circulation Parameters
  limits[110] = 0.0; limits[111] = 0.0;  // L_uba - Upper body arterial inductance
  limits[112] = 0.0; limits[113] = 0.0;  // R_uba - Upper body arterial resistance
  limits[114] = 0.0; limits[115] = 0.0;  // R_ubv - Upper body venous resistance
  // --- Superior Vena Cava
  limits[116] = 0.0; limits[117] = 0.0;  // C_svc - Compliance of SVC
  limits[118] = 0.0; limits[119] = 0.0;  // R_svc - SVC Resistance
  // --- Thoracic Aorta
  limits[120] = 0.0; limits[121] = 0.0;  // L_thao - Inductance of thoracic aorta
  limits[122] = 0.0; limits[123] = 0.0;  // R_thao - Resistance of thoracic aorta
  limits[124] = 0.0; limits[125] = 0.0;  // C_thao - Capacitance of thoracic aorta
  // --- Abdominal Aorta
  limits[126] = 0.0; limits[127] = 0.0;  // R_abao - Resistance of abdominal aorta
  limits[128] = 0.0; limits[129] = 0.0;  // L_abao - Inductance of abdominal aorta
  limits[130] = 0.0; limits[131] = 0.0;  // C_abao - Capacitance of abdominal aorta
  // --- Legs
  limits[132] = 0.0; limits[133] = 0.0;  // L_lega - Inductance of leg arteries
  limits[134] = 0.0; limits[135] = 0.0;  // R_lega - Resistance of leg arteries
  limits[136] = 0.0; limits[137] = 0.0;  // C_lega - Capacitance of leg arteries
  limits[138] = 0.0; limits[139] = 0.0;  // R_legc - Resistance of leg capillary circulation
  limits[140] = 0.0; limits[141] = 0.0;  // C_legv - Capacitance of leg venous circulation
  limits[142] = 0.0; limits[143] = 0.0;  // R_legv - Resistance of leg venous circulation
  // --- Abdominal IVC - Inferior Vena Cava
  limits[144] = 0.0; limits[145] = 0.0;  // C_abivc - Capacitance of abdominal IVC
  limits[146] = 0.0; limits[147] = 0.0;  // R_abivc - Resistance of abdominal IVC
  // --- Thoracic IVC
  limits[148] = 0.0; limits[149] = 0.0;  // C_thivc - Capacitance of thoracic IVC
  limits[150] = 0.0; limits[151] = 0.0;  // R_thivc - Resistance of thoracic IVC
  // --- Liver
  limits[152] = 0.0; limits[153] = 0.0;  // L_lla - Inductance of Liver arterial circulation
  limits[154] = 0.0; limits[155] = 0.0;  // R_lla - Resistance of Liver arterial circulation
  limits[156] = 0.0; limits[157] = 0.0;  // C_ll - Capacitance of Liver circulation
  limits[158] = 0.0; limits[159] = 0.0;  // R_llv - Resistance of Liver venous circulation
  // --- Kidneys  
  limits[160] = 0.0; limits[161] = 0.0;  // L_ka - Inductance of arterial circulation in the Kidneys
  limits[162] = 0.0; limits[162] = 0.0;  // R_ka - Resistance of arterial circulation in the Kidneys
  limits[164] = 0.0; limits[165] = 0.0;  // C_k - Capacitance of circulation in the Kidneys
  limits[166] = 0.0; limits[167] = 0.0;  // R_kv - Resistance of venous circulation in the Kidneys
  // --- Intestine
  limits[168] = 0.0; limits[169] = 0.0;  // L_ia - Inductance of arterial circulation in the Intestine
  limits[170] = 0.0; limits[171] = 0.0;  // R_ia - Resistance of arterial circulation in the Intestine
  limits[172] = 0.0; limits[173] = 0.0;  // C_i - Capacitance of circulation in the Intestine
  limits[174] = 0.0; limits[175] = 0.0;  // R_iv - Resistance of venous circulation in the Intestine
}

double evalElastance(double tr){

  // Set Parameters
  double eMax = 2.31;
  double eMin = 0.06;
  double a1 = 0.303;
  double a2 = 0.508;
  double n1 = 1.32;
  double n2 = 21.9;

  // Eval Elastance
  double num1 = pow(tr/a1,n1);
  double den1 = 1.0 + num1;
  double factor2 = 1.0 / (1.0 + pow(tr/a2,n2));
  return eMin + eMax* (( num1 / den1 ) * factor2);
}

void odeNormalAdult::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut){

  // Assign the Parameters
  double HR         = params[0]; // Heart Rate
  // Atrial Model Parameters
  double tsas       = params[1]; // Atrial relative activation duration
  double tpws       = params[2]; // Atrial relative activation time shift
  double K_pas_ra_1 = params[3];
  double K_pas_ra_2 = params[4];
  double Emax_ra    = params[5];
  double Vra0       = params[6];
  double K_pas_la_1 = params[7];
  double K_pas_la_2 = params[8];
  double Emax_la    = params[9];
  double Vla0       = params[10];
  // Ventricular Model Parameters
  double Ervs       = params[11]; // Right Ventricular Elastance Scaling Factor
  double Elvs       = params[12]; // Left Ventricular Elastance Scaling Factor
  double Elvps      = params[13]; // Left Ventricular Elastance Derivative Scaling Factor
  double Vrv0       = params[14];
  double Vlv0       = params[15];
  // Atrial and Ventricular Inductances and Resistances
  double L_ra_rv    = params[16];
  double R_ra_rv    = params[17];
  double L_rv_pa    = params[18];
  double R_rv_pa    = params[19];
  double L_la_lv    = params[20];
  double R_la_lv    = params[21];
  double L_lv_ao    = params[22];
  double R_lv_ao    = params[23];
  // Pulmonary Resistance and Capacitance
  double C_pa       = params[24];
  double R_pa       = params[25];
  // Aortic Arch
  double C_ao       = params[26];
  // Upped Body Circulation Parameters
  double L_uba      = params[27];
  double R_uba      = params[28];
  double C_ub       = params[29];
  double R_ubv      = params[30];
  // svc
  double C_svc      = params[31];
  double R_svc      = params[32];
  // Thoracic Aorta
  double L_thao     = params[33];
  double R_thao     = params[34];
  double C_thao     = params[35];
  // Abdominal Aorta
  double R_abao     = params[36];
  double L_abao     = params[37];
  double C_abao     = params[38];
  // Legs
  double L_lega     = params[39];
  double R_lega     = params[40];
  double C_lega     = params[41];
  double R_legc     = params[42];
  double C_legv     = params[43];
  double R_legv     = params[44];
  // Abdominal ivc
  double C_abivc    = params[45];
  double R_abivc    = params[46];
  // Thoracic ivc
  double C_thivc    = params[47];
  double R_thivc    = params[48];
  // Liver
  double L_lla      = params[49];
  double R_lla      = params[50];
  double C_ll       = params[51];
  double R_llv      = params[52];
  // Kidneys  
  double L_ka       = params[53];
  double R_ka       = params[54];
  double C_k        = params[55];
  double R_kv       = params[56];
  // Intestine
  double L_ia       = params[57];
  double R_ia       = params[58];
  double C_i        = params[59];
  double R_iv       = params[60];

  // Assign state variables
  double V_ra    = Xk[0];
  double V_la    = Xk[1];
  double V_rv    = Xk[2];
  double V_lv    = Xk[3];
  double Q_ra_rv = Xk[4];
  double P_pa    = Xk[5];
  double Q_rv_pa = Xk[6];
  double Q_la_lv = Xk[7];
  double P_ao    = Xk[8];
  double Q_lv_ao = Xk[9];
  double P_ub    = Xk[10];
  double Q_uba   = Xk[11];
  double P_svc   = Xk[12];
  double Q_thao  = Xk[13];
  double P_thao  = Xk[14];
  double P_abao  = Xk[15];
  double Q_abao  = Xk[16];
  double P_i     = Xk[17];
  double P_lega  = Xk[18];
  double P_abivc = Xk[19];
  double P_legv  = Xk[20];
  double Q_lega  = Xk[21];
  double P_thivc = Xk[22];
  double Q_lla   = Xk[23];
  double P_ll    = Xk[24];
  double Q_ka    = Xk[25];
  double P_k     = Xk[26];
  double Q_ia    = Xk[27];	

  // SET OPTIONS
  bool completeModel = true;
  bool printMessages = false;
  int totalStates = Xk.size();

  // Set Time integration parameters
  double tc = 60.0/HR;
  double tsa = tc * tsas;
  double tpw = tc/(double)tpws;
  double tcr = fmod(t,tc);

  // ====================================================
  // PART I - ATRIAL ACTIVATION AND VENTRICULAR ELASTANCE
  // ====================================================

  // Evaluate activation Function for atrial contraction
  double fAA = 0.0;
  if(tcr <= tpw){
    fAA = (0.5)*(1.0-cos(2.0*M_PI*(tcr-tpw+tsa)/tsa));
  }else if((tcr>=(tc-tsa)+tpw)and(tcr<tc)){
    fAA = (0.5)*(1.0-cos(2.0*M_PI*(tcr-tpw-(tc-tsa))/tsa));
  }else{
    fAA = 0.0;
  }

  // Compute exponential atrial passive pressures curves
  double P_pas_ra = K_pas_ra_1*(exp((V_ra-Vra0)*K_pas_ra_2)-1.0);
  double P_pas_la = K_pas_la_1*(exp((V_la-Vla0)*K_pas_la_2)-1.0);

  // Compute linear atrial active pressure curves
  double P_act_ra = Emax_ra*(V_ra-Vra0);
  double P_act_la = Emax_la*(V_la-Vla0);

  // Obtain the right and left ventricular pressures using passive values,
  // active values and activation
  double P_ra = (P_pas_ra + fAA * (P_act_ra - P_pas_ra)) * convertmmHgToBarye;
  double P_la = (P_pas_la + fAA * (P_act_la - P_pas_la)) * convertmmHgToBarye;

  // Compute elastance functions for ventricular contraction
  double El = evalElastance(tcr/tc);

  // Scale Elastances for right and left ventricles
  double Erv = El * Ervs;
  double Elv = El * Elvs;

  // Compute the right and left ventricular pressure using 
  // the elastance formulation
  double P_rv = Erv * (V_rv - Vrv0) * convertmmHgToBarye;
  double P_lv = Elv * (V_lv - Vlv0) * convertmmHgToBarye;

  // ========================
  // PART II - HEART CHAMBERS
  // ========================

  // Initialize variables for valves 
  // 1.0 - Valve Open; 0.0 - Valve Closed
  stdVec Ind(totalStates,0.0);
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

  // Check if RV-PA Valve is open
  if(P_pa >= P_rv){
    Ind[6]=0.0;
  }

  // Right Ventricle
  double Q_rv_pa_p = 0.0;
  if(Ind[6]>0.0){
    Q_rv_pa_p = (1.0/L_rv_pa)*(P_rv - P_pa - R_rv_pa * Q_rv_pa);
  }

  // Pulmonary Circulation: Only Capacitance Equation
  double Q_pul = (P_pa - P_la)/R_pa;
  double P_pa_p = ( 1.0 / C_pa )*( Q_rv_pa * Ind[6] - Q_pul );

  // Check if LA-LV Valve is open
  if(P_lv >= P_la){
    Ind[7]=0.0;
  }

  // Left Atrium
  double Q_la_lv_p = 0.0;
  if(Ind[7]>0.0){
    Q_la_lv_p = (1.0/L_la_lv)*( P_la - P_lv - R_la_lv * Q_la_lv );
  }

  // Check if LV-AO Valve is open
  if(P_ao >= P_lv){
    Ind[9]=0.0;
  }

  // Left Ventricle
  double Q_lv_ao_p = 0.0;
  if(Ind[9]>0.0){
    Q_lv_ao_p = (1.0/L_lv_ao)*(P_lv - P_ao - R_lv_ao * Q_lv_ao);
  }

  // Flow in SVC
  double Q_svc = ( P_svc - P_ra ) / R_svc;

  // Flow in Thoracic IVC
  double Q_thivc = ( P_thivc - P_ra ) / R_thivc;

  // ====================================================
  // COMPUTE THE VARIATION IN VOLUME FOR ALL THE CHAMBERS
  // ====================================================

  double V_ra_p = 0.0;
  double V_la_p = 0.0;
  double V_rv_p = 0.0;
  double V_lv_p = 0.0; 
  double Q_sys = 0.0;
  if(completeModel){
    V_ra_p = ( Q_svc + Q_thivc ) - Q_ra_rv * Ind[4];
    V_rv_p = Q_ra_rv * Ind[4] - Q_rv_pa * Ind[6];
    V_la_p = Q_pul - Q_la_lv * Ind[7];
    V_lv_p = Q_la_lv * Ind[7] - Q_lv_ao * Ind[9];
  }else{
    // Add Systemic Resistance
    Q_sys = ( P_ao - P_ra ) / R_svc;
    //
    V_ra_p = Q_sys - Q_ra_rv * Ind[4];
    V_rv_p = Q_ra_rv * Ind[4] - Q_rv_pa * Ind[6];
    V_la_p = Q_pul - Q_la_lv * Ind[7];
    V_lv_p = Q_la_lv * Ind[7] - Q_lv_ao * Ind[9];
  }

  // ======================
  // PART III - AORTIC ARCH
  // ======================

  // Aortic arch capacitance
  double P_ao_p = 0.0;
  if(completeModel){
    P_ao_p = (1.0/C_ao) * ( Q_lv_ao * Ind[9] - Q_uba - Q_thao );
  }else{
  	P_ao_p = (1.0/C_ao) * ( Q_lv_ao - Q_sys );
  }

  // ================================
  // PART IV - UPPER BODY CIRCULATION
  // ================================

  // Upper Body Circulation: Inductance Equation
  double Q_uba_p = 0.0;
  if(completeModel){
    Q_uba_p = (1.0/L_uba) * ( P_ao - R_uba * Q_uba - P_ub );
  }
  // Upper Body Circulation: Capacitance Equation
  double Q_ubv = 0.0;
  double P_ub_p = 0.0;
  if(completeModel){
    Q_ubv = ( P_ub - P_svc ) / R_ubv;
    P_ub_p = (1.0/C_ub) * ( Q_uba - Q_ubv );
  }

  // ============
  // PART V - SVC
  // ============

  // Superior Vena Cava
  double P_svc_p = 0.0;
  if(completeModel){
    P_svc_p = (1.0/C_svc) * ( Q_ubv - Q_svc );
  }

  // =====================================
  // PART V - THORACIC AND ABDOMINAL AORTA
  // =====================================

  // Thoracic Aorta
  double Q_thao_p = 0.0;
  double P_thao_p = 0.0;
  if(completeModel){
    Q_thao_p = (1.0/L_thao) * ( P_ao - R_thao * Q_thao - P_thao );
    P_thao_p = (1.0/C_thao) * ( Q_thao - Q_abao - Q_lla - Q_ka );
  }

  // Abdominal Aorta
  double Q_abao_p = 0.0;
  double P_abao_p = 0.0;
  if(completeModel){
    Q_abao_p = (1.0/L_abao) * ( P_thao - R_abao * Q_abao - P_abao );
    P_abao_p = (1.0/C_abao) * ( Q_abao - Q_ia - Q_lega );
  }

  // ================================
  // PART VI - LOWER BODY CIRCULATION
  // ================================

  // Check if LV-AO Valve is open
  double legValve = 1.0;
  if(P_abivc >= P_legv){
    legValve = 0.0;
  }

  // Lower Body Circulation
  double Q_lega_p = 0.0;
  double Q_legc = 0.0;
  double P_lega_p = 0.0;
  double Q_legv = 0.0;
  double P_legv_p = 0.0;
  if(completeModel){
    Q_lega_p = (1.0/L_lega) * ( P_abao - R_lega * Q_lega - P_lega );
    Q_legc = ( P_lega - P_legv ) / R_legc;
    P_lega_p = (1.0/C_lega) * ( Q_lega - Q_legc );
    Q_legv = ( P_legv - P_abivc ) / R_legv ;
    P_legv_p = ( 1.0 / C_legv ) * ( Q_legc - Q_legv * legValve);
  }

  // =====================================
  // PART VII - ABDOMINAL AND THORACIC IVC
  // =====================================

  // Abdominal IVC
  double Q_abivc = 0.0;
  double P_abivc_p = 0.0;
  if(completeModel){
    Q_abivc = ( P_abivc - P_thivc ) / R_abivc;
    P_abivc_p = ( 1.0 / C_abivc ) * ( Q_legv * legValve - Q_abivc );    
  }

  // Thoracic IVC
  double P_thivc_p = 0.0;
  double Q_llv = ( P_ll - P_thivc ) / R_llv ;
  double Q_kv = ( P_k - P_thivc ) / R_kv ;
  if(completeModel){
    P_thivc_p = ( 1.0 / C_thivc ) * ( Q_abivc + Q_llv + Q_kv - Q_thivc );
  }

  // ===========================
  // PART VIII - INTERNAL ORGANS
  // ===========================

  // Liver Circulation
  double Q_lla_p = 0.0;
  double Q_iv = 0.0;
  double P_ll_p = 0.0;
  if(completeModel){
    Q_lla_p = ( 1.0 / L_lla ) * ( P_thao - R_lla * Q_lla - P_ll );
    Q_iv = ( P_i - P_ll ) / R_iv ;    
    P_ll_p = ( 1.0 / C_ll ) * ( Q_lla + Q_iv - Q_llv );
  }

  // Circulation in the kidneys
  double Q_ka_p = 0.0;
  double P_k_p = 0.0;
  if(completeModel){
    Q_ka_p = (1.0 / L_ka ) * ( P_thao - R_ka * Q_ka - P_k );
    P_k_p = (1.0 / C_k ) * ( Q_ka - Q_kv );
  }

  // Circulation in the intestine
  double Q_ia_p = 0.0;
  double P_i_p = 0.0;
  if(completeModel){
    Q_ia_p = (1.0 / L_ia ) * ( P_abao - R_ia * Q_ia - P_i );
    P_i_p = (1.0 / C_i ) * ( Q_ia - Q_iv );
  }

  // Store the derivatives
  DXk[0] = V_ra_p;
  DXk[1] = V_la_p;
  DXk[2] = V_rv_p;
  DXk[3] = V_lv_p;
  DXk[4] = Q_ra_rv_p;
  DXk[5] = P_pa_p;
  DXk[6] = Q_rv_pa_p;
  DXk[7] = Q_la_lv_p;
  DXk[8] = P_ao_p;
  DXk[9] = Q_lv_ao_p;
  DXk[10] = P_ub_p;
  DXk[11] = Q_uba_p;
  DXk[12] = P_svc_p;
  DXk[13] = Q_thao_p;
  DXk[14] = P_thao_p;
  DXk[15] = P_abao_p;
  DXk[16] = Q_abao_p;
  DXk[17] = P_i_p;
  DXk[18] = P_lega_p;
  DXk[19] = P_abivc_p;
  DXk[20] = P_legv_p;
  DXk[21] = Q_lega_p;
  DXk[22] = P_thivc_p;
  DXk[23] = Q_lla_p;
  DXk[24] = P_ll_p;
  DXk[25] = Q_ka_p;
  DXk[26] = P_k_p;
  DXk[27] = Q_ia_p;

  // Get Auxiliary Results
  auxOut[0] = t; // Current Time
  auxOut[1] = tcr; // Relative Cycle Time
  auxOut[2] = fAA; // Atrial Activation Function
  auxOut[3] = Erv; // Right Ventricle Elastance
  auxOut[4] = Elv; // Left Ventricular Elastance
  auxOut[5] = P_ra; // Right Atrial Pressure
  auxOut[6] = P_la; // Left Atrial Pressure
  auxOut[7] = P_rv; // Right Ventricular Pressure
  auxOut[8] = P_lv; // Left Ventricular Pressure
  auxOut[9] = Q_pul; // Pulmonary flow rate
  auxOut[10] = Q_sys; // Systemic flow Rate (Reduced Model)
  auxOut[11] = Q_ubv; // Upper Body flow rate
  auxOut[12] = Q_legc; // Capillary Leg flow rate
  auxOut[13] = Q_legv; // Venous leg flow rate
  auxOut[14] = Q_abivc; // Abdominal IVC flow rate
  auxOut[15] = Q_thivc; // Thoracic IVC flow rate
  auxOut[16] = Q_iv; // Venous flow rate in the intestine
  auxOut[17] = Ind[4]; // RA-RV valve status
  auxOut[18] = Ind[6]; // RV-PA valve status
  auxOut[19] = Ind[7]; // LA-LV valve status
  auxOut[20] = Ind[9]; // LV-AO valve status
  auxOut[21] = legValve; // Leg valve status
}

// Post-process results
void odeNormalAdult::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  int numCycles = int(totalSteps/(double)totalStepsOnSingleCycle);

  // Cardiac Output
  int startLastCycle = (numCycles-1) * totalStepsOnSingleCycle;
  int stopLastCycle = numCycles * totalStepsOnSingleCycle;
  double output[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = outVals[9][loopA];
  }
  double CO = getMean(startLastCycle, stopLastCycle, output);

  // Write Results
  results.push_back(CO);
}

