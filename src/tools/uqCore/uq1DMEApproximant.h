#ifndef UQ1DMEAPPROXIMANT_H
#define UQ1DMEAPPROXIMANT_H

# include "uq1DApproximant.h"
# include "uq1DSEApproximant.h"
# include "uqMWBasis.h"

// CLass for Multi-element Approximants
class uq1DMEApproximant: public uq1DApproximant{
  public:
    // Data Members
    // One basis for each Element
    vector<uq1DSEApproximant*> approx;
    
    // Constructor and Destructor        
    uq1DMEApproximant();
    uq1DMEApproximant(vector<uq1DSEApproximant*> locApprox);
    virtual ~uq1DMEApproximant();
    
    // Member Functions
    // Evaluate Intepolator
    virtual double eval(double XValue);

    // Evaluate the element ID whose the value belongs to
    int getElementID(double value);

    // Export Multi-element Approximant to File
    virtual void exportToTextFile(string fileName, bool append = false);
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0);
    virtual void normalizeByConstant(double normValue);
    virtual void getExtremes(stdVec& result);
};

#endif // UQ1DMWAPPROXIMANT_H
