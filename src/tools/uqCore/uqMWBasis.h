#ifndef UQMWBASIS_H
#define UQMWBASIS_H

# include <math.h>

# include "uqConstants.h"
# include "uqTypes.h"
# include "uqBasis.h"
# include "uqPolyBasis.h"
# include "uqOrthoPolyBasis.h"
# include "uq1DQuadratureRule_CC.h"
# include "uqUtils.h"
# include "uqException.h"

class uqMWBasis: public uqBasis{
  public:
    // Data Members
    // Probability Measure Evaluated at double Integration Points
    bool   isMeasureUniform;
    // Internal Multiwavelet Coefficients
    stdMat    monoCoeffs;
    stdIntVec refPoly;
    stdMat    productMat;
    stdMat    gsCoeffs;
    stdVec    normCoeffs;
    // Orthogonal Polynomial Basis
    uqOrthoPolyBasis* orthoPoly;
    // Constructor and Destructor
    uqMWBasis(int order);
    uqMWBasis(int order, int quadpoints);
    uqMWBasis(int order,int quadOrder, stdVec probMeasure);
    virtual ~uqMWBasis();
    // Allocation and Deallocation
    void AllocateMWCoefficients();
    void DeallocateMWCoefficients();
    void formUniformMeasure(int quadOrder);
    
    // Member Functions
    void FormMultiWaveletCoeffs();    
    
    // Eval MultiWavelets
    double EvalMS(double XValue, int scaleIndex, int shiftIndex, int polyOrderIndex);
    double EvalMotherMW(double XValue, int mwIndex);
    double EvalMW(double XValue, int scaleIndex, int shiftIndex, int polyOrderIndex);
    
    // Eval Exact Multiwavelets
    double EvalExactMotherMW(double XValue, int polyOrder, int mwOrder);
    double EvalSingleExactMW(double XValue, int scaleIndex, int shiftIndex, int polyOrder, int mwOrder);

    // Virtual evaluation function
    virtual int getPolynomialType();
    virtual double eval(double XValue, int order);
};

#endif // UQMWBASIS_H
