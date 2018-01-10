#ifndef ODESINGLEVENTRICLEHEARTIND_H
#define ODESINGLEVENTRICLEHEARTIND_H

# include "odeModel.h"

using namespace std;

// Output State Constants
const int ipVsv = 0;
const int ipVsa = 1;
const int ipQav = 2;
const int ipPAA = 3;

// Auxiliary Output Constants
const int ipPsvEff     = 0;
const int ipPsa        = 1;
const int ipQven       = 2;
const int ipLVOF       = 3;
const int iptc         = 4;
const int ipAA         = 5;
const int ipAV         = 6;
const int ipPsaActive  = 7;
const int ipPsaPassive = 8;
const int ipPsvActive  = 9;
const int ipPsvPassive = 10;
const int ipPAo        = 11;
const int ipQreg       = 12;
const int ipQAA        = 13;

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleHeartIND: public odeModel{
  //! Default constructor
  odeSingleVentricleHeartIND();
  //! Virtual destructor
  virtual ~odeSingleVentricleHeartIND();

  // Virtual functions to be implemented by child classes
  virtual int    getParameterTotal() = 0;
  virtual int    getStateTotal() = 0;
  virtual int    getResultTotal() = 0;
  virtual int    getAuxStateTotal() = 0;
  virtual void   getDefaultParams(stdVec& params) = 0;
  virtual void   getDefaultParameterLimits(stdVec& limits) = 0;
  virtual string getParamName(int parID) = 0;
  virtual string getResultName(int resID) = 0;
  virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind) = 0;
  virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;
  virtual void   getResultKeys(stdStringVec& keys) = 0;
  virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs) = 0;
  virtual void   getDataSTD(stdVec& stds) = 0;
  virtual void   getResultWeigths(stdVec& weights) = 0;    
  virtual int    getHRIndex() = 0;
};

#endif // ODESINGLEVENTRICLEHEARTIND_H