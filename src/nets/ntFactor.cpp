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

int ntFactor::getDownstreamDetProbNodeLocalID(){
  bool found = false;
  int count = 0;
  while((!found) && (count<factorNodes.size())){
    found = (isDownstreamNode[count]) && ((factorNodes[count]->nodeType == ntDeterministic) || (factorNodes[count]->nodeType == ntProbabilistic));
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

  int downstreamDetProbNodeID = getDownstreamDetProbNodeLocalID();
  if(downstreamDetProbNodeID == -1){
    throw ntException("ERROR: Cannot find neither a deterministic nor a probabilistic downstream node in ntFactor::sendMsgToNodes.\n");
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

          // printf("Samples in original message\n");
          // for(int loopB=0;loopB<currMgs->msg.size();loopB++){
          //   for(int loopC=0;loopC<currMgs->msg[loopB].size();loopC++){
          //     printf("%f ",currMgs->msg[loopB][loopC]);
          //   }
          //   printf("\n");
          // }
        }
      }

      // Print limits for msgs
      // printf("Limits before aggregation\n");
      // for(int loopB=0;loopB<msgs.size();loopB++){
      //   for(int loopC=0;loopC<msgs[loopB]->varLimits.size();loopC++){
      //     printf("%f ",msgs[loopB]->varLimits[loopC]);
      //   }
      //   printf("\n");
      // }

      // Create a New Message by aggregating the others
      ntMessage* newMsg = new ntMessage();
      newMsg->aggregateFromList(msgs);

      // Print limits for msgs
      // printf("Limits after aggregation\n");
      // for(int loopB=0;loopB<newMsg->varLimits.size();loopB++){
      //   printf("%f ",newMsg->varLimits[loopB]);
      // }
      // printf("\n");

      // printf("Number of Aggregated Messages: %d\n",int(msgs.size()));

      // printf("Samples in Aggregated Message\n");
      // for(int loopB=0;loopB<newMsg->msg.size();loopB++){
      //   for(int loopC=0;loopC<newMsg->msg[loopB].size();loopC++){
      //     printf("%f ",newMsg->msg[loopB][loopC]);
      //   }
      //   printf("\n");
      // }

      // Pass message to factor loopA
      ntMessage* msgToSend;
      if(isDownstreamNode[loopA]){
        // Propagate using the model in the downstream node
        if(factorNodes[loopA]->nodeType == ntDeterministic){
          printf("+++ Performing Forward Propagation\n");
          msgToSend = factorNodes[loopA]->forwardUQ(newMsg);
        }else if (factorNodes[loopA]->nodeType == ntProbabilistic){
          printf("+++ Generate Forward Samples Through CPT\n");
          msgToSend = factorNodes[loopA]->forwardCPT(newMsg);
        }
        factorNodes[loopA]->updateMsg(factorID,msgToSend);
      }else{                
        if(factorNodes[downstreamDetProbNodeID]->nodeType == ntDeterministic){
          // Solve an inverse problem using the solver of the node downstream
          printf("+++ Performing Inverse Propagation\n");
          msgToSend = factorNodes[downstreamDetProbNodeID]->inverseUQ(factorNodes[loopA]->varNames,
                                                                      factorNodes[loopA]->varSTD,
                                                                      factorNodes[loopA]->limits,
                                                                      newMsg);        
        }else if (factorNodes[downstreamDetProbNodeID]->nodeType == ntProbabilistic){
          printf("+++ Generate Inverse Samples Through CPT\n");
          msgToSend = factorNodes[downstreamDetProbNodeID]->InverseCPT(factorNodes[loopA]->varNames,
                                                                       factorNodes[loopA]->varSTD,
                                                                       factorNodes[loopA]->limits,
                                                                       newMsg);
        }
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