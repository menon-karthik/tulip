#ifndef NTNETIO_H
#define NTNETIO_H

# include <boost/algorithm/string.hpp>
# include <iostream>
# include <fstream>

# include "uqTypes.h"
# include "ntConstants.h"
# include "ntException.h"

class ntNetIO{
  public:
    // Node Info
    stdIntVec nodeID;
    vector<nodeDependenceTypes> nodeType;
    stdStringVec nodeFile;

    // Edge Info
    stdIntVec edgeNode1;
    stdIntVec edgeNode2;

    // Evidence for each node
    stdIntVec evidenceNodeID;
    stdIntMat evidenceVarID;
    stdMat evidenceVarValue;

  	//! Default Constructor
  	ntNetIO();
    //! Virtual Destructor
    virtual ~ntNetIO();

    // Read Network Information from file
    void readFromFile(string fileName);
    void readVariableFile(int nodeID,
                          int& numVariables,
                          stdStringVec& varNames,
                          stdVec& varSTD,
                          stdMat& varSamples);
};

#endif // NTNETIO_H
