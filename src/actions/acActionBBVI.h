#ifndef ACACTIONBBVI_H
#define ACACTIONBBVI_H

# include "acAction.h"
# include "cmModel.h"

//! Options for the optimizer
struct bbviOptimizerOptions{
  string opt_method;
  stdVec adj;
  stdVec a;
  stdVec b;
  long numSaves;
}

/*! 
Action class for black box variational inference
*/
class acActionBBVI: public acAction{
  private:

    // Adam specific constants
    static const double ggamma = 0.9;
    static const double eps = 1E-8;
    static const double eta = 1E-3; 
    // Adadelta specific constant
    static const double beta1 = 0.9;
    static const double beta2 = 0.999;

  	//! Vector of variational parameter
  	stdVec lam;
    //! Vector with parameter distributions: each component can be "normal", "gamma", "beta"
  	stdStringVec paramDist;
    //! Matrix storing the lam parameters at various iterations
  	stdMat record;

  protected:

  
  public:
    //! Optimization options
    bbviOptimizerOptions opts;
  	//! Default Constructor
  	acActionBBVI();
    //! Virtual Destructor
    virtual ~acActionBBVI();
    // Virtual Go Function
  	virtual int go();
};

#endif //ACACTIONBBVI_H
