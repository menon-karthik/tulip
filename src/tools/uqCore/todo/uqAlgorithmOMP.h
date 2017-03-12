#ifndef UQALGORITHMOMP_H
#define UQALGORITHMOMP_H

# include "uqOperator.h"
# include "uqVector.h"
# include "uqAlgorithm.h"
# include "uqOptions.h"
# include "uqUtils.h"
# include "time.h"

// Define Struct to store time info
struct ompTimes {
  double sweep;
  double updateSupport;
  double ls;
  double resUpdate;
};

// OMP Algortihm
class uqAlgorithmOMP: public uqAlgorithm{
  protected:
    ompTimes ompTime;
  
  public:
    // Constructor and Distructor
    uqAlgorithmOMP();
    ~uqAlgorithmOMP();
    // Solve
    int solve(uqOperator* AMat, uqVector* rhs,
              uqOptionsOMP* ompOptions,
              bool useSensingDictionary,
              uqVector* solution);
};

#endif // UQALGORITHMOMP_H
