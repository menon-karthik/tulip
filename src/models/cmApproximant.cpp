# include "cmApproximant.h"

using namespace std;

cmApproximant::cmApproximant(uqApproximant_SE* approx,
                             const stdVec& varSTD,
                             stdMat& data){
  this->approx = approx;
}

cmApproximant::~cmApproximant(){
  delete approx;
}

int cmApproximant::getParameterTotal(){
  return int(approx->limits.size()/2);
}

int cmApproximant::getStateTotal(){
  return 0;
}
int cmApproximant::getResultTotal(){
  return approx->coeffs.size();
}

void cmApproximant::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  for(int loopA=0;loopA<getParameterTotal();loopA++){
    params[loopA] = 0.5*(approx->limits[2*loopA + 0] + approx->limits[2*loopA + 1]);
  }
}

void cmApproximant::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  for(int loopA=0;loopA<2*getParameterTotal();loopA++){
    limits.push_back(approx->limits[loopA]);
  }
}

void cmApproximant::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("cmApproximant::getPriorMapping not Implemented.");
}

string cmApproximant::getParamName(int parID){
  return string("par_") + to_string(parID);
}

string cmApproximant::getResultName(int resID){
  return string("res_") + to_string(resID);
}

double cmApproximant::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  stdMat inputMat;
  stdMat outputMat;

  inputMat.push_back(inputs);
  outputMat = approx->evaluate(inputMat);

  outputs.clear();
  for(int loopA=0;loopA<outputMat[0].size();loopA++){
    outputs.push_back(outputMat[0][loopA]);
  }

  // Save error codes
  errorCode.clear();
  errorCode.push_back(0);

  // Eval Negative log-likelihood
  double result = 0.0;

  for(int loopA=0;loopA<data.size();loopA++){
    for(int loopB=0;loopB<getResultTotal();loopB++){
      result += (data[loopA][loopB] - outputs[loopB])*(data[loopA][loopB] - outputs[loopB])/(varSTD[loopB]*varSTD[loopB]);
    }
  }

  // Return
  return result;
}





