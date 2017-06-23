#ifndef ODENORMALADULT_H
#define ODENORMALADULT_H

# include "odeModel.h"
# include "cmConstants.h"
# include "cmUtils.h"

// GENERIC CLASS FOR SAMPLES
class odeNormalAdult: public odeModel{
  public:
    // Default constructor
    odeNormalAdult();
    // Virtual destructor
    virtual ~odeNormalAdult();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal();
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual int    getAuxStateTotal();
    virtual void   getParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
};

#endif // ODENORMALADULT_H
