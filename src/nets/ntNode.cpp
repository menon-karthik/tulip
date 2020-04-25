# include "ntNode.h"

ntNode::ntNode(int nodeID,ntNetIO* netIO){
  // The node has not been processed yet
  this->processed = false;

  // Read veriable names and samples
  stdStringVec varNames;
  stdMat varSamples;
  stdVec varSTD;
  netIO->readVariableFile(nodeID,numVariables,varNames,varSTD,varSamples);

    // Evidence: variable ID and evidence value
    stdIntVec evidenceVarID;
    stdVec evidenceVarValue;

	  // In a processed node the messages have been propagated
  processed = false;
}

ntNode::~ntNode(){

}

// Check if the node has evidence
bool ntNode::hasEvidence(){
  if(evidenceVarID.size() > 0){
    return true;
  }else{
    return false;
  }
}


