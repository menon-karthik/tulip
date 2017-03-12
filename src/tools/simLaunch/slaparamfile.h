#ifndef SLAPARAMFILE_H
#define SLAPARAMFILE_H

#include <string>
#include <vector>

#include "slaparams.h"

class SLAParamFile
{
  public:
    // CONSTRUCTOR
    SLAParamFile();
    // DISTRUCTOR
    ~SLAParamFile();
    // DATA MEMBERS
    // Prototype File
    std::vector<std::string> prototypeFile;
    // Realization File
    std::vector<std::string> realizationFile;
    // Check if the realization file was generated
    bool isRealizationAvailable;
    // MEMBER FUNCTIONS
    // Read Prototype Parameter File
    void ReadParamFile(std::string prototypeFile);
    // Replace the parameters in the file
    void SetParameters(int currentRun,SLAInfo* InputParams);
    // Write the parameters to a target file
    void WriteParamFile(std::string outputFile);
};

#endif // SLAPARAMFILE_H
