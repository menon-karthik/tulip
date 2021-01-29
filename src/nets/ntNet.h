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

    //! Print messages during execution
    //! 0-silent, >0 increasing messages
    int printLevel;

    // Gaussian and uniform samplers
    uqPDF* nSampler;
    uqPDF* uSampler;

  	//! Default Constructor
  	ntNet(string netFile);
    //! Virtual Destructor
    virtual ~ntNet();

    // Set Print Level
    void setPrintLevel(int level);

    // Create the network and its topology
    void createNetworkEntities(ntNetIO* netInfo);
    void createFactorGraph();
    void initMsgsOnRootFactorsLeafNodes();

    // Assign Evidence
    void assignEvidence(int nodeID,const stdIntVec& varIDs,const stdVec& varVals,const stdVec& varStd);
    void removeEvidence(int nodeID);
    void removeEvidence();
    void printEvidence();

    // Utils
    int getNodeListOrder(int nodeID);
    stdIntVec getUpsteamNodeList(int nodeListID);

    // Debug: Print a graphviz file to show the network topology
    void checkFactorGraphTopology(string fileName);
    // Debug: Print a graphviz file to the number of messages in te network
    void checkFactorGraphMessages(string fileName);

    void printAllMessages();

    // Write all messages and samples to file
    void writeAllMessages();

    // Perform Belief Propagation
    int runBP();
};

#endif // NTNET_H
