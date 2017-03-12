#ifndef SLAPARAMS_H
#define SLAPARAMS_H

#include <vector>
#include <string>

// ENUM TYPES
enum paramTypes{ptSOLVERINPUT,ptGENBC,ptMORPH};
enum paramFormats{pfInteger,pfDouble,pfString};

// PROTOTYPE FILES
class SLAPrototypeFile{
  public:
    // CONSTRUCTOR
    SLAPrototypeFile(){}
    // DATA MEMBERS
    std::string fileName;
    std::string nameToUse;
    paramTypes type;
};

// POST OPERATION
class SLAPostOperation{
  public:
    SLAPostOperation(){}
    // DATA MEMBERS
    std::string command;
    int order;
    // FUNCTIONS
    SLAPostOperation(int currOrder, std::string currCommand);
};

// SINGLE PARAM DEFINITION
class SLAParam{
  public:
    // CONSTRUCTOR
    SLAParam(std::string localName, paramFormats localType,std::string localValue);
    // DISTRUCTOR
    ~SLAParam();
    // QUANTITIES
    std::string name;
    paramFormats type;
    // VALUES
    double doubleValue;
    int intValue;
    std::string stringValue;
    // DATA MEMBERS
    // Get Information from file
    std::string GetParamName();
    std::string GetParamType();
    std::string GetParamValue();
};

// SINGLE RUN DEFINITION
class SLARun{
  public:
    // CONSTRUCTOR
    SLARun(std::string localName){name = localName;}
    // DISTRUCTOR
    ~SLARun();
    // QUANTITIES
    std::string name;
    std::vector<SLAParam*> params;
};

// SLA PARAMETERS
class SLAInfo{
  public:
    // CONSTRUCTOR
    SLAInfo();
    // DISTRUCTOR
    ~SLAInfo();
    // QUANTITIES
    // Data Members
    bool useSolverInputFile;
    bool useGenBC;
    bool usePresolver;
    bool useMorph;
    bool launchSimulations;
    // All parameters
    std::vector<SLARun*> runs;
    // List of Prototype Files
    std::vector<SLAPrototypeFile*> prototypeFiles;
    // List of Param Files
    std::vector<std::string> paramFiles;
    // LIST OF ALL PARAMETERS
    std::vector<std::string> ParamList;
    // LIST OF ALL POST OPERATIONS
    std::vector<SLAPostOperation*> PostCommands;
    // MEMBER FUNCTIONS
    // READ FROM FILE
    void ReadFromFile(std::string inputFile);
    // ADD PARAM FILE
    void AddParamFile(std::string currParamFile);
    // ADD PROTOTYPE FILE
    void AddPrototypeFile(std::string currProtoFile);
    // ACTIONS
    void GenerateRuns();
    void GetParametersFromFile(std::string paramFile);
    void PrintRuns(std::string echoFileName);
};

#endif // SLAPARAMS_H
