#ifndef NTMESSAGE_H
#define NTMESSAGE_H

# include "uqTypes.h"
# include "ntUtils.h"

enum messageTypes{mtFactorToNode,mtNodeToFactor};

class ntMessage{
  public:
    
    // Type of message
  	messageTypes messageType;
  	// Source Entity that sent the message
    int sourceID;
    // Target Entity that received the message
    int targetID;
    // Samples forming the message
    stdMat msg;

  	//! Default Constructor
  	ntMessage(messageTypes messageType,int sourceID,int targetID,const stdMat& msg);
    //! Virtual Destructor
    virtual ~ntMessage();
  
};

#endif // NTMESSAGE_H
