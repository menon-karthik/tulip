
# include "cmResistanceModel.h"

using namespace std;

// Constructor
cmResistanceModel::cmResistanceModel(double resValue, stdMat distPressHistory){
  stdVec temp;
  resistance = resValue;
  // Distal Pressure time history
  for(int loopA=0;loopA<distPressHistory.size();loopA++){
  	temp.clear();
  	for(int loopB=0;loopB<2;loopB++){
      temp.push_back(distPressHistory[loopA][loopB]);
  	}
  	distPressure.push_back(temp);
  }
}

// FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
int cmResistanceModel::getParameterTotal(){
  return 1;
}
string cmResistanceModel::getParamName(int parID){
  if(parID == 0){
    return string("Resistance");
  }else{
  	throw cmException("ERROR: Invalid Parameter ID in getParamName.\n");
  }
}

int cmResistanceModel::getStateTotal(){
  return 0;
}

int cmResistanceModel::getResultTotal(){
  return 1;
}

string cmResistanceModel::getResultName(int resID){
  throw cmException("ERROR: cmResistanceModel::getResultName not implemented.\n");
}

void cmResistanceModel::getParameterLimits(stdVec& limits){
  limits.resize(2);
  limits[0] = 0.0;
  limits[0] = 2.0;
}

void cmResistanceModel::getDefaultParams(stdVec& params){
  params.resize(1);
  params[0] = 1.0;
}

void cmResistanceModel::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping not implemented.\n");
}

// EVAL MODEL 
double cmResistanceModel::evalModelError(stdVec inputs,stdVec& outputs,stdIntVec& errorCode){
  // First Input: Current Time
  double currTime = inputs[0];
  // Second Input: Current Flow Rate
  double currFlowRate = inputs[1];
  // Get Distal pressure at current time
  double currDistPressure = linInterp(distPressure,0,1,currTime);
  // Output: Pressure at current time
  double currPressure = currDistPressure + currFlowRate * resistance;
  // Write outputs
  outputs.clear();
  outputs.push_back(currPressure);
  // Need to evaluate the resulting likelihood/posterior and return !!!
  return 0;
}

// EVAL MODEL: MULTIPLE INPUTS, TO IMPLEMENT
stdVec cmResistanceModel::evalModelError(stdMat inputs,stdMat& outputs, stdIntVec &errorCode){
  throw cmException("ERROR: evalModelError not implemented.\n");
}
