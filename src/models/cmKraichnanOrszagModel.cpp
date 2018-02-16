
# include <string>
# include <math.h>

# include "cmKraichnanOrszagModel.h"
# include "cmException.h"

using namespace std;

// CONSTRUCTOR
cmKraichnanOrszagModel::cmKraichnanOrszagModel(int localDims,double localTimeStep,double localTotalTime, double localSaveEvery){
  dims = localDims;
  timeStep = localTimeStep;
  totalTime = localTotalTime;
  saveEvery = localSaveEvery;
}

string cmKraichnanOrszagModel::getResultName(int resID){
  return string("qty");
}

// FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
int cmKraichnanOrszagModel::getParameterTotal(){
  return dims;
};

string cmKraichnanOrszagModel::getParamName(int parID){
  string result("");
  switch(parID){
    case 0:
      result = "param1";
      break;
    case 1:
      result = "param2";
      break;
    case 2:
      result = "param3";
      break;      
  }
  return result;
}

// GET THE TOTAL NUMBER OF STATES
int cmKraichnanOrszagModel::getStateTotal(){
  return 0;
}

// GET NUMBER OF RESULTS
int cmKraichnanOrszagModel::getResultTotal(){
  return 3;
}

// GET LIMITS FOR INITIAL CONDITIONS
void cmKraichnanOrszagModel::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  if(dims == 1){
    limits[0] = -0.1; limits[1] = 0.1;
  }else if(dims == 2){
    limits[0] = -0.1;  limits[1] = 0.1;
    limits[2] = -1.0; limits[3] = 1.0;
  }else if(dims == 3){
    limits[0] = -1.0; limits[1] = 1.0;
    limits[2] = -0.1; limits[3] = 0.1;
    limits[4] = -1.0; limits[5] = 1.0;
  }else{
    throw new cmException("Error: Invalid Number of Dimensions");
  }
}

// GET DEFAULT PARAMETER VALUES
void cmKraichnanOrszagModel::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  if(dims == 1){
    params[0] = 0.0;
  }else if(dims == 2){
    params[0] = 0.0;
    params[1] = 0.0;
  }else if(dims == 3){
    params[0] = 0.0;
    params[1] = 0.0;
    params[2] = 0.0;
  }else{
    throw new cmException("Error: Invalid Number of Dimensions");
  }
}

// EVAL PRIOR MAPPING
void cmKraichnanOrszagModel::getPriorMapping(int priorModelType,int* prPtr){

}

// SET INITIAL CONDITIONS FOR THE KRAICHNAN-ORSZAG PROBLEM
void setKOInitialConditions(int dims,stdVec inputs,stdVec& rho_0){
  if(dims == 1){
    rho_0[0] = 1.0;
    rho_0[1] = 0.2 * inputs[0] - 0.1;
    rho_0[2] = 0.0;
  }else if(dims == 2){
    rho_0[0] = 1.0;
    rho_0[1] = 0.2 * inputs[0] - 0.1;
    rho_0[2] = 2.0 * inputs[1] - 1.0;
  }else if(dims == 3){
    rho_0[0] = 2.0 * inputs[0] - 1.0;
    rho_0[1] = 0.2 * inputs[1] - 0.1;
    rho_0[2] = 2.0 * inputs[2] - 1.0;
  }else{
    throw new cmException("Error: Invalid Number of Dimensions");
  }
}

// Eval KO Function
void Eval_KO_RK4(stdVec Value,stdVec& result){
  result[0] = Value[0] * Value[2];
  result[1] = -Value[1] * Value[2];
  result[2] = -Value[0] * Value[0]+Value[1] * Value[1];
}

// Solve Time Step for KO Problem
void Solve_KO_RK4(double dt, stdVec rho_0, stdVec& rho_1){

  stdVec tempValue;
  tempValue.resize(3);
  stdVec K1;
  stdVec K2;
  stdVec K3;
  stdVec K4;
  K1.resize(3);
  K2.resize(3);
  K3.resize(3);
  K4.resize(3);

  // K1
  for(int loopA=0;loopA<3;loopA++){
    tempValue[loopA] = rho_0[loopA];
  }
  Eval_KO_RK4(tempValue,K1);
  // K2
  for(int loopA=0;loopA<3;loopA++){
    tempValue[loopA] = rho_0[loopA] + dt/2.0 * K1[loopA];
  }
  Eval_KO_RK4(tempValue,K2);
  // K3
  for(int loopA=0;loopA<3;loopA++){
    tempValue[loopA] = rho_0[loopA] + dt/2.0 * K2[loopA];
  }
  Eval_KO_RK4(tempValue,K3);
  // K4
  for(int loopA=0;loopA<3;loopA++){
    tempValue[loopA] = rho_0[loopA] + dt * K3[loopA];
  }
  Eval_KO_RK4(tempValue,K4);
  // Write Values
  for(int loopA=0;loopA<3;loopA++){
    rho_1[loopA] = rho_0[loopA] + dt/6.0 * (K1[loopA] + 2.0 * K2[loopA] + 2.0 * K3[loopA] + K4[loopA]);
  }
}

// ================================
// EVAL MODEL: ONE MODEL EVALUATION
// ================================
double cmKraichnanOrszagModel::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  
  // Init Time
  double currentTime = 0.0;
  int savedSteps = 0;
  int totSteps = int(totalTime/timeStep);
  double currentSaveIncrement = 0.0;

  // Assign Initial Conditions
  stdVec rho_0;
  stdVec rho_1;
  rho_0.resize(3);
  rho_1.resize(3);
  setKOInitialConditions(dims,inputs,rho_0);

  // Allocate Response
  stdVec historyT;
  stdVec historyX;
  stdVec historyY;
  stdVec historyZ;
  historyT.resize(totSteps);
  historyX.resize(totSteps);
  historyY.resize(totSteps);
  historyZ.resize(totSteps);

  // Store Initial Values
  historyT[savedSteps] = currentTime;
  historyX[savedSteps] = rho_0[0];
  historyY[savedSteps] = rho_0[1];
  historyZ[savedSteps] = rho_0[2];


  // Time Loop
  for(int loopA=0;loopA<totSteps;loopA++){    

    // Update Current Time
    currentTime += timeStep;
    currentSaveIncrement += timeStep;

    // Solve Time Step for KO Problem
    Solve_KO_RK4(timeStep,rho_0,rho_1);

    // Store Values
    if(currentSaveIncrement >= saveEvery){
      // Reset Save Increment
      currentSaveIncrement = 0.0;

      // Increment Steps
      savedSteps++;

      // Store
      historyT[savedSteps] = currentTime;
      historyX[savedSteps] = rho_1[0];
      historyY[savedSteps] = rho_1[1];
      historyZ[savedSteps] = rho_1[2];

    }
    
    // Update in time
    for(int loopB=0;loopB<rho_1.size();loopB++){
      rho_0[loopB] = rho_1[loopB];
    }
  }

  // Return the Latest Values
  errorCode.push_back(0);
  outputs.clear();
  outputs.push_back(rho_1[0]);
  outputs.push_back(rho_1[1]);
  outputs.push_back(rho_1[2]);
  //Return 
  return 0.0;
}

