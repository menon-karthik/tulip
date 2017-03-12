#ifndef ODESINGLEVENTRICLESTAGE1BLOCKSIND2_H
#define ODESINGLEVENTRICLESTAGE1BLOCKSIND2_H

# include "odeModel.h"
# include "cmUtils.h"

using namespace std;

// Output State Constants
const int ipVsv   = 0;
const int ipVsa   = 1;
const int ipPAo   = 2;
const int ipQav   = 3;
const int ipPubv  = 4;
const int ipQuba  = 5;
const int ipPuba  = 6;
const int ipPlung = 7;
const int ipPvp   = 8;
const int ipQapol = 9;
const int ipPpa   = 10;
const int ipPAA   = 11;
const int ipQAA   = 12;

// Auxiliary Output Constants
const int ipPsv        = 0;
const int ipPsa        = 1;
const int ipQAo        = 2;
const int ipT          = 5;
const int ipAA         = 6;
const int ipAV         = 7;
const int ipQvc        = 8;
const int ipQvp        = 9;
const int ipQsh        = 10;
const int ipQaftCao    = 11;
const int ipPsaactive  = 12;
const int ipPsapassive = 13;
const int ipPsvactive  = 14;
const int ipPsvpassive = 15;
const int ipQreg       = 16;

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleStage1BlocksIND2: public odeModel{
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
  	odeSingleVentricleStage1BlocksIND2();

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);

    // EVAL MODEL 
    virtual void   eval(double t,const stdVec& Xk,const stdVec& params,const stdVec& fn, stdVec& DXk, stdVec& auxOut);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
};

#endif // ODESINGLEVENTRICLESTAGE1BLOCKSIND2_H
