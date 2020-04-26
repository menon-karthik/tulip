#include "uqApproximant_ME.h"

using namespace std;

// Default Constructor
uqApproximant_ME::uqApproximant_ME(){
}

uqApproximant_ME::uqApproximant_ME(const vector<uqApproximant_SE*>& locApproxList){
  this->approxList.clear();
  for(int loopA=0;loopA<locApproxList.size();loopA++){
    this->approxList.push_back(locApproxList[loopA]);
  }
}

uqApproximant_ME::~uqApproximant_ME(){
  for(int loopA=0;loopA<approxList.size();loopA++){
    delete approxList[loopA];
  }
}

// EVALUATION FUNCTION
stdMat uqApproximant_ME::evaluate(const stdMat& XVals){

  // Create Sample
  uqSamples* samples = new uqSamples(XVals);
  uqSamples* subsamples = new uqSamples();
  stdIntVec subindices;
  stdMat subResults;
  stdVec currLimits;

  stdMat res;
  res.resize(XVals.size());
  for(int loopA=0;loopA<XVals.size();loopA++){
    res[loopA].resize(XVals[loopA].size());
    for(int loopB=0;loopB<XVals[loopA].size();loopB++){
      res[loopA][loopB] = 0.0;
    }
  }

  // Loop through the single element approximants
  for(size_t loopA=0;loopA<approxList.size();loopA++){
    // Extract sub-samples
    currLimits = approxList[loopA]->getLimits();
    subsamples->extractPartitionSamples(samples,currLimits,subindices);
    // Evaluate these samples with the associated SE approximant
    subResults = approxList[loopA]->evaluate(subsamples->getValues());
    // Fill the matrix with at the sub indices
    for(int loopA=0;loopA<subResults.size();loopA++){
      for(int loopB=0;loopB<subResults[0].size();loopB++){
        res[subindices[loopA]][loopB] = subResults[loopA][loopB];
      }
    }
  }
  // Return the outputs
  return res;
}

// Get the extremes 
stdVec uqApproximant_ME::getLimits(){
  stdVec tempLimits;
  stdVec currLimits = approxList[0]->getLimits();
  for(int loopA=1;loopA<approxList.size();loopA++){
    tempLimits = approxList[loopA]->getLimits();
    // Keep the bigger domain
    for(int loopB=0;loopB<currLimits.size()/2;loopB++){
      // Update Minimum
      if(tempLimits[2*loopB+0] < currLimits[2*loopB+0]){
        currLimits[2*loopB+0] = tempLimits[2*loopB+0];
      }
      // Update Maximum 
      if(tempLimits[2*loopB+1] > currLimits[2*loopB+1]){
        currLimits[2*loopB+1] = tempLimits[2*loopB+1];
      }
    }
  }
}

// Export Multi-element Approximant to File
void uqApproximant_ME::exportToFile(string fileName, bool append){
  // Open and close new file
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  fclose(outFile);
  // Write content
  for(int loopA=0;loopA<approxList.size();loopA++){
    approxList[loopA]->exportToFile(fileName,true);
  }
}

// Import Multi-element Approximant from file
int uqApproximant_ME::importFromFile(string fileName, bool startFromTop, int startLine){
  int numElements = uqUtils::countLinesInFile(fileName)/8 + 1;
  int line = 0;
  int error = 0;
  approxList.clear();
  for(int loopA=0;loopA<numElements;loopA++){
    uqApproximant_SE* currApprox = new uqApproximant_SE();
    error = currApprox->importFromFile(fileName,false,line);
    if(error != 0){
      uqException("ERROR. Cannot Read Multi-element approximant from file.\n");
    }
    approxList.push_back(currApprox);
    // Increment Line
    line += 7;
  } 
  // Return
  return 0;
}
