#ifndef CMDARPABEAMSTRESS_H
#define CMDARPABEAMSTRESS_H

# include <string>

# include "cmModel.h"
# include "cmException.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmDarpaBeamStress: public cmModel{
  public:

  	//! Default constructor
  	cmDarpaBeamStress();
    //! Virtual destructor
    virtual ~cmDarpaBeamStress();

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

#endif //CMDARPABEAMSTRESS_H
