# include "odeIntegrator.h"

// Constructor
odeIntegrator::odeIntegrator(odeModel* odeModel, double timeStep, int totalSteps, int totalCycles){
  this->ode         = odeModel;
  this->timeStep    = timeStep;
  this->totalSteps  = totalSteps;
  this->totalCycles = totalCycles;
}

odeIntegrator::~odeIntegrator(){
}

