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
    stdMat evidenceVarAvg;
    stdMat evidenceVarStd;

  	//! Default Constructor
  	ntNetIO();
    //! Virtual Destructor
    virtual ~ntNetIO();

    // Read Network Information from file
    void readFromFile(string fileName);

    // Read file for root nodes
    void readRootNodeFile(string fileName,
                          int& numVariables,
                          int& NumSamples,
                          stdStringVec& varNames,
                          stdVec& varSTD,
                          stdVec& limits,
                          stdMat& varSamples);

    // Read file for deterministic nodes
    void readDeterministicNodeFile(string fileName,
                                   int& numVariables,
                                   int& numSamples,
                                   stdStringVec& varNames,
                                   stdVec& varSTD,
                                   stdVec& limits,
                                   modelTypes& detVarType,
                                   string& detModelName);

    // Read file for probabilistic nodes
    void readProbabilisticNodeFile(string fileName,
                                   int& numVariables,
                                   int& NumSamples,
                                   stdStringVec& varNames,
                                   stdVec& varSTD,
                                   modelTypes& detVarType,
                                   string& detModelName);
};

#endif // NTNETIO_H
