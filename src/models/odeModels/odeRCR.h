#ifndef odeRCR_KKH_H
#define odeRCR_KKH_H

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

// Constants for Result Quantities
const int ip_min_P_0 = 0;
const int ip_max_P_0 = 1;
const int ip_av_P_0  = 2;

// GENERIC CLASS FOR SAMPLES
class odeRCR: public odeModel{
  public:
    
    // CONSTRUCTOR
    odeRCR();

    // DESTRUCTOR
    virtual ~odeRCR();

    // VIRTUAL FUNCTIONS TO IMPLEMENT
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual int    getAuxStateTotal();
    virtual string getResultName(int resID);
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

#endif // odeRCR_KKH_H
