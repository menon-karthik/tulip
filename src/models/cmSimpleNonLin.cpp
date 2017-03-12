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

void cmSimpleNonLin::getParameterLimits(double* limits){
  limits[0] = 0.0;
  limits[1] = 2.0;
}

void cmSimpleNonLin::getDefaultParams(double* params){
  params[0] = 1.0;
}

void cmSimpleNonLin::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: Not Implemented.");
}

double cmSimpleNonLin::evalModelError(stdVec inputs,stdVec& outputs, int& errorCode){
  double theta = inputs[0];
  double average = 1.0;
  double sigma = 0.1;
  double a = 2.0*theta*theta;
  double out = a*theta;
  outputs.clear();
  outputs.push_back(out);

  double ll = (out-average)*(out-average)/(sigma*sigma);
  errorCode = 0;
  return ll;
}

stdVec cmSimpleNonLin::evalModelError(stdMat inputs,stdMat& outputs, stdIntVec &errorCode){
  throw cmException("ERROR: Not Implemented.");	
}

