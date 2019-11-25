#ifndef CM_DARPASIMPLE_CODE1_H
#define CM_DARPASIMPLE_CODE1_H

# include <string>

# include "uqTypes.h"
# include "cmException.h"
# include "daData.h"
# include "cmModel.h"
# include "cmUtils.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cm_darpaSimple_code1: public cmModel{
  public:

  	//! Default constructor
  	cm_darpaSimple_code1();
    //! Virtual destructor
    virtual ~cm_darpaSimple_code1();
    
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

#endif //CM_DARPASIMPLE_CODE1_H
