# include "odeRCR.h"

using namespace std;

odeRCR::odeRCR(){
}

odeRCR::~odeRCR(){

}

int odeRCR::getParameterTotal(){
  return 6;
}

int odeRCR::getHRIndex(){
 return 5;
}

string odeRCR::getParamName(int index){
  string returnString;
  switch(index){
    case 0:
      returnString = string("P1_ini");
      break;
    case 1:
      returnString = string("R_1");
      break;
    case 2:
      returnString = string("R_2");
      break;
    case 3:
      returnString = string("C");
      break;
    case 4:
      returnString = string("P_D");
      break;
    case 5:
      returnString = string("HR");
      break;
  }
  return returnString;
}

int odeRCR::getStateTotal(){
  return 1; // P_1
}

int odeRCR::getAuxStateTotal(){
  return 5; // t (time), P_0, Q_2
}

void odeRCR::getDefaultParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());
    // --- Initial conditions note: max and min
  limits[0]  = 30.0*convertmmHgToBarye; limits[1]  = 30.0*convertmmHgToBarye; // Initia value of P1
  limits[2]  = 0.0;                     limits[3]  = 1500.0; // R_1
  limits[4]  = 0.0;                     limits[5]  = 1500.0; // R_2
  limits[6]  = 1.0e-8;                  limits[7]  = 1.0e-2; // C
  limits[8]  = 30.0*convertmmHgToBarye; limits[9]  = 30.0*convertmmHgToBarye; // P_D, units: CGS
  limits[10] = 60/1.09;                 limits[11] = 60/1.09; // HR
}

void odeRCR::getDefaultParams(stdVec& params){ // Question: what do I set these to initially? A: assigned to mean of range

  // Resize Parameter Array
  params.resize(getParameterTotal());
  
  // NOTE: CGS Units: Pressures in Barye, Flowrates in mL/s
  params[0] = 30.0*convertmmHgToBarye;
  params[1] = 1000.0;
  params[2] = 1000.0;
  params[3] = 0.0001;
  params[4] = 30.0*convertmmHgToBarye;
  params[5] = 60/1.09;
}

void odeRCR::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind){

  // Assign the Parameters
  double R_1 = params[0]; // R_1
  double R_2 = params[1]; // R_2
  double C   = params[2]; // C
  double P_D = params[3]; // P_D

  // Assign state variables
  double P_1 = Xk[0];

  // Compute other variables
  double Q_1    = cmUtils::linInterp(fn , 0, 1, fmod(t,fn[fn.size()-1][0]) );
  double P_0    = P_1 + R_1*Q_1;
  double Q_2    = (P_1 - P_D)/ R_2; // Q2 = delta(P) = P1 - P2, since P2 < P1 (P2 = P_D)
  double V_P1_t = (Q_1 - Q_2) / (double)C;
  
  // Store the derivatives
  DXk[0] = V_P1_t;

  // Get Auxiliary Results  Question. Do I keep t as an auxOut variable?  If so, then I should change # auxVar to 3
  auxOut[0] = t;   // Current Time
  auxOut[1] = P_D; // P_D
  auxOut[2] = P_0; // P_0
  auxOut[3] = Q_1; // Q_1
  auxOut[4] = Q_2; // Q_2
}

// Question.  We discussed both P_1 and P_0 last time, but I though that the min,max,mean of P_0 were the only results
void odeRCR::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params,const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  // DETERMINE START AND END OF LAST HEART CYCLE
  double cycletime = totalStepsOnSingleCycle * timeStep;
  double heartRate = 60.0/(cycletime);
  int numCycles = totalSteps/totalStepsOnSingleCycle;
  int startLastCycle = (numCycles-1) * totalStepsOnSingleCycle;
  int stopLastCycle = numCycles * totalStepsOnSingleCycle;
  double output[totalSteps];
  double time[totalSteps];
  double valveOpening[totalSteps];

  // P_0 PRESSURE
  for(int loopA=0;loopA<totalSteps;loopA++){
    output[loopA] = auxOutVals[2][loopA];
    time[loopA] = auxOutVals[0][loopA];
  }
  double minP_0Press  = cmUtils::getMin(startLastCycle, stopLastCycle, output);
  double maxP_0Press  = cmUtils::getMax(startLastCycle, stopLastCycle, output);
  double avP_0Press   = cmUtils::trapz(startLastCycle, stopLastCycle, time, output)/double(cycletime);

  // Assign Results Based on Model Version
  results.clear();
  // P_0 PRESSURE
  results.push_back(minP_0Press);
  results.push_back(maxP_0Press);
  results.push_back(avP_0Press);
}

void odeRCR::getResultKeys(stdStringVec& keys){
  // KEYS
  keys.clear();
  keys.push_back(string("min_P_0"));
  keys.push_back(string("max_P_0"));
  keys.push_back(string("av_P_0"));
}

// question.  What are the outputs?  Seems like it these are coming from above, but I thought there were only 3?
void odeRCR::getFinalOutputs(const stdVec& outputs,stdVec& outs){
  // COMPUTED VALUES
  outs.clear();
  outs.push_back(outputs[0]/1333.22);
  outs.push_back(outputs[1]/1333.22);
  outs.push_back(outputs[2]/1333.22);
}

void odeRCR::getDataSTD(stdVec& stds){
  // STANDARD DEVIATIONS
  stds.clear();
  stds.push_back(1.0); // min P_0
  stds.push_back(1.0); // max P_0
  stds.push_back(1.0); // av P_0
}

void odeRCR::getResultWeigths(stdVec& weights){
  weights.clear();
  weights.push_back(1.0); // min_P_0
  weights.push_back(1.0); // max_P_0
  weights.push_back(1.0); // av_P_0
}
