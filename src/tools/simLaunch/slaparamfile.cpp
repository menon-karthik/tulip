#include <fstream>

#include "slaparamfile.h"

// ===========
// CONSTRUCTOR
// ===========
SLAParamFile::SLAParamFile(){
  isRealizationAvailable = false;
}

// ===================
// READ PARAMETER FILE
// ===================
void SLAParamFile::ReadParamFile(std::string prototypeFile){
  // OPEN FILE

  std::string buffer;
  int lineCount = 0;

  // RESET CURRENT STORAGE
  this->prototypeFile.clear();

  // OPEN FILE
  std::ifstream inFile;
  inFile.open(prototypeFile.c_str());

  // GET LINES IN FILE
  while (std::getline(inFile,buffer)){
    // READ LINE
    lineCount++;
    this->prototypeFile.push_back(std::string(buffer));
  }

  // CLOSE FILE
  inFile.close();
}

// =======================
// REPLACE STRING FUNCTION
// =======================
std::string ReplaceString(std::string s,std::string toReplace,std::string replaceWith){
  // RETURN REPLACED STRING
  std::string result = "";
  std::size_t found = s.find(toReplace);
  if (found!=std::string::npos){
    result = s.replace(found, toReplace.length(), replaceWith);
  }else{
    result = s;
  }
  // Return
  return result;
}

// ====================================
// REPLACE FIELDS WITH PARAMETER VALUES
// ====================================
void SLAParamFile::SetParameters(int currentRun,SLAInfo* InputParams){
  // LOOP ON THE LINES IN THE FILE
  std::string paramName;
  std::string paramValue;
  std::string currFileRow;
  // CLEAR REALIZATION FILE
  realizationFile.clear();
  for(unsigned int loopA=0;loopA<prototypeFile.size();loopA++){
    currFileRow = prototypeFile[loopA];
    // LOOP ON THE NUMBER OF PARAMETERS
    for(int loopB=0;loopB<InputParams->runs[currentRun]->params.size();loopB++){
      // GET NAME
      paramName = "<-PARAM=" + InputParams->runs[currentRun]->params[loopB]->GetParamName() + "->";
      paramValue = InputParams->runs[currentRun]->params[loopB]->GetParamValue();
      // REPLACE WITH PARAM
      currFileRow = ReplaceString(currFileRow,paramName,paramValue);
    }
    realizationFile.push_back(currFileRow);
  }
  // Make Realization Available
  isRealizationAvailable = true;
}

// ====================
// WRITE PARAMETER FILE
// ====================
void SLAParamFile::WriteParamFile(std::string outputFile){
  // Open Output File
  FILE* outFile;
  outFile = fopen(outputFile.c_str(),"w");

  // WRITE ALL LINES IN FILE
  for(int loopA=0;loopA<realizationFile.size();loopA++){
    fprintf(outFile,"%s\n",realizationFile[loopA].c_str());
  }

  // Close File
  fclose(outFile);

}



