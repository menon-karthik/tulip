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
    //! Virtual destructor
    virtual ~odeRCRCR();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal();
    virtual int    getStateTotal();
    virtual int    getAuxStateTotal();
    virtual void   getDefaultParams(stdVec& params);
    virtual void   getDefaultParameterLimits(stdVec& limits);
    virtual string getParamName(int parID);
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
    virtual void   getResultKeys(stdStringVec& keys);
    virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs);
    virtual void   getDataSTD(stdVec& stds);
    virtual void   getResultWeigths(stdVec& weights);
    virtual int    getHRIndex();
};
