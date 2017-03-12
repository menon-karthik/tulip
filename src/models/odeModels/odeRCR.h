#ifndef ODERCR_H
#define ODERCR_H

# include "odeModel.h"
# include "cmUtils.h"

// Constants
const int ipP0 = 0;
const int ipP1 = 1;
const int ipP2 = 2;
const int ipQ1 = 3;
const int ipQ2 = 4;

using namespace std;

// GENERIC CLASS FOR SAMPLES
class odeRCR: public odeModel{
  
  protected:
    // Data Members
    double proxResistance;
    double capacitance;
    double distResistance;
    double iniPressure;
    // Distal Pressure time history
    stdMat distPressure;  
  
  public:
  	// CONSTRUCTOR
  	odeRCR(double r1, double c, double r2, stdMat p);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
    virtual void   eval(double t,const stdVec& Xk,const stdVec& params,stdVec& DXk,stdVec& auxOut);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
};

#endif // ODERCR_H
