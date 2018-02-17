#ifndef CMKRAICHNANORSZAGMODEL_H
#define CMKRAICHNANORSZAGMODEL_H

# include <string>

# include "cmModel.h"
# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cmKraichnanOrszagModel: public cmModel{
  protected:
    int dims;
    double timeStep;
    double totalTime;
    double saveEvery;

  public:
  	// CONSTRUCTOR
  	cmKraichnanOrszagModel(int localDims,double localTimeStep,double localTotalTime, double localSaveEvery);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getDefaultParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif //CMKRAICHNANORSZAGMODEL_H
