# include "ntNet.h"

using namespace std;

ntNet::ntNet(string netFile){

  // Init Gaussian sampler
  uqPDF* nSampler = new uqGaussianPDF();
  // Init Uniform sampler
  uqPDF* uSampler = new uqUniformPDF();

  // Create network information
  netIO = new ntNetIO();
  
  // Read all information from file
  netIO->readFromFile(netFile);

  // Create Nodes and Edges
  printf("--- Creating Network Entities...\n");
  createNetworkEntities(netIO);
  printf("--- Done Creating Network Entities.\n");

  // Create Factor Graph for Messgae Passing
  printf("--- Creating Factor Graph...");
  createFactorGraph();
  printf("Done.\n");

  // Generate dot file for graphviz graphical representation
  // checkFactorGraphTopology("factorGraphTopology.txt");

  // Initialize the Messages on root and leaf nodes
  printf("--- Initializing Root Factors and Leaf Nodes...");
  initMsgsOnRootFactorsLeafNodes();
  printf("Done.\n");
}

ntNet::~ntNet(){
  delete netIO;
  delete nSampler;
  delete uSampler;
}

// Assign Single-node Evidence
void ntNet::assignEvidence(int nodeID,const stdIntVec& varIDs,const stdVec& varAvg,const stdVec& varStd){
  // Check evidence vectors
  if(varIDs.size() != varAvg.size()||(varIDs.size() != varStd.size())){
    throw ntException("ERROR: Incompatible evidence vectors in ntNet::assignEvidence.");
  }
  // Find Node in the network
  bool found = false;
  int count = 0;
  while((!found)&&(count<nodeList.size())){
    found = (nodeList[count]->nodeID == nodeID);
  }
  if(!found){
    throw ntException("ERROR: Cound not find node ID in ntNet::assignEvidence."); 
  }else{
    nodeList[count]->evidenceVarID.clear();
    nodeList[count]->evidenceVarID = varIDs;
    nodeList[count]->evidenceVarAvg.clear();
    nodeList[count]->evidenceVarAvg = varAvg;
    nodeList[count]->evidenceVarStd.clear();
    nodeList[count]->evidenceVarStd = varStd;
  }
}

void ntNet::checkFactorGraphTopology(string fileName){
  // Create a dot file with the graph topology

  FILE* f;
  f = fopen(fileName.c_str(),"w");
  
  fprintf(f,"digraph factorGraph {\n");

  // Write all nodes
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    if(nodeList[loopA]->nodeType == ntRoot){
      fprintf(f,"N%d [shape=circle]\n",int(nodeList[loopA]->nodeID));
    }else if(nodeList[loopA]->nodeType == ntDeterministic){
      fprintf(f,"N%d [shape=doublecircle]\n",int(nodeList[loopA]->nodeID));
    }else if(nodeList[loopA]->nodeType == ntProbabilistic){
      fprintf(f,"N%d [shape=diamond]\n",int(nodeList[loopA]->nodeID));
    }
  }

  // Write all factors
  for(size_t loopA=0;loopA<factorList.size();loopA++){
    fprintf(f,"F%d [shape=square,color=red]\n",int(factorList[loopA]->factorID));
  }

  // Write node links to factors
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    for(size_t loopB=0;loopB<nodeList[loopA]->nodeFactors.size();loopB++){      
      if(nodeList[loopA]->isDownstreamFactor[loopB]){
        fprintf(f,"N%d -> F%d\n",int(nodeList[loopA]->nodeID),int(nodeList[loopA]->nodeFactors[loopB]->factorID));
      }else{
        fprintf(f,"F%d -> N%d\n",int(nodeList[loopA]->nodeFactors[loopB]->factorID),int(nodeList[loopA]->nodeID));
      }
    }
  }

  // Write factor links to node
  for(size_t loopA=0;loopA<factorList.size();loopA++){

    for(size_t loopB=0;loopB<factorList[loopA]->factorNodes.size();loopB++){      
      if(factorList[loopA]->isDownstreamNode[loopB]){
        fprintf(f,"F%d -> N%d [color=red]\n",int(factorList[loopA]->factorID),int(factorList[loopA]->factorNodes[loopB]->nodeID));
      }else{
        fprintf(f,"N%d -> F%d [color=red]\n",int(factorList[loopA]->factorNodes[loopB]->nodeID),int(factorList[loopA]->factorID));
      }
    }
  }

  // Close File
  fprintf(f,"}\n");
  fclose(f);
}

void ntNet::initMsgsOnRootFactorsLeafNodes(){
  
  stdMat currMessage;
  stdVec tmpSample;
  ntMessage* msg;
  double currSample;

  // Initialize Root Factor Messages with Priors
  for(int loopA=0;loopA<factorList.size();loopA++){
    if(factorList[loopA]->factorNodes.size() == 1){
      if(factorList[loopA]->isDownstreamNode[0]){
        // Copy samples from downstream node
        currMessage = ntUtils::copySamples(factorList[loopA]->factorNodes[0]->varSamples);
        // Create New Message
        msg = new ntMessage(mtFactorToNode,
                            factorList[loopA]->factorID,
                            factorList[loopA]->factorNodes[0]->nodeID,
                            currMessage);
        factorList[loopA]->messages.push_back(msg);
        factorList[loopA]->processed = true;
      }
    }
  }

  // Initialize Leaf Nodes with Uniform Distributions
  for(int loopA=0;loopA<nodeList.size();loopA++){
    if(nodeList[loopA]->nodeFactors.size() == 1){
      if(!nodeList[loopA]->isDownstreamFactor[0]){        
        // Loop on number of samples
        currMessage.clear();
        for(int loopB=0;loopB<nodeList[loopA]->numSamples;loopB++){
          tmpSample.clear();
          for(int loopC=0;loopC<nodeList[loopA]->numVariables;loopC++){
            // Generate a random sample for this variable in the range
            currSample = uSampler->sample(nodeList[loopA]->limits[loopC*2],nodeList[loopA]->limits[loopC*2+1]);
            // Add to temporary sample
            tmpSample.push_back(currSample);
          }
          currMessage.push_back(tmpSample);
        }
        // Add to message to single factor
        // Create New Message
        msg = new ntMessage(mtNodeToFactor,
                            nodeList[loopA]->nodeID,
                            nodeList[loopA]->nodeFactors[0]->factorID,
                            currMessage);
        nodeList[loopA]->messages.push_back(msg);
        nodeList[loopA]->processed = true;        
      }
    }
  }
}

// Assign Evidence for multiple nodes by reading it from a file
void ntNet::assignEvidence(string fileName){
  throw ntException("ERROR: assignEvidence not yet implemented."); 
}

// Remove evidence from single nodes
void ntNet::removeEvidence(int nodeID){
  // Find Node in the network
  bool found = false;
  int count = 0;
  while((!found)&&(count<nodeList.size())){
    found = (nodeList[count]->nodeID == nodeID);
  }
  if(!found){
    throw ntException("ERROR: Cound not find node ID in ntNet::assignEvidence."); 
  }else{
    nodeList[count]->evidenceVarID.clear();
    nodeList[count]->evidenceVarAvg.clear();
    nodeList[count]->evidenceVarStd.clear();
  }
}

// Remove evidence from all nodes
void ntNet::removeEvidence(){
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    nodeList[loopA]->evidenceVarID.clear();
    nodeList[loopA]->evidenceVarAvg.clear();
    nodeList[loopA]->evidenceVarStd.clear();
  }
}

// Print Evidence in Networks
void ntNet::printEvidence(){
  printf("%10s %10s %15s %15s\n","NodeID","VarID","Average","Std");
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    for(size_t loopB=0;loopB<nodeList[loopA]->evidenceVarID.size();loopB++){
      printf("%10d %10d %15.3e %15.3e\n",nodeList[loopA]->nodeID,
                                         nodeList[loopA]->evidenceVarID[loopB],
                                         nodeList[loopA]->evidenceVarAvg[loopB],
                                         nodeList[loopA]->evidenceVarStd[loopB]);
    }
  }
}

// Create Nodes and Edges
void ntNet::createNetworkEntities(ntNetIO* netInfo){
  // Create Nodes
  ntNode* node;
  ntEdge* edge;

  for(size_t loopA=0;loopA<netInfo->nodeID.size();loopA++){
    // Add root node
    node = new ntNode(loopA,netInfo);
    // printf("Node ID: %d\n",netInfo->nodeID[loopA]);
    // printf("Node ID: %d\n",node->nodeID);
    // Add to the list of Nodes
    nodeList.push_back(node);
  }

  // Create Edges
  for(size_t loopA=0;loopA<netInfo->edgeNode1.size();loopA++){
    // Add root node
    edge = new ntEdge(netInfo->edgeNode1[loopA],netInfo->edgeNode2[loopA]);
    edgeList.push_back(edge);
  }
}

int ntNet::getNodeListOrder(int nodeID){
  int count = 0;
  bool found = false;
  while((!found)&&(count<nodeList.size())){
    found = (nodeList[count]->nodeID == nodeID);
    if(!found){
      count++;
    }
  }
  return count;
}

stdIntVec ntNet::getUpsteamNodeList(int nodeListID){
  stdIntVec tmp;
  for(size_t loopA=0;loopA<edgeList.size();loopA++){
    if(edgeList[loopA]->nodes[1] == nodeList[nodeListID]->nodeID){
      tmp.push_back(getNodeListOrder(edgeList[loopA]->nodes[0]));
    }
  }
  return tmp;
}

void ntNet::createFactorGraph(){
  stdIntVec upNodeList;
  ntFactor* ntf;
  vector<ntFactor*> tmpFactors;
  vector<ntNode*> tmpNodes;
  stdBoolVec tmpDown;

  for(size_t loopA=0;loopA<nodeList.size();loopA++){

    // Get List of upstream nodes
    upNodeList = getUpsteamNodeList(loopA);
  
    // Create Factor
    ntf = new ntFactor();

    // Current node is downstream
    tmpNodes.clear();
    tmpDown.clear();
    tmpNodes.push_back(nodeList[loopA]);
    tmpDown.push_back(true);
    ntf->appendToFactorNodes(tmpNodes,tmpDown);

    // Make the factor point to the downstream node
    if(upNodeList.size()>0){
      // Place Factor Upstream
      tmpNodes.clear();
      tmpDown.clear();
      for(size_t loopB=0;loopB<upNodeList.size();loopB++){
        tmpNodes.push_back(nodeList[upNodeList[loopB]]);
        tmpDown.push_back(false);
      }
      ntf->appendToFactorNodes(tmpNodes,tmpDown);
    }
    
    // Set factorID consistent with the order in factorList
    ntf->setFactorID(factorList.size());
    // Add Factor to the list
    factorList.push_back(ntf);    

    // Current node is downstream
    tmpFactors.clear();
    tmpDown.clear();
    tmpFactors.push_back(factorList[factorList.size()-1]);
    tmpDown.push_back(false);
    nodeList[loopA]->appendToNodeFactors(tmpFactors,tmpDown);

    // Make upstream nodes to point to this new factor
    if(upNodeList.size()>0){
      // Place Factor Upstream
      tmpFactors.clear();
      tmpDown.clear();
      tmpFactors.push_back(factorList[factorList.size()-1]);
      tmpDown.push_back(true);
      for(size_t loopB=0;loopB<upNodeList.size();loopB++){
        nodeList[upNodeList[loopB]]->appendToNodeFactors(tmpFactors,tmpDown);
      }
    }
  }
  
  // Add Factor index to nodes
  //for(size_t loopA=0;loopA<factorList.size();loopA++){
  //  for(size_t loopB=0;loopB<factorList[loopA]->factorNodes.size();loopB++){
  //    factorList[loopA]->factorNodes[loopB]->nodeFactors.push_back(factorList[loopA]);
  //    factorList[loopA]->factorNodes[loopB]->isDownstreamFactor.push_back(!factorList[loopA]->isDownstreamNode[loopB]);
  //  }
  //}
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
  int iterCount = 0;
  while(!finished){

    printf("Running BP Iteration %d\n",iterCount+1);

    // Propagate messages from nodes to factors
    for(int loopA=0;loopA<nodeList.size();loopA++){
      // Propagate nodes that are not processed
      if(!nodeList[loopA]->processed){
        nodeList[loopA]->sendMsgToFactors();
        nodeList[loopA]->processed = true;
      }
    }

    printf("Eccolo 2\n");

    // Propagate messages from factors to nodes
    for(int loopA=0;loopA<factorList.size();loopA++){
      // Propagate nodes that are not processed
      if(!factorList[loopA]->processed){
        factorList[loopA]->sendMsgToNodes();
        factorList[loopA]->processed = true;
      }
    }

    printf("Eccolo 3\n");

    // Update finished: is finished when no more nodes or edges are to be processed
    finished = true;
    for(int loopA=0;loopA<nodeList.size();loopA++){
      finished = finished && nodeList[loopA]->processed;
    }
    for(int loopA=0;loopA<factorList.size();loopA++){
      finished = finished && factorList[loopA]->processed;
    }

    // Update Iteration Count
    iterCount++;

  }
}

