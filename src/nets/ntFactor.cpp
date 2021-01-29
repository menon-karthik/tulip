# include "ntFactor.h"

using namespace std;

ntFactor::ntFactor(){
}

ntFactor::~ntFactor(){
}

void ntFactor::setFactorID(int factorID){
  this->factorID = factorID;
}

bool ntFactor::findInMsg(int nodeID){
  bool found = false;
  int count = 0;
  while((!found) && (count<inMsgs.size())){
    // printf("In findInMsg: Source: %d, Target: %d\n",inMsgs[count]->sourceID,inMsgs[count]->targetID);
    found = (inMsgs[count]->sourceID == nodeID) && (inMsgs[count]->targetID == factorID);
    if(!found){
      count++;
    }
  }
  return found;  
}

// Check that all incoming messages are present
bool ntFactor::hasProcessedAllMsgs(){
  bool foundAll = true;
  for(int loopA=0;loopA<factorNodes.size();loopA++){
    foundAll = foundAll && factorNodes[loopA]->findInMsg(factorID);
  }
  return foundAll;
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

ntMessage* ntFactor::copyInMsg(int nodeID){
  bool found = false;
  int count = 0;
  while((!found) && (count<inMsgs.size())){
    found = (inMsgs[count]->sourceID == nodeID) && (inMsgs[count]->targetID == this->factorID);
    if(!found){
      count++;
    }
  }
  if(!found){
    throw ntException("ERROR: Message not found in ntFactor::copyInMsg.\n");
  }
  // Return the sample table
  ntMessage* newMsg = new ntMessage(inMsgs[count]);
  return newMsg;
}

void ntFactor::updateMsg(int nodeID,ntMessage* currMsg){
  bool found = false;
  int count = 0;

  while((!found)&&(count<inMsgs.size())){
    found  = (inMsgs[count]->messageType == mtNodeToFactor)&&(inMsgs[count]->sourceID == nodeID);
    if(!found){
      count++;
    }
  }

  if(!found){
    inMsgs.push_back(currMsg);
  }else{
    delete inMsgs[count];
    inMsgs[count] = currMsg;
  }
}

int ntFactor::getDownstreamDetNodeLocalID(){
  bool found = false;
  int count = 0;
  while((!found) && (count<factorNodes.size())){
    found = (isDownstreamNode[count]) && (factorNodes[count]->nodeType == ntDeterministic);
    if(!found){
      count++;
    }
  }
  if(!found){
    return -1;
  }else{
    return count;
  }
}

// Propagate from the node to the factors that are connected
void ntFactor::sendMsgToNodes(){  

  bool okToSend = true;
  ntMessage* currMgs;

  printf("++ Sending message from factor %d\n",this->factorID);

  int downstreamDetNodeID = getDownstreamDetNodeLocalID();
  if(downstreamDetNodeID == -1){
    throw ntException("ERROR: Cannot find deterministic downstream node in ntFactor::sendMsgToNodes.\n");
  }

  // Loop on all nodes connected to this factor
  for(int loopA=0;loopA<factorNodes.size();loopA++){

    printf("Sending to node %d...",factorNodes[loopA]->nodeID);

    // Check all other nodes are ready
    okToSend = messagesAreReadyFor(factorNodes[loopA]->nodeID);

    if(okToSend){

      printf("OK\n");

      // Get Messages from all factors but loopA and aggegate
      vector<ntMessage*> msgs;
      for(int loopB=0;loopB<factorNodes.size();loopB++){
        if(loopB != loopA){
          // Get message from node
          // printf("Gather message from node %d\n",factorNodes[loopB]->nodeID);
          currMgs = copyInMsg(factorNodes[loopB]->nodeID);
          // Chenge the message information so it is consistent
          currMgs->sourceID = this->factorID;
          currMgs->targetID = factorNodes[loopA]->nodeID;
          currMgs->messageType = mtFactorToNode;
          msgs.push_back(currMgs);
        }
      }

      // Create a New Message by aggregating the other
      ntMessage* newMsg = new ntMessage();
      newMsg->aggregateFromList(msgs);

      // Pass message to factor loopA
      ntMessage* msgToSend;
      if(isDownstreamNode[loopA]){
        // Propagate using the model in the downstream node
        printf("+++ Performing Forward Propagation\n");
        msgToSend = factorNodes[loopA]->forwardUQ(newMsg);
        factorNodes[loopA]->updateMsg(factorID,msgToSend);
      }else{        
        printf("+++ Performing Inverse Propagation\n");
        // Solve an inverse problem using the solver of the node downstream
        msgToSend = factorNodes[downstreamDetNodeID]->inverseUQ(factorNodes[loopA]->varNames,
                                                                factorNodes[loopA]->varSTD,
                                                                factorNodes[loopA]->limits,
                                                                newMsg);
        // Pass message to node upstream
        // if((this->factorID == 4)&&(factorNodes[loopA]->nodeID == 1)){
        //   exit(-1);
        // }
        factorNodes[loopA]->updateMsg(factorID,msgToSend);
      }
    }else{
      printf("NO\n");
    }
  }
}

bool ntFactor::messagesAreReadyFor(int nodeID){
  bool msgReady = true;
  for(int loopA=0;loopA<factorNodes.size();loopA++){
    if(factorNodes[loopA]->nodeID != nodeID){
      msgReady = msgReady && findInMsg(factorNodes[loopA]->nodeID);  
    }else{
      msgReady = msgReady && (!factorNodes[loopA]->findInMsg(factorID));
    }
  }    
  return msgReady;
}

void ntFactor::printMessages(){
  if(inMsgs.size()>0){
    printf("%-7s %-18s %-10s %-10s %-10s %-10s\n","Factor","Type","SourceID","TargetID","n. Samples","n. Dims");  
  }  
  for(int loopA=0;loopA<inMsgs.size();loopA++){
    printf("%-7d %-18s %-10d %-10d %-10d %-10d\n",factorID,
                                           inMsgs[loopA]->getMsgTypeString().c_str(),
                                           inMsgs[loopA]->sourceID,
                                           inMsgs[loopA]->targetID,
                                           int(inMsgs[loopA]->msg.size()),
                                           int(inMsgs[loopA]->msg[0].size()));
  }
}