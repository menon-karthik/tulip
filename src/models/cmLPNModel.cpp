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
  return this->integrator->ode->getParameterTotal();
}

int cmLPNModel::getStateTotal(){
  return this->integrator->ode->getStateTotal();
}

int cmLPNModel::getResultTotal(){
  return this->integrator->ode->getResultTotal();
}

void cmLPNModel::getParameterLimits(stdVec& limits){
  this->integrator->ode->getParameterLimits(limits);
}

void cmLPNModel::getDefaultParams(stdVec& params){
  this->integrator->ode->getDefaultParams(params);
}

string cmLPNModel::getParamName(int parID){
  return this->integrator->ode->getParamName(parID);
}

string cmLPNModel::getResultName(int resID){
  return this->integrator->ode->getResultName(resID);
}

void cmLPNModel::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: cmLPNModel::getPriorMapping Not Implemented.");
}

double cmLPNModel::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){

  // Get Number of states from ode model
  int totSates = integrator->ode->getStateTotal();
  int totParams = integrator->ode->getParameterTotal();

  // Check compatibility between inputs, initial values and parameters
  if(inputs.size() != totSates + totParams){
    throw cmException("ERROR: Input size is not compatible with ODE model.");
  }

  // Set the initial conditions
  stdVec iniVals(totSates,0.0);
  for(int loopA=0;loopA<totSates;loopA++){
    iniVals[loopA] = inputs[loopA];
  }

  // Set the model parameters
  stdVec params(totParams,0.0);
  for(int loopA=totSates;loopA<totSates + totParams;loopA++){
    params[loopA-totSates] = inputs[loopA];
  }

  // Run the integrator
  stdMat outVals;
  stdMat auxOutVals;
  integrator->run(iniVals,params,outVals,auxOutVals);

  // Post-process and write outputs
  int totalStepsOnSingleCycle = int(integrator->totalSteps/(double)integrator->totalCycles);
  integrator->ode->postProcess(integrator->timeStep,totalStepsOnSingleCycle,integrator->totalSteps,outVals,auxOutVals,outputs);

  // Need to evaluate log-likelihood/posterior and return
  return 0;
}
