# include "odeSingleVentricleHeartIND.hpp"

using namespace std;




  virtual int    getAuxStateTotal() = 0;

  
  
  
  
  virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;
  virtual void   getResultKeys(stdStringVec& keys) = 0;
  virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs) = 0;
  virtual void   getDataSTD(stdVec& stds) = 0;
  virtual void   getResultWeigths(stdVec& weights) = 0;    
  virtual int    getHRIndex() = 0;


// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleHeartIND::getParameterTotal(){
  return 27;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleHeartIND::getStateTotal(){
  return 4; 
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int odeSingleVentricleHeartIND::getResultTotal(){
  return 9;  
}

// ===========================
// GET NAME OF RESULT QUANTITY
// ===========================
string odeSingleVentricleHeartIND::getResultName(int resID){
  string res;
  switch(resID){
    case 0:
      res = string("mPascAo");
      break;
    case 1:
      res = string("maxPAo");
      break;
    case 2:
      res = string("mPsa");
      break;
    case 3:
      res = string("maxPsv");
      break;
    case 4:
      res = string("maxVsv");
      break;
    case 5:
      res = string("maxQAA");
      break;
    case 6:
      res = string("PsvZero");
      break;
    case 7:
      res = string("maxPsa");
      break;
    case 8:
      res = string("mQreg");
      break;
  }
  return res;
}

// ==================
// GET PARAMETER NAME
// ==================
string odeSingleVentricleHeartIND::getParamName(int parID){
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
      res = string("RAA");
      break;
    case 22:
      res = string("CAA");
      break;
    case 23:
      res = string("Vsv");
      break;
    case 24:
      res = string("Vsa");
      break;
    case 25:
      res = string("Qav");
      break;
    case 26:
      res = string("PAA");
      break;
    case 27:
      res = string("QAA");
      break;
  }
  return res;
}

// ====================
// GET MODEL PARAMETERS
// ====================
void odeSingleVentricleHeartIND::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0]  = 117.65; //HR 
  params[1]  = 1.0; //Csa 
  params[2]  = 0.78541; //csa 
  params[3]  = 0.20142; //dsa 
  params[4]  = 0.96742; //Vsa0 
  params[5]  = 0.20149; //tsas 
  params[6]  = 0.11024; //t1_ratio 
  params[7]  = -0.10783; //a 
  params[8]  = 14.106; //b 
  params[9]  = 0.97372; //csv 
  params[10] = 0.065337; //dsv 
  params[11] = 9.5631; //Vsv0 
  params[12] = 0.31523; //tsvs 
  params[13] = 0.091782; //Rmyo 
  params[14] = 24.203; //Kav 
  params[15] = 0.0002; //Kao 
  params[16] = 0.15; //CAo 
  params[17] = 0; //tdelay1 
  params[18] = 0; //tdelay2 
  params[19] = 0.72; //Lav 
  params[20] = 0.0; //k_bw
  params[21] = 0.1; //RAA 
  params[22] = 0.1; //CAA  
  params[23] = 32.937; //Vsv 
  params[24] = 1.632; //Vsa 
  params[25] = 0; //Qav 
  params[26] = 35.0; //PAA 
  params[27] = 0; //QAA  
}

// ====================
// GET PARAMETER RANGES
// ====================
void odeSingleVentricleHeartIND::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  // COMMON PARAMETER RANGES
  limits[0]=117.65;    limits[1]=117.65; // HR      
  limits[2]=0.05;      limits[3]=2.0; // Csa
  limits[4]=0.01;      limits[5]=0.8; // csa
  limits[6]=0.16;      limits[7]=0.8; // dsa
  limits[8]=0.1;       limits[9]=15.0; // Vsa0
  limits[10]=0.1739;   limits[11]=0.2599; // tsas
  limits[12]=0.01;     limits[13]=0.15; // t1_ratio
  limits[14]=-0.20;    limits[15]=-0.005; // a
  limits[16]=8.0;      limits[17]=15.0; // b
  limits[18]=0.1;      limits[19]=5.0; // csv
  limits[20]=0.04;     limits[21]=0.08; // dsv
  limits[22]=2.0;      limits[23]=15.0; // Vsv0
  limits[24]=0.2478;   limits[25]=0.3718; // tsvs
  limits[26]=0.001;    limits[27]=0.1; // Rmyo
  limits[28]=1.0;      limits[29]=30.0; // alpha
  limits[30]=1.00E-04; limits[31]=2.50E-04; // Kao
  limits[32]=0.05;     limits[33]=1.0; // CAo
  limits[34]=-0.2;     limits[35]=0.2; // tdelay1
  limits[36]=-0.2;     limits[37]=0.2; // tdelay2
  limits[38]=0.0;      limits[39]=1.00; // beta
  limits[40]=0.000;    limits[41]=0.000; // k_bw
  limits[42]=0.1;      limits[43]=0.7; //RAA
  limits[44]=0.05;     limits[45]=1.0; //CAA
  limits[46]=5.0;      limits[47]=40.0; // Vsv
  limits[48]=1.0;      limits[49]=15.0; // Vsa
  limits[50]=-0.1;     limits[51]=100.0; // Qav
  limits[52]=10.0;     limits[53]=80.0; //PAA
  limits[54]=-10.0;    limits[55]=100.0; //QAA
}

// =======================================
// GET FOURIER COEFFICIENTS FOR ALL MODELS
// =======================================
void getFourierCoefficients(double &ain1,double &ain2,double &ain3,double &ain4,double &ain5,double &ain6,double &ain7,double &ain8,double &ain9,
                            double &bin1,double &bin2,double &bin3,double &bin4,double &bin5,double &bin6,double &bin7,double &bin8,
                            double &aout1,double &aout2,double &aout3,double &aout4,double &aout5,double &aout6,double &aout7,double &aout8,double &aout9,
                            double &bout1,double &bout2,double &bout3,double &bout4,double &bout5,double &bout6,double &bout7,double &bout8){

  ain9  = 25.27; ain1  = 4.215; bin1  = 0.8914; ain2  = -15.74;
  bin2  = 18.16; ain3  = 0.8737; bin3  = 13.62; ain4  = 1.966;
  bin4  = 1.185; ain5  = -0.9142; bin5  = -0.928; ain6  = -0.8623;
  bin6  = -0.7006; ain7  = -0.7609; bin7  = -0.7781; ain8  = -0.3597;
  bin8  = -0.2047;

  aout9 = 52.41; aout1 = -10.71; bout1 = 16.74; aout2 = -8.7; bout2 = 0.7979;
  aout3 = -3.689; bout3 = -3.076; aout4 = -0.3673; bout4 = -2.749; aout5 = 0.8319;       
  bout5 = -1.328; aout6 = 0.7661; bout6 = -0.3316; aout7 = 0.4237;
  bout7 = 0.006969; aout8 = 0.2383; bout8 = 0.04639;
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void odeSingleVentricleHeartIND::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, 
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
  double RAA = params[21];
  double CAA = params[22];

  // ======================
  // ATRIO-VENTRICULAR AREA
  // ======================
  double A_fw = 0.0;
  switch(patientID){
    case piMUSC7:
      A_fw = 2.815;
      break;
    case piGOSH22:
      A_fw = 3.263;
      break;
    case piUM5:
      A_fw = 4.207;
      break;
    case piUM10:
      A_fw = 3.674;
      break;
  }  

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
  getFourierCoefficients(patientID,
                         ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9,
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
  //double Qout1 = aout9 + aout1*cos(teff2*w) + bout1*sin(teff2*w) + aout2*cos(2*teff2*w);
  //double Qout2 = bout2*sin(2*teff2*w)+aout3*cos(3*teff2*w)+bout3*sin(3*teff2*w);
  //double Qout3 = aout4*cos(4*teff2*w)+bout4*sin(4*teff2*w)+aout5*cos(5*teff2*w);
  //double Qout4 = bout5*sin(5*teff2*w)+aout6*cos(6*teff2*w)+bout6*sin(6*teff2*w);
  //double Qout5 = aout7*cos(7*teff2*w)+bout7*sin(7*teff2*w)+aout8*cos(8*teff2*w);
  //double Qout6 = bout8*sin(8*teff2*w);
  //double QAo = Qout1+Qout2+Qout3+Qout4+Qout5+Qout6;
  
  // EVAL AORTIC PRESSURE
  double Pout1 = aout9 + aout1*cos(teff2*w) + bout1*sin(teff2*w) + aout2*cos(2*teff2*w);
  double Pout2 = bout2*sin(2*teff2*w)+aout3*cos(3*teff2*w)+bout3*sin(3*teff2*w);
  double Pout3 = aout4*cos(4*teff2*w)+bout4*sin(4*teff2*w)+aout5*cos(5*teff2*w);
  double Pout4 = bout5*sin(5*teff2*w)+aout6*cos(6*teff2*w)+bout6*sin(6*teff2*w);
  double Pout5 = aout7*cos(7*teff2*w)+bout7*sin(7*teff2*w)+aout8*cos(8*teff2*w);
  double Pout6 = bout8*sin(8*teff2*w);
  double PAo = Pout1+Pout2+Pout3+Pout4+Pout5+Pout6;


  // UNPACK STATE VARIABLES
  double Vsv = Xn[0]; // Volume single ventricle
  double Vsa = Xn[1]; // Volume single atrium
  double Qav = Xn[2]; // Flow Atrial-Ventricular valve
  double PAA = Xn[3]; // Pressure aorta
  //double QAA = Xn[4]; // Flow Atrial-Ventricular valve
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
  if(Psv>PAA){
    //LVOF = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAo)))/(2.0*Kao);
    LVOF = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAA)))/(2.0*Kao);
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
      Xn[2] = 0.0;
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
  double QAA = (PAA-PAo)/(double)RAA;
  double PAAp = (LVOF-QAA)/(double)CAA;
  //double QAAp = (PAA-PAo)/(double)LAA;



  // STORE TIME DERIVATIVES
  Xn1[0] = Vsvp;
  Xn1[1] = Vsap;
  Xn1[2] = Qavp;
  Xn1[3] = PAAp;
  //Xn1[4] = 0;

  // STORE SECONDARY OUTPUT
  out[0] = Psveff;
  out[1] = Psa;
  out[2] = Qven;
  out[3] = LVOF;
  out[4] = tc;
  out[5] = AA;
  out[6] = AV;
  out[7] = Psaactive;
  out[8] = Psapassive;
  out[9] = Psvactive;
  out[10] = Psvpassive;
  out[11] = PAo;
  out[12] = Qreg;
  out[13] = QAA;
}


// =================================
// EVAL SIMULATION TARGET QUANTITIES
// =================================
void EvalSimulationTargets(int totalSteps,double timeStep,double singleCycleTime,double** outVals,
                           double& minPAo, double& maxPAo, double& minPpa, double& maxPpa, double& strokeVolLV){
  double minVol = std::numeric_limits<double>::max();
  double maxVol = -std::numeric_limits<double>::max();
  minPAo = std::numeric_limits<double>::max();
  maxPAo = -std::numeric_limits<double>::max();
  minPpa = std::numeric_limits<double>::max();
  maxPpa = -std::numeric_limits<double>::max();
  strokeVolLV = 0.0;
  double currPAo = 0.0;
  double currPpa = 0.0;
  double currVol = 0.0;
  // WRITE OUTPUT QUANTITIES
  // TARGET - LAST Cycle
  // Max Aortic Pressure: 120 mmHg - max X[9]
  // Min Aortic Pressure: 80 mmHg - min X[9]
  // Min Pulmonary Pressure: 15 mmHg - min X[4]
  // Max Pulmonary Pressure: 35 mmHg - max X[4]
  // Stroke Volume: 80 LV PV loop - fabs(max X[7] - min X[7])
  int startStep = totalSteps - floor(singleCycleTime/(double)timeStep) + 1;
  for(int loopA=startStep;loopA<totalSteps;loopA++){
    currPAo = outVals[loopA][9];
    currPpa = outVals[loopA][4];
    currVol = outVals[loopA][7];
    // PAo
    if(currPAo<minPAo){
      minPAo = currPAo;
    }
    if(currPAo>maxPAo){
      maxPAo = currPAo;
    }
    // Ppa
    if(currPpa<minPpa){
      minPpa = currPpa;
    }
    if(currPpa>maxPpa){
      maxPpa = currPpa;
    }
    // Volume
    if(currVol<minVol){
      minVol = currVol;
    }
    if(currVol>maxVol){
      maxVol = currVol;
    }
  }
  strokeVolLV = maxVol - minVol;
}

// ================================
// MAIN FUNCTION FOR HEART SUBMODEL
// ================================
int solveLPN(int modelID, lpnOptions options, int totalStates, double* iniVals,int totalParams, double* params, double* results){

  // SET PARAMETERS
  // FIXED HR
  double heartRate = params[0]; 
  double cycleTime = 60.0/(double)heartRate;

  // TIME RESULTS
  int numCycles = 8;
  double totalTime = numCycles*cycleTime;
  double timeStep = 0.0008;
  int totalSteps = floor(totalTime/(double)timeStep);
  int totalStepsOnSingleCycle = floor(cycleTime/(double)timeStep);

    // Write Debug Info
  if(options.flushData){
    // Print Parameter Table
    printParamTable(cycleTime,numCycles,totalTime,timeStep,totalSteps,totalStepsOnSingleCycle,params,iniVals);
    // Print Parameter Limits
    printParamLimitsTable(modelID,totalParams+totalStates);
  }

  // CHECK PARAMS ARE VALID
  if(options.applyConstraints){

    if(!areParamsInsideLimits(modelID,totalStates,iniVals,totalParams,params)){
      printf("ERROR: VIOLATED PARAMETER LIMITS.\n");
      return 1;
    }
  }else{
    // CHECK PARAMS INSIDE LIMITS
    if(!isParamsSignCorrect(modelID,totalStates,iniVals,totalParams,params)){
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

  // NUMBER OF AUXILIARY VARIABLES
  int totAuxStates = 14;

  // NUMBER OF RESULTS
  int totalResults = getModelResultTotal();

  // State Variables
  double** outVals = NULL;
  outVals = new double*[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    outVals[loopA] = new double[totalStates];
  }

  // Auxiliary Outputs
  double** auxOutVals;
  auxOutVals = new double*[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    auxOutVals[loopA] = new double[totAuxStates];
  }

  // Solve All Cycles
  PerformRK4Steps(modelID,totalSteps,timeStep,totalStates,totAuxStates,iniVals,totalParams,params,outVals,auxOutVals);

  if(options.writeDebugData){
    WriteResultsToFile(std::string("states_output.txt"),totalSteps,totalStates,t,outVals);
    WriteResultsToFile(std::string("auxStates_output.txt"),totalSteps,totAuxStates,t,auxOutVals);
  }

  // Get thoracic cavity pressure
  // Pth=Pf(t);

  // Copy Auxiliary Variables From result for the last heart cycle
  double Psv[totalStepsOnSingleCycle];
  double Psa[totalStepsOnSingleCycle];
  double Qven[totalStepsOnSingleCycle];
  double LVOF[totalStepsOnSingleCycle];
  double tc2[totalStepsOnSingleCycle];
  double AA[totalStepsOnSingleCycle];
  double AV[totalStepsOnSingleCycle];
  double PsaActive[totalStepsOnSingleCycle];
  double PsaPassive[totalStepsOnSingleCycle];
  double PsvActive[totalStepsOnSingleCycle];
  double PsvPassive[totalStepsOnSingleCycle];
  double PAo[totalStepsOnSingleCycle];
  double Qreg[totalStepsOnSingleCycle];
  double QAA[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Psv[loopA] = auxOutVals[globalInd][ipPsvEff];
    Psa[loopA] = auxOutVals[globalInd][ipPsa];
    Qven[loopA] = auxOutVals[globalInd][ipQven];
    LVOF[loopA] = auxOutVals[globalInd][ipLVOF];
    AA[loopA] = auxOutVals[globalInd][ipAA];
    AV[loopA] = auxOutVals[globalInd][ipAV];
    PsaActive[loopA] = auxOutVals[globalInd][ipPsaActive];
    PsaPassive[loopA] = auxOutVals[globalInd][ipPsaPassive];
    PsvActive[loopA] = auxOutVals[globalInd][ipPsvActive];
    PsvPassive[loopA] = auxOutVals[globalInd][ipPsvPassive];
    PAo[loopA] = auxOutVals[globalInd][ipPAo];
    Qreg[loopA] = auxOutVals[globalInd][ipQreg];
    QAA[loopA] = auxOutVals[globalInd][ipQAA];
  }

  // Copy State Variables From result for the last heart cycle
  double Vsv[totalStepsOnSingleCycle];
  double Vsa[totalStepsOnSingleCycle];
  double Qav[totalStepsOnSingleCycle];
  double PAA[totalStepsOnSingleCycle];
  //double QAA[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Vsv[loopA] = outVals[globalInd][ipVsv];
    Vsa[loopA] = outVals[globalInd][ipVsa];
    Qav[loopA] = outVals[globalInd][ipQav];
    PAA[loopA] = outVals[globalInd][ipPAA];
   // QAA[loopA] = outVals[globalInd][ipQAA];  
 }

  // =========================
  // CHECK FURTHER CONSTRAINTS
  // =========================
  if(options.applyConstraints){
    double maxDiff = 0.0;
    double currDiff = 0.0;
    for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
      currDiff = fabs(PsaActive[loopA]-PsaPassive[loopA]);  
      if(currDiff>maxDiff){
        maxDiff = currDiff;
      }
    }
    if(maxDiff<3.0){
      printf("ERROR: ACTIVE PASSIVE ATRIAL DIFFERENCE.\n");
      return 1;
    }    
  }

  // ===============================
  // CHECK POSITION OF MAX PSVACTIVE
  // ===============================
  int maxInd = 0;
  if(options.applyConstraints){
    maxInd = getMaxLoc(totalSteps,totalSteps-totalStepsOnSingleCycle+1,PsvActive);
    double maxVsv = getMax(totalSteps,totalSteps-totalStepsOnSingleCycle+1,Psv);
    if((Vsv[maxInd]-maxVsv)<-1.0e-3){
      printf("ERROR: PSV LOCATION.\n");
      return 1;
    }
  }

  // WHY 0.5?????
  // GET AVERAGE VALUES
  double CO = trapz(0,totalStepsOnSingleCycle,t,QAA)/(double)cycleTime;
  double mPsa = trapz(0,totalStepsOnSingleCycle,t,Psa)/(double)cycleTime;
  double mPascAo = trapz(0,totalStepsOnSingleCycle,t,PAo)/(double)cycleTime;
  double mQAA = trapz(0,totalStepsOnSingleCycle,t,QAA)/(double)cycleTime;
  double mQven = trapz(0,totalStepsOnSingleCycle,t,Qven)/(double)cycleTime;
  double mQreg = trapz(0,totalStepsOnSingleCycle,t,Qreg)/(double)cycleTime;

  // GET MAXIMUM AND MINIMUM VALUES
  double maxPsv = getMax(0,totalStepsOnSingleCycle,Psv);
  double maxPsa = getMax(0,totalStepsOnSingleCycle,Psa);
  double maxVsv = getMax(0,totalStepsOnSingleCycle,Vsv);
  double minVsv = getMin(0,totalStepsOnSingleCycle,Vsv);
  double maxPAo = getMax(0,totalStepsOnSingleCycle,PAo);
  double minPAo = getMin(0,totalStepsOnSingleCycle,PAo);
  double maxQAA = getMax(0,totalStepsOnSingleCycle,QAA);


  // =====================
  // PLOT DATA IF REQUIRED
  // =====================
  if(options.flushData){
    string outFileName("heartModelData.txt");
    flushHeartModelDataToFile(outFileName,totalSteps-3*totalStepsOnSingleCycle,totalSteps,t,outVals,auxOutVals);
  }

  //% =============
  //% STORE PV-LOOP
  //% =============
  //if(storePVLoop)
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Vsa_UM10.txt'),Vsa_temp','-append');
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Psa_UM10.txt'),Psa_temp','-append');
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Vsv_UM10.txt'),Vsv_temp','-append');
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Psv_UM10.txt'),Psv_temp','-append');
  //end

  // Copy Final Results
  results[0] = mPascAo;
  results[1] = maxPAo;
  results[2] = mPsa;
  results[3] = maxPsv;
  results[4] = maxVsv;
  results[5] = maxQAA;
  results[6] = Psv[0];
  results[7] = maxPsa;
  results[8] = mQreg;

}

// ==============================
// GET TARGETS FOR VARIOUS MODELS
// ==============================
int getAVSD(int model,int res_num,double* meanData,double* stdData){
  double sdDefault = DATA_UNCERTAINTY;
  switch(model){
    case piMUSC7:
      // Average
      meanData[0] = 53.0;   // mPascAo
      meanData[1] = 80.0;   // maxPAo
      meanData[2] = 7.0;    // mPsa
      meanData[3] = 82.0;   // maxPsv
      meanData[4] = 29.0;   // maxVsv
      meanData[5] = 95.2;   // maxQAA
      meanData[6] = 9.88;   // PsvZero
      meanData[7] = 11.437; // maxPsa
      meanData[8] = 0.0;    // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0];
      stdData[1] = sdDefault*meanData[1];
      stdData[2] = sdDefault*meanData[2]; 
      stdData[3] = sdDefault*meanData[3];
      stdData[4] = sdDefault*meanData[4];
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = 0.83;
      stdData[7] = 0.559;
      stdData[8] = sdDefault*meanData[8];
      break;
    case piGOSH22:
      // Average
      meanData[0] = 51.0; // mPascAo
      meanData[1] = 78.0; // maxPAo
      meanData[2] = 6.0;  // mPsa
      meanData[3] = 91.0; // maxPsv
      meanData[4] = 31.0; // maxVsv
      meanData[5] = 97.0; // maxQAA
      meanData[6] = 10.0; // PsvZero
      meanData[7] = 11.0; // maxPsa
      meanData[8] = 0.0;  // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0];
      stdData[1] = sdDefault*meanData[1];
      stdData[2] = sdDefault*meanData[2];
      stdData[3] = sdDefault*meanData[3];
      stdData[4] = sdDefault*meanData[4];
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = sdDefault*meanData[6];
      stdData[7] = sdDefault*meanData[7];
      stdData[8] = sdDefault*meanData[8];
      break;
    case piUM5:
      // Average
      meanData[0] = 53.0; // mPascAo
      meanData[1] = 87.0; // maxPAo
      meanData[2] = 5.0;  // mPsa
      meanData[3] = 90.0; // maxPsv
      meanData[4] = 35.5; // maxVsv
      meanData[5] = 97.4; // maxQAA
      meanData[6] = 6.50; // PsvZero
      meanData[7] = 7.00; // maxPsa
      meanData[8] = 0.0;  // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0]; 
      stdData[1] = sdDefault*meanData[1]; 
      stdData[2] = sdDefault*meanData[2]; 
      stdData[3] = sdDefault*meanData[3]; 
      stdData[4] = sdDefault*meanData[4]; 
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = 1.56;
      stdData[7] = 0.212;
      stdData[8] = sdDefault*meanData[8];
      break;
    case piUM10:
      // Average
      meanData[0] = 72.0;  // mPascAo
      meanData[1] = 113.0; // maxPAo
      meanData[2] = 4.0;   // mPsa
      meanData[3] = 114.0; // maxPsv
      meanData[4] = 30.0;  // maxVsv
      meanData[5] = 81.3;  // maxQAA
      meanData[6] = 6.63;  // PsvZero
      meanData[7] = 8.807; // maxPsa
      meanData[8] = 10.5;  // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0];  
      stdData[1] = sdDefault*meanData[1]; 
      stdData[2] = sdDefault*meanData[2];    
      stdData[3] = sdDefault*meanData[3];  
      stdData[4] = sdDefault*meanData[4];   
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = 1.70;      
      stdData[7] = 0.639;  
      stdData[8] = sdDefault*meanData[8];    
      break;
  }

  // EXCLUDE INVASIVE TARGETS
  if(EXCLUDE_INVASIVE_TARG){
    meanData[2] = 0.0; // mPsa
    meanData[3] = 0.0; // maxPsv
    meanData[6] = 0.0; // PsvZero
    meanData[7] = 0.0; // maxPsa

    stdData[2] = 0.0; // mPsa
    stdData[3] = 0.0; // maxPsv
    stdData[6] = 0.0; // PsvZero
    stdData[7] = 0.0; // maxPsa
  }

  return 0;
}

// ============================
// EVAL LOG-LIKELIHOOD FUNCTION
// ============================
double evalHeartModelLogLikelihood(int model, lpnOptions options, int res_num, double* results){

  // Get Average Values and Standard Deviation for Measurements
  double meanData[res_num];
  double stdData[res_num];
  int error = getAVSD(model,res_num,meanData,stdData);
  if(error != 0){
    printf("Invalid Result Data. Terminating.\n");
    exit(1);
  }

  // PRINT TABLE WITH TARGETS
  if(options.flushData){
    FILE* outFile;
    string resName;
    outFile = fopen("targetTable.txt","w");
    // Print Header
    fprintf(outFile,"\\begin{table}[h!]\n");
    fprintf(outFile,"\\centering\n");
    fprintf(outFile,"\\begin{tabular}{l c c c c}\n");
    fprintf(outFile,"\\toprule\n");
    fprintf(outFile,"Target Quantity & Model Results & Target AV & Target SD & Weight\\\\\n");
    fprintf(outFile,"\\midrule\n");
    for(int loopA=0;loopA<res_num;loopA++){
      resName = getResultName(loopA);
      fprintf(outFile,"%20s & %20f &  %20f & %20f & %20f\\\\\n",resName.c_str(),results[loopA],meanData[loopA],stdData[loopA],1.0);
    }
    fprintf(outFile,"\\bottomrule\n");
    fprintf(outFile,"\\end{tabular}\n");
    fprintf(outFile,"\\caption{Heart model (with Inductance) results for patient \\patient}\n");
    fprintf(outFile,"\\end{table}\n");
    // CLOSE THE FILE
    fclose(outFile);
  }

  // Compute the Log-likelihood
  double logLH = 0.0;
  double meanDiff = 0.0;
  for(int loopA=0;loopA<res_num;loopA++){
    meanDiff = results[loopA]-meanData[loopA];
    if(stdData[loopA]>1.0e-8){
      logLH += (meanDiff*meanDiff)/(stdData[loopA]*stdData[loopA]);   
    }    
  }
  // Return Value
  return logLH;  
}

// =====================================================
// MAIN FUNCTION: SOLVE THE MODEL AND GET THE LIKELIHOOD
// =====================================================
double eval_LPN_loglikelihood(int model, int state_num, int par_num, int res_num, double* params, int &error){
  
  // OPTIONS  
  lpnOptions options;
  options.applyConstraints = false;
  options.flushData = true;
  options.writeDebugData = false;

  // Partition Parameters and Initial Conditions Based on state_num and par_num
  int totalStates = state_num;
  int totalParams = par_num - state_num;
  // Initial Values
  double iniVals[totalStates];
  for(int loopA=0;loopA<totalStates;loopA++){
    iniVals[loopA] = params[totalParams+loopA];
  }
  // Parameters
  double paramsVals[totalParams];
  for(int loopA=0;loopA<totalParams;loopA++){
    paramsVals[loopA] = params[loopA];
  }
  // Results
  double results[res_num];
  for(int loopA=0;loopA<res_num;loopA++){
    results[loopA] = 0.0;
  }

  // SOLVE LPN MODEL 
  error = 0;
  string outCome;
  try{
    error = solveLPN(model,options,totalStates,iniVals,totalParams,paramsVals,results);
  }catch(...){
    error = 1;
  }

  // COMPUTE THE POSTERIOR DISTRIBUTION
  double logLIKE = 0.0;
  if(error == 0){
    logLIKE = evalHeartModelLogLikelihood(model,options,res_num,results);    
  }else{
    logLIKE = 10000.0;
  }

  // PRINT MESSAGE
  if(error == 0){
    printf("Model Results: %d %s, LL: %f\n",simulCount,"OK",logLIKE);	
  }else{
  	printf("Model Results: %d %s, LL: %f\n",simulCount,"ERROR",logLIKE);	
  }
  simulCount++;
  
  // RETURN POSTERIOR VALUE
  return logLIKE;
}
