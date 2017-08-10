#ifndef CMSOBOLFUNCTION_H
#define CMSOBOLFUNCTION_H

# include <string>

# include "uqTypes.h"
# include "cmModel.h"
# include "cmException.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmSobolFunction: public cmModel{
  public:

    //! Default constructor
    cmSobolFunction();
    //! Virtual destructor
    virtual ~cmSobolFunction();

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

#endif // CMSOBOLFUNCTION_H
