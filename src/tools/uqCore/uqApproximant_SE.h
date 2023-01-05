#ifndef UQAPPROXIMANT_SE_H
#define UQAPPROXIMANT_SE_H

# include "uqApproximant.h"
# include "uqConstants.h"
# include "uqSamples.h"
# include "uqMatrix.h"
# include "uqMWMatrix.h"
# include "uqPolyMatrix.h"

using namespace std;

// General class for one-dimensional approximants
class uqApproximant_SE: public uqApproximant{
  public:
    // Type of Approximant
    approxTypes approxType;
    // Type of Polynomials 
    int polyType;
    // Approximation polynomial order
    int order;
    // Coefficient Representation
    stdMat coeffs;
    // Limits for translation/scaling
    stdVec limits;

    // CONSTRUCTOR AND DESTRUCTOR
    uqApproximant_SE();
    // Construct Ortho Poly or MW Approximant
    uqApproximant_SE(approxTypes approxType,int polyType,int order,const stdMat& coeffs,const stdVec& limits);
    virtual ~uqApproximant_SE();
   
    virtual stdMat evaluate(const stdMat& XValue);
    virtual int importFromFile(string fileName, bool startFromTop = true, int startLine = 0);
    virtual void exportToFile(string fileName, bool append = false);
    virtual stdVec getLimits();
};

#endif // UQAPPROXIMANT_SE_H
