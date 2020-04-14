#ifndef UQ1DAPPROXIMANT_SE_H
#define UQ1DAPPROXIMANT_SE_H

# include "uq1DApproximant.h"
# include "uqMWBasis.h"
# include <boost/algorithm/string.hpp>

// APPROXIMANT ON A SINGLE ELEMENT
class uq1DApproximant_SE: public uq1DApproximant{
  public:
    // Data Members
    // Type of Approximant
    approxTypes approxType;
    // Type of Polynomials 
    int polyType;
    // Underlying Basis
    uqBasis* basis;
    // Coefficient Representation
    stdVec coeff;
    // Limits for translation/scaling
    stdVec limits;

    // CONSTRUCTOR AND DESTRUCTOR
    uq1DApproximant_SE();
    // Construct Polynomial Approximant
    uq1DApproximant_SE(int polyType,int order,stdVec locCoeffs,stdVec limits);
    // Construct Ortho Poly or MW Approximant
    uq1DApproximant_SE(approxTypes approxType,int order,int quadOrder,stdVec probMeasure,stdVec locCoeffs,stdVec limits);    
    virtual ~uq1DApproximant_SE();
    
    // Member Functions
    // Evaluate Approximant
    virtual double evaluate(double XValue);

    // Export Multi-element Approximant to File
    virtual void exportToTextFile(string fileName, bool append = false);
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0);
    virtual void normalizeByConstant(double normValue);
    virtual void getExtremes(stdVec& result);
};

#endif // UQ1DPPROXIMANT_SE_H
