# ifndef ACACTIONUP_H
# define ACACTIONUP_H

# include "acAction.h"
# include "acException.h"
# include "uqSamples.h"

// GENERIC CLASS FOR UNCERTAINTY PROPAGATION ACTION
class acActionUP: public acAction{
  protected:
  	// Input Samples
  	uqSamples* inputs;

    // Output Samples
    uqSamples* outputs;

  	// Output Statistics Vector
    stdVec stats;
  public:

  	// Constructor
  	acActionUP(uqSamples* locInputs=NULL,uqSamples* locOutputs=NULL);

  	// Get Statistics
  	double getStats(int index){return stats[index];}

  	// Perform Action
  	virtual int go(){throw new acException("Not Implemented");}
};

# endif //ACACTIONUP_H
