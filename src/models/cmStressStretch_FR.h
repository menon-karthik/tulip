#ifndef CMSTRESSSTRETCH_FR
#define CMSTRESSSTRETCH_FR

# include <string>
# include <math.h>

# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmStressStretch_FR{
  public:
    // Experimental Stretches
    stdVec sigmaZ;    

    //! Default constructor
    cmStressStretch_FR(const stdVec& sigma);
    //! Virtual destructor
    virtual ~cmStressStretch_FR();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual void getParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif // CMSTRESSSTRETCH_FR
