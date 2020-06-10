# include "ntNet.h"

using namespace std;

ntNet::ntNet(string netFile){
  // Create network information
  netIO = new ntNetIO();
  
  // Read all information from file
  netIO->readFromFile(netFile);

  // Create Nodes and Edges
  createNetworkEntities(netIO);

  // Assign edge samples for root nodes
  assignEdgeRootSamples(netIO);

  // Assign approximants to nodes
  assignNodeApproximants(netIO);
}

ntNet::~ntNet(){
  delete netIO;
}

// Assign Evidence
void ntNet::assignEvidence(){

}

// Create Nodes and Edges
void ntNet::createNetworkEntities(ntNetIO* netInfo){
  // Create Nodes
  ntNode* node;
  ntEdge* edge;

  for(size_t loopA=0;loopA<netInfo->nodeID.size();loopA++){
    // Add root node
    node = new ntNode(loopA,netInfo);
    // Add to the list of Nodes
    nodeList.push_back(node);
  }
  // Create Edges
  for(size_t loopA=0;loopA<netInfo->edgeNode1.size();loopA++){
    // Add root node
    edge = new ntEdge();
    edgeList.push_back(edge);
  }

}

// Assign edge samples for root nodes
void ntNet::assignEdgeRootSamples(ntNetIO* netInfo){
}

// Assign approximants to nodes
void ntNet::assignNodeApproximants(ntNetIO* netInfo){

}

// Perform Belief Propagation
int ntNet::runBP(){

  // Initialize processing bookmarks for nodes and edges
  for(int loopA=0;loopA<nodeList.size();loopA++){
    nodeList[loopA]->processed = false;
  }
  for(int loopA=0;loopA<factorList.size();loopA++){
    factorList[loopA]->processed = false;
  }
  
  // Iterative complete message passing for the other nodes
  bool finished = false;
  while(!finished){
    // Propagate messages from nodes to factors
    for(int loopA=0;loopA<nodeList.size();loopA++){
      // Propagate nodes that are not processed
      if(!nodeList[loopA]->processed){
        nodeList[loopA]->sendMsgToFactors();
        nodeList[loopA]->processed = true;
      }
    }
    // Propagate messages from factors to nodes
    for(int loopA=0;loopA<factorList.size();loopA++){
      // Propagate nodes that are not processed
      if(!factorList[loopA]->processed){
        factorList[loopA]->sendMsgToNodes();
        factorList[loopA]->processed = true;
      }
    }
    // Update finished: is finished when no more nodes or edges are to be processed
    finished = true;
    for(int loopA=0;loopA<nodeList.size();loopA++){
      finished = finished && nodeList[loopA]->processed;
    }
    for(int loopA=0;loopA<factorList.size();loopA++){
      finished = finished && factorList[loopA]->processed;
    }
  }
}

