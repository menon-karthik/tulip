# include "cm_darpaSimple_code3.h"

using namespace std;

cm_darpaSimple_code3::cm_darpaSimple_code3(){
  data = NULL;
  printLevel = 0;
}
cm_darpaSimple_code3::~cm_darpaSimple_code3(){
}

int cm_darpaSimple_code3::getParameterTotal(){
  return 3;
}
int cm_darpaSimple_code3::getStateTotal(){
  return 0;
}
int cm_darpaSimple_code3::getResultTotal(){
  return 1;
}
void cm_darpaSimple_code3::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 0.1125e6; // Pa
  params[1] = 1500.0; // W/m2
  params[2] = 1533.0; // Pa
}
void cm_darpaSimple_code3::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 1.00e3; limits[1] = 1.00e8; // stress [Pa]
  limits[2] = 10.0; limits[3] = 1.0e4; // temperature [W/m2]
  limits[4] = 10.0; limits[5] = 1.0e4; // max temperature [Pa]
}
void cm_darpaSimple_code3::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented for cm_darpaSimple_code3.\n");
}
string cm_darpaSimple_code3::getParamName(int parID){
  if(parID == 0){
    return string("sigma_vm");
  }else if(parID == 1){
    return string("temperature");
  }else if(parID == 2){
    return string("temperature_limit");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}
string cm_darpaSimple_code3::getResultName(int resID){
  if(resID == 0){
    return string("failure_ratio");
  }else{
    throw cmException("ERROR: invalid result ID.\n");
  }  
}

double cm_darpaSimple_code3::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  
  double sigma     = inputs[0];
  double T         = inputs[1];
  double T_max     = inputs[2]; // K
  double alpha     = 0.5;
  double beta      = 0.5;
  double sigma_max = 0.12e6; // Pa
  
  double fr = alpha*(sigma/sigma_max) + beta*(T/T_max);

  // Return pressure and heat loads
  errorCode.clear();
  errorCode.push_back(0);
  outputs.clear();
  outputs.push_back(fr);

  // Set output Std
  stdVec stds;
  stds.push_back(0.2);  // Std on failure ratio : 0.2
  // Set output weight
  stdVec weights;
  weights.push_back(1.0);
  weights.push_back(1.0);
  // Fill keys with result names
  stdStringVec keys;
  for(int loopA=0;loopA<getResultTotal();loopA++){
    keys.push_back(getResultName(loopA));
  }    

  // Eval the log-likelihood
  double result = 0.0;
  if(data != NULL){
    result = data->evalLogLikelihood(keys,outputs,stds,weights);    
  }

  // Return 
  return result;
}



