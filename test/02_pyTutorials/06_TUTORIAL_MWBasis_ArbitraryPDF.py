// ================================
// TUTORIAL EXCERCISE 9
// Build a Multiwavelet Basis
// from a multi-element approximant 
// found in files
// ================================

# include "uqTypes.h"
# include "uqConstants.h"
# include "uqException.h"

# include "uq1DMEApproximant.h"

using namespace std;

void performMWOrthogonalityTest(uqSamples* grid, uqMatrix* mwMat, stdMat measure){

  // Get Maximum Weight Order
  int maxOrder = grid->getMaxWeightOrder();

  // Assumes the matrix is evaluated at a grid of integration points
  double integral    = 0.0;
  double firstBasis  = 0.0;
  double secondBasis = 0.0;
  double currMeasure = 0.0;
  double intWeigth   = 0.0;
  int    currIDX     = 0;
  int    testIDX     = 0;
  printf("\n");
  printf("MW ORTHOGONALITY TEST\n");
  printf("Number of Basis: %d\n",mwMat->getColCount());
  printf("Number of Samples: %d\n",mwMat->getRowCount());
  for(int loopA=0;loopA<mwMat->getColCount();loopA++){    
    for(int loopB=0;loopB<mwMat->getColCount();loopB++){
      integral = 0.0; 
      for(int loopC=0;loopC<mwMat->getRowCount();loopC++){
        firstBasis  = mwMat->getMatrixAt(loopC,loopA);
        secondBasis = mwMat->getMatrixAt(loopC,loopB);
        intWeigth   = grid->getWeightAt(loopC,maxOrder);
        currMeasure = 1.0;
        currIDX = loopC;        
        for(int loopD=0;loopD<grid->getTotDims();loopD++){
          testIDX = currIDX % grid->getTotSamples();
          currMeasure *= measure[loopD][testIDX];
          currIDX = currIDX / grid->getTotSamples();
        }
        integral += firstBasis * secondBasis * currMeasure * intWeigth;
      }
      printf("%15.6e ",integral);
    }
    printf("\n");
  }
  printf("\n");
}

void scale1DGridOnPartition(uqSamples* points,double lb,double ub,stdVec& outLocs){
  double sampleValue = 0.0;
  outLocs.clear();
  for(int loopA=0;loopA<points->getTotSamples();loopA++){
    sampleValue = points->getValuesAt(loopA,0);
    outLocs.push_back(lb + sampleValue * (ub-lb));
  }
}

// =============
// MAIN FUNCTION
// =============
int main(int argc, char* argv[]){

  try{

    // READ MARGINAL FROM FILE
    uq1DMEApproximant* approx = new uq1DMEApproximant();
    string marginalFile(argv[1]);
    int error = approx->importFromTextFile(marginalFile);
    if(error != 0){
      throw uqException("ERROR: Cannot Read Approximant From File.\n");
    }
    // EVALUATE RANGE
    stdVec tmpLimits;
    approx->getExtremes(tmpLimits);
    double measureSize = fabs(tmpLimits[1] - tmpLimits[0]);
    printf("CURRENT LIMITS: %f %f\n",tmpLimits[0],tmpLimits[1]);
    
    // FORM SAMPLES USING INTEGRATION GRID IN 1D
    int mwOrder = 2;
    int mwQuadOrder = 30;    
    uqSamples* measure1DGridPoints = new uqSamples();
    measure1DGridPoints->AddVariable("grid1D",kSAMPLEUniform,0.0,1.0);
    measure1DGridPoints->generateCartesianGrid(mwQuadOrder,kDoubleCC,kHaarRange);

    stdVec intLocations;
    scale1DGridOnPartition(measure1DGridPoints,tmpLimits[0],tmpLimits[1],intLocations);

    // EVALUATE MARGINAL AT QUADRATURE POINTS
    stdMat measure;
    stdVec tmp;
    double currInt = 0.0;
    double currLoc = 0.0;
    double currVal = 0.0;
    for(int loopA=0;loopA<intLocations.size();loopA++){
      currLoc = intLocations[loopA];
      currVal = approx->eval(currLoc);
      currInt += currVal * measure1DGridPoints->getWeightAt(loopA,measure1DGridPoints->getMaxWeightOrder());
      tmp.push_back(currVal);
    }
    printf("CURRENT INTEGRAL: %f\n",currInt);
    for(int loopA=0;loopA<intLocations.size();loopA++){
      tmp[loopA] /= currInt;
    }
    measure.push_back(tmp);

    // PLOT MEASURE TO FILE
    writeVectorToFile("measLocations.txt",intLocations);
    writeVectorToFile("measVals.txt",tmp);

    // CONSTRUCT MULTIWAVELET BASIS WITH MARGINAL    
    bool addLegendrePoly         = true;
    bool addMW                   = true;
    bool useBinPartitions        = true;
    int  mwMatType               = kMWFixedMaxDetailLevel;
    bool mwMatIncludeNullColumns = true;
    bool useExactMW              = false;    
    int  maxColumns              = 0;
    int  maxDetailLevel          = 0;

    // FORM MW MATRIX
    uqMWMatrix* mwMat = new uqMWMatrix(mwOrder,measure1DGridPoints,
                                       addLegendrePoly,addMW,useBinPartitions,
                                       mwMatType,mwMatIncludeNullColumns,
                                       useExactMW,mwQuadOrder,measure,
                                       maxColumns,maxDetailLevel);

    // Export the Grid
    string gridFile("oneDGrid.out");
    measure1DGridPoints->printToFile(gridFile,false);

    // Write Matrix To File
    string outMatFile("mwMat.out");
    mwMat->printToFile(outMatFile,false);

    // Perform Orthogonality Test
    performMWOrthogonalityTest(measure1DGridPoints,mwMat,measure);

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
