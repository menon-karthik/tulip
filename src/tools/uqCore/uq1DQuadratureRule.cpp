#include "uq1DQuadratureRule.h"

// Constructor
uq1DQuadratureRule::uq1DQuadratureRule(const int order, const int suppType){
  // Assign The Total Number Of Points
  quadOrder = order;
  // Type of Support
  supportType = suppType;
}

// Distructor
uq1DQuadratureRule::~uq1DQuadratureRule()
{
  // Delete Point Vectors
  points.clear();
  weights.clear();
}

// Get Quadrature Point Coordinate
stdVec uq1DQuadratureRule::getPoints(){
  return points;
    
}

// Get Quadrature Point Weight
stdVec uq1DQuadratureRule::getWeights(){
  return weights;
}

// Print quadrature Points and Weight to File
void uq1DQuadratureRule::printToFile(std::string fileName){
  // Open Output File
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  // Write Header
  fprintf(outFile,"QUADRATURE POINTS PRINTOUT\n");
  fprintf(outFile,"Total Points: %d\n",getTotalPoints());
  fprintf(outFile,"----------------------------------\n");
  fprintf(outFile,"%15s %15s\n","Point X","Weight");
  for(int loopA=0;loopA<getTotalPoints();loopA++){
    fprintf(outFile,"%15f %15f\n",points[loopA],weights[loopA]);
  }
	// Close Output file
	fclose(outFile);	 
}

// Print quadrature Points and weights to File
void uq1DQuadratureRule::printToScreen(){
  // Write Header
  printf("\n");
  printf("QUADRATURE POINTS PRINTOUT\n");
  printf("Total Points: %d\n",getTotalPoints());
  printf("----------------------------------\n");
  printf("%15s %15s\n","Point X","Weight");
  for(int loopA=0;loopA<getTotalPoints();loopA++){
    printf("%15.10f %15.10f\n",points[loopA],weights[loopA]);
  }
  printf("\n");
}
