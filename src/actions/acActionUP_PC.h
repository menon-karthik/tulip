# ifndef ACACTIONUP_PC_H
# define ACACTIONUP_PC_H

# include "acActionUP.h"
# include "acException.h"
# include "uqSamples.h"
# include "uqApproximant.h"
# include "uqApproximant_SE.h"
# include "uqMatrix.h"
# include "uqPolyMatrix.h"
# include "uqAlgorithmBCS.h"

class acActionUP_PC: public acActionUP{
  protected:
  	int order;
  	int polyType;
  	int completeOrderType;
    // Store Coefficient representation for all outputs
    stdMat coeffs;
    // Store Sample limits
    stdVec limits;

  public:
  	acActionUP_PC(uqSamples* in, uqSamples* out, int order, int polyType, int completeOrderType);
    virtual ~acActionUP_PC();

  	// Construct stochastic approximation or evaluate samples
  	virtual int go();
    // Get Approximant
    virtual uqApproximant* getApproximant();
    // Get Statistics
    virtual stdMat getStatistics();

};

# endif // ACACTIONUP_PC_H
