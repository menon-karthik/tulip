# include "odeIntegrator.h"

// Constructor
odeIntegrator::odeIntegrator(odeModel* odeModel, double timeStep, int totalCycles){
  this->ode         = odeModel;
  this->timeStep    = timeStep;
  this->totalCycles = totalCycles;
}

odeIntegrator::~odeIntegrator(){
}

