#ifndef odeRC_H
#define odeRC_H

# include <stdio.h>
# include <string>
# include "stdlib.h"
# include "math.h"
# include "uqTypes.h"
# include "odeModel.h"
# include "cmModel.h"
# include "cmConstants.h"
# include "cmUtils.h"
# include "cmException.h"

// GENERIC CLASS FOR SAMPLES
class odeRC: public odeModel{
  public:
    
    // CONSTRUCTOR
    odeRC();

    // DESTRUCTOR
    virtual ~odeRC();

    // VIRTUAL FUNCTIONS TO IMPLEMENT
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getAuxStateTotal();
    virtual void   getDefaultParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
    virtual void   getResultKeys(stdStringVec& keys);
    virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs);
    virtual void   getDataSTD(stdVec& stds);
    virtual void   getResultWeigths(stdVec& weights);
    virtual int    getHRIndex();
};

#endif // odeRC_H
