#include "uq1DMEApproximant.h"

// Default Constructor
uq1DMEApproximant::uq1DMEApproximant(){
}

uq1DMEApproximant::uq1DMEApproximant(vector<uq1DSEApproximant*> locApprox){
  this->approx.clear();
  for(int loopA=0;loopA<locApprox.size();loopA++){
    this->approx.push_back(locApprox[loopA]);
  }
}

uq1DMEApproximant::~uq1DMEApproximant(){
  for(int loopA=0;loopA<approx.size();loopA++){
    delete approx[loopA];
  }
}

// EVALUATION FUNCTION
double uq1DMEApproximant::eval(double XValue){

  // Find the element to use for approximation
  int elID = getElementID(XValue);
  double result = 0.0;
  if(elID>-1){
    result = approx[elID]->eval(XValue);
  }else{
    result = 0.0;
  }
  // Approximate using this element
  return result;
}

// Get Element ID from Value
int uq1DMEApproximant::getElementID(double value){
  stdVec extremes;
  getExtremes(extremes);
  bool found = false;
  int count = 0;
  while((!found)&&(count<approx.size())){
    // Check If Found
    found = ((value >= approx[count]->limits[0])&&(value < approx[count]->limits[1]));
    // Check the special case in the upper limit
    if((fabs(approx[count]->limits[1] - extremes[1])<1.0e-3)&&(fabs(value - approx[count]->limits[1])<1.0e-3)){
      found = true;
    }
    // Update Count
    if(!found){
      count++; 
    }
  }
  // Check if found
  if(!found){
    printf("Value: %f, Extremes: %f %f\n",value,extremes[0],extremes[1]);
    printf("Warning: Element not found for MEApproximant. Returning 0 at value %f...\n",value);
    return -1;
  }
  // Return the counter
  return count;
}

// Get the extremes 
void uq1DMEApproximant::getExtremes(stdVec& result){
  result.resize(2);
  result[0] = std::numeric_limits<double>::max();
  result[1] = -std::numeric_limits<double>::max();
  for(int loopA=0;loopA<approx.size();loopA++){
    if(approx[loopA]->limits[0] < result[0]){
      result[0] = approx[loopA]->limits[0];
    }
    if(approx[loopA]->limits[1] > result[1]){
      result[1] = approx[loopA]->limits[1];
    }
  }
}

// Export Multi-element Approximant to File
void uq1DMEApproximant::exportToTextFile(string fileName, bool append){
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
int uq1DMEApproximant::importFromTextFile(string fileName, bool startFromTop, int startLine){
  int numElements = uqUtils::countLinesInFile(fileName)/8 + 1;
  int line = 0;
  int error = 0;
  approx.clear();
  for(int loopA=0;loopA<numElements;loopA++){
    uq1DSEApproximant* currApprox = new uq1DSEApproximant();
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
void uq1DMEApproximant::normalizeByConstant(double normValue){
  for(int loopA=0;loopA<approx.size();loopA++){
    approx[loopA]->normalizeByConstant(normValue);
  }
}
