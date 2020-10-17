#ifndef NTNET_H
#define NTNET_H

# include "ntNetIO.h"
# include "ntNode.h"
# include "ntEdge.h"

using namespace std;

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

    // Gaussian and uniform samplers
    uqPDF* nSampler;
    uqPDF* uSampler;

  	//! Default Constructor
  	ntNet(string netFile);
    //! Virtual Destructor
    virtual ~ntNet();

    // Create the network and its topology
    void createNetworkEntities(ntNetIO* netInfo);
    void createFactorGraph();
    void initMsgsOnRootFactorsLeafNodes();

    // Assign Evidence
    void assignEvidence(string fileName);
    void assignEvidence(int nodeID,const stdIntVec& varIDs,const stdVec& varVals,const stdVec& varStd);
    void removeEvidence(int nodeID);
    void removeEvidence();
    void printEvidence();

    // Utils
    int getNodeListOrder(int nodeID);
    stdIntVec getUpsteamNodeList(int nodeListID);

    // Debug
    void checkFactorGraphTopology(string fileName);

    // Perform Belief Propagation
    int runBP();

  
};

#endif // NTNET_H
