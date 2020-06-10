#ifndef CMAPPROXIMANT_H
#define CMAPPROXIMANT_H

# include <string>

# include "cmModel.h"
# include "uqTypes.h"
# include "uqApproximant_SE.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmApproximant: public cmModel{
  public:

    // Single element approximant 
    uqApproximant_SE* approx;
    // Standard deviations for all outputs
    stdVec varSTD;
    // Matrix with output data realizations
    stdMat data;

  	//! Default constructor
  	cmApproximant(uqApproximant_SE* approx,
                  const stdVec& varSTD,
                  stdMat& data);
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
