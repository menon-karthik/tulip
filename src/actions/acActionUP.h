# ifndef ACACTIONUP_H
# define ACACTIONUP_H

# include "acAction.h"
# include "acException.h"
# include "uqSamples.h"
# include "uqApproximant.h"

class acActionUP: public acAction{
  protected:
  	// Input Samples
  	uqSamples* inputs;
    // Output Samples
    uqSamples* outputs;
  	// Output Statistics Vector
    stdMat stats;

  public:
  	acActionUP(uqSamples* locInputs=NULL,uqSamples* locOutputs=NULL);
    virtual ~acActionUP();

  	// Construct stochastic approximation or evaluate samples
  	virtual int go() = 0;
    // Get Approximant
    virtual uqApproximant* getApproximant() = 0;
    // Get Statistics
    virtual stdMat getStatistics() = 0;

};

# endif // ACACTIONUP_H
