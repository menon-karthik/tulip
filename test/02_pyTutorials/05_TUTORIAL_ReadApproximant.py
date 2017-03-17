// =======================
// TUTORIAL EXCERCISE 7
// Matrix of Interpolators
// =======================

# include "uqTypes.h"
# include "uqConstants.h"
# include "uqException.h"

# include "uq1DMEApproximant.h"

using namespace std;

// =============
// MAIN FUNCTION
// =============
int main(int argc, char* argv[]){

  try{

    string marginalFile("marginal_0.txt");

    // Create an 1D multi-element approximant file
    uq1DApproximant* currApprox = new uq1DMEApproximant();

    // Import the multi-element approximant from a file
    int error = currApprox->importFromTextFile(marginalFile);
    if(error != 0){
      throw uqException("ERROR: Cannot Read Approximant From File.\n");
    }

    // Evaluate Extremes
    stdVec limits;
    ((uq1DMEApproximant*)currApprox)->getExtremes(limits);

    int numberOfPoints = 100;
    stdVec xVals;
    double currVal = 0.0;
    for(int loopA=0;loopA<numberOfPoints;loopA++){
      currVal = limits[0] + (((loopA)/(double)(numberOfPoints-1)))*(limits[1] - limits[0]);
      xVals.push_back(currVal);
    }

    // Eval all Points
    stdVec yVals;
    currApprox->eval(xVals,yVals);

    // Write Vectors to File
    writeVectorToFile("xVals.txt",xVals);
    writeVectorToFile("yVals.txt",yVals);

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
