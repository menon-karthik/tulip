#ifndef CM3DPQSURROGATEMODEL_H
#define CM3DPQSURROGATEMODEL_H

# include <string>

# include "uqTypes.h"
# include "uqConstants.h"
# include "uqSamples.h"
# include "uqPolyMatrix.h"
# include "cmModel.h"
# include "cmException.h"

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cm3DPQSurrogateModel: public cmModel{
  protected:
    int polyType;
    int polyOrder;
    int totDims;
    bool normalizeData;
    stdMat limits;
    stdMat expCoeffs;

  public:
  	// CONSTRUCTOR
  	cm3DPQSurrogateModel(int polyType, int polyOrder, bool normalizeData,
                         stdMat limits, stdMat locExpCoeffs);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int getParameterTotal();
    virtual string getParamName(int parID);
    virtual void getParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual string getResultName(int resID);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCodes);
};

#endif //CM3DPQSURROGATEMODEL_H
