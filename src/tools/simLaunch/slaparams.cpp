#include <string>
#include <boost/algorithm/string.hpp>

#include "slautils.h"
#include "slaparams.h"
#include "slaexception.h"

// CONSTRUCTOR
SLAInfo::SLAInfo(){
  useGenBC = false;
  usePresolver = false;
  useMorph = false;
  useSolverInputFile = false;
}


// CONSTRUCTOR FOR PARAMETER
SLAParam::SLAParam(std::string localName, paramFormats localType,std::string localValue){
  // QUANTITIES
  name = localName;
  type = localType;
  // VALUE
  // Initialize
  doubleValue = 0.0;
  intValue = 0;
  stringValue = "";
  // ASSIGN VALUE
  if(localType == pfInteger){
    // CONVERT INTEGER VALUE
    intValue = atoi(localValue.c_str());
  }else if(localType == pfDouble){
    // CONVERT INTEGER VALUE
    doubleValue = atof(localValue.c_str());
  }else if(localType == pfString){
    // CONVERT INTEGER VALUE
    stringValue = localValue;
  }
}

// ======================
// GET THE PARAMETER NAME
// ======================
std::string SLAParam::GetParamName(){
  return name;
}

// ======================
// GET THE PARAMETER TYPE
// ======================
std::string SLAParam::GetParamType(){
  // ASSIGN VALUE
  std::string result = "";
  if(type == pfInteger){
    // CONVERT INTEGER VALUE
    result = "Integer";
  }else if(type == pfDouble){
    // CONVERT DOUBLE VALUE
    result = "Double";
  }else if(type == pfString){
    // CONVERT STRING VALUE
    result = "String";
  }
  return result;
}

// ==========================
// GET PARAMETER VALUE STRING
// ==========================
std::string SLAParam::GetParamValue(){
  // ASSIGN VALUE
  std::string result = "";
  if(type == pfInteger){
    // CONVERT INTEGER VALUE
    result = std::to_string(intValue);
  }else if(type == pfDouble){
    // CONVERT DOUBLE VALUE
    result = std::to_string(doubleValue);
  }else if(type == pfString){
    // CONVERT STRING VALUE
    result = stringValue;
  }
  return result;
}

// ===========================
// CONSTRUCTOR FOR SLA POST OP
// ===========================
SLAPostOperation::SLAPostOperation(int currOrder, std::string currCommand){
  // Copy Order
  order = currOrder;
  // Copy Current Command
  command = currCommand;
}

// ===================
// READ INFO FROM FILE
// ===================
void SLAInfo::ReadFromFile(std::string inputFile){

  // OPEN INPUT FILE
  std::ifstream inFile;
  inFile.open(inputFile.c_str());
  // Write Message
  SLAUtils::WriteMessage(std::string("Reading Parameter File ") + inputFile + std::string("...\n"));

  // Read Data From File
  std::string buffer;
  std::vector<std::string> tokenizedString;

  // Initialize
  int lineCount = 0;
  while (std::getline(inFile,buffer)){
    // Increment line count
    lineCount++;
    // Trim String
    boost::trim(buffer);
    if((buffer != "")||(buffer[0] != (int)'#')){
      // TOKENIZE
      boost::split(tokenizedString, buffer, boost::is_any_of(" ,="), boost::token_compress_on);
      // CHECK TYPE
      if(boost::to_upper_copy(tokenizedString[0]) == "RUNNAME"){
        // CREATE NEW RUN
        SLARun* run = new SLARun(tokenizedString[1]);
        // ADD TO PARAM STRUCTURE
        runs.push_back(run);
      }else if(boost::to_upper_copy(tokenizedString[0]) == "PARAMFILE"){
          // ADD PARAMETER FILE
          paramFiles.push_back(tokenizedString[1]);
      }else if(boost::to_upper_copy(tokenizedString[0]) == "PROTOTYPEFILE"){

          // Create New File
          SLAPrototypeFile* currFile = new SLAPrototypeFile();

          // Assign File Name
          currFile->fileName = tokenizedString[1];

          // CHECK TYPE
          if(boost::to_upper_copy(tokenizedString[2]) == "SOLVERINPUT"){
            currFile->type = ptSOLVERINPUT;
          }else if(boost::to_upper_copy(tokenizedString[2]) == "GENBC"){
            currFile->type = ptGENBC;
          }else if(boost::to_upper_copy(tokenizedString[2]) == "MORPH"){
            currFile->type = ptMORPH;
          }else{
            throw SLAException(std::string("Invalid Prototype File type: " + std::to_string(lineCount) + "\n").c_str());
          }

          // CHECK NAME TO USE
          currFile->nameToUse = tokenizedString[3];

          // ADD PROTOTYPE FILE NAME
          prototypeFiles.push_back(currFile);

      }else if(boost::to_upper_copy(tokenizedString[0]) == "USESOLVERINPUTFILE"){
        if(boost::to_upper_copy(tokenizedString[1]) == "FALSE"){
          useSolverInputFile = false;
        }else{
          useSolverInputFile = true;
        }
      // READ PRESOLVER FLAG
      }else if(boost::to_upper_copy(tokenizedString[0]) == "USEGENBC"){
          if(boost::to_upper_copy(tokenizedString[1]) == "FALSE"){
            useGenBC = false;
          }else{
            useGenBC = true;
          }
        // READ PRESOLVER FLAG
      }else if(boost::to_upper_copy(tokenizedString[0]) == "USEPRESOLVER"){
        if(boost::to_upper_copy(tokenizedString[1]) == "FALSE"){
          usePresolver = false;
        }else{
          usePresolver = true;
        }
      // READ MORPH FLAG
      }else if(boost::to_upper_copy(tokenizedString[0]) == "USEMORPH"){
          if(boost::to_upper_copy(tokenizedString[1]) == "FALSE"){
            usePresolver = false;
          }else{
            usePresolver = true;
          }
      // READ LAUNCH SIMULATION FLAG
      }else if(boost::to_upper_copy(tokenizedString[0]) == "LAUNCHSIMULATIONS"){
        if(boost::to_upper_copy(tokenizedString[1]) == "FALSE"){
          launchSimulations = false;
        }else{
          launchSimulations = true;
        }
      }else if(boost::to_upper_copy(tokenizedString[0]) == "POST"){
        // CREATE NEW POST OPERATION
        SLAPostOperation* currOp = new SLAPostOperation(atoi(tokenizedString[1].c_str()),tokenizedString[2]);
        // ADD PARAMETER TO LAST RUN
        PostCommands.push_back(currOp);
      }else{
        // WRITE COMPLETION MESSAGE
        SLAUtils::WriteMessage("Invalid line in input File: " + SLAUtils::IntToStr(lineCount) + "\n");
      }
    }
  }

  // CLOSE FILE
  inFile.close();
}

// ====================================
// GENERATE PARAMETERS FROM SINGLE FILE
// ====================================
void SLAInfo::GetParametersFromFile(std::string paramFile){

  // Open Input File
  std::ifstream inFile;
  inFile.open(paramFile.c_str());

  // Read Data From File
  std::string buffer;
  std::vector<std::string> tokenizedString;
  std::vector<std::string> paramNames;

  // Local Data
  std::string currParamName;
  std::string currParamValue;
  paramFormats currParamType;

  // Initialize
  int lineCount = 0;
  while (std::getline(inFile,buffer)){

    // Increment line count
    lineCount++;

    // Trim String
    boost::trim(buffer);
    if((buffer != "")||(buffer[0] != (int)'#')){

      // TOKENIZE
      boost::split(tokenizedString, buffer, boost::is_any_of(" ,="), boost::token_compress_on);

      // IF FIRST ROW GET PARAMETER NAMES
      if(lineCount == 1){
        paramNames = tokenizedString;
        // Add to ParamList
        for(int loopA=0;loopA<paramNames.size();loopA++){
          ParamList.push_back(paramNames[loopA]);
        }

      }else{
        // Create a new Run
        SLARun* run = new SLARun(std::string("run_" + std::to_string(lineCount-1)).c_str());
        for(int loopA=0;loopA<tokenizedString.size();loopA++){
          currParamName = paramNames[loopA];
          currParamValue = tokenizedString[loopA].c_str();
          currParamType = pfDouble;
          // Create New Parameter
          SLAParam* param = new SLAParam(currParamName,currParamType,currParamValue);
          // Add to run
          run->params.push_back(param);
        }
        // Add run
        runs.push_back(run);
      }
    }
  }

  // Close File
  inFile.close();
}

// =================================
// GENERATE RUNS FROM PARAMETER FILE
// =================================
void SLAInfo::GenerateRuns(){
  std::string currFile;
  for(int loopA=0;loopA<paramFiles.size();loopA++){
    currFile = paramFiles[loopA];
    GetParametersFromFile(currFile.c_str());
  }
}

// ==================================
// PRINT RUN INFORMATION TO ECHO FILE
// ==================================
void SLAInfo::PrintRuns(std::string echoFileName){
  FILE* outFile;
  outFile = fopen(echoFileName.c_str(),"w");

  // Loop through the runs
  for(int loopA=0;loopA<runs.size();loopA++){
    // Print Name of Run
    fprintf(outFile,"Name Of Run: %s\n",runs[loopA]->name.c_str());
    // Loop through the parameters
    for(int loopB=0;loopB<runs[loopA]->params.size();loopB++){
      fprintf(outFile,"Parameter: %s;",runs[loopA]->params[loopB]->GetParamName().c_str());
      fprintf(outFile,"Type: %s;",runs[loopA]->params[loopB]->GetParamType().c_str());
      fprintf(outFile,"Value: %s\n",runs[loopA]->params[loopB]->GetParamValue().c_str());
    }
    fprintf(outFile,"=====================================\n");
  }

  // Close File
  fclose(outFile);
}
