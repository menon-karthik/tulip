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
    vector<double> lambdaZ;

    //! Default constructor
    cmStressStretch_GOH(const stdVec& lambda);
    //! Virtual destructor
    virtual ~cmStressStretch_GOH();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual void getParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);

    void setModelResults(stdVec outputs,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths);
};

#endif // CMSTRESSSTRETCH_GOH
