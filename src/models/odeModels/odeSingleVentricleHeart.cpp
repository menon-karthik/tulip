#include "odeSingleVentricleHeart.h"

using namespace std;

int odeSingleVentricleHeart::getParameterTotal(){
  return 25;
}

int odeSingleVentricleHeart::getStateTotal(){
  return 4; 
}

int odeSingleVentricleHeart::getAuxStateTotal(){
  return 13;  
}

int odeSingleVentricleHeart::getHRIndex(){
  return 0;
}

// ==================
// GET PARAMETER NAME
// ==================
string odeSingleVentricleHeart::getParamName(int parID){
  string res;
  switch(parID){
    case 0:
      res = string("Heart Rate");
      break;
    case 1:
      res = string("Csa");
      break;
    case 2:
      res = string("csa");
      break;
    case 3:
      res = string("dsa");
      break;
    case 4:
      res = string("Vsa0");
      break;
    case 5:
      res = string("tsas-ratio");
      break;
    case 6:
      res = string("t1-ratio");
      break;
    case 7:
      res = string("a");
      break;
    case 8:
      res = string("b");
      break;
    case 9:
      res = string("csv");
      break;
    case 10:
      res = string("dsv");
      break;
    case 11:
      res = string("Vsv0");
      break;
    case 12:
      res = string("tsvs-ratio");
      break;
    case 13:
      res = string("Rmyo");
      break;
    case 14:
      res = string("alpha");
      break;
    case 15:
      res = string("Kao");
      break;
    case 16:
      res = string("CAo");
      break;
    case 17:
      res = string("tdelay1");
      break;
    case 18:
      res = string("tdelay2");
      break;
    case 19:
      res = string("beta");
      break;
    case 20:
      res = string("k-bw");
      break;
    case 21:
      res = string("Vsv");
      break;
    case 22:
      res = string("Vsa");
      break;
    case 23:
      res = string("PAo");
      break;
    case 24:
      res = string("Qav");
      break;
  }
  return res;
}

// ====================
// GET MODEL PARAMETERS
// ====================
void odeSingleVentricleHeart::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0]  = 117.65;  // HR
  params[1]  = 0.766;   // Csa
  params[2]  = 0.01;    // csa
  params[3]  = 0.8;     // dsa
  params[4]  = 2.5;     // Vsa0
  params[5]  = 0.25;    // tsas
  params[6]  = 0.1;     // t1_ratio
  params[7]  = -0.12;   // a
  params[8]  = 14.0;    // b
  params[9]  = 1.0;     // csv
  params[10] = 0.065;   // dsv
  params[11] = 9.7786;  // Vsv0
  params[12] = 0.31;    // tsvs
  params[13] = 0.1;     // Rmyo
  params[14] = 24.474;  // Alpha
  params[15] = 0.00025; // Kao
  params[16] = 0.24;    // CAo
  params[17] = 0.0;     // t_d1
  params[18] = 0.0;     // t_d2
  params[19] = 0.95;    // beta
  params[20] = 0.0;     // k_bw
  params[21] = 38.87;   // Vsv
  params[22] = 2.5537;  // Vsa
  params[23] = 38.0;    // PAo
  params[24] = -0.1;    // Qav
}

// ====================
// GET PARAMETER RANGES
// ====================
void odeSingleVentricleHeart::getDefaultParameterLimits(stdVec& limits){
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
void odeSingleVentricleHeart::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, 
                                        stdVec& DXk, stdVec& auxOut, stdVec& Ind){

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
  double w = 2.0*M_PI/(double)tc;

  // Get The Coefficients for different Models
  getFourierCoefficients(ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9,
                         bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8,
                         aout1,aout2,aout3,aout4,aout5,aout6,aout7,aout8,aout9,
                         bout1,bout2,bout3,bout4,bout5,bout6,bout7,bout8);
  
  // SHIFT TIME TO ACCOUNT FOR INPUT/OUTPUT CURVE DELAY
  double teff1 = t + tdelay1;
  double teff2 = t + tdelay2;

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
  double Vsv = Xk[0]; // Volume single ventricle
  double Vsa = Xk[1]; // Volume single atrium
  double PAo = Xk[2]; // Pressure aorta
  double Qav = Xk[3]; // Flow Atrial-Ventricular valve

  // ===================
  // EVAL HEART FUNCTION
  // ===================
  double tmv = fmod(t,tc);           // Ventricle time
  double tma = fmod((t+tsas-t1),tc); // Atrium time

  // VENTRICULAR ACTIVATION TIME
  double AV = 0.0;
  if(tmv<tsvs){
    AV = (1-cos(2*M_PI*tmv/(double)tsvs))/2.0;
  }else{
    AV = 0.0;
  }

  // ATRIAL ACTIVATION TIME
  double AA = 0.0;
  if(tma<tsas){
    AA = (1-cos(2*M_PI*tma/(double)tsas))/2.0;
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
    Lav = beta*2*1.060*sqrt(4.0/(M_PI*A_fw))/1333.2;

    if((Psa<Psveff)&&(Qav<1.0e-5)){
      Qavp = 0.0;
    }else{
      Qavp = (Psa-Psv-Kav*Qav*fabs(Qav))/(double)Lav;
    }

    if(Qav<1.0e-5){
      Qav = 0.0;
    }
    
  }else{

    // Regurgitant valve     

    A_bw = k_bw*A_fw;

    Kav_fw = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2 ;  // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav_fw = beta*2*1.060*sqrt(4/(M_PI*A_fw))/1333.2;  // Factor 1333.2 to have inertance in [mmHg*s^2/ml]

    Kav_bw = alpha*0.5*1.060*(2.4/(A_bw*A_bw))/1333.2;   
    Lav_bw = beta*2*1.060*sqrt(4/(M_PI*A_bw))/1333.2; 

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
  DXk[0] = Vsvp;
  DXk[1] = Vsap;
  DXk[2] = PAop;
  DXk[3] = Qavp;

  // STORE SECONDARY OUTPUT
  auxOut[0] = Psveff;
  auxOut[1] = Psa;
  auxOut[2] = QAo;
  auxOut[3] = Qven;
  auxOut[4] = LVOF;
  auxOut[5] = tc;
  auxOut[6] = AA;
  auxOut[7] = AV;
  auxOut[8] = Psaactive;
  auxOut[9] = Psapassive;
  auxOut[10] = Psvactive;
  auxOut[11] = Psvpassive;
  auxOut[12] = Qreg;
}

// ================================
// MAIN FUNCTION FOR HEART SUBMODEL
// ================================
void odeSingleVentricleHeart::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, 
                                          const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, 
                                          stdVec& results){

  // Create the time vector
  double t[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    t[loopA] = loopA*timeStep;
  }

  double heartRate = 60.0/(totalStepsOnSingleCycle*timeStep); 
  double cycleTime = totalStepsOnSingleCycle*timeStep;
  int numCycles = totalSteps/totalStepsOnSingleCycle;
  double totalTime = numCycles*cycleTime;
  
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

  // Copy Final Results
  results.clear();
  results.push_back(mPascAo);
  results.push_back(maxPAo);
  results.push_back(mPsa);
  results.push_back(maxPsv);
  results.push_back(maxVsv);
  results.push_back(Psv[0]); // Ventricle Pressure at Zero 
  results.push_back(maxPsa); // Max Atrial Pressure
  results.push_back(mQreg);  // Mean Regurgitation
}

void odeSingleVentricleHeart::getResultKeys(stdStringVec& keys){
  keys.clear();
  keys.push_back(string("mPascAo"));
  keys.push_back(string("maxPAo"));
  keys.push_back(string("mPsa"));
  keys.push_back(string("maxPsv"));
  keys.push_back(string("maxVsv"));
  keys.push_back(string("Psv0"));
  keys.push_back(string("maxPsa"));
  keys.push_back(string("mQreg"));
}

void odeSingleVentricleHeart::getFinalOutputs(const stdVec& outputs, stdVec& outs){
  outs.clear();
  outs.push_back(outputs[0]);
  outs.push_back(outputs[1]);
  outs.push_back(outputs[2]);
  outs.push_back(outputs[3]);
  outs.push_back(outputs[4]);
  outs.push_back(outputs[5]);
  outs.push_back(outputs[6]);
  outs.push_back(outputs[7]);
}

void odeSingleVentricleHeart::getDataSTD(stdVec& stds){
  stds.clear();
  stds.push_back(0.05);
  stds.push_back(0.05);
  stds.push_back(0.05);
  stds.push_back(0.05);
  stds.push_back(0.05);
  stds.push_back(0.05);
  stds.push_back(0.05);
  stds.push_back(0.05);
}

void odeSingleVentricleHeart::getResultWeigths(stdVec& weights){
  weights.clear();
  weights.push_back(1.0); // mPascAo
  weights.push_back(1.0); // maxPAo
  weights.push_back(1.0); // mPsa
  weights.push_back(1.0); // maxPsv
  weights.push_back(1.0); // maxVsv
  weights.push_back(1.0); // Psv0
  weights.push_back(1.0); // maxPsa
  weights.push_back(1.0); // mQreg
}
