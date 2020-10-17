# include "ntMessage.h"

ntMessage::ntMessage(messageTypes messageType,int sourceID,int targetID,const stdMat& msg){
  this->messageType = messageType;
  this->sourceID = sourceID;
  this->targetID = targetID;
  this->msg = ntUtils::copySamples(msg);
}

ntMessage::~ntMessage(){
}

