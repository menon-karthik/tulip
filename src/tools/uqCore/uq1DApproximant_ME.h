#ifndef UQ1DAPPROXIMANT_ME_H
#define UQ1DAPPROXIMANT_ME_H

# include "uq1DApproximant.h"
# include "uq1DApproximant_SE.h"
# include "uqMWBasis.h"

// CLass for Multi-element Approximants
class uq1DApproximant_ME: public uq1DApproximant{
  public:
    // Data Members
    // One basis for each Element
    vector<uq1DApproximant_SE*> approx;
    
    // Constructor and Destructor        
    uq1DApproximant_ME();
    virtual ~uq1DApproximant_ME();

    uq1DApproximant_ME(vector<uq1DApproximant_SE*> locApprox);
    
    virtual double evaluate(double XValue);
    virtual void exportToTextFile(string fileName, bool append = false);
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0);
    virtual void normalizeByConstant(double normValue);
    virtual void getExtremes(stdVec& result);

    // Evaluate the element ID whose the value belongs to
    int getElementID(double value);

};

#endif // UQ1DMWAPPROXIMANT_H
