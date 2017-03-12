# include "cmLPNModel.h"

using namespace std;

// Constructor
cmLPNModel::cmLPNModel(odeIntegrator* integrator){
  this->integrator = integrator;
}

// Distuctor
cmLPNModel::~cmLPNModel(){

}

int cmLPNModel::getParameterTotal(){
  this->integrator->ode->getParameterTotal();
}

int cmLPNModel::getStateTotal(){
  this->integrator->ode->getStateTotal();
}

int cmLPNModel::getResultTotal(){
  this->integrator->ode->getResultTotal();
}

void cmLPNModel::getParameterLimits(stdVec& limits){
  this->integrator->ode->getParameterLimits(limits);
}

void cmLPNModel::getDefaultParams(stdVec& params){
  this->integrator->ode->getDefaultParams(params);
}

string cmLPNModel::getParamName(int parID){
  this->integrator->ode->getParamName(parID);
}

string cmLPNModel::getResultName(int resID){
  this->integrator->ode->getResultName(resID);
}

double cmLPNModel::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){

  // Run the integrator
  integrator->run(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals);

  // Post-process and write outputs
  integrator->postProcess();

}
