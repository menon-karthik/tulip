#ifndef ODEINTEGRATOR_H
#define ODEINTEGRATOR_H

# include "odeModel.h"

// Generic class for ODE integrator
class odeIntegrator{
  private:
    // ODE model to integrate
    odeModel* model;

  public:

    // Constructor
    odeIntegrator(odeModel* model);

    // Destructor
    virtual ~odeIntegrator();

    // Perform numerical integration     
    virtual int run(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals) = 0;
};

#endif // ODEINTEGRATOR_H
