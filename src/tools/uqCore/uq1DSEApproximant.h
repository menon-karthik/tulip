#ifndef UQ1DSEAPPROXIMANT_H
#define UQ1DSEAPPROXIMANT_H

# include "uq1DApproximant.h"
# include "uqMWBasis.h"

// APPROXIMANT ON A SINGLE ELEMENT
class uq1DSEApproximant: public uq1DApproximant{
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
    uq1DSEApproximant();
    // Construct Polynomial Approximant
    uq1DSEApproximant(int polyType,int order,stdVec locCoeffs,stdVec limits);
    // Construct Ortho Poly or MW Approximant
    uq1DSEApproximant(approxTypes approxType,int order,int quadOrder,stdVec probMeasure,stdVec locCoeffs,stdVec limits);    
    virtual ~uq1DSEApproximant();
    
    // Member Functions
    // Evaluate Approximant
    virtual double eval(double XValue);

    // Export Multi-element Approximant to File
    virtual void exportToTextFile(string fileName, bool append = false);
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0);
    virtual void normalizeByConstant(double normValue);
    virtual void getExtremes(stdVec& result);
};

#endif // UQ1DSEAPPROXIMANT_H
