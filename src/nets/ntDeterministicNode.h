#ifndef NTDETERMINISTICNODE_H
#define NTDETERMINISTICNODE_H

# include "ntNode.h"

class ntDeterministicNode: public ntNode{
  public:

  	//! Default Constructor
  	ntDeterministicNode(int nodeID,ntNetIO* netIO);
    //! Virtual Destructor
    virtual ~ntDeterministicNode();

    // Propagate through the node
    virtual void propagate();  
};

#endif // NTDETERMINISTICNODE_H
