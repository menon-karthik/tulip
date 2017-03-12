#ifndef ODERCR2_H
#define ODERCR2_H

# include <stdio.h>
# include <string>

# include "cmModel.h"
# include "cmException.h"
# include "lpnUtils.h"

# include "uqTypes.h"
# include "math.h"

// Constants
const int ipP0 = 0;
const int ipP1 = 1;
const int ipP2 = 2;
const int ipQ1 = 3;
const int ipQ2 = 4;

using namespace std;

// GENERIC CLASS FOR SAMPLES
class odeRCR2: public odeModel{
  protected:
    //! Time in seconds for a single heart cycle
    double heartCycleTime;
    //! Total time steps for single heart cycle
    int totalStepsOnSingleCycle;
    //! Total number of heart cycles to be computed
    int numHeartCycles;
    //! Input flow rate time history stored as a two-column matrix (time,flowrate)
    stdMat inletFlowRate; 
    // SOLVE RCR
    int solveRCR(double timeStep, int totalSteps, int totalParams, double* params, double* results);
  public:
  	/*! 
    \verbatim embed:rst
    **Purpose**
    Constructor
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] flowRate the inlet flowrate curve
    \param[in] heartCycleTime the total time for one heart cycle
    \param[in] totalStepsOnSingleCycle total time steps per heart cycles
    \param[in] numHeartCycles total number of heart cycles to solve
    */
  	odeRCR2(stdMat& flowRate,double heartCycleTime,int totalStepsOnSingleCycle,int numHeartCycles);

    // Virtual Function from cmModel
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif // ODERCR2_H
