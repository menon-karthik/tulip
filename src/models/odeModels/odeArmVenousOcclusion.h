#ifndef ODEARMVENOUSOCCLUSION_H
#define ODEARMVENOUSOCCLUSION_H

# include <string>

# include "uqTypes.h"
# include "odeModel.h"
# include "cmUtils.h"

// Define Constants
const int cssCircular   = 0;
const int cssElliptical = 1;
const int cssCollapsed  = 2;

using namespace std;

// GENERIC CLASS FOR SAMPLES
class odeArmVenousOcclusion: public odeModel{
  public:
    // Distal Pressure time history
    stdMat cuffPressureHistory;

    // Constructor
    odeArmVenousOcclusion(const stdMat& pressureHist);

    // Destructor
    virtual ~odeArmVenousOcclusion();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal();
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual int    getAuxStateTotal();
    virtual void   getParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual void   eval(double t,const stdVec& Xk,const stdVec& params,const stdVec& fn, stdVec& DXk, stdVec& auxOut);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
};

#endif // ODEARMVENOUSOCCLUSION_H
