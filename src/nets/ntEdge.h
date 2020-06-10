#ifndef NTEDGE_H
#define NTEDGE_H

# include "uqTypes.h"

class ntEdge{
  protected:

    // Store direct incidence
    int nodes[2];
 
  public:

  	//! Default Constructor
  	ntEdge();    
    //! Virtual Destructor
    virtual ~ntEdge();
  
};

#endif // NTEDGE_H
