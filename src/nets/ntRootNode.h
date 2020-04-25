#ifndef NTROOTNODE_H
#define NTROOTNODE_H

# include "ntNode.h"

class ntRootNode: public ntNode{
  public:

  	//! Default Constructor
  	ntRootNode(int nodeID,ntNetIO* netIO);
    //! Virtual Destructor
    virtual ~ntRootNode();

    // Propagate through the node
    virtual void propagate();

};

#endif // NTROOTNODE_H
