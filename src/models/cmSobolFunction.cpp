# include "cmSobolFunction.h"

using namespace std;

//! Default constructor
cmSobolFunction::cmSobolFunction(int numDim, int ftype, const stdVec& pk){
  if(pk.size() != numDim){
    throw cmException("ERROR: Parameter vector with incorrect size.");
  }
  if((ftype < 0) || (ftype > 1)){
    throw cmException("ERROR: Invalid function type.");
  }
  dims = numDim;
  params = pk;
  functionType = ftype;
}

//! Virtual destructor
cmSobolFunction::~cmSobolFunction(){

}

int cmSobolFunction::getParameterTotal(){
  return dims;
}
int cmSobolFunction::getStateTotal(){
  return 0;
}
int cmSobolFunction::getResultTotal(){
  return 1;
}
void cmSobolFunction::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  for(int loopA=0;loopA<getParameterTotal();loopA++){
    limits[2*loopA + 0]=0.0; 
    limits[2*loopA + 1]=1.0;  
  }
}
void cmSobolFunction::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  for(int loopA=0;loopA<getParameterTotal();loopA++){
    params[loopA]=0.5;
  }    
}
void cmSobolFunction::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: cmSobolFunction::getPriorMapping Not Implemented.");
}
string cmSobolFunction::getParamName(int parID){
  if(parID > dims-1){
    throw cmException("ERROR: invalid parameter ID.\n");
  }else{
    return string("par" + to_string(parID));
  }
}
string cmSobolFunction::getResultName(int resID){
  if(resID != 0){
    throw cmException("ERROR: invalid result ID.\n");
  }else{
    return string("res");
  }
}

double cmSobolFunction::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  double res = 1.0;
  if(functionType == 0){
    for(int loopA=0;loopA<dims;loopA++){
      res *= (fabs(4.0*inputs[loopA]-2.0) + params[loopA])/(1.0 + params[loopA]);
    }
  }else{
    for(int loopA=0;loopA<dims;loopA++){
      res *= (1.0 + 3.0*params[loopA]*inputs[loopA]*inputs[loopA])/(1.0 + params[loopA]);
    }
  }
  outputs.clear();
  outputs.push_back(res);
  errorCode.clear();
  errorCode.push_back(0);
  return 0.0;
}



