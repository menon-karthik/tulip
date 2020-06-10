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

    bool processed;

    vector<ntNode*> factorNodes;
    stdBoolVec isDownstreamNode;

    vector<ntMessage*> messages;

  	//! Default Constructor
  	ntFactor();
    //! Virtual Destructor
    virtual ~ntFactor();

    stdMat getMsg(int targetNodeID);

    void updateMsg(int factorID,const stdMat& msg);

    // Propagate from the node to the factors that are connected
    virtual void sendMsgToNodes();

};

#endif // NTFACTOR_H
