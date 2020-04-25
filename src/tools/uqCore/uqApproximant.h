#ifndef UQAPPROXIMANT_H
#define UQAPPROXIMANT_H

# include "uqBasis.h"
# include "uqTypes.h"

// General class for one-dimensional approximants
class uqApproximant{
  public:
    
    uqApproximant();
    virtual ~uqApproximant();

    virtual stdMat evaluate(const stdMat& XValue) = 0;
    virtual int    importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0) = 0;
    virtual void   exportToTextFile(string fileName, bool append = false) = 0;
    virtual stdVec getLimits() = 0;
};

#endif // UQAPPROXIMANT_H
