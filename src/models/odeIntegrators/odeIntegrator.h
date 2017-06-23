#ifndef ODEINTEGRATOR_H
#define ODEINTEGRATOR_H

# include "odeModel.h"

// Generic class for ODE integrator
class odeIntegrator{
  public:

    // Time steps value
    double timeStep;
    // Total Number of cicles in simulation
    int    totalCycles;

    // ODE model to integrate    
    odeModel* ode;    

    // Forcing
    stdMat forcing;

    // Constructor
    odeIntegrator(odeModel* odeModel, double timeStep, int totalCycles);

    // Destructor
    virtual ~odeIntegrator();

    // Perform numerical integration     
    virtual int run(int totalSteps, const stdVec& iniVals,const stdVec& params,stdMat& outVals,stdMat& auxOutVals) = 0;
};

#endif // ODEINTEGRATOR_H
