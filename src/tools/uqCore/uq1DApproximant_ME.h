#ifndef UQ1DAPPROXIMANT_ME_H
#define UQ1DAPPROXIMANT_ME_H

# include "uq1DApproximant.h"
# include "uq1DApproximant_SE.h"
# include "uqMWBasis.h"

using namespace std;

// Class for Multi-Element Approximants
class uq1DApproximant_ME: public uq1DApproximant{
  public:
    // Data Members
    // One basis for each Element
    vector<uq1DApproximant_SE*> approx;
    
    // Constructor and Destructor        
    uq1DApproximant_ME();
    uq1DApproximant_ME(vector<uq1DApproximant_SE*> locApprox);
    virtual ~uq1DApproximant_ME();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Evaluate the element ID whose the value belongs to
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] value an abscissa that corresponds to the element whose ID you would like to retrieve
    \return Element number at the specified abscissa
    */
    int getElementID(double value);    
    
    virtual double evaluate(double XValue);
    virtual void exportToTextFile(string fileName, bool append = false);
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0);
    virtual void normalizeByConstant(double normValue);
    virtual void getLimits(stdVec& res);
};

#endif // UQ1DMWAPPROXIMANT_H
