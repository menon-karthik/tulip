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
  printf("Reading file: %s ... \n",fileName.c_str());

  // Declare
  stdStringVec tokenizedString;
  stdIntVec    tempIntVec;
  stdVec       tempVec;
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
        // # Node, ID, nodeType, variable group
        // Node,0,root,theta1.txt
        // Node,1,root,theta2.txt
        // Node,2,root,theta3.txt
        // # Deterministic Nodes
        // # Node, ID, nodeType, file with functional approximants
        // Node,3,deterministic,approx_L.txt
        // Node,4,deterministic,approx_S.txt
        // Node,5,deterministic,approx_F.txt
        try{
          // Get Node Name
          this->nodeID.push_back(atoi(tokenizedString[1].c_str()));
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
      }else if(to_upper_copy(tokenizedString[0]) == string("EDGE")){

        try{
          // Get Node Name
          this->edgeNode1.push_back(atoi(tokenizedString[1].c_str()));
          this->edgeNode2.push_back(atoi(tokenizedString[2].c_str()));
        }catch(...){
          throw ntException(string("ERROR: Invalid NODE Format. Line " + to_string(lineCount) + "\n").c_str());
        }

      // EVIDENCE TOLKEN
      }else if(to_upper_copy(tokenizedString[0]) == string("EVIDENCE")){

        try{
          // Get Node ID
          this->evidenceNodeID.push_back(atoi(tokenizedString[1].c_str()));
          tempIntVec.clear();
          tempVec.clear();
          auxCount = 2;
          while(auxCount<tokenizedString.size()){
            tempIntVec.push_back(atoi(tokenizedString[auxCount].c_str()));
            tempVec.push_back(atof(tokenizedString[auxCount+1].c_str()));
            auxCount += 2;
          }
          this->evidenceVarID.push_back(tempIntVec);
          this->evidenceVarValue.push_back(tempVec);
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

void ntNetIO::readVariableFile(int nodeID,
                          int& numVariables,
                          stdStringVec& varNames,
                          stdVec& varSTD,
                          stdMat& varSamples){

}

