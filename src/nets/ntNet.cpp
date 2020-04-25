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
    if(netInfo->nodeType[loopA] == ntRoot){
      
      // Add root node
      node = new ntRootNode(loopA,netInfo);

    }else if(netInfo->nodeType[loopA] == ntDeterministic){
      
      // Add deterministic node
      node = new ntDeterministicNode(loopA,netInfo);

    }else if(netInfo->nodeType[loopA] == ntProbabilistic){
      
      // Add probabilistic node
      node = new ntProbabilisticNode(loopA,netInfo);

    }
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
int ntNet::propagateBeliefs(){
	
  // // Propagate Forward Root Nodes and Nodes with Evidence
  // for(int loopA=0;loopA<nodeList.size();loopA++){
  //   if(nodeList[loopA]->isRoot()||nodeList[loopA]->hasEvidence()){
  //     nodeList[loopA]->propagate();
  //   }
  // }

  // // Iterative complete message passing for the other nodes
  // bool finishedNodeProcessing = false;
  // while(!finishedNodeProcessing){
  //   // Complete another loop through all the nodes
  //   for(int loopA=0;loopA<nodeList.size();loopA++){
  //     // Propagate nodes that are not processed
  //     if(!nodeList[loopA]->processed){
  //       nodeList[loopA]->propagate();
  //     }
  //   }
  //   // Update finished: is finished when no more nodes to be processed
  //   finishedNodeProcessing = true;
  //   for(int loopA=0;loopA<nodeList.size();loopA++){
  //     finishedNodeProcessing = finishedNodeProcessing && nodeList[loopA]->processedcq;
  //   }
  // }
}

