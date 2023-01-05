#ifndef UQALGORITHMBCS_H
#define UQALGORITHMBCS_H

# include <vector>

# include "uqTypes.h"
# include "uqAlgorithm.h"

using namespace std;

// Definition of Types
const int kActionReestimate       = 0;
const int kActionAdd              = 1;
const int kActionDelete           = 2;
const int kActionFinish           = 3;
const int kActionSkipForAlignment = 4;

// OPTION STRUCT
struct rvmOptionRecord {
  bool estimateNoise;
  bool printProgressToScreen;
  bool printDBGMessages;
  int  maxIteration;
  int  maxBetaIterations;
};

class uqAlgorithmBCS: public uqAlgorithm{
  public:
    // Algorithm Options
    rvmOptionRecord opts;

  	// CONSTRUCTOR
    uqAlgorithmBCS();

    // RUN
    double run(int totSamples, int totBasis, 
               stdVec yValues,stdMat basisMat,
               stdVec &coeffs,stdMat &coeffsCov, 
               double &noiseEstimate);
};
 

#endif // UQALGORITHMBCS_H

