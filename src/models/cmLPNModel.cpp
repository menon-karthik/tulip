# include "cmLPNModel.h"

using namespace std;

// Constructor
cmLPNModel::cmLPNModel(){
  this->integrator = integrator;
}

// Distuctor
cmModel::~cmModel(){

}

int getParameterTotal(){
  this->integrator->model->getParameterTotal();
}

int getStateTotal(){
  this->integrator->model->getStateTotal();
}

int getResultTotal(){
  this->integrator->model->getResultTotal();
}

void getParameterLimits(stdVec& limits){
  this->integrator->model->getParameterLimits(limits);
}

void getDefaultParams(stdVec& params){
  this->integrator->model->getDefaultParams(params);
}

string getParamName(int parID){
  this->integrator->model->getParamName(parID);
}

string getResultName(int resID){
  this->integrator->model->getResultName(resID);
}

double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
  integrator->run(inputs,outputs);
  int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals
}
