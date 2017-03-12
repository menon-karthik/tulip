# include "odeRCR.h"

// Constructor
odeRCR::odeRCR(double r1, double c, double r2, stdMat p){
  stdVec temp;
  proxResistance = r1;
  capacitance = c;
  distResistance = r2;
  for(int loopA=0;loopA<p.size();loopA++){
    temp.clear();
    for(int loopB=0;loopB<p[0].size();loopB++){
      temp.push_back(p[loopA][0]);
      temp.push_back(p[loopA][1]);
    }
    distPressure.push_back(temp);
  }
  iniPressure = 0.0;
}

// EVAL FORCING TERM
void evalForcing(double currTime,double inletFlowRate,const stdMat& distPressure, double& fnQ, double& fnP){
  fnQ = inletFlowRate;
  fnP = linInterp(distPressure,0,1,currTime);
}

// SOLVE SINGLE ODE STEP
void odeRCR::eval(double t, const stdVec& Xk, const stdVec& params, stdVec& DXk, stdVec& auxOut){

  double fnQ = 0.0;
  double fnP = 0.0;

  // STORE LOCAL COPIES OF PARAMETERS
  double R1  = params[0];
  double Cp  = params[1];
  double R2  = params[2];

  // Evaluate Forcing
  evalForcing(t,params[3],distPressure,fnQ,fnP);

  double QIN = fnQ;
  double P2  = fnP;

  // UNPACK STATE VARIABLES
  double P1 = Xk[0]; // Pressure in 1

  // Eval other quantities
  double P0 = P1 + QIN*R1;
  double Q2 = (P1 - P2)/R2;
  // Eval Derivatives
  double P1p = (QIN-Q2)/(double)Cp;
  
  // STORE TIME DERIVATIVES
  DXk[0] = P1p;

  // STORE SECONDARY OUTPUT
  auxOut[0] = P0;
  auxOut[1] = P2;
  auxOut[2] = QIN;
  auxOut[3] = Q2;
}

// Post-process outputs to get results
void odeRCR::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  // TIME STEP ARRAY
  double t[totalSteps];
  t[0] = 0.0;
  for(int loopA=1;loopA<totalSteps;loopA++){
    t[loopA] = t[loopA-1] + timeStep;
  }

  // COPY FINAL RESULTS
  results[0] = auxOutVals[totalSteps-1][ipP0];
  results[1] = outVals[totalSteps-1][0];
  results[2] = auxOutVals[totalSteps-1][ipP2];
  results[3] = auxOutVals[totalSteps-1][ipQ1];
  results[4] = auxOutVals[totalSteps-1][ipQ2];
}

int odeRCR::getParameterTotal(){
  return 3;
}

string odeRCR::getParamName(int parID){
  if(parID == 0){
    return string("Proximal_Resistance");
  }else if(parID == 1){
    return string("Capacitance");
  }else if(parID == 2){
    return string("Distal_Resistance");
  }else{
    throw cmException("ERROR: Invalid Parameter ID in getParamName.\n");
  }
}

int odeRCR::getStateTotal(){
  return 1;
}

int odeRCR::getResultTotal(){
  // Three Pressures and two flowrates
  return 5;
}

string odeRCR::getResultName(int resID){
  throw cmException("ERROR: cmRCRModel::getResultName not implemented.\n");
}

void odeRCR::getParameterLimits(stdVec& limits){
  limits.resize(6);
  for(int loopA=0;loopA<3;loopA++){
    limits[2*loopA + 0] = 0.0;
    limits[2*loopA + 1] = 0.0;
  }
}
void odeRCR::getDefaultParams(stdVec& params){
  params.resize(3);
  params[0] = 0.0;
  params[1] = 0.0;
  params[2] = 0.0;
}
