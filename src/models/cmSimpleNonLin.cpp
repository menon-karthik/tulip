# include <string>
# include "cmSimpleNonLin.h"
# include "cmException.h"

using namespace std;

// CONSTRUCTOR
cmSimpleNonLin::cmSimpleNonLin(){
}

int cmSimpleNonLin::getParameterTotal(){
  return 1;
}

string cmSimpleNonLin::getParamName(int parID){
  if(parID == 0){
    return string("Theta");
  }else{
  	throw cmException("ERROR: Invalid Parameter Index.");
  }
}

int cmSimpleNonLin::getStateTotal(){
  return 1;
}

int cmSimpleNonLin::getResultTotal(){
  return 1;
}

string cmSimpleNonLin::getResultName(int resID){
  return string("noName");
}

void cmSimpleNonLin::getParameterLimits(stdVec& limits){
  limits[0] = 0.0;
  limits[1] = 2.0;
}

void cmSimpleNonLin::getDefaultParams(stdVec& params){
  params[0] = 1.0;
}

void cmSimpleNonLin::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: Not Implemented.\n");
}

double cmSimpleNonLin::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  double theta = inputs[0];
  double average = 1.0;
  double sigma = 0.1;
  double a = 2.0*theta*theta;
  double out = a*theta;
  outputs.clear();
  outputs.push_back(out);

  double ll = (out-average)*(out-average)/(sigma*sigma);
  errorCode.clear();
  errorCode.push_back(0);
  return ll;
}
