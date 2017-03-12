
#include "cmLPN_SV_heart.h"

using namespace std;

// SIMULATION COUNTER
int simulCount = 0;

// Numerical Constants
const double pi = 3.1415926535897;

// Output State Constants
const int ipVsv = 0;
const int ipVsa = 1;
const int ipPAo = 2;
const int ipQav = 3;

// Auxiliary Output Constants
const int ipPsvEff     = 0;
const int ipPsa        = 1;
const int ipQAo        = 2;
const int ipQven       = 3;
const int ipLVOF       = 4;
const int iptc         = 5;
const int ipAA         = 6;
const int ipAV         = 7;
const int ipPsaActive  = 8;
const int ipPsaPassive = 9;
const int ipPsvActive  = 10;
const int ipPsvPassive = 11;
const int ipQreg       = 12;

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int cmLPN_SV_Heart::getParameterTotal(){
  return 25;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int cmLPN_SV_Heart::getStateTotal(){
  return 4; 
}

// ========================
// GET NUMBER OF AUX STATES
// ========================

int cmLPN_SV_Heart::getAuxStateTotal(){
  return 13;  
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int cmLPN_SV_Heart::getResultTotal(){
  return 8;  
}

// ===========================
// GET NAME OF RESULT QUANTITY
// ===========================
string cmLPN_SV_Heart::getResultName(int index){
  switch(index){
    case 0:
    {      
      return string("mPascAo [mmHg]");
      break;
    }
    case 1:
    {      
      return string("maxPAo [mmHg]");
      break;
    }
    case 2:
    {      
      return string("mPsa [mmHg]");
      break;
    }
    case 3:
    {      
      return string("maxPsv [mmHg]");
      break;
    }
    case 4:
    {      
      return string("maxVsv [mL]");
      break;
    }
    case 5:
    {      
      return string("PsvZero [mmHg]");
      break;
    }    
    case 6:
    {      
      return string("maxPsa [mmHg]");
      break;
    }
    case 7:
    {      
      return string("mQreg [mL/s]");
      break;
    }        

  }
}

// ==================
// GET PARAMETER NAME
// ==================
string cmLPN_SV_Heart::getParamName(int index){
  switch(index){
    case 0:
    {
      return string("Heart Rate");
      break;
    }
    case 1:
    {
      return string("Csa");
      break;
    }
    case 2:
    {      
      return string("csa");
      break;
    }
    case 3:
    {      
      return string("dsa");
      break;
    }
    case 4:
    {      
      return string("Vsa0");
      break;
    }
    case 5:
    {      
      return string("tsas-ratio");
      break;
    }
    case 6:
    {      
      return string("t1-ratio");
      break;
    }
    case 7:
    {      
      return string("a");
      break;
    }
    case 8:
    {      
      return string("b");
      break;
    }
    case 9:
    {      
      return string("csv");
      break;
    }
    case 10:
    {      
      return string("dsv");
      break;
    }
    case 11:
    {      
      return string("Vsv0");
      break;
    }
    case 12:
    {      
      return string("tsvs-ratio");
      break;
    }    
    case 13:
    {      
      return string("Rmyo");
      break;
    }
    case 14:
    {      
      return string("alpha");
      break;
    }
    case 15:
    {      
      return string("Kao");
      break;
    }
    case 16:
    {      
      return string("CAo");
      break;
    }
    case 17:
    {      
      return string("tdelay1");
      break;
    }
    case 18:
    {      
      return string("tdelay2");
      break;
    }
    case 19:
    {      
      return string("beta");
      break;
    }
    case 20:
    {      
      return string("k-bw");
      break;
    }
    case 21:
    {      
      return string("Vsv");
      break;
    }
    case 22:
    {      
      return string("Vsa");
      break;
    }
    case 23:
    {      
      return string("PAo");
      break;
    }
    case 24:
    {      
      return string("Qav");
      break;
    }
  }
}

// ====================
// GET MODEL PARAMETERS
// ====================
void cmLPN_SV_Heart::getDefaultParams(stdVec& zp){
  zp.resize(getParameterTotal());
  zp[0]  = 117.65;  // HR
  zp[1]  = 0.766;   // Csa
  zp[2]  = 0.01;    // csa
  zp[3]  = 0.8;     // dsa
  zp[4]  = 2.5;     // Vsa0
  zp[5]  = 0.25;    // tsas
  zp[6]  = 0.1;     // t1_ratio
  zp[7]  = -0.12;   // a
  zp[8]  = 14.0;    // b
  zp[9]  = 1.0;     // csv
  zp[10] = 0.065;   // dsv
  zp[11] = 9.7786;  // Vsv0
  zp[12] = 0.31;    // tsvs
  zp[13] = 0.1;     // Rmyo
  zp[14] = 24.474;  // Alpha
  zp[15] = 0.00025; // Kao
  zp[16] = 0.24;    // CAo
  zp[17] = 0.0;     // t_d1
  zp[18] = 0.0;     // t_d2
  zp[19] = 0.95;    // beta
  zp[20] = 0.0;     // k_bw
  zp[21] = 38.87;   // Vsv
  zp[22] = 2.5537;  // Vsa
  zp[23] = 38.0;    // PAo
  zp[24] = -0.1;    // Qav
}

// ====================
// GET PARAMETER RANGES
// ====================
void cmLPN_SV_Heart::getDefaultParameterLimits(stdVec& limits){
  // Set Common Parameter Limits
  limits.resize(2*getParameterTotal());
  limits[0]  = 117.65;   limits[1]  = 117.65; // HR    
  limits[2]  = 0.05;     limits[3]  = 2.0;    // Csa
  limits[4]  = 0.01;     limits[5]  = 0.8;    // csa
  limits[6]  = 0.16;     limits[7]  = 0.8;    // dsa
  limits[8]  = 0.10;     limits[9]  = 15.0;   // Vsa0
  limits[10] = 0.1739;   limits[11] = 0.2599; // tsas,r
  limits[12] = 0.01;     limits[13] = 0.15;   // t1_ratio
  limits[14] = -0.20;    limits[15] = -0.005; // a
  limits[16] = 8.0;      limits[17] = 15.0;   // b
  limits[18] = 0.1;      limits[19] = 5.0;    // csv
  limits[20] = 0.04;     limits[21] = 0.08;   // dsv
  limits[22] = 2.0;      limits[23] = 15.0;   // Vsv0
  limits[24] = 0.2478;   limits[25] = 0.3718; // tsvs
  limits[26] = 0.001;    limits[27] = 0.1;    // Rmyo
  limits[28] = 1.0;      limits[29] = 30.0;   // alpha
  limits[30] = 1.0E-04;  limits[31] = 2.5E-4; // Kao
  limits[32] = 0.05;     limits[33] = 1.0;    // CAo
  limits[34] = -0.2;     limits[35] = 0.2;    // tdelay1
  limits[36] = -0.2;     limits[37] = 0.2;    // tdelay2
  limits[38] = 0.0;      limits[39] = 1.0;    // beta
  limits[40] = 0.000;    limits[41] = 0.000;  // k_bw
  limits[42] = 5.0;      limits[43] = 40.0;   // Vsv
  limits[44] = 1.0;      limits[45] = 15.0;   // Vsa
  limits[46] = 10.0;     limits[47] = 80.0;   // PAo
  limits[48] = -0.1;     limits[49] = 100.0;  // Qav
}


// =======================================
// GET FOURIER COEFFICIENTS FOR ALL MODELS
// =======================================
void getFourierCoefficients(double &ain1,double &ain2,double &ain3,double &ain4,double &ain5,double &ain6,double &ain7,double &ain8,double &ain9,
                            double &bin1,double &bin2,double &bin3,double &bin4,double &bin5,double &bin6,double &bin7,double &bin8,
                            double &aout1,double &aout2,double &aout3,double &aout4,double &aout5,double &aout6,double &aout7,double &aout8,double &aout9,
                            double &bout1,double &bout2,double &bout3,double &bout4,double &bout5,double &bout6,double &bout7,double &bout8){

  // Qven IN
  ain9 = 26.34; ain1 = -0.5992; bin1 = -7.741; ain2 = 10.43; bin2 = 7.625; ain3 = 1.681; bin3 = 4.993;
  ain4 = 4.647; bin4 = -0.194; ain5 = 0.4065; bin5 = 1.681; ain6 = -0.877; bin6 = -0.7002;
  ain7 = 0.05779; bin7 = 0.795; ain8 = -0.4661; bin8 = 0.1192;

  // Qao OUT
  aout9 = 26.74; aout1 = 36.47; bout1 = 37.2; aout2 = 0.05574; bout2 = 47.74; aout3 = -27.16; bout3 = 29.7;
  aout4 = -30.17; bout4 = 3.669; aout5 = -16.89; bout5 = -10.59; aout6 = -3.755; bout6 = -10.25; aout7 = 1.338;
  bout7 = -4.433; aout8 = 1.012; bout8 = -0.7531;
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void cmLPN_SV_Heart::heartODE(double tn, stdVec Xn, stdVec params, stdVec& Xn1, stdVec& out){

  // INITIALIZE VARIABLES
  // STORE LOCAL COPIES OF PARAMETERS
  double HR = params[0]; // FIXED HEART RATE
  double tc = (60.0/(double)HR);
  double Csa = params[1];
  double csa = params[2];
  double dsa = params[3];
  double Vsa0 = params[4];
  // 21JAN2014 - Modified DES
  // ========================
  double tsas_ratio = params[5];
  double tsas = tsas_ratio * sqrt(tc);
  // ========================
  double tsvs_ratio = params[12];  // Apply tsvs=0.16+0.3*tc
  double tsvs = tsvs_ratio * sqrt(tc);
  // ========================
  double t1_ratio = params[6];
  double t1 = t1_ratio*tc;
  double a = params[7];
  double b = params[8];
  double csv = params[9];
  double dsv = params[10];
  double Vsv0 = params[11];
  double Rmyo = params[13];
  double alpha = params[14];
  double Kao = params[15]; // Discharge coefficient Aortic valve
  double CAo = params[16];
  double tdelay1 = params[17];
  double tdelay2 = tdelay1;
  double beta = params[19];
  double k_bw = params[20];
  // ======================
  // ATRIO-VENTRICULAR AREA
  // ======================
  // Virtual Patient
  double A_fw = 2.815;

  // ====================
  // FOURIER COEFFICIENTS
  // ====================
  // IN
  double ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9;
  double bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8;
  // OUT  
  double aout1,aout2,aout3,aout4,aout5,aout6,aout7,aout8,aout9;
  double bout1,bout2,bout3,bout4,bout5,bout6,bout7,bout8;
  // Freq
  double w = 2.0*pi/(double)tc;

  // Get The Coefficients for different Models
  getFourierCoefficients(ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9,
                         bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8,
                         aout1,aout2,aout3,aout4,aout5,aout6,aout7,aout8,aout9,
                         bout1,bout2,bout3,bout4,bout5,bout6,bout7,bout8);
  
  // SHIFT TIME TO ACCOUNT FOR INPUT/OUTPUT CURVE DELAY
  double teff1 = tn + tdelay1;
  double teff2 = tn + tdelay2;

  // SUM UP VENOUS FLUX
  double Qin1 = ain9+ain1*cos(teff1*w)+bin1*sin(teff1*w)+ain2*cos(2*teff1*w);
  double Qin2 = bin2*sin(2*teff1*w)+ain3*cos(3*teff1*w)+bin3*sin(3*teff1*w);
  double Qin3 = ain4*cos(4*teff1*w)+bin4*sin(4*teff1*w)+ain5*cos(5*teff1*w);
  double Qin4 = bin5*sin(5*teff1*w)+ain6*cos(6*teff1*w)+bin6*sin(6*teff1*w);
  double Qin5 = ain7*cos(7*teff1*w)+bin7*sin(7*teff1*w)+ain8*cos(8*teff1*w) ;
  double Qin6 = bin8*sin(8*teff1*w);
  double Qven = Qin1+Qin2+Qin3+Qin4+Qin5+Qin6;

  // EVAL AORTIC FLOW RATE
  double Qout1 = aout9 + aout1*cos(teff2*w) + bout1*sin(teff2*w) + aout2*cos(2*teff2*w);
  double Qout2 = bout2*sin(2*teff2*w)+aout3*cos(3*teff2*w)+bout3*sin(3*teff2*w);
  double Qout3 = aout4*cos(4*teff2*w)+bout4*sin(4*teff2*w)+aout5*cos(5*teff2*w);
  double Qout4 = bout5*sin(5*teff2*w)+aout6*cos(6*teff2*w)+bout6*sin(6*teff2*w);
  double Qout5 = aout7*cos(7*teff2*w)+bout7*sin(7*teff2*w)+aout8*cos(8*teff2*w);
  double Qout6 = bout8*sin(8*teff2*w);
  double QAo = Qout1+Qout2+Qout3+Qout4+Qout5+Qout6;

  // UNPACK STATE VARIABLES
  double Vsv = Xn[0]; // Volume single ventricle
  double Vsa = Xn[1]; // Volume single atrium
  double PAo = Xn[2]; // Pressure aorta
  double Qav = Xn[3]; // Flow Atrial-Ventricular valve

  // ===================
  // EVAL HEART FUNCTION
  // ===================
  double tmv = fmod(tn,tc);           // Ventricle time
  double tma = fmod((tn+tsas-t1),tc); // Atrium time

  // VENTRICULAR ACTIVATION TIME
  double AV = 0.0;
  if(tmv<tsvs){
    AV = (1-cos(2*pi*tmv/(double)tsvs))/2.0;
  }else{
    AV = 0.0;
  }

  // ATRIAL ACTIVATION TIME
  double AA = 0.0;
  if(tma<tsas){
    AA = (1-cos(2*pi*tma/(double)tsas))/2.0;
  }else{
    AA = 0.0;
  }

  // ATRIUM MODEL
  double Psaactive = (Vsa-Vsa0)/(double)Csa; 
  double Psapassive = csa*(exp(dsa*(Vsa-Vsa0))-1.0);
  double Psa = AA*Psaactive + Psapassive;

  // VENTRICLE MODEL
  double Psvactive = (a*(Vsv-Vsv0) + b)*(Vsv-Vsv0);
  double Psvpassive = csv*(exp(dsv*(Vsv-Vsv0))-1.0);
  double Psv = AV*Psvactive + Psvpassive;

  // AORTIC VALVE
  double LVOF = 0.0;
  if(Psv>PAo){
    LVOF = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAo)))/(2.0*Kao);
  }else{
    LVOF = 0.0;  
  }
  
  // EFFECTIVE VENTRICULAR PRESSURE
  double Psveff = Psv-Rmyo*LVOF;

  // =================
  // REGURGITANT VALVE
  // =================
  double Kav = 0.0;
  double Lav = 0.0;
  double Kav_fw = 0.0;
  double Lav_fw = 0.0;
  double Kav_bw = 0.0;
  double Lav_bw = 0.0;
  double A_bw = 0.0;
  double Qavp = 0.0;
  double AV_vel = 0.0;
  double Qreg = 0.0;

  if(fabs(k_bw)<1.0e-5){

    // Non regurgitant valve

    Kav = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2; // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav = beta*2*1.060*sqrt(4.0/(pi*A_fw))/1333.2;

    if((Psa<Psveff)&&(Qav<1.0e-5)){
      Qavp = 0.0;
    }else{
      Qavp = (Psa-Psv-Kav*Qav*fabs(Qav))/(double)Lav;
    }

    if(Qav<1.0e-5){
      Qav = 0.0;
      Xn[3] = 0.0;
    }
    
  }else{

    // Regurgitant valve     

    A_bw = k_bw*A_fw;

    Kav_fw = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2 ;  // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav_fw = beta*2*1.060*sqrt(4/(pi*A_fw))/1333.2;  // Factor 1333.2 to have inertance in [mmHg*s^2/ml]

    Kav_bw = alpha*0.5*1.060*(2.4/(A_bw*A_bw))/1333.2;   
    Lav_bw = beta*2*1.060*sqrt(4/(pi*A_bw))/1333.2; 

    if (Qav<0.0){
      Qavp=(Psa-Qav*fabs(Qav)*Kav_bw-Psv)/(double)Lav_bw; // One- way quadratic valve plus inertance
    }else {
      Qavp=(Psa-Qav*fabs(Qav)*Kav_fw-Psv)/(double)Lav_fw; // One- way quadratic valve plus inertance
    }

    if(Qav<0.0){
      AV_vel = Qav/(double)A_bw;
    }else{
      AV_vel = Qav/(double)A_fw;
    }
    
    if(Qav<0.0){
      Qreg = fabs(Qav);
    }else{
      Qreg = 0.0;
    }    
  }

  // COMPUTE THE FINAL DERIVATIVES
  double Vsvp = Qav-LVOF;
  double Vsap = Qven-Qav;
  double PAop = (LVOF-QAo)/(double)CAo;

  // STORE TIME DERIVATIVES
  Xn1[0] = Vsvp;
  Xn1[1] = Vsap;
  Xn1[2] = PAop;
  Xn1[3] = Qavp;

  // STORE SECONDARY OUTPUT
  out[0] = Psveff;
  out[1] = Psa;
  out[2] = QAo;
  out[3] = Qven;
  out[4] = LVOF;
  out[5] = tc;
  out[6] = AA;
  out[7] = AV;
  out[8] = Psaactive;
  out[9] = Psapassive;
  out[10] = Psvactive;
  out[11] = Psvpassive;
  out[12] = Qreg;
}

// ==================
// SOLVE ODE WITH RK4
// ==================
int cmLPN_SV_Heart::PerformRK4Steps(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = 0;
  const bool printIC = false;
  bool isValidDouble;

  // Get Totals
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

  // TIME LOOP
  for(int loopA=0;loopA<totalSteps;loopA++){
    // Increment Time Step
    stepId++;

    // Update Current Time
    if(loopA>0){
      currTime += timeStep;
    }
    // Eval K1
    heartODE(currTime,Xn,params,k1,k1AuxOut);
    // Eval K2
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk2[loopB] = Xn[loopB] + (0.5*timeStep) * k1[loopB];
    }
    heartODE(currTime + 0.5 * timeStep,Xk2,params,k2,k2AuxOut);
    // Eval K3
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk3[loopB] = Xn[loopB] + (0.5*timeStep) * k2[loopB];
    }
    heartODE(currTime + 0.5 * timeStep,Xk3,params,k3,k3AuxOut);
    // Eval K4
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk4[loopB] = Xn[loopB] + timeStep * k3[loopB];
    }
    heartODE(currTime + timeStep,Xk4,params,k4,k4AuxOut);

    // Eval Xn1
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn1[loopB] = Xn[loopB] + (1.0/6.0)*timeStep*(k1[loopB] + 2.0 * k2[loopB] + 2.0 * k3[loopB] + k4[loopB]);
    }

    // CHECK IF SOLUTION IS VALID
    if((loopA % 100) == 0){
      for(int loopB=0;loopB<totalStates;loopB++){
        isValidDouble = (Xn[loopB] != Xn[loopB]);
        if(isValidDouble){
          return 1;
        }
      }
    }

    // Update Xn
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn[loopB] = Xn1[loopB];
    }

    // Copy back k1AuxOut for each time step
    for(int loopB=0;loopB<totAuxStates;loopB++){
      auxOutVals[loopA][loopB] = k1AuxOut[loopB];
    }
    // Copy The solution back for each time step
    for(int loopB=0;loopB<totalStates;loopB++){
      outVals[loopA][loopB] = Xn1[loopB];
    }
  }
  // RETURN OK
  return 0;
}

// ========================
// FLUSH MODEL DATA TO FILE
// ========================
void flushHeartModelDataToFile(string outFileName,int start, int stop ,double* t,stdMat outVals,stdMat auxOutVals){

  FILE* outFile;
  outFile = fopen(outFileName.c_str(),"w");
  fprintf(outFile,"%20s %20s %20s %20s %20s %20s %20s %20s %20s\n","t","Vsv","Vsa","Qven","Qav","QAo","Psv","Psa","PAo");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e %20e %20e %20e %20e %20e\n",
         t[loopA],outVals[loopA][ipVsv],outVals[loopA][ipVsa],auxOutVals[loopA][ipQven],outVals[loopA][ipQav],auxOutVals[loopA][ipQAo],
            auxOutVals[loopA][ipPsvEff],auxOutVals[loopA][ipPsa],outVals[loopA][ipPAo]);
  }
  fclose(outFile);
}

/*! 
### Function areParamsInsideLimits

#### Purpose
Checks if a given set of parameters is inside the 
admissible limits. 

#### Discussion
The test fails when at least one parameter is
outside the admissible range. 

#### Modified
2 May 2015 - Daniele Schiavazzi

#### Reference:
No Reference

\param[in] params the vector with the parameters to test
result boolean test: true if the parameters are inside the 
                     admissible ranges, false otherwise. 
*/
bool cmLPN_SV_Heart::areParamsInsideLimits(stdVec params){
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

/*! 
### Function cmLPN_NA_S::isParamsSignCorrect

#### Purpose
Check if sign of parameter is consistent with associated limits.

#### Discussion
None

#### Modified
2 May 2015 - Daniele Schiavazzi

#### Reference:
No Reference

\param[in]      params (stdVec): Vector containing the parameter set to test. 
*/
bool cmLPN_SV_Heart::isParamsSignCorrect(stdVec params){
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

// ================================
// MAIN FUNCTION FOR HEART SUBMODEL
// ================================
int cmLPN_SV_Heart::solveLPN(lpnOptions options, stdVec inputs, stdVec& outputs){

  // SET PARAMETERS
  // FIXED HR
  double heartRate = inputs[getStateTotal()]; 
  double cycleTime = 60.0/(double)heartRate;

  // TIME RESULTS
  double totalTime = numCycles*cycleTime;
  double timeStep = cycleTime/(double)totalStepsOnSingleCycle;
  int totalSteps = floor(totalTime/(double)timeStep);
  
    // CHECK PARAMS INSIDE LIMITS
  if(options.checkLimits){
    if(!areParamsInsideLimits(inputs)){
      printf("ERROR: VIOLATED PARAMETER LIMITS.\n");
      return 1;
    }
  }

  // CHECK PARAMETER SIGN
  if(options.checkSign){
    if(!isParamsSignCorrect(inputs)){
      printf("ERROR: VIOLATED PARAMETER SIGN.\n");
      return 1;
    }
  }

  // TIME STEP ARRAY
  double t[totalSteps];
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
      iniVals[loopA] = inputs[loopA];
    }else{
      trueParams[loopA-totalStates] = inputs[loopA];
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

  // Solve All Cycles
  int error = PerformRK4Steps(totalSteps,timeStep,iniVals,trueParams,outVals,auxOutVals);
  if(error !=0 ){
    throw cmException("RK4 Time Integration not Converged!");
  }

  // Get thoracic cavity pressure
  // Pth=Pf(t);

  // Copy Auxiliary Variables From result for the last heart cycle
  double Psv[totalStepsOnSingleCycle];
  double Psa[totalStepsOnSingleCycle];
  double QAo[totalStepsOnSingleCycle];
  double Qven[totalStepsOnSingleCycle];
  double LVOF[totalStepsOnSingleCycle];
  double tc2[totalStepsOnSingleCycle];
  double AA[totalStepsOnSingleCycle];
  double AV[totalStepsOnSingleCycle];
  double PsaActive[totalStepsOnSingleCycle];
  double PsaPassive[totalStepsOnSingleCycle];
  double PsvActive[totalStepsOnSingleCycle];
  double PsvPassive[totalStepsOnSingleCycle];
  double Qreg[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Psv[loopA] = auxOutVals[globalInd][ipPsvEff];
    Psa[loopA] = auxOutVals[globalInd][ipPsa];
    QAo[loopA] = auxOutVals[globalInd][ipQAo];
    Qven[loopA] = auxOutVals[globalInd][ipQven];
    LVOF[loopA] = auxOutVals[globalInd][ipLVOF];
    AA[loopA] = auxOutVals[globalInd][ipAA];
    AV[loopA] = auxOutVals[globalInd][ipAV];
    PsaActive[loopA] = auxOutVals[globalInd][ipPsaActive];
    PsaPassive[loopA] = auxOutVals[globalInd][ipPsaPassive];
    PsvActive[loopA] = auxOutVals[globalInd][ipPsvActive];
    PsvPassive[loopA] = auxOutVals[globalInd][ipPsvPassive];
    Qreg[loopA] = auxOutVals[globalInd][ipQreg];
  }

  // Copy State Variables From result for the last heart cycle
  double Vsv[totalStepsOnSingleCycle];
  double Vsa[totalStepsOnSingleCycle];
  double PAo[totalStepsOnSingleCycle];
  double Qav[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Vsv[loopA] = outVals[globalInd][ipVsv];
    Vsa[loopA] = outVals[globalInd][ipVsa];
    PAo[loopA] = outVals[globalInd][ipPAo];
    Qav[loopA] = outVals[globalInd][ipQav];
  }

  // GET AVERAGE VALUES
  double CO      = trapz(0,totalStepsOnSingleCycle,t,QAo)/(double)cycleTime;
  double mPsa    = trapz(0,totalStepsOnSingleCycle,t,Psa)/(double)cycleTime;
  double mPascAo = trapz(0,totalStepsOnSingleCycle,t,PAo)/(double)cycleTime;
  double mQAo    = trapz(0,totalStepsOnSingleCycle,t,QAo)/(double)cycleTime;
  double mQven   = trapz(0,totalStepsOnSingleCycle,t,Qven)/(double)cycleTime;
  double mQreg   = trapz(0,totalStepsOnSingleCycle,t,Qreg)/(double)cycleTime;

  // GET MAXIMUM AND MINIMUM VALUES
  double maxPsv = getMax(0,totalStepsOnSingleCycle,Psv);
  double maxPsa = getMax(0,totalStepsOnSingleCycle,Psa);
  double maxVsv = getMax(0,totalStepsOnSingleCycle,Vsv);
  double minVsv = getMin(0,totalStepsOnSingleCycle,Vsv);
  double maxPAo = getMax(0,totalStepsOnSingleCycle,PAo);
  double minPAo = getMin(0,totalStepsOnSingleCycle,PAo);


  // =====================
  // PLOT DATA IF REQUIRED
  // =====================
  if(options.flushData){
    string outFileName("heartModelData.txt");
    flushHeartModelDataToFile(outFileName,totalSteps-3*totalStepsOnSingleCycle,totalSteps,t,outVals,auxOutVals);
  }

  // Copy Final Results
  outputs.clear();
  outputs.push_back(mPascAo);
  outputs.push_back(maxPAo);
  outputs.push_back(mPsa);
  outputs.push_back(maxPsv);
  outputs.push_back(maxVsv);
  outputs.push_back(Psv[0]); // Ventricle Pressure at Zero 
  outputs.push_back(maxPsa); // Max Atrial Pressure
  outputs.push_back(mQreg);  // Mean Regurgitation

  // Solution Successful
  return 0;

}

/*! 
### Function evalModelError

#### Purpose
Solve LPN model and determines outputs and errors

#### Discussion
None

#### Modified
2 May 2015 - Daniele Schiavazzi

#### Reference
No Reference

\param inputs A std float vector with all the input parameters
\param outputs A std float vector with all the LPN outputs
\param errorCode if 0 the execution is OK otherwise some error were encountered during execution
\return the log-likelihood/cost function evaluated at the current parameters
*/
double cmLPN_SV_Heart::evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCode) {
  
  // Set Options
  lpnOptions options;
  options.writeDebugData = false;
  options.flushData = true;
  options.checkLimits = false;
  options.checkSign = false;

  // Solve LPN Model
  int locErrorCode = solveLPN(options,inputs,outputs);

  // Store Error Code
  errorCode.push_back(locErrorCode);  

  // KEYS
  vector<string> keys;
  keys.push_back(string("mPascAo"));
  keys.push_back(string("maxPAo"));
  keys.push_back(string("mPsa"));
  keys.push_back(string("maxPsv"));
  keys.push_back(string("maxVsv"));
  keys.push_back(string("Psv0"));
  keys.push_back(string("maxPsa"));
  keys.push_back(string("mQreg"));

  stdVec computedValues;
  computedValues.push_back(outputs[0]);
  computedValues.push_back(outputs[1]);
  computedValues.push_back(outputs[2]);
  computedValues.push_back(outputs[3]);
  computedValues.push_back(outputs[4]);
  computedValues.push_back(outputs[5]);
  computedValues.push_back(outputs[6]);
  computedValues.push_back(outputs[7]);

  stdVec stdFactors;
  for(int loopA=0;loopA<computedValues.size();loopA++){
    stdFactors.push_back(0.05);
  }

  stdVec weigths;
  weigths.push_back(1.0); // mPascAo
  weigths.push_back(1.0); // maxPAo
  weigths.push_back(1.0); // mPsa
  weigths.push_back(1.0); // maxPsv
  weigths.push_back(1.0); // maxVsv
  weigths.push_back(1.0); // Psv0
  weigths.push_back(1.0); // maxPsa
  weigths.push_back(1.0); // mQreg

  // Print and compare
  double result = 0.0;
  if(data != NULL){
    
    // Print Info
    data->printAndCompare(datasetColumn,keys,computedValues,weigths);
  
    // Evaluate Objective Function
    result = data->evalLogLikelihood(datasetColumn,keys,computedValues,stdFactors,weigths);
    // result = data->evalOBJ(datasetColumn,keys,computedValues,weigths);
  }

  // Return 
  return result;
}

