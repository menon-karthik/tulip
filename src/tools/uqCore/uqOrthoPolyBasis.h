#ifndef UQORTHOPOLYBASIS_H
#define UQORTHOPOLYBASIS_H

#include "uqBasis.h"
#include "uqTypes.h"

class uqOrthoPolyBasis: public uqBasis{
  public:
    // Data Members: TEMP
    stdVec alphaCoeff;
    stdVec betaCoeff;
    stdVec normCoeff;
    
    // Constructor and Destructor
    uqOrthoPolyBasis(int order, int level, const stdVec& measureAtQuadPoints);
    virtual ~uqOrthoPolyBasis();
  
    // Eval Basis
    void buildRecursiveCoefficients();
    virtual int getPolynomialType();
    virtual double evaluate(double XValue, int order);
};


#endif // UQORTHOPOLYBASIS_H
