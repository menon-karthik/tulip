#ifndef UQAPPROXIMANT_H
#define UQAPPROXIMANT_H

# include "uqTypes.h"

using namespace std;

const int seFileLength = 7;

// General class for one-dimensional approximants
class uqApproximant{
  public:
    
    uqApproximant();
    virtual ~uqApproximant();

    virtual stdMat evaluate(const stdMat& XValue) = 0;
    virtual stdVec getLimits() = 0;
    virtual int importFromFile(string fileName, bool startFromTop = true, int startLine = 0) = 0;
    virtual void exportToFile(string fileName, bool append = false) = 0;

};

#endif // UQAPPROXIMANT_H
