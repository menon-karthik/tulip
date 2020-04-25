#ifndef NTNODE_H
#define NTNODE_H

# include "uqTypes.h"
# include "ntNetIO.h"

/*! 
Abstract parent class for probabilistic networks in UQ
*/
class ntNode{
  protected:
    // Has the node been processed
    bool processed;
    // Number of variables
    int numVariables;
    // Variables Name strings
    stdStringVec varNames;
    // Variable standard deviations
    stdVec varSTD;
    // Variable realizations
    stdMat varSamples;
    // Evidence: variable ID and evidence value
    stdIntVec evidenceVarID;
    stdVec evidenceVarValue;

  public:

  	//! Default Constructor
  	ntNode(int nodeID,ntNetIO* netIO);
    //! Virtual Destructor
    virtual ~ntNode();

    // Check if the node has evidence
    bool hasEvidence();

    // Propagate through the node
    virtual void propagate() = 0;

};

#endif // NTNODE_H
