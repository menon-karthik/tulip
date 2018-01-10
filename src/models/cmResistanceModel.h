#ifndef CMRESISTANCEMODEL_H
#define CMRESISTANCEMODEL_H

# include <string>

# include "uqTypes.h"
# include "cmModel.h"
# include "cmResistanceModel.h"
# include "cmException.h"
# include "cmUtils.h"

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cmResistanceModel: public cmModel{
  protected:
    // Data Members
    double resistance;
    // Distal Pressure time history
    stdMat distPressure;
  public:
  	// CONSTRUCTOR
  	cmResistanceModel(double resValue, stdMat distPressHistory);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int getParameterTotal();
    virtual string getParamName(int parID);
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual string getResultName(int resID);
  	virtual void getParameterLimits(stdVec& limits);
  	virtual void getDefaultParams(stdVec& params);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif //CMRESISTANCEMODEL_H
