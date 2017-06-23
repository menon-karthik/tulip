# include <string>
# include <vector>

# include "odeModel.h"
# include "cmUtils.h"

// Output State Constants
const int ipP1 = 0;
const int ipP2 = 1;

// Auxiliary Output Constants
const int ipP0 = 0;
const int ipP3 = 1;
const int ipQ1 = 2;
const int ipQ2 = 3;
const int ipQ3 = 4;
const int ipT  = 5;

using namespace std;

// RCRCR Circuit
class odeRCRCR: public odeModel{
  
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
  	odeRCRCR(double r1, double c, double r2, stdMat p);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,stdVec& DXk,stdVec& auxOut);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
};
