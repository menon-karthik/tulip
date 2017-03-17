#ifndef UQ1DAPPROXIMANT_H
#define UQ1DAPPROXIMANT_H

# include "uqBasis.h"
# include "uqTypes.h"

// Types of Approximants
enum approxTypes{atPoly,atOrthoPoly,atMW};

// General class for one-dimensional approximants
class uq1DApproximant{
  public:
    // Constructor and Destructor
    uq1DApproximant();
    virtual ~uq1DApproximant();

    // Member Functions
    // Evaluate Approximant
    virtual double evaluate(double XValue) = 0;
    void evaluate(stdVec XValue,stdVec& results);

    // Export Multi-element Approximant to File
    virtual void exportToTextFile(string fileName, bool append = false) = 0;
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0) = 0;
    virtual void normalizeByConstant(double normValue) = 0;
    virtual void getExtremes(stdVec& result) = 0;
};

#endif // UQ1DAPPROXIMANT_H
