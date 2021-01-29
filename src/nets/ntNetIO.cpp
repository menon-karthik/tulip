# include "ntNetIO.h"

using namespace std;
using namespace boost::algorithm;

ntNetIO::ntNetIO(){
}

ntNetIO::~ntNetIO(){
}

// Read Network Variables and Layout
void ntNetIO::readFromFile(string fileName){

  // Message
  printf("\n");
  printf("Reading network file: %s ... \n",fileName.c_str());

  // Declare
  stdStringVec tokenizedString;
  stdIntVec    tempIntVec;
  stdVec       tempVec;
  stdVec       tempVec2;
  string       matType;
  stdVec       temp;
  bool doInclude = false;
  int auxCount = 0;

  // Declare input File
  ifstream infile;
  infile.open(fileName);
  if(infile.fail()){
    throw ntException("ERROR: Input file does not exist.\n");
  }
  int lineCount = 1;

  // Read Data From File
  string buffer;
  while (getline(infile,buffer)){
    // Trim String
    trim(buffer);
    // Tokenize String
    split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
    // Check for Empty buffer
    if(!buffer.empty()){
      // NODE TOLKEN
      if(to_upper_copy(tokenizedString[0]) == string("NODE")){
      // # Define Nodes
      // # Root Nodes
      // # Node, ID, nodeType, variable file (contains names, stds, samples)
      // Node,0,root,theta1.txt
      // # Deterministic Nodes
      // # Node, ID, nodeType, file with functional approximants
      // Node,3,deterministic,node_L.txt
        try{
          // Get Node ID
          this->nodeID.push_back(stoi(tokenizedString[1].c_str()));
          // Get Node Type
          if(to_upper_copy(tokenizedString[2]) == string("ROOT")){
            this->nodeType.push_back(ntRoot);
          }else if(to_upper_copy(tokenizedString[2]) == string("DETERMINISTIC")){
            this->nodeType.push_back(ntDeterministic);
          }else if(to_upper_copy(tokenizedString[2]) == string("PROBABILISTIC")){
            this->nodeType.push_back(ntProbabilistic);
          }
          // Node info is the variable group or 
          this->nodeFile.push_back(tokenizedString[3]);
        }catch(...){
          throw ntException(string("ERROR: Invalid NODE Format. Line " + to_string(lineCount) + "\n").c_str());
        }
      // EDGE TOLKEN
      // # Define Edges
      // Edge,0,3        
      }else if(to_upper_copy(tokenizedString[0]) == string("EDGE")){

        try{
          // Get Node Name
          this->edgeNode1.push_back(stoi(tokenizedString[1].c_str()));
          this->edgeNode2.push_back(stoi(tokenizedString[2].c_str()));
        }catch(...){
          throw ntException(string("ERROR: Invalid NODE Format. Line " + to_string(lineCount) + "\n").c_str());
        }

      // EVIDENCE TOLKEN
      // # Evidence
      // Evidence,4,1,1,1000.0,50.0
      }else if(to_upper_copy(tokenizedString[0]) == string("EVIDENCE")){

        try{
          // Get Node ID
          this->evidenceNodeID.push_back(stoi(tokenizedString[1].c_str()));
          auxCount = stoi(tokenizedString[2].c_str());
          tempIntVec.clear();
          tempVec.clear();
          tempVec2.clear();
          for(int loopA=0;loopA<auxCount;loopA++){
            tempIntVec.push_back(stoi(tokenizedString[loopA*3+3].c_str()));
            tempVec.push_back(atof(tokenizedString[loopA*3+4].c_str()));
            tempVec2.push_back(atof(tokenizedString[loopA*3+5].c_str()));
          }
          this->evidenceVarID.push_back(tempIntVec);
          this->evidenceVarAvg.push_back(tempVec);
          this->evidenceVarStd.push_back(tempVec2);
        }catch(...){
          throw ntException(string("ERROR: Invalid NODE Format. Line " + to_string(lineCount) + "\n").c_str());
        }

      }else if((tokenizedString.size() == 0)||(tokenizedString[0].at(0) == '#')||(tokenizedString[0].find_first_not_of(' ') == std::string::npos)){
        // COMMENT OR BLANK LINE: DO NOTHING
      }else{
        // TOKEN NOT RECOGNIZED
        throw ntException(string("ERROR: Invalid Token in input file, line: "  + to_string(lineCount) + "\n").c_str());
      }
    }
    // Increment Line Count
    lineCount++;
  }
  // Close File
  infile.close();
}


void ntNetIO::readRootNodeFile(string fileName,
                               int& numVariables,
                               int& numSamples,
                               stdStringVec& varNames,
                               stdVec& varSTD,
                               stdVec& limits,
                               stdMat& varSamples){

  // Message
  printf("Reading root node file: %s ... \n",fileName.c_str());

  // Declare
  stdStringVec tokenizedString;
  stdVec       tmpVec;
  stdVec       minLimits;
  stdVec       maxLimits;
  string       matType;
  stdVec       temp;
  bool doInclude = false;
  int auxCount = 0;

  // Declare input File
  ifstream infile;
  infile.open(fileName);
  if(infile.fail()){
    throw ntException("ERROR: Input file does not exist.\n");
  }
  int lineCount = 1;

  // Read Data From File
  string buffer;
  while (getline(infile,buffer)){
    // Trim String
    trim(buffer);
    // Tokenize String
    split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
    // Check for Empty buffer
    if(!buffer.empty()){
      // # Num Variables, num samples
      // file.write('1,'+str(numSamples) + '\n') 
      // # Name of variables 
      // file.write('TPS Elastic module\n') 
      // # Standard Deviation of all variables
      // file.write('10.0e6\n') 
      // # Limits of all variables: first row min and second row max
      // file.write('20.0e6\n') 
      // file.write('80.0e6\n') 
      // # Matrix of variable realizations
      if(lineCount == 1){
        try{
          numVariables = stoi(tokenizedString[0].c_str());
          numSamples = stoi(tokenizedString[1].c_str());
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 2){
        try{
          varNames.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            varNames.push_back(tokenizedString[loopA]);
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 3){
        try{
          varSTD.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            varSTD.push_back(atof(tokenizedString[loopA].c_str()));
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 4){
        try{
          minLimits.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            minLimits.push_back(atof(tokenizedString[loopA].c_str()));
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 5){
        try{
          maxLimits.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            maxLimits.push_back(atof(tokenizedString[loopA].c_str()));
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        break;
      }else if((tokenizedString.size() == 0)||(tokenizedString[0].at(0) == '#')||(tokenizedString[0].find_first_not_of(' ') == std::string::npos)){
        // COMMENT OR BLANK LINE: DO NOTHING
      }else{
        // TOKEN NOT RECOGNIZED
        throw ntException("ERROR: Invalid Token in input file.\n");
      }
    }
  }

  // Read the Sample Matrix
  int countSamples = 0;
  while(getline(infile,buffer)&&(countSamples<numSamples)){
    trim(buffer);
    // Tokenize String
    split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
    // Check for Empty buffer
    if(!buffer.empty()){
      if((tokenizedString.size() == 0)||(tokenizedString[0].at(0) == '#')||(tokenizedString[0].find_first_not_of(' ') == std::string::npos)){
        // DO NOTHING
      }else{
        tmpVec.clear();
        for(int loopA=0;loopA<tokenizedString.size();loopA++){
          tmpVec.push_back(atof(tokenizedString[loopA].c_str()));
        }
        varSamples.push_back(tmpVec);
        countSamples++;
      }
    }
  }

  // Check if all samples have been read
  if(countSamples != numSamples){
    throw ntException("ERROR: Partial read samples in readRootNodeFile.\n");
  }

  // Assemble Limits in unique std vector
  if(minLimits.size() != maxLimits.size()){
    throw ntException("ERROR: Limits array not compatible in readRootNodeFile.\n");
  }

  limits.clear();
  for(int loopA=0;loopA<minLimits.size();loopA++){
    limits.push_back(minLimits[loopA]);
    limits.push_back(maxLimits[loopA]);
  }

  // Close File
  infile.close();
}

void ntNetIO::readDeterministicNodeFile(string fileName,
                                        int& numVariables,
                                        int& numSamples,
                                        stdStringVec& varNames,
                                        stdVec& varSTD,
                                        stdVec& limits,
                                        modelTypes& detVarType,
                                        string& detModelName){
  // Message
  printf("Reading deterministic node file: %s ... \n",fileName.c_str());

  // Declare
  stdStringVec tokenizedString;
  stdVec       tmpVec;
  stdVec       minLimits;
  stdVec       maxLimits;
  bool doInclude = false;
  int auxCount = 0;

  // Declare input File
  ifstream infile;
  infile.open(fileName);
  if(infile.fail()){
    throw ntException("ERROR: Input file does not exist.\n");
  }
  int lineCount = 1;

  // Read Data From File
  string buffer;
  while (getline(infile,buffer)){
    // Trim String
    trim(buffer);
    // Tokenize String
    split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
    // Check for Empty buffer
    if(!buffer.empty()){
      // # File is comma separated
      // # Number of Variables, Number of Samples
      // file.write('2,100\n') 
      // # Name of variables 
      // file.write('pressure,heatflux\n') # These are mapped to the model results is type is MODEL
      // # Standard Deviation of all variables
      // file.write('1000.0,0.10e6\n') # Pressure [Pa], Heat Flux [W/m2]
      // # Approximant Type for each variable
      // file.write('model,model\n') # can be MODEL, FILE
      // # Approximant Name or file name for each variable
      // file.write('darpaSimpleCode1,darpaSimpleCode1\n') # name of the model/file
      if(lineCount == 1){
        try{
          numVariables = stoi(tokenizedString[0].c_str());
          numSamples = stoi(tokenizedString[1].c_str());
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 2){
        try{
          varNames.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            varNames.push_back(tokenizedString[loopA]);
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 3){
        try{
          varSTD.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            varSTD.push_back(atof(tokenizedString[loopA].c_str()));
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 4){
        try{
          minLimits.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            minLimits.push_back(atof(tokenizedString[loopA].c_str()));
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 5){
        try{
          maxLimits.clear();
          for(int loopA=0;loopA<tokenizedString.size();loopA++){
            maxLimits.push_back(atof(tokenizedString[loopA].c_str()));
          }
        }catch(...){
          throw ntException("ERROR: Invalid ROOT NODE file.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 6){
        try{
          if(to_upper_copy(tokenizedString[0]) == string("MODEL")){
            detVarType = mtModel;
          }else if(to_upper_copy(tokenizedString[0]) == string("FILE")){
            detVarType = mtApproximant;
          }else{
            throw ntException("ERROR: Invalid node model type.\n");
          }
        }catch(...){
          throw ntException("ERROR: Invalid deterministic node model type.\n");
        }
        // Increment Line Count
        lineCount++;
      }else if(lineCount == 7){
        try{
          detModelName = tokenizedString[0];
        }catch(...){
          throw ntException("ERROR: Invalid deterministic node model name.\n");
        }      
      }else if((tokenizedString.size() == 0)||(tokenizedString[0].at(0) == '#')||(tokenizedString[0].find_first_not_of(' ') == std::string::npos)){
        // COMMENT OR BLANK LINE: DO NOTHING
      }else{
        // TOKEN NOT RECOGNIZED
        throw ntException("ERROR: Invalid Token in input file.\n");
      }
    }
  }

  // Assemble Limits in unique std vector
  if(minLimits.size() != maxLimits.size()){
    throw ntException("ERROR: Limits array not compatible in readDeterministicNodeFile.\n");
  }

  limits.clear();
  for(int loopA=0;loopA<minLimits.size();loopA++){
    limits.push_back(minLimits[loopA]);
    limits.push_back(maxLimits[loopA]);
  }

  // Close File
  infile.close();
}


void ntNetIO::readProbabilisticNodeFile(string fileName,
                                        int& numVariables,
                                        int& NumSamples,
                                        stdStringVec& varNames,
                                        stdVec& varSTD,
                                        modelTypes& detVarType,
                                        string& detModelName){
  throw ntException("ERROR: readProbabilisticNodeFile not implemented.\n");
}


