#ifndef CMSTRESSSTRETCH_SHRA_H
#define CMSTRESSSTRETCH_SHRA_H

using namespace std;

// GENERIC CLASS FOR MODELS
class cmStressStretch_SHRA: public cmModel{

  //! Default constructor
  cmStressStretch_SHRA();
  //! Virtual destructor
  virtual ~cmStressStretch_SHRA();

  // Virtual Functions
  virtual int getParameterTotal();
  virtual int getStateTotal();
  virtual int getResultTotal();
  virtual void getParameterLimits(stdVec& limits);
  virtual void getDefaultParams(stdVec& params);
  virtual void getPriorMapping(int priorModelType,int* prPtr);
  virtual string getParamName(int parID);
  virtual string getResultName(int resID);
  virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif //CMSTRESSSTRETCH_SHRA_H
