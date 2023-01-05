#ifndef CMAPPROXIMANT_H
#define CMAPPROXIMANT_H

# include <string>

# include "cmModel.h"
# include "uqTypes.h"
# include "uqApproximant_ME.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmApproximant: public cmModel{
  public:

    // Multielement Approximant
    uqApproximant_ME* approx;

  	//! Default constructor
  	cmApproximant(string fileName);
    //! Virtual destructor
    virtual ~cmApproximant();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
  	virtual void getDefaultParams(stdVec& params);
    virtual void getDefaultParameterLimits(stdVec& limits);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif // CMAPPROXIMANT_H
