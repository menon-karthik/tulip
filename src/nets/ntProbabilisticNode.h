#ifndef NTPROBABILISTICNODE_H
#define NTPROBABILISTICNODE_H

# include "ntNode.h"

class ntProbabilisticNode: public ntNode{
  public:

  	//! Default Constructor
  	ntProbabilisticNode(int nodeID,ntNetIO* netIO);
    //! Virtual Destructor
    virtual ~ntProbabilisticNode();

    // Propagate through the node
    virtual void propagate();
};

#endif // NTPROBABILISTICNODE_H
