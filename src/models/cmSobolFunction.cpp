# include "cmSobolFunction.h"

using namespace std;

//! Default constructor
cmSobolFunction::cmSobolFunction(){

}

//! Virtual destructor
cmSobolFunction::~cmSobolFunction(){

}

int cmSobolFunction::getParameterTotal(){
  return 4;
}
int cmSobolFunction::getStateTotal(){
  return 0;
}
int cmSobolFunction::getResultTotal(){
  return 1;
}
void cmSobolFunction::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0]=0.0; limits[1]=1.0;
  limits[2]=0.0; limits[3]=1.0;
  limits[4]=0.0; limits[5]=1.0;
  limits[6]=0.0; limits[7]=1.0;
}
void cmSobolFunction::getDefaultParams(stdVec& params){
  params.resize(2*getParameterTotal());
  params[0]=0.5;
  params[1]=0.5;
  params[2]=0.5;
  params[3]=0.5;
}
void cmSobolFunction::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: cmSobolFunction::getPriorMapping Not Implemented.");
}
string cmSobolFunction::getParamName(int parID){
  if(parID > 5){
    throw cmException("ERROR: invalid parameter ID.\n");
  }else{
    return string("Q" + to_string(parID));
  }
}
string cmSobolFunction::getResultName(int resID){
  if(resID > 0){
    throw cmException("ERROR: invalid result ID.\n");
  }else{
    return string("res");
  }
}

double cmSobolFunction::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  stdVec params;
  params.push_back(1.0);
  params.push_back(2.0);
  params.push_back(5.0);
  params.push_back(10.0);
  params.push_back(20.0);
  params.push_back(50.0);
  params.push_back(100.0);
  params.push_back(500.0);

  double res=1.0;
  for(int loopA=0;loopA<4;loopA++){
    res *= (fabs(4.0*inputs[loopA]-2.0) + params[loopA])/(1.0 + params[loopA]);
  }
  for(int loopA=0;loopA<4;loopA++){
    res *= (fabs(4.0*0.5-2.0) + params[loopA])/(1.0 + params[loopA]);
  }

  outputs.clear();
  outputs.push_back(res);
  errorCode.clear();
  errorCode.push_back(0);
  return 0.0;
}



