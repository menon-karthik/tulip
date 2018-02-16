#ifndef CMSTRESSSTRETCH_GOH
#define CMSTRESSSTRETCH_GOH

# include <string>
# include <vector>

# include "uqTypes.h"
# include "cmModel.h"
# include "cmException.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmStressStretch_GOH: public cmModel{
  public:
    // Experimental Stretches
    stdVec lambdaZ;
    bool includeDataStdAsParam;

    //! Default constructor
    cmStressStretch_GOH(const stdVec& lambda,bool includeDataStdAsParam);
    //! Virtual destructor
    virtual ~cmStressStretch_GOH();

    // Non virtual methods
    void setModelResults(const stdVec& outputs,double dataStd,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths);
  
    // Virtual methods
    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual void getDefaultParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif // CMSTRESSSTRETCH_GOH
