#ifndef UQPOLYBASIS_H
#define UQPOLYBASIS_H

#include <math.h>

#include "uqBasis.h"
#include "uqConstants.h"

class uqPolyBasis: public uqBasis{
  public:
    // Data Members
    int polyType;
    
    // Constructor and Destructor
    uqPolyBasis(const int Type, int order);
    uqPolyBasis(const int Type);
    ~uqPolyBasis();
  
    // Eval Interpolator
    virtual int getPolynomialType();
    virtual double eval(double XValue, int order);
};

#endif // UQPOLYBASIS_H
