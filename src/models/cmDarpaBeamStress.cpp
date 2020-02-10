# include "cmDarpaBeamStress.h"

using namespace std;

cmDarpaBeamStress::cmDarpaBeamStress(){

}
cmDarpaBeamStress::~cmDarpaBeamStress(){
}
int cmDarpaBeamStress::getParameterTotal(){
  return 2;
}
int cmDarpaBeamStress::getStateTotal(){
  return 0;
}
int cmDarpaBeamStress::getResultTotal(){
  return 1;
}
void cmDarpaBeamStress::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 1333.33;
  params[1] = 100.0;
}
void cmDarpaBeamStress::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 1300.0; limits[1] = 1370.0; // Wy
  limits[2] = 50.0; limits[3] = 200.0; // sigmay
}
void cmDarpaBeamStress::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented in cmDarpaBeamStress.\n");
}
string cmDarpaBeamStress::getParamName(int parID){
  if(parID == 0){
    return string("W_y");
  }else if(parID == 1){
    return string("sigma_y");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}
string cmDarpaBeamStress::getResultName(int resID){
  if(resID == 0){
    return string("f_u");
  }else{
    throw cmException("ERROR: invalid result ID.\n");
  }
}
double cmDarpaBeamStress::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  double f_u = 0.0;
  double W_y = inputs[0];
  double sigma_y = inputs[1];
  double l = 200.0;
  double d = 20.0;

  // Compute the ultimate load carrying capacity
  f_u = (4.0*W_y*sigma_y)/(l);

  // Sync Outputs
  outputs.clear();
  outputs.push_back(f_u);

  // Set keys, std and weights
  stdStringVec keys;
  keys.push_back(string("fu"));
  
  // Set standard deviations
  stdVec stds;
  stds.push_back(0.01*2.666e3);

  // Weights
  stdVec weights;
  weights.push_back(1.0);

  // Need to evaluate log-likelihood/posterior and return
  double result = 0.0;
  if(data != NULL){
    if(printLevel > 0){
      // Print Info
      data->printAndCompare(keys,outputs,weights); 
    }
    // Evaluate Objective Function
    result = data->evalLogLikelihood(keys,outputs,stds,weights);
  }
  // Return 
  errorCode.clear();
  errorCode.push_back(0);
  return result;
}


