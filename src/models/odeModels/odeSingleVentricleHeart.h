#ifndef ODESINGLEVENTRICLEHEART_H
#define ODESINGLEVENTRICLEHEART_H

# include <string>
# include "math.h"

# include "uqTypes.h"
# include "odeModel.h"
# include "cmUtils.h"
# include "cmException.h"

// Output State Constants
const int ipVsv = 0;
const int ipVsa = 1;
const int ipPAo = 2;
const int ipQav = 3;

// Auxiliary Output Constants
const int ipPsvEff     = 0;
const int ipPsa        = 1;
const int ipQAo        = 2;
const int ipQven       = 3;
const int ipLVOF       = 4;
const int iptc         = 5;
const int ipAA         = 6;
const int ipAV         = 7;
const int ipPsaActive  = 8;
const int ipPsaPassive = 9;
const int ipPsvActive  = 10;
const int ipPsvPassive = 11;
const int ipQreg       = 12;

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleHeart: public odeModel{
    
    //! Default constructor
    odeSingleVentricleHeart();
    //! Virtual destructor
    virtual ~odeSingleVentricleHeart();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal() = 0;
    virtual int    getStateTotal() = 0;
    virtual int    getAuxStateTotal() = 0;
    virtual void   getDefaultParams(stdVec& params) = 0;
    virtual void   getDefaultParameterLimits(stdVec& limits) = 0;
    virtual string getParamName(int parID) = 0;
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind) = 0;
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;
    virtual void   getResultKeys(stdStringVec& keys) = 0;
    virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs) = 0;
    virtual void   getDataSTD(stdVec& stds) = 0;
    virtual void   getResultWeigths(stdVec& weights) = 0;    
    virtual int    getHRIndex() = 0;
};

#endif // ODESINGLEVENTRICLEHEART_H
