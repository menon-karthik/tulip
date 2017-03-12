#ifndef UQ1DQUADRATURERULE_CC_H
#define UQ1DQUADRATURERULE_CC_H

#include "uq1DQuadratureRule.h"

// Clenshaw-Curtis quandrature: SINGLE
class uq1DQuadratureRule_CC: public uq1DQuadratureRule
{
  public:
    // Constructor and Destructor
    uq1DQuadratureRule_CC(const int quadOrder,const int suppType);
    virtual ~uq1DQuadratureRule_CC(){};
    // Member Functions
    virtual void generatePointsAndWeights();
};

// Clenshaw-Curtis quadrature: DOUBLE
class uq1DQuadratureRule_CCDouble: public uq1DQuadratureRule
{
  public:
    // Constructor and Destructor
    uq1DQuadratureRule_CCDouble(const int quadOrder, const int suppType);
    virtual ~uq1DQuadratureRule_CCDouble(){};
    // Member Functions
    virtual void generatePointsAndWeights();
};


#endif // UQ1DQUADRATURERULE_CC_H
