# ifndef UQQUADRATURERULE_REGULAR_H
# define UQQUADRATURERULE_REGULAR_H

# include <string>
# include "uq1DQuadratureRule.h"
# include "uqException.h"
# include "uqConstants.h"

class uq1DQuadratureRule_Regular: public uq1DQuadratureRule{
  public:
    
    // Constructor and Destructor
    uq1DQuadratureRule_Regular(const int order, const int suppType);
    virtual ~uq1DQuadratureRule_Regular();

    // Generate Points and Weights
    virtual void generatePointsAndWeights();
};

#endif // UQQUADRATURERULE_REGULAR_H
