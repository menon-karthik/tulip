#ifndef NTEDGE_H
#define NTEDGE_H

# include "uqTypes.h"

class ntEdge{
  public:

    // Store direct incidence
    int nodes[2];
    
  	//! Default Constructor
  	ntEdge(int node1,int node2);
    //! Virtual Destructor
    virtual ~ntEdge();
  
};

#endif // NTEDGE_H
