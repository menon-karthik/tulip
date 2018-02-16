
# include "cmTutorial.h"

using namespace std;

// CONSTRUCTOR
cmTutorial::cmTutorial(){
}

int cmTutorial::getParameterTotal(){
  return 3;
}

string cmTutorial::getParamName(int parID){
  if(parID == 0){
    return string("startingPosition");
  }else if(parID == 1){
    return string("initialVelocity");
  }else if(parID == 2){
    return string("angle");
  }else{
  	throw cmException("ERROR: Invalid Parameter Index.");
  }
}

int cmTutorial::getStateTotal(){
  return 0;
}

int cmTutorial::getResultTotal(){
  // Location of arrival, total time and maximum height
  return 3;
}

void cmTutorial::getDefaultParameterLimits(stdVec& limits){
  limits.resize(6);
  // Starting Position [m]
  limits[0] = 0.0;
  limits[1] = 1.0;
  // Initial Velocity [m/s]
  limits[2] = 0.0;
  limits[3] = 10.0;
  // Angle [degrees]
  limits[4] = 30.0;
  limits[5] = 80.0;
}

void cmTutorial::getDefaultParams(stdVec& params){
  params.resize(3);
  // Starting Position [m]
  params[0] = 1.0;
  // Initial Velocity [m/s]
  params[1] = 5.0;
  // Angle [degrees]
  params[2] = 60.0;
}

void cmTutorial::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: Not Implemented.");
}

double cmTutorial::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  
  double startPos = inputs[0];
  double iniVel   = inputs[1];
  double angle    = inputs[2]*(M_PI/180.0);

  double maxHeight     = (iniVel * iniVel * sin(angle) * sin(angle))/(2.0 * gConst);
  double finalLocation = startPos + (iniVel * iniVel * sin(2.0*angle))/gConst;
  double totalTime     = (2.0 * iniVel * sin(angle))/gConst;
  
  // Create the output quantities
  outputs.clear();
  // Location of arrival
  outputs.push_back(finalLocation);
  // Total time
  outputs.push_back(totalTime);
  // Maximum height
  outputs.push_back(maxHeight);

  // Return Error Code
  errorCode.clear();
  errorCode.push_back(0);

  // Set Keys
  stdStringVec keys;
  keys.push_back(string("finalLocation"));
  keys.push_back(string("totalTime"));
  keys.push_back(string("maxHeight"));

  // Set Computed Values
  stdVec computedValues;
  computedValues.push_back(outputs[0]);
  computedValues.push_back(outputs[1]);
  computedValues.push_back(outputs[2]);

  // Standard Deviations
  stdVec stdFactors;
  stdFactors.push_back(0.1); // finalLocation
  stdFactors.push_back(0.3); // totalTime
  stdFactors.push_back(0.5); // maxHeight

  stdVec weights;
  weights.push_back(1.0);
  weights.push_back(1.0);
  weights.push_back(1.0);

  // Print and compare
  double result = 0.0;
  if(data != NULL){

    // Print the comparison
    data->printAndCompare(keys,computedValues,weights); 
  
    // Evaluate Objective Function
    result = data->evalLogLikelihood(keys,computedValues,stdFactors,weights);
  }

  // Return 
  return result;
}

string cmTutorial::getResultName(int resID){
  return string("");
}
