// =======================
// TUTORIAL EXCERCISE 7
// Matrix of Interpolators
// =======================

# include "uqTypes.h"
# include "uqConstants.h"

# include "uqSamples.h"
# include "uqMatrix.h"
# include "uqPolyMatrix.h"
# include "uqAlgorithmBCS.h"

using namespace std;

// =============
// MAIN FUNCTION
// =============
int main(int argc, char* argv[]){

  try{

    // Construct samples
    uqSamples* samples = new uqSamples();
    samples->AddVariable(string("Var1"),kSAMPLEUniform,-1.0,1.0);
    samples->AddVariable(string("Var2"),kSAMPLEUniform,-1.0,1.0);
    samples->generateCartesianGrid(10,kCC,kHaarRange);

    // Construct Polynomial Matrix on samples
    uqMatrix* polyMat;
    polyMat = new uqPolyMatrix(samples,5,kPolyLegendre,kMIPartialOrder);

    // Eval functional value with simple function
    stdVec rhs;
    rhs.resize(polyMat->getRowCount());
    for(int loopA=0;loopA<polyMat->getRowCount();loopA++){
      rhs[loopA] = samples->getValuesAt(loopA,0) + samples->getValuesAt(loopA,1);
    }    

    // Construct the regression algorithm
    uqAlgorithmBCS* bcs = new uqAlgorithmBCS();

    // Print Progress to screen
    bcs->opts.printProgressToScreen = false;
    bcs->opts.printDBGMessages      = false;

    stdVec bcsCoeffs;
    stdMat coeffPrec;
    double resNorm;
    double bcsReturn = bcs->run(polyMat->getRowCount(),polyMat->getColCount(), 
                                rhs,polyMat->getMatrix(),
                                bcsCoeffs,coeffPrec,resNorm);

    // Print coefficients on screen
    printf("\n");
    printf("Regression Coefficients\n");
    printf("%15s\n","BCS");
    for(int loopA=0;loopA<polyMat->getColCount();loopA++){
      printf("%15f\n",bcsCoeffs[loopA]);
    }
    printf("\n");

  }catch(exception &e){
    // ERROR: TERMINATED!
    printf("\n");
    printf("Msg: %s\n",e.what());
    printf("TERMINATED!\n");
    return 0;
  }

  // COMPLETED
  printf("\n");
  printf("COMPLETED!\n");
  return 0;
}
