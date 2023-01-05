# include "cmCombineInputOutput.h"

using namespace std;

cmCombineInputOutput::cmCombineInputOutput(cmModel* model){
  this->model = model;
}

cmCombineInputOutput::~cmCombineInputOutput(){
  this->model = NULL;
}

int cmCombineInputOutput::getParameterTotal(){
  return model->getParameterTotal();
}

int cmCombineInputOutput::getStateTotal(){
  return model->getStateTotal(); 
}

int cmCombineInputOutput::getResultTotal(){
  return model->getParameterTotal() + model->getResultTotal();
}

void cmCombineInputOutput::getDefaultParams(stdVec& params){
  model->getDefaultParams(params);
}

void cmCombineInputOutput::getDefaultParameterLimits(stdVec& limits){
  model->getDefaultParameterLimits(limits);
}

void cmCombineInputOutput::getPriorMapping(int priorModelType,int* prPtr){
  model->getPriorMapping(priorModelType,prPtr);
}

string cmCombineInputOutput::getParamName(int parID){
  return model->getParamName(parID);
}

string cmCombineInputOutput::getResultName(int resID){
  if(resID<model->getParameterTotal()){
    return model->getParamName(resID);
  }else{
    return model->getResultName(resID-model->getParameterTotal());
  }
}

double cmCombineInputOutput::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  
  stdVec outs;
  double res = model->evalModelError(inputs,outs,errorCode);
  outputs.clear();
  
  // Add inputs
  //printf("Model Evaluation\n");
  for(int loopA=0;loopA<inputs.size();loopA++){
    //printf("%f ",inputs[loopA]);
    outputs.push_back(inputs[loopA]);
  }
  
  // Add outputs
  for(int loopA=0;loopA<outs.size();loopA++){
    //printf("%f ",outs[loopA]);
    outputs.push_back(outs[loopA]);
  }
  //printf("\n");

  stdVec stds;
  stdVec weights;
  stdStringVec keys;
  for(int loopA=0;loopA<getResultTotal();loopA++){
    keys.push_back(getResultName(loopA));
    stds.push_back(0.0);
    weights.push_back(1.0);
  }  

  if(data != NULL){
    res = data->evalLogLikelihood(keys,outputs,stds,weights);
    data->printAndCompare(keys,outputs,weights);
  }else{
    res = 0.0;
  }

  return res;
}

