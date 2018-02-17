#ifndef CMSIMPLENONLIN_H
#define CMSIMPLENONLIN_H

# include <string>

# include "cmModel.h"
# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cmSimpleNonLin: public cmModel{
  public:
  	// CONSTRUCTOR
  	cmSimpleNonLin();

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual void getDefaultParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif //CMSIMPLENONLIN_H
