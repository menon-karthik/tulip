#ifndef ODEMODEL_H
#define ODEMODEL_H

# include <string>

# include "uqTypes.h"
# include "daData.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class odeModel{
  public:
    //! Data Object containing patient data
    daData* data;
    //! dataset column identifying the specific patient for multiple patients datasets
    int datasetColumn;
    //! List of frozen parameters IDs
    stdIntVec frozenParamIDX;
    //! List of frozen parameters values
    stdVec    frozenParamVAL;

    //! Default constructor
    odeModel();
    //! Virtual destructor
    virtual ~odeModel();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal() = 0;
    virtual int    getStateTotal() = 0;
    virtual int    getResultTotal() = 0;
    virtual int    getAuxStateTotal() = 0;
    virtual void   getParameterLimits(stdVec& limits) = 0;
    virtual void   getDefaultParams(stdVec& params) = 0;
    virtual string getParamName(int parID) = 0;
    virtual string getResultName(int resID) = 0;
    virtual void   eval(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut) = 0;
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;
    
};

#endif // ODEMODEL_H
