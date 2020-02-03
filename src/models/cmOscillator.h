#ifndef CMOSCILLATOR_H
#define CMOSCILLATOR_H

# include "cmModel.h"
# include "cmException.h"
# include <cmath>

using namespace std;

// GENERIC CLASS FOR MODELS
class cmOscillator: public cmModel{
  private:
    //static constexpr double M           = 45;       // kg
    //static constexpr double K           = 181875;   // N/m
    //static constexpr double Xi          = 0.05;     // ratio
    //static constexpr double P0          = 2100;     // N
    //static constexpr double Wb          = 20;       // rad/s
    //static constexpr double W           = sqrt(K / M);
    //static constexpr double Wd          = W * sqrt(1 - Xi * Xi);
    //static constexpr double tolerance   = 1.0e-7;
    //static constexpr double kmax        = 220000;
    //static constexpr double kmin        = 140000;
    //static constexpr double DBL_EPSILON = 2.2204460492503131e-16;
    
    //static double  k;
    //static double  w;
    //static double  bbeta;
    //static double  wd;
    //static double  inter;
    //static double  A;
    //static double  B;

    // Private Functions
    //static double f (double t);
    //static double neg_f (double t);
    double f (double t, double  w, double  bbeta, double  wd, double  inter, double  A, double  B);
    double neg_f (double t, double  w, double  bbeta, double  wd, double  inter, double  A, double  B);

    void Min_Search_Golden_Section(double k, bool isPos, double& a, double& fa, double& b, double& fb, double tolerance);

  public:

  	//! Default constructor
  	cmOscillator();
    //! Virtual destructor
    virtual ~cmOscillator();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
  	virtual void getDefaultParams(stdVec& params);
    virtual void getDefaultParameterLimits(stdVec& limits);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif //CMOSCILLATOR_H
