#ifndef UQAPPROXIMANT_ME_H
#define UQAPPROXIMANT_ME_H

# include "uqApproximant.h"
# include "uqApproximant_SE.h"

// Class for Multi-Element Approximants
class uqApproximant_ME: public uqApproximant{
  public:
    // Data Members
    // One basis for each Element
    vector<uqApproximant_SE*> approxList;
    
    // Constructor and Destructor        
    uqApproximant_ME();
    uqApproximant_ME(const vector<uqApproximant_SE*>& locApproxList);
    virtual ~uqApproximant_ME();
    
    virtual stdMat evaluate(const stdMat& XValue);
    virtual void   exportToTextFile(string fileName, bool append = false);
    virtual int    importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0);
    virtual stdVec getLimits();
};

#endif // UQMWAPPROXIMANT_H
