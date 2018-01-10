#ifndef CMSTRESSSTRETCH_SHRA_H
#define CMSTRESSSTRETCH_SHRA_H

# include <vector>

# include "uqTypes.h"
# include "cmModel.h"
# include "cmException.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmStressStretch_SHRA: public cmModel{
  public:
    vector<double> lambdaZ;
    bool includeDataStdAsParam;

    //! Default constructor
    cmStressStretch_SHRA(const stdVec& lambda,bool includeDataStdAsParam);
    //! Virtual destructor
    virtual ~cmStressStretch_SHRA();

    // Non virtual methods
    void setModelResults(stdVec outputs,double dataStd,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths);    

    // Virtual Functions
    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual void getParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif //CMSTRESSSTRETCH_SHRA_H
