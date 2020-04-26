#include "uq1DApproximant_ME.h"

using namespace std;

// Default Constructor
uq1DApproximant_ME::uq1DApproximant_ME(){
}

uq1DApproximant_ME::uq1DApproximant_ME(vector<uq1DApproximant_SE*> locApprox){
  this->approx.clear();
  for(int loopA=0;loopA<locApprox.size();loopA++){
    this->approx.push_back(locApprox[loopA]);
  }
}

uq1DApproximant_ME::~uq1DApproximant_ME(){
  for(int loopA=0;loopA<approx.size();loopA++){
    delete approx[loopA];
  }
}

// EVALUATION FUNCTION
double uq1DApproximant_ME::evaluate(double XValue){

  // Find the element to use for approximation
  int elID = getElementID(XValue);
  double result = 0.0;
  if(elID>-1){
    result = approx[elID]->evaluate(XValue);
  }else{
    result = 0.0;
  }
  // Approximate using this element
  return result;
}

// Get Element ID from Value
int uq1DApproximant_ME::getElementID(double value){
  stdVec limits;
  getLimits(limits);
  bool found = false;
  int count = 0;
  while((!found)&&(count<approx.size())){
    // Check If Found
    found = ((value >= approx[count]->limits[0])&&(value < approx[count]->limits[1]));
    // Check the special case in the upper limit
    if((fabs(approx[count]->limits[1] - limits[1])<1.0e-3)&&(fabs(value - approx[count]->limits[1])<1.0e-3)){
      found = true;
    }
    // Update Count
    if(!found){
      count++; 
    }
  }
  // Check if found
  if(!found){
    printf("Value: %f, limits: %f %f\n",value,limits[0],limits[1]);
    printf("Warning: Element not found for MEApproximant. Returning 0 at value %f...\n",value);
    return -1;
  }
  // Return the counter
  return count;
}

// Get the limits 
void uq1DApproximant_ME::getLimits(stdVec& res){
  res.resize(2);
  res[0] = std::numeric_limits<double>::max();
  res[1] = -std::numeric_limits<double>::max();
  for(int loopA=0;loopA<approx.size();loopA++){
    if(approx[loopA]->limits[0] < res[0]){
      res[0] = approx[loopA]->limits[0];
    }
    if(approx[loopA]->limits[1] > res[1]){
      res[1] = approx[loopA]->limits[1];
    }
  }
}

// Export Multi-element Approximant to File
void uq1DApproximant_ME::exportToTextFile(string fileName, bool append){
  // Open and close new file
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  fclose(outFile);
  // Write content
  for(int loopA=0;loopA<approx.size();loopA++){
    approx[loopA]->exportToTextFile(fileName,true);
  }
}

// Import Multi-element Approximant from file
int uq1DApproximant_ME::importFromTextFile(string fileName, bool startFromTop, int startLine){
  int numElements = uqUtils::countLinesInFile(fileName)/8 + 1;
  int line = 0;
  int error = 0;
  approx.clear();
  for(int loopA=0;loopA<numElements;loopA++){
    uq1DApproximant_SE* currApprox = new uq1DApproximant_SE();
    error = currApprox->importFromTextFile(fileName,false,line);
    if(error != 0){
      uqException("ERROR. Cannot Read Multi-element approximant from file.\n");
    }
    approx.push_back(currApprox);
    // Increment Line
    line += 7;
  } 
  // Return
  return 0;
}

// Normalize by constant
void uq1DApproximant_ME::normalizeByConstant(double normValue){
  for(int loopA=0;loopA<approx.size();loopA++){
    approx[loopA]->normalizeByConstant(normValue);
  }
}
