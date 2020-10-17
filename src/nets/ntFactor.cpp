# include "ntFactor.h"

using namespace std;

ntFactor::ntFactor(){
  this->processed = false;
}

ntFactor::~ntFactor(){
}

void ntFactor::setFactorID(int factorID){
  this->factorID = factorID;
}

void ntFactor::appendToFactorNodes(const vector<ntNode*>& nodes,const stdBoolVec& isDownNode){
  if(nodes.size() != isDownNode.size()){
    throw ntException("ERROR: Invalid node list in appendToFactorNodes.\n");
  }
  // Append nodes
  for(size_t loopA=0;loopA<nodes.size();loopA++){
    this->factorNodes.push_back(nodes[loopA]);
  }
  // Append downstream nodes
  for(size_t loopA=0;loopA<isDownNode.size();loopA++){
    this->isDownstreamNode.push_back(isDownNode[loopA]);
  }
}

stdMat ntFactor::getMsg(int targetNodeID){
  bool found = false;
  int count = 0;
  while(!found){
    found = (messages[count]->targetID == targetNodeID);
    if(!found){
      count += 1;  
    }
  }
  // Return the sample table
  return messages[count]->msg;
}

void ntFactor::updateMsg(int factorID,const stdMat& msg){
  for(size_t loopA=0;loopA<messages.size();loopA++){
    if((messages[loopA]->messageType == mtFactorToNode)&&(messages[loopA]->sourceID == factorID)){
      messages[loopA]->msg = msg;
    }
  }
}

// Propagate from the node to the factors that are connected
void ntFactor::sendMsgToNodes(){

  stdMat msgToSend;

  // Loop on all nodes connected to this factor
  for(int loopA=0;loopA<factorNodes.size();loopA++){

    // Get Messages from all nodes but loopA and aggegate
    stdMat currMsg;
    for(int loopB=0;loopB<factorNodes.size();loopB++){
      if(loopB != loopA){
        ntUtils::aggregateMsgs(currMsg,factorNodes[loopB]->getMsg(factorID));
      } 
    }

    // Pass message to factor loopA
    if(isDownstreamNode[loopA]){
      // Need to 
      msgToSend = factorNodes[loopA]->forwardUQ(currMsg);
      factorNodes[loopA]->updateMsg(factorID,msgToSend);  
    }else{
      // Solve 
      msgToSend = factorNodes[loopA]->inverseUQ(currMsg);
      factorNodes[loopA]->updateMsg(factorID,msgToSend);
    }

  }
}
