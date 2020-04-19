# include "odeRCRCR.h"

using namespace std;

odeRCRCR::odeRCRCR(){
}

odeRCRCR::~odeRCRCR(){

}

int odeRCRCR::getParameterTotal(){
  return 9;
}

int odeRCRCR::getHRIndex(){
 return 8;
}

string odeRCRCR::getParamName(int index){
  string returnString;
  switch(index){
    case 0:
      returnString = string("P1_ini");
      break;
    case 1:
      returnString = string("P2_ini");
      break;
    case 2:
      returnString = string("R_1");
      break;
    case 3:
      returnString = string("R_2");
      break;
    case 4:
      returnString = string("R_3");
      break;
    case 5:
      returnString = string("C_1");
      break;
    case 6:
      returnString = string("C_2");
      break;
    case 7:
      returnString = string("P_D");
      break;
    case 8:
      returnString = string("HR");
      break;
  }
  return returnString;
}

int odeRCRCR::getStateTotal(){
  return 2; // P_1, P_2
}

int odeRCRCR::getAuxStateTotal(){
  return 6;
}

void odeRCRCR::getDefaultParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());
    // --- Initial conditions note: max and min
  limits[0]  = 30.0*convertmmHgToBarye; limits[1]  = 30.0*convertmmHgToBarye; // Initia value of P1
  limits[2]  = 30.0*convertmmHgToBarye; limits[3]  = 30.0*convertmmHgToBarye; // Initia value of P2
  limits[4]  = 0.0;                     limits[5]  = 1500.0; // R_1
  limits[6]  = 0.0;                     limits[7]  = 1500.0; // R_2
  limits[8]  = 0.0;                     limits[9]  = 1500.0; // R_3
  limits[10] = 1.0e-8;                  limits[11] = 1.0e-2; // C_1
  limits[12] = 1.0e-8;                  limits[13] = 1.0e-2; // C_2
  limits[14] = 30.0*convertmmHgToBarye; limits[15] = 30.0*convertmmHgToBarye; // P_D, units: CGS
  limits[16] = 60/1.09;                 limits[17] = 60/1.09; // HR
}

void odeRCRCR::getDefaultParams(stdVec& params){ // Question: what do I set these to initially? A: assigned to mean of range

  // Resize Parameter Array
  params.resize(getParameterTotal());
  
  // NOTE: CGS Units: Pressures in Barye, Flowrates in mL/s
  params[0] = 30.0*convertmmHgToBarye;
  params[1] = 30.0*convertmmHgToBarye;
  params[2] = 1000.0;
  params[3] = 1000.0;
  params[4] = 1000.0;
  params[5] = 0.0001;
  params[6] = 0.0001;
  params[7] = 30.0*convertmmHgToBarye;
  params[8] = 60/1.09;
}

void odeRCRCR::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind){

  // Assign the Parameters
  double R_1 = params[0]; // R_1
  double R_2 = params[1]; // R_2
  double R_3 = params[2]; // R_3
  double C_1 = params[3]; // C
  double C_2 = params[4]; // C
  double P_D = params[5]; // P_D

  // Assign state variables
  double P_1 = Xk[0];
  double P_2 = Xk[1];

  // Compute other variables
  double Q_1    = cmUtils::linInterp(fn , 0, 1, fmod(t,fn[fn.size()-1][0]) );
  double Q_2    = (P_1 - P_2)/ R_2;
  double Q_3    = (P_2 - P_D)/ R_3;
  double P_0    = P_1 + R_1*Q_1;
  double V_P1_t = (Q_1 - Q_2) / (double)C_1;
  double V_P2_t = (Q_2 - Q_3) / (double)C_2;
  
  // Store the derivatives
  DXk[0] = V_P1_t;
  DXk[1] = V_P2_t;

  // Get Auxiliary Results  Question. Do I keep t as an auxOut variable?  If so, then I should change # auxVar to 3
  auxOut[0] = t;   // Current Time
  auxOut[1] = P_D; // P_D
  auxOut[2] = P_0; // P_0
  auxOut[3] = Q_1; // Q_1
  auxOut[4] = Q_2; // Q_2
  auxOut[5] = Q_3; // Q_3
}

// Question.  We discussed both P_1 and P_0 last time, but I though that the min,max,mean of P_0 were the only results
void odeRCRCR::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params,const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  // DETERMINE START AND END OF LAST HEART CYCLE
  double cycleTime = totalStepsOnSingleCycle * timeStep;
  double heartRate = 60.0/cycleTime;
  int numCycles = totalSteps/totalStepsOnSingleCycle;
  int startLastCycle = (numCycles-1) * totalStepsOnSingleCycle;
  int stopLastCycle = numCycles * totalStepsOnSingleCycle;
  double output[totalSteps];
  double valveOpening[totalSteps];
  double time[totalSteps];

  // P_0 PRESSURE
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = auxOutVals[2][loopA];
    time[totalSteps] = auxOutVals[0][loopA];
  }
  double minP_0Press = cmUtils::getMin(startLastCycle, stopLastCycle, output);
  double maxP_0Press = cmUtils::getMax(startLastCycle, stopLastCycle, output);          
  double avP_0Press  = cmUtils::trapz(startLastCycle, stopLastCycle, time, output)/(double)cycleTime;

  // Assign Results Based on Model Version
  results.clear();
  // P_0 PRESSURE
  results.push_back(minP_0Press);
  results.push_back(maxP_0Press);
  results.push_back(avP_0Press);
}

void odeRCRCR::getResultKeys(stdStringVec& keys){
  // KEYS
  keys.clear();
  keys.push_back(string("min_P_0"));
  keys.push_back(string("max_P_0"));
  keys.push_back(string("av_P_0"));
}

// question.  What are the outputs?  Seems like it these are coming from above, but I thought there were only 3?
void odeRCRCR::getFinalOutputs(const stdVec& outputs,stdVec& outs){
  // COMPUTED VALUES
  outs.clear();
  outs.push_back(outputs[0]/1333.22);
  outs.push_back(outputs[1]/1333.22);
  outs.push_back(outputs[2]/1333.22);
}

void odeRCRCR::getDataSTD(stdVec& stds){
  // STANDARD DEVIATIONS
  stds.clear();
  stds.push_back(1.0); // min P_0
  stds.push_back(1.0); // max P_0
  stds.push_back(1.0); // av P_0
}

void odeRCRCR::getResultWeigths(stdVec& weights){
  weights.clear();
  weights.push_back(1.0); // min_P_0
  weights.push_back(1.0); // max_P_0
  weights.push_back(1.0); // av_P_0
}
