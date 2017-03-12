#ifndef ODEINTEGRATOR_H
#define ODEINTEGRATOR_H

# include "odeModel.h"

// Generic class for ODE integrator
class odeIntegrator{
  public:

    // ODE model to integrate
    odeModel* ode;    

    // Constructor
    odeIntegrator(odeModel* odeModel);

    // Destructor
    virtual ~odeIntegrator();

    // Perform numerical integration     
    virtual int run(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals) = 0;
};

#endif // ODEINTEGRATOR_H
