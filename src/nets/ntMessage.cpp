# include "ntMessage.h"

ntMessage::ntMessage(){
}

ntMessage::ntMessage(messageTypes messageType,int sourceID,int targetID,
                     const stdStringVec& labels,const stdVec& stds,const stdVec& limits,
                     const stdMat& msg){
  this->messageType = messageType;
  this->sourceID = sourceID;
  this->targetID = targetID;
  this->varLabels = labels;
  this->varSTDs = stds;
  this->varLimits = limits;
  this->msg = ntUtils::copySamples(msg);
}

// Copy Contructor
ntMessage::ntMessage(ntMessage* newMsg){
  this->messageType = newMsg->messageType;
  this->sourceID = newMsg->sourceID;
  this->targetID = newMsg->targetID;
  this->varLabels = newMsg->varLabels;
  this->varSTDs = newMsg->varSTDs;
  this->varLimits = newMsg->varLimits;
  this->msg = ntUtils::copySamples(newMsg->msg);
}

ntMessage::~ntMessage(){
}

void ntMessage::aggregate(ntMessage* newMsg){

  // If the current message is empty then copy the new one
  if(msg.size() == 0){

  	this->messageType = newMsg->messageType;
  	// Source Entity that sent the message
    this->sourceID = newMsg->sourceID;
    // Target Entity that received the message
    this->targetID = newMsg->targetID;
    // Label variables in the message
    this->varLabels = newMsg->varLabels;
    // Variable standard deviations
    this->varSTDs = newMsg->varSTDs;
    // Variable limits
    this->varLimits = newMsg->varLimits;
    // Samples forming the message
   this->msg = ntUtils::copySamples(newMsg->msg);

  }else{

    // Check the number of samples to be the same 
    // otherwise merging is not possible
    if(this->msg.size() != newMsg->msg.size()){

      throw ntException("ERROR: Incompatible messages in ntMessage::aggregate.");

    }else{

      // Add variable labels, standard deviations and limits
      for(int loopA=0;loopA<newMsg->varLabels.size();loopA++){
        this->varLabels.push_back(newMsg->varLabels[loopA]);
        this->varSTDs.push_back(newMsg->varSTDs[loopA]);
        this->varLimits.push_back(newMsg->varLimits[loopA*2]);
        this->varLimits.push_back(newMsg->varLimits[loopA*2+1]);
      }

      // Since these are different variables add to the dimensionality
      for(int loopA=0;loopA<this->msg.size();loopA++){
        for(int loopB=0;loopB<newMsg->msg[loopA].size();loopB++){
          this->msg[loopA].push_back(newMsg->msg[loopA][loopB]);
        }
      }
    }
  }
}

void ntMessage::aggregateFromList(vector<ntMessage*> msgs){

  // Check that all messages have the same sourceID and targetID
  bool allEq = true;
  if(msgs.size() > 1){
    int sID = msgs[0]->sourceID;
    int tID = msgs[0]->targetID;
    for(int loopA=1;loopA<msgs.size();loopA++){  
      allEq = allEq && ((sID == msgs[loopA]->sourceID)&&(tID == msgs[loopA]->targetID));
    }
  }
  if(!allEq){
    throw ntException("ERROR: Incompatible messages in ntMessage::aggregateFromList.");
  }else{
    // Aggregate
    for(int loopA=0;loopA<msgs.size();loopA++){
      // printf("aggregateFromList iteration %d\n",loopA);
      this->aggregate(msgs[loopA]);
    }
  }
}

void ntMessage::addEvidence(int varID,const stdVec& vals){
  double noise = 0.0;
  // Check compatibility
  if(vals.size() != msg.size()){
    throw ntException("ERROR: Incompatible messages in ntMessage::addEvidence.");
  }
  for(int loopB=0;loopB<vals.size();loopB++){
    msg[loopB][varID] = vals[loopB];
  }
}

string ntMessage::getMsgTypeString(){
  if(messageType == mtFactorToNode){
    return string("Factor_To_Node");
  }else if(messageType == mtNodeToFactor){
    return string("Node_To_Factor");
  }else{
    throw ntException("ERROR: Invalid message type in getMsgTypeString."); 
  }
}

bool areCompatibleMsgs(vector<ntMessage*> msgs){
  bool check = true;
  for(int loopA=1;loopA<msgs.size();loopA++){
    check = check && ntUtils::sameStringVec(msgs[0]->varLabels,msgs[loopA]->varLabels);
    check = check && ntUtils::sameVec(msgs[0]->varSTDs,msgs[loopA]->varSTDs);
    check = check && ntUtils::sameVec(msgs[0]->varLimits,msgs[loopA]->varLimits);
  }
  return check;
}

void ntMessage::aggregateMarginals(vector<ntMessage*> msgs){

  if(msgs.size() == 1){
    this->messageType = msgs[0]->messageType;
    this->sourceID    = msgs[0]->sourceID;
    this->targetID    = msgs[0]->targetID;
    this->varLabels   = msgs[0]->varLabels;
    this->varSTDs     = msgs[0]->varSTDs;
    this->varLimits   = msgs[0]->varLimits;
    this->msg = ntUtils::copySamples(msgs[0]->msg);
  }else{
    // Multiple Messages
    if(areCompatibleMsgs(msgs)){
      this->messageType = msgs[0]->messageType;
      this->sourceID    = msgs[0]->sourceID;
      this->targetID    = msgs[0]->targetID;
      this->varLabels   = msgs[0]->varLabels;
      this->varSTDs     = msgs[0]->varSTDs;
      this->varLimits   = msgs[0]->varLimits;
      for(int loopA=0;loopA<msgs.size();loopA++){
        ntUtils::appendSamples(msgs[loopA]->msg,this->msg);
      }      
    }else{
      throw ntException("ERROR: Incoming messages are not compatible in ntMessage::aggregateMarginals."); 
    }
  }
}

void ntMessage::writeToFile(string fileName){
  FILE* f;
  f = fopen(fileName.c_str(),"w");
  // Message type, sourceID and targetID
  fprintf(f,"%s,%d,%d\n",getMsgTypeString().c_str(),sourceID,targetID);
  // Variable labels
  for(int loopA=0;loopA<varLabels.size();loopA++){
    fprintf(f,"%s,",varLabels[loopA].c_str());
  }
  fprintf(f,"\n");
  // Variable STDs
  for(int loopA=0;loopA<varSTDs.size();loopA++){
    fprintf(f,"%.4e,",varSTDs[loopA]);
  }
  fprintf(f,"\n");
  // Variable Limits
  for(int loopA=0;loopA<varLimits.size();loopA++){
    fprintf(f,"%.4e,",varLimits[loopA]);
  }
  fprintf(f,"\n");
  // Print Samples
  for(int loopA=0;loopA<msg.size();loopA++){
    for(int loopB=0;loopB<msg[loopA].size();loopB++){
      fprintf(f,"%.4e,",msg[loopA][loopB]);
    }
    fprintf(f,"\n");
  }
  // Close State File
  fclose(f);
}

void ntMessage::show(){
  // Message type, sourceID and targetID
  printf("%s,%d,%d\n",getMsgTypeString().c_str(),sourceID,targetID);
  // Variable labels
  for(int loopA=0;loopA<varLabels.size();loopA++){
    printf("%s,",varLabels[loopA].c_str());
  }
  printf("\n");
  // Variable STDs
  for(int loopA=0;loopA<varSTDs.size();loopA++){
    printf("%.4e,",varSTDs[loopA]);
  }
  printf("\n");
  // Variable Limits
  for(int loopA=0;loopA<varLimits.size();loopA++){
    printf("%.4e,",varLimits[loopA]);
  }
  printf("\n");
  // Print Samples
  for(int loopA=0;loopA<msg.size();loopA++){
    for(int loopB=0;loopB<msg[loopA].size();loopB++){
      printf("%.4e,",msg[loopA][loopB]);
    }
    printf("\n");
  }
}

