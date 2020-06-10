#ifndef NTNET_H
#define NTNET_H

# include "ntNetIO.h"
# include "ntNode.h"
# include "ntEdge.h"

/*! 
Abstract parent class for probabilistic networks in UQ
*/
class ntNet{
  public:

    // Information on the network from file
    ntNetIO* netIO;
    
    //! List of nodes
    vector<ntNode*> nodeList;
    //! List of edges
    vector<ntFactor*> factorList;
    //! List of edges
    vector<ntEdge*> edgeList;

  	//! Default Constructor
  	ntNet(string netFile);
    //! Virtual Destructor
    virtual ~ntNet();

    void createNetworkEntities(ntNetIO* netInfo);
    void assignEdgeRootSamples(ntNetIO* netInfo);
    void assignNodeApproximants(ntNetIO* netInfo);


    // Assign Evidence
    void assignEvidence();

    // Perform Belief Propagation
    int runBP();

  
};

#endif // NTNET_H
