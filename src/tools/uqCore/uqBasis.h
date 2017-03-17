#ifndef UQBASIS_H
#define UQBASIS_H

# include "uqTypes.h"
# include "uqException.h"

class uqBasis{
  public:
  	// Data Member
  	// Order of the Basis
    int basisOrder;
    // Quadrature Order (carefull: not the actual number of quad points)
    int quadratureOrder;
    // Probability Measure at the quadrature points
    stdVec probabilityMeasure;

    // Constructor and Destructor
    uqBasis();
    virtual ~uqBasis();
    // Member Functions
    // Evaluate Intepolator
    virtual int getPolynomialType() = 0;
    virtual double evaluate(double XValue, int order) = 0;
};

#endif // UQBASIS_H
