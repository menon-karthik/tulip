#ifndef CMCOMBINEINPUTOUTPUT_H
#define CMCOMBINEINPUTOUTPUT_H

# include "uqTypes.h"
# include "cmException.h"
# include "cmModel.h"

using namespace std;

// This wraps any model and returns both the inputs and the outputs. 

class cmCombineInputOutput: public cmModel{
  public: 
    cmModel* model;

  	cmCombineInputOutput(cmModel* model);
    virtual ~cmCombineInputOutput();
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

#endif 
