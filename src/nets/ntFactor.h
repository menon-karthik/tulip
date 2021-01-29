#ifndef NTFACTOR_H
#define NTFACTOR_H

# include "uqTypes.h"
# include "ntNode.h"
# include "ntMessage.h"

using namespace std;

class ntNode;

class ntFactor{
  public:

    int factorID;

    vector<ntNode*> factorNodes;
    stdBoolVec isDownstreamNode;

    vector<ntMessage*> inMsgs;

  	//! Default Constructor
  	ntFactor();
    //! Virtual Destructor
    virtual ~ntFactor();

    // Find incoming message for a given factor ID
    bool findInMsg(int nodeID);
    // Check if the factor is ready to send messages to node nodeID
    bool messagesAreReadyFor(int nodeID);
    // Check that all incoming messages are present
    bool hasProcessedAllMsgs();

    void setFactorID(int factorID);

    void appendToFactorNodes(const vector<ntNode*>& nodes,const stdBoolVec& isDownNode);

    ntMessage* copyInMsg(int nodeID);

    void updateMsg(int nodeID,ntMessage* currMsg);

    // Propagate from the node to the factors that are connected
    void sendMsgToNodes();

    void printMessages();

    int getDownstreamDetNodeLocalID();

};

#endif // NTFACTOR_H
