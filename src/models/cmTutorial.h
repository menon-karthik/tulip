#ifndef CMTUTORIAL_H
#define CMTUTORIAL_H

# include <string>

# include "cmModel.h"
# include "cmException.h"
# include "uqTypes.h"

const double gConst = 9.81;

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cmTutorial: public cmModel{
  public:
  	// CONSTRUCTOR
  	cmTutorial();

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif //CMTUTORIAL_H
