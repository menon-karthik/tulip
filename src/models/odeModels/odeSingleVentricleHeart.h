#ifndef ODESINGLEVENTRICLEHEART_H
#define ODESINGLEVENTRICLEHEART_H

# include <string>
# include "uqTypes.h"
# include "cmModel.h"
# include "lpnUtils.h"
# include "cmException.h"

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleHeart: public odeModel{
  protected:
    //! Total number of heart cycles to be computed
    int numCycles;
    //! Total number of time step per heart Cycle
    int totalStepsOnSingleCycle;

  private:

    void heartODE(double tn, stdVec Xn, stdVec params, stdVec& Xn1, stdVec& out);
    int  PerformRK4Steps(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals);
    int  solveLPN(lpnOptions options, stdVec inputs, stdVec& outputs);

    bool areParamsInsideLimits(stdVec params);
    bool isParamsSignCorrect(stdVec params);

  public:

    // Constructor
    odeSingleVentricleHeart(int cycles, int stepsPerCycle);

    // Functions that need implementation
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual string getResultName(int index);
    virtual int    getStateTotal();
    virtual int    getAuxStateTotal();
    virtual int    getResultTotal();
    virtual void   getParameterLimits(stdVec& limits);
            void   getDefaultParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCode);
};

#endif // ODESINGLEVENTRICLEHEART_H
