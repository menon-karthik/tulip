#include "odeRCRCR.h"

using namespace std;


odeRCRCR::odeRCRCR(double r1, double c, double r2, stdMat p){

}
  
odeRCRCR::~odeRCRCR(){

}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeRCRCR::getParameterTotal(){
  return 8;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeRCRCR::getStateTotal(){
  return 2; 
}

int odeRCRCR::getAuxStateTotal(){
  return 6;
}

int odeRCRCR::getHRIndex(){
  return 7;
}

// ==================
// GET PARAMETER NAME
// ==================
string odeRCRCR::getParamName(int parID){
  string result;
  switch(parID){
    case 0:
    {
      result = "Rtot";
      break;
    }
    case 1:
    {
      result = "Ctot";
      break;
    }
    case 2:
    {      
      result = "C1C2ratio";
      break;
    }
    case 3:
    {      
      result = "R1R2ratio";
      break;
    }
    case 4:
    {      
      result = "R3R2ratio";
      break;
    }
    case 5:
    {      
      result = "Initial-P1";
      break;
    }
    case 6:
    {      
      result = "Initial-P2";
      break;
    }    
    case 7:
    {      
      result = "HR";
      break;
    }    

  }
  return result;
}

// GET MODEL PARAMETERS
void odeRCRCR::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 3.07;  // Rtot
  params[1] = 0.56;  // Ctot
  params[2] = 0.2;   // C1C2_Ratio
  params[3] = 0.2;   // R1R2_Ratio
  params[4] = 0.2;   // R3R2_Ratio
  params[5] = 5.0;   // Initial P1
  params[6] = 5.0;   // Initial P2
  params[7] = 60.0;  // HR
}

// GET PARAMETER RANGES
void odeRCRCR::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] =2.0;  limits[1] =4.0;   // Rtot
  limits[2] =0.5;  limits[3] =3.0;   // tau
  limits[4] =0.1;  limits[5] =1.0;   // C1C2_Ratio
  limits[6] =0.1;  limits[7] =1.0;   // R1R2_Ratio
  limits[8] =0.1;  limits[9] =1.0;   // R3R2_Ratio
  limits[10]=5.0;  limits[11]=5.0; // Initial P1
  limits[12]=5.0;  limits[13]=5.0; // Initial P2
  limits[14]=60.0; limits[15]=60.0;// HR
}

// GET FOURIER COEFFICIENTS FOR ALL MODELS
void getFourierCoefficients(int patientID,
                            double &ain1,double &ain2,double &ain3,double &ain4,double &ain5,double &ain6,double &ain7,double &ain8,double &ain9,
                            double &bin1,double &bin2,double &bin3,double &bin4,double &bin5,double &bin6,double &bin7,double &bin8,
                            double &aout1,double &aout2,double &aout3,double &aout4,double &aout5,double &aout6,double &aout7,double &aout8,double &aout9,
                            double &bout1,double &bout2,double &bout3,double &bout4,double &bout5,double &bout6,double &bout7,double &bout8){

  ain9 = 25.27; ain1 = 4.215; bin1 = 0.8914; ain2 = -15.74; bin2 = 18.16; ain3 = 0.8737; bin3 = 13.62; ain4 = 1.966;
  bin4 = 1.185; ain5 = -0.9142; bin5 = -0.928; ain6 = -0.8623; bin6 = -0.7006; ain7 = -0.7609; bin7 = -0.7781;
  ain8 = -0.3597; bin8 = -0.2047;

  aout9 = 25.26; aout1 = 2.985; bout1 = 38.75; aout2 = -20.02; bout2 = 5.519; aout3 = -4.378; bout3 = -2.792; aout4 = -3.966;
  bout4 = -2.693; aout5 = 0.3378; bout5 = -1.732; aout6 = -1.418; bout6 = -0.7536; aout7 = 0.4665; bout7 = -1.528; 
  aout8 = 0.1099; bout8 = -0.1978;
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void odeRCRCR::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind){
  // INITIALIZE VARIABLES
  // STORE LOCAL COPIES OF PARAMETERS
  double Rtot = params[0];
  double tau = params[1];
  double C1C2ratio = params[2];
  double R1R2ratio = params[3];
  double R3R2ratio = params[4];

  double HR = params[7];
  double tc = (60.0/(double)HR);

  // GET FLOW
  double QAo = linInterp(fn , 0, 1, fmod(t,fn[fn.size()-1][0]) );
  double Qven = 0.0;

  // UNPACK STATE VARIABLES
  double P1 = Xk[0]; // Pressure in 1
  double P2 = Xk[1]; // Pressure in 2

  // Resistances
  double R2 = Rtot/(1.0 + R1R2ratio + R3R2ratio);
  double R1 = R1R2ratio*R2;
  double R3 = R3R2ratio*R2;
  // Capacitance
  double Ctot = tau/(double)Rtot;
  double C2 = Ctot/(1.0 + C1C2ratio);
  double C1 = C1C2ratio*C2;
  // Eval other quantities
  double P0 = P1 + QAo*R1;
  double P3 = P2 - Qven*R3;
  // Eval Derivatives
  double Q2 = (P1-P2)/(double)R2;
  double P1p = (QAo-Q2)/(double)C1;
  double P2p = (Q2-Qven)/(double)C2;
  // Other quantities
  double Q1 = QAo;
  double Q3 = Qven;
  
  // STORE TIME DERIVATIVES
  DXk[0] = P1p;
  DXk[1] = P2p;

  // STORE SECONDARY OUTPUT
  auxOut[0] = P0;
  auxOut[1] = P3;
  auxOut[2] = Q1;
  auxOut[3] = Q2;
  auxOut[4] = Q3;
  auxOut[5] = t;
}


// EXTRACT MODEL RESULTS
void odeRCRCR::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  double cycleTime = timeStep*totalStepsOnSingleCycle;

  // Copy Auxiliary Variables From result for the last heart cycle
  double t[totalStepsOnSingleCycle];
  double P0[totalStepsOnSingleCycle];
  double P3[totalStepsOnSingleCycle];
  double Q1[totalStepsOnSingleCycle];
  double Q2[totalStepsOnSingleCycle];
  double Q3[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    P0[loopA] = auxOutVals[globalInd][ipP0];
    P3[loopA] = auxOutVals[globalInd][ipP3];
    Q1[loopA] = auxOutVals[globalInd][ipQ1];
    Q2[loopA] = auxOutVals[globalInd][ipQ2];
    Q3[loopA] = auxOutVals[globalInd][ipQ3];
    t[loopA]  = auxOutVals[globalInd][ipT];
    
  }

  // Copy State Variables From result for the last heart cycle
  double P1[totalStepsOnSingleCycle];
  double P2[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    P1[loopA] = outVals[globalInd][ipP1];
    P2[loopA] = outVals[globalInd][ipP2];
  }

  // GET AVERAGE VALUES
  double mP0 = trapz(0,totalStepsOnSingleCycle,t,P0)/(double)cycleTime;
  double mP1 = trapz(0,totalStepsOnSingleCycle,t,P1)/(double)cycleTime;
  double mP2 = trapz(0,totalStepsOnSingleCycle,t,P2)/(double)cycleTime;
  double mP3 = trapz(0,totalStepsOnSingleCycle,t,P3)/(double)cycleTime;
  double mQ1 = trapz(0,totalStepsOnSingleCycle,t,Q1)/(double)cycleTime;
  double mQ2 = trapz(0,totalStepsOnSingleCycle,t,Q2)/(double)cycleTime;
  double mQ3 = trapz(0,totalStepsOnSingleCycle,t,Q3)/(double)cycleTime;

  // GET MAXIMUM AND MINIMUM VALUES
  double minP0 = getMin(0,totalStepsOnSingleCycle,P0);
  double maxP0 = getMax(0,totalStepsOnSingleCycle,P0);
  double minP1 = getMin(0,totalStepsOnSingleCycle,P1);
  double maxP1 = getMax(0,totalStepsOnSingleCycle,P1);
  double minP2 = getMin(0,totalStepsOnSingleCycle,P2);
  double maxP2 = getMax(0,totalStepsOnSingleCycle,P2);
  double minP3 = getMin(0,totalStepsOnSingleCycle,P3);
  double maxP3 = getMax(0,totalStepsOnSingleCycle,P3);
  double minQ1 = getMin(0,totalStepsOnSingleCycle,Q1);
  double maxQ1 = getMax(0,totalStepsOnSingleCycle,Q1);
  double minQ2 = getMin(0,totalStepsOnSingleCycle,Q2);
  double maxQ2 = getMax(0,totalStepsOnSingleCycle,Q2);
  double minQ3 = getMin(0,totalStepsOnSingleCycle,Q3);
  double maxQ3 = getMax(0,totalStepsOnSingleCycle,Q3);

  // COPY FINAL RESULTS
  results.clear();
  // P_0 PRESSURE
  results.push_back(minP0);
  results.push_back(mP0);
  results.push_back(maxP0);
  // P_1 PRESSURE
  results.push_back(minP1);
  results.push_back(mP1);
  results.push_back(maxP1);
  // P_2 PRESSURE
  results.push_back(minP2);
  results.push_back(mP2);
  results.push_back(maxP2);
  // P_3 PRESSURE
  results.push_back(minP3);
  results.push_back(mP3);
  results.push_back(maxP3);
  // Q_1 PRESSURE
  results.push_back(minQ1);
  results.push_back(mQ1);
  results.push_back(maxQ1);
  // Q_2 PRESSURE
  results.push_back(minQ2);
  results.push_back(mQ2);
  results.push_back(maxQ2);
  // Q_3 PRESSURE
  results.push_back(minQ3);
  results.push_back(mQ3);
  results.push_back(maxQ3);
}

void odeRCRCR::getResultKeys(stdStringVec& keys){
  // KEYS
  keys.clear();
  // P_0 PRESSURE
  keys.push_back(string("minP0"));
  keys.push_back(string("mP0"));
  keys.push_back(string("maxP0"));
  // P_1 PRESSURE
  keys.push_back(string("minP1"));
  keys.push_back(string("mP1"));
  keys.push_back(string("maxP1"));
  // P_2 PRESSURE
  keys.push_back(string("minP2"));
  keys.push_back(string("mP2"));
  keys.push_back(string("maxP2"));
  // P_3 PRESSURE
  keys.push_back(string("minP3"));
  keys.push_back(string("mP3"));
  keys.push_back(string("maxP3"));
  // Q_1 PRESSURE
  keys.push_back(string("minQ1"));
  keys.push_back(string("mQ1"));
  keys.push_back(string("maxQ1"));
  // Q_2 PRESSURE
  keys.push_back(string("minQ2"));
  keys.push_back(string("mQ2"));
  keys.push_back(string("maxQ2"));
  // Q_3 PRESSURE
  keys.push_back(string("minQ3"));
  keys.push_back(string("mQ3"));
  keys.push_back(string("maxQ3"));

}

// question.  What are the outputs?  Seems like it these are coming from above, but I thought there were only 3?
void odeRCRCR::getFinalOutputs(const stdVec& outputs,stdVec& outs){
  // COMPUTED VALUES
  outs.clear();
  // P0 in mmHg
  outs.push_back(outputs[0]/1333.22);
  outs.push_back(outputs[1]/1333.22);
  outs.push_back(outputs[2]/1333.22);
  // P1 in mmHg
  outs.push_back(outputs[3]/1333.22);
  outs.push_back(outputs[4]/1333.22);
  outs.push_back(outputs[5]/1333.22);
  // P2 in mmHg
  outs.push_back(outputs[6]/1333.22);
  outs.push_back(outputs[7]/1333.22);
  outs.push_back(outputs[8]/1333.22);
  // P3 in mmHg
  outs.push_back(outputs[9]/1333.22);
  outs.push_back(outputs[10]/1333.22);
  outs.push_back(outputs[11]/1333.22);
  // Q1 in L/min
  outs.push_back(outputs[12]*60.0/1000.0);
  outs.push_back(outputs[13]*60.0/1000.0);
  outs.push_back(outputs[14]*60.0/1000.0);
  // Q2 in L/min
  outs.push_back(outputs[15]*60.0/1000.0);
  outs.push_back(outputs[16]*60.0/1000.0);
  outs.push_back(outputs[17]*60.0/1000.0);
  // Q3 in L/min
  outs.push_back(outputs[18]*60.0/1000.0);
  outs.push_back(outputs[19]*60.0/1000.0);
  outs.push_back(outputs[20]*60.0/1000.0);
}

void odeRCRCR::getDataSTD(stdVec& stds){
  // STANDARD DEVIATIONS
  stds.clear();
  stds.push_back(2.0); // min P_0
  stds.push_back(2.0); // max P_0
  stds.push_back(2.0); // av P_0
  // P1 in mmHg
  stds.push_back(2.0);
  stds.push_back(2.0);
  stds.push_back(2.0);
  // P2 in mmHg
  stds.push_back(2.0);
  stds.push_back(2.0);
  stds.push_back(2.0);
  // P3 in mmHg
  stds.push_back(2.0);
  stds.push_back(2.0);
  stds.push_back(2.0);
  // Q1 in L/min
  stds.push_back(0.6);
  stds.push_back(0.6);
  stds.push_back(0.6);
  // Q2 in L/min
  stds.push_back(0.6);
  stds.push_back(0.6);
  stds.push_back(0.6);
  // Q3 in L/min
  stds.push_back(0.6);
  stds.push_back(0.6);
  stds.push_back(0.6);
}

void odeRCRCR::getResultWeigths(stdVec& weights){
  // STANDARD DEVIATIONS
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

}



    
