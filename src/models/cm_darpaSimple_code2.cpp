# include "cm_darpaSimple_code2.h"

using namespace std;

cm_darpaSimple_code2::cm_darpaSimple_code2(){
}

cm_darpaSimple_code2::~cm_darpaSimple_code2(){
}

int cm_darpaSimple_code2::getParameterTotal(){
  return 3;
}

int cm_darpaSimple_code2::getStateTotal(){
  return 0;
}

int cm_darpaSimple_code2::getResultTotal(){
  return 2;
}

void cm_darpaSimple_code2::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 0.1125e6; // Pa
  params[1] = 1.0e6; // W/m2
  params[2] = 50.0e6; // Pa
}

void cm_darpaSimple_code2::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 1.0; limits[1] = 1.00e8; // pressure [Pa]
  limits[2] = 1.0; limits[3] = 1.0e8; // heat flux [W/m2]
  limits[4] = 10.0e3; limits[5] = 10.0e8; // elastic modulus [Pa]
}

void cm_darpaSimple_code2::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented for cm_darpaSimple_code2.\n");
}

string cm_darpaSimple_code2::getParamName(int parID){
  if(parID == 0){
    return string("pressure");
  }else if(parID == 1){
    return string("heatflux");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}

string cm_darpaSimple_code2::getResultName(int resID){
  if(resID == 0){
    return string("sigma_vm");
  }else if(resID == 1){
    return string("temperature");
  }else{
    throw cmException("ERROR: invalid result ID.\n");
  }  
}

double cm_darpaSimple_code2::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  // Set parameters
  double p               = inputs[0];
  double qdot            = inputs[1];
  double ElasticMod      = inputs[2];
  double alt             = 45000.0;  
  double tileSectionSize = 0.15;
  double tileLength      = 0.1;
  double convCoeff       = 1000.0;
  double alphaCoeff      = 7.2e-7;

  // Compute air properties
  double airTemperature = 0.0;
  double airPressure = 0.0;
  double airDensity = 0.0;
  getAirProps(alt,airTemperature,airPressure,airDensity);

  // Determine max temperature and assume it to be uniform 
  double T = airTemperature + qdot/convCoeff;
  // Stress xx due to pressure
  double sigmaXX = p;
  // Stress yy due to temperature
  double sigmaYY = alphaCoeff * (T - 293.15) * ElasticMod;
  // Shear Determined from equilibrium
  double tauXY = tileLength*sigmaYY/(tileSectionSize/2.0);
  // Determine max thermal stress
  double sigmaVM = sqrt(sigmaXX*sigmaXX + sigmaYY*sigmaYY - sigmaXX*sigmaYY + 3.0*tauXY*tauXY);

  // Return pressure and heat loads
  errorCode.clear();
  errorCode.push_back(0);
  outputs.clear();
  outputs.push_back(sigmaVM);
  outputs.push_back(T);

  // Set output Std
  stdVec stds;
  stds.push_back(5.0e3);  // Std on sigma: 5 KPa
  stds.push_back(50.0);  // Std on Temperature: 50 K
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
    // data->printAndCompare(keys,outputs,weights);
  }

  // Sigma di Von Mises and Temperature
  return result;
}




