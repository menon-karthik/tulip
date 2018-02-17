#ifndef CMARMVENOUSOCCLUSION_H
#define CMARMVENOUSOCCLUSION_H

# include <string>

# include "uqTypes.h"
# include "cmModel.h"
# include "cmException.h"
# include "cmUtils.h"

const int ipArterialFlux   = 0;
const int ipVenousFlux     = 1;
const int ipVenousPressure = 2;

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cmArmVenousOcclusion: public cmModel{
  public:

  	// CONSTRUCTOR
  	cmArmVenousOcclusion();

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getDefaultParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs, stdIntVec& errorCodes);

};

#endif //CMARMVENOUSOCCLUSION_B_H
