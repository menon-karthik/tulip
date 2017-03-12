#ifndef ODEINTEGRATORRK4_H
#define ODEINTEGRATORRK4_H

# include "odeIntegrator.h"

// GENERIC CLASS FOR SAMPLES
class odeIntegratorRK4: public odeIntegrator{
  public:

    // Constructor
    odeIntegratorRK4(odeModel* model);

    // Destructor
    virtual ~odeIntegratorRK4();

    // Perform numerical integration     
    virtual int run(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals) = 0;
};

#endif // ODEINTEGRATORRK4_H
