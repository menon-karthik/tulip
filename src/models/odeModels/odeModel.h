#ifndef ODEMODEL_H
#define ODEMODEL_H

# include <string>

# include "uqTypes.h"
# include "daData.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class odeModel{
  public:

    //! Default constructor
    odeModel();
    //! Virtual destructor
    virtual ~odeModel();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal() = 0;
    virtual int    getStateTotal() = 0;
    virtual int    getResultTotal() = 0;
    virtual int    getAuxStateTotal() = 0;
    virtual void   getDefaultParams(stdVec& params) = 0;
    virtual void   getDefaultParameterLimits(stdVec& limits) = 0;
    virtual string getParamName(int parID) = 0;
    virtual string getResultName(int resID) = 0;
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind) = 0;
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;
    virtual void   getResultKeys(stdStringVec& keys) = 0;
    virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs) = 0;
    virtual void   getDataSTD(stdVec& stds) = 0;
    virtual void   getResultWeigths(stdVec& weights) = 0;    
    virtual int    getHRIndex() = 0;
    
};

#endif // ODEMODEL_H
