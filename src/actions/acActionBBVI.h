#ifndef ACACTIONBBVI_H
#define ACACTIONBBVI_H

# include "acAction.h"
# include "acException.h"
# include "cmModel.h"
# include <random>
# include <iostream>

//! Options for the optimizer
struct bbviOptimizerOptions{
};

/*! 
Action class for black box variational inference
*/
class acActionBBVI: public acAction{
  private:

    // Adam specific constants
    static constexpr double ggamma = 0.9;
    static constexpr double eps = 1E-8;
    static constexpr double eta = 1E-3; 
    // Adadelta specific constant
    static constexpr double beta1 = 0.9;
    static constexpr double beta2 = 0.999;

    // Evaluate log_prior
    double log_prior(const stdVec& params);
    double log_joint(const stdVec& params);

    //! Matrix storing the lam parameters at various iterations
  	stdMat record;

  protected:

  
  public:

  	//! Optimization options
    //! Vector of variational parameter and initial guess
    stdVec lam;
    //! Vector with parameter distributions: each component can be "normal", "gamma", "beta"
    stdStringVec paramDist;    
    //! Optimization-related parameters
    string opt_method;
    stdVec adj;
    stdVec a;
    stdVec b;
    long numSaves;
    long totIt;
    int batchSize;
  	//! Default Constructor
  	acActionBBVI();
    //! Virtual Destructor
    virtual ~acActionBBVI();
    //! Initialize Parameters
    void initParameters();
    // Virtual Go Function
  	virtual int go();
};

#endif //ACACTIONBBVI_H
