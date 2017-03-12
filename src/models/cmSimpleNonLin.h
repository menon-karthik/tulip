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
    virtual string getParamName(int parID);
    virtual int getStateTotal();
    virtual int getResultTotal();
  	virtual void getParameterLimits(double* limits);
  	virtual void getDefaultParams(double* params);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);

    // EVAL MODEL 
    virtual double evalModelError(stdVec inputs,stdVec& outputs, int& errorCode);
    virtual stdVec evalModelError(stdMat inputs,stdMat& outputs, stdIntVec &errorCode);
};

#endif //CMSIMPLENONLIN_H
