# include "cm_darpaSimple_code1.h"

using namespace std;

cm_darpaSimple_code1::cm_darpaSimple_code1(){
}

cm_darpaSimple_code1::~cm_darpaSimple_code1(){
}

int cm_darpaSimple_code1::getParameterTotal(){
  return 1;
}

int cm_darpaSimple_code1::getStateTotal(){
  return 0;
}

int cm_darpaSimple_code1::getResultTotal(){
  return 2;
}

void cm_darpaSimple_code1::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 7000.0; // m/s
}

void cm_darpaSimple_code1::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 500.0; limits[1] = 10000.0; // m/s 
}

void cm_darpaSimple_code1::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented for cm_darpaSimple_code1.\n");
}
string cm_darpaSimple_code1::getParamName(int parID){
  if(parID == 0){
    return string("velocity");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}
string cm_darpaSimple_code1::getResultName(int resID){
  if(resID == 0){
    return string("pressure");
  }else if(resID == 1){
    return string("heatflux");
  }else{
    throw cmException("ERROR: invalid result ID.\n");
  }
}

double cm_darpaSimple_code1::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  // Get Velocity Parameter
  double vel = inputs[0];

  // Set Parameter Constants
  double alt = 45000.0;
  double sgCoeff = 1.7415e-4;
  double noseCurvRad = 2.0;

  // Get Air property according to atmospheric model
  double airTemperature = 0.0;
  double airPressure = 0.0;
  double airDensity = 0.0;
  getAirProps(alt,airTemperature,airPressure,airDensity);

  // Get heat capacity ratio
  double gamma = 1.4;
    
  // Compute speed of sound at that altitude
  double soundSpeed = 331.3*sqrt(1.0 + (airTemperature/273.15)); // in m/s

  // Compute Mach Number
  double mach = vel/soundSpeed;

  // Solve for heat flux at stagnation point
  double qdot = sgCoeff*(vel*vel*vel)*sqrt(airDensity/noseCurvRad);   

  // Solve for maximum pressure at the stagnation point

  // Compute CpMax
  double coef1 = ((gamma + 1.0)*(gamma + 1.0) * mach*mach)/(4.0 * gamma * mach*mach - 2.0 * (gamma - 1.0));
  double exponent = 0.0;
  double coef2 = 0.0;
  double cpmax = 0.0;  
  if(coef1 > 0.0){
    exponent = (gamma)/(gamma - 1.0);
    coef2 = (1.0 - gamma + 2.0*mach*mach)/(gamma + 1.0);
    cpmax = (2.0/(gamma * mach*mach))*((pow(coef1,exponent))*coef2 - 1.0);
  }else{
    cpmax = 0.0;
  }

  // Determine the angle of each normal
  double dynamicPressure = 0.5*cpmax*airDensity*(vel*vel);
  if(dynamicPressure < 0.0){
    dynamicPressure = 0.0;
  }
  double p = airPressure + dynamicPressure;

  // Return pressure and heat loads
  errorCode.clear();
  errorCode.push_back(0);
  outputs.clear();
  outputs.push_back(p);
  outputs.push_back(qdot);

  // Set output Std
  stdVec stds;
  stds.push_back(277461.0);  // Std on pressure: 0.2 MPa
  stds.push_back(49367.32476940044);  // Std on heatflux: 50 W/m2
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

  // Return Log-likelihood
  return result;
}






