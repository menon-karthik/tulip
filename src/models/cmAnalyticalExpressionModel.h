#ifndef CMANALYTICALEXPRESSIONMODEL_H
#define CMANALYTICALEXPRESSIONMODEL_H

# include <string>

# include "cmModel.h"
# include "uqTypes.h"

using namespace std;

// MODEL TYPES
const int kAnalyticalModelUnity2D      = 0;
const int kAnalyticalModelLinear2D     = 1;
const int kModelDiscAgarwal            = 2;
const int kModelZabarasSP              = 3;
const int kAnalyticalPiecewiseLinear1D = 4;
const int kAnalyticalConstant          = 5;
const int kAnalyticalLinear            = 6;
const int kAnalyticalQuadratic         = 7;
const int kIdentity                    = 8;
const int kTransformedGaussian         = 12;

// GENERIC CLASS FOR SAMPLES
class cmAnalyticalExpressionModel: public cmModel{
  protected:
    int modelType;
  public:
  	// CONSTRUCTOR
  	cmAnalyticalExpressionModel(int localModelType);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    
    // EVAL MODEL 
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif //CMANALYTICALEXPRESSIONMODEL_H
