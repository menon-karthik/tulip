# include "ntNet.h"

using namespace std;

ntNet::ntNet(string netFile){

  // Init Gaussian sampler
  nSampler = new uqGaussianPDF();
  // Init Uniform sampler
  uSampler = new uqUniformPDF();

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
  checkFactorGraphTopology("factorGraphTopology.txt");

  // Set Default Print Level to silent
  printLevel = 0;
}

ntNet::~ntNet(){
  delete netIO;
  delete nSampler;
  delete uSampler;
}

void ntNet::setPrintLevel(int level){
  this->printLevel = level;
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
    if(!found){
      count++;
    }
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

void ntNet::checkFactorGraphMessages(string fileName){
  // Create a dot file with the graph topology

  FILE* f;
  f = fopen(fileName.c_str(),"w");
  
  fprintf(f,"digraph factorGraph {\n");

  fprintf(f,"ratio=\"fill\";\n");
  fprintf(f,"size=\"4.0,4.0!\";\n");
  fprintf(f,"margin=0;\n");

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

  // Write factor to node messages
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    for(size_t loopB=0;loopB<nodeList[loopA]->inMsgs.size();loopB++){
      fprintf(f,"F%d -> N%d [color=red,labelfontcolor=red,label = """"%d""""]\n",nodeList[loopA]->inMsgs[loopB]->sourceID,
                                                                             nodeList[loopA]->inMsgs[loopB]->targetID,
                                                                             int(nodeList[loopA]->inMsgs[loopB]->varLabels.size()));
    }
  }

  // Write factor links to node
  for(size_t loopA=0;loopA<factorList.size();loopA++){
    for(size_t loopB=0;loopB<factorList[loopA]->inMsgs.size();loopB++){      
      fprintf(f,"N%d -> F%d [label = """"%d""""]\n",int(factorList[loopA]->inMsgs[loopB]->sourceID),
                                                int(factorList[loopA]->inMsgs[loopB]->targetID),
                                                int(factorList[loopA]->inMsgs[loopB]->varLabels.size()));
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
  double lb = 0.0;
  double ub = 0.0;
  int subDivs = 0;
  stdVec tmp;
  int varID = 0;
  double avg = 0.0;
  double std = 0.0;

  // Initialize Root Factor Messages with Priors
  for(int loopA=0;loopA<factorList.size();loopA++){
    if(factorList[loopA]->factorNodes.size() == 1){
      if(factorList[loopA]->isDownstreamNode[0]){
        printf("Initializing Factor %d\n",factorList[loopA]->factorID);
        
        // Copy samples from downstream node
        currMessage = ntUtils::copySamples(factorList[loopA]->factorNodes[0]->varSamples);

        // Create New Message
        msg = new ntMessage(mtFactorToNode,
                            factorList[loopA]->factorID,
                            factorList[loopA]->factorNodes[0]->nodeID,
                            factorList[loopA]->factorNodes[0]->varNames,
                            factorList[loopA]->factorNodes[0]->varSTD,
                            factorList[loopA]->factorNodes[0]->limits,                            
                            currMessage);

        // Add evidence to the message if needed
        if(factorList[loopA]->factorNodes[0]->evidenceVarID.size() > 0){        
          // If there is evidence for certain variables, replace these variables with the evidence
          for(int loopB=0;loopB<factorList[loopA]->factorNodes[0]->evidenceVarID.size();loopB++){
            tmp.clear();
            varID = factorList[loopA]->factorNodes[0]->evidenceVarID[loopB];
            avg = factorList[loopA]->factorNodes[0]->evidenceVarAvg[loopB];
            std = factorList[loopA]->factorNodes[0]->evidenceVarStd[loopB];
            for(int loopC=0;loopC<msg->msg.size();loopC++){
              tmp.push_back(avg + nSampler->sample(0.0,std));
            }
            msg->addEvidence(varID,tmp);
          }
        }  

        // Add Message
        factorList[loopA]->factorNodes[0]->inMsgs.push_back(msg);
      }
    }
  }  

  // Initialize Leaf Nodes with Uniform Distributions
  for(int loopA=0;loopA<nodeList.size();loopA++){
    if(nodeList[loopA]->nodeFactors.size() == 1){
      if(!nodeList[loopA]->isDownstreamFactor[0]){                
        printf("Initializing Node %d\n",nodeList[loopA]->nodeID);
        // Copy samples to a message
        currMessage = ntUtils::copySamples(nodeList[loopA]->varSamples);
        // Create New Message
        msg = new ntMessage(mtNodeToFactor,
                            nodeList[loopA]->nodeID,
                            nodeList[loopA]->nodeFactors[0]->factorID,
                            nodeList[loopA]->varNames,
                            nodeList[loopA]->varSTD,
                            nodeList[loopA]->limits,
                            currMessage);

        // Add evidence to the message if needed
        if(nodeList[loopA]->evidenceVarID.size() > 0){ 
          // If there is evidence for certain variables, replace these variables with the evidence
          for(int loopB=0;loopB<nodeList[loopA]->evidenceVarID.size();loopB++){
            tmp.clear();
            varID = nodeList[loopA]->evidenceVarID[loopB];
            avg = nodeList[loopA]->evidenceVarAvg[loopB];
            std = nodeList[loopA]->evidenceVarStd[loopB];
            for(int loopC=0;loopC<msg->msg.size();loopC++){
              tmp.push_back(avg + nSampler->sample(0.0,std));
            }
            msg->addEvidence(varID,tmp);
          }
        }  

        // Provide the message as an incoming message to the factor
        nodeList[loopA]->nodeFactors[0]->inMsgs.push_back(msg);
      }
    }
  }
}

void ntNet::assignUniformSamplesToLeaves(){
  // Initialize Leaf Nodes with Uniform Distributions
  stdVec tmpSample;
  double currSample = 0.0;
  for(int loopA=0;loopA<nodeList.size();loopA++){
    if(nodeList[loopA]->nodeFactors.size() == 1){
      if(!nodeList[loopA]->isDownstreamFactor[0]){                
        // This is a leaf node
        printf("Assigning Uniform Samples to Node %d\n",nodeList[loopA]->nodeID);
        nodeList[loopA]->varSamples.clear();
        // Loop on number of samples
        for(int loopB=0;loopB<nodeList[loopA]->numSamples;loopB++){
          tmpSample.clear();
          for(int loopC=0;loopC<nodeList[loopA]->numVariables;loopC++){
            // Generate a random sample for this variable in the range
            currSample = uSampler->sample(nodeList[loopA]->limits[loopC*2],nodeList[loopA]->limits[loopC*2+1]);
            // Add to temporary sample
            tmpSample.push_back(currSample);
          }
          nodeList[loopA]->varSamples.push_back(tmpSample);
        }
      }
    }
  }
}

void ntNet::reassignRootAndLeafSamplesFromPreviousRun(){

  // All Incoming Messages for the root factors become samples for the downstream node  
  for(int loopA=0;loopA<factorList.size();loopA++){
    if(factorList[loopA]->factorNodes.size() == 1){
      if(factorList[loopA]->isDownstreamNode[0]){
        printf("Initializing Factor %d\n",factorList[loopA]->factorID);
        // Get Incoming Message from previous run and Assign it as samples for the downstream node
        factorList[loopA]->factorNodes[0]->varSamples = ntUtils::copySamples(factorList[loopA]->factorNodes[0]->marginal->msg);
      }
    }
  }

  // Initialize Leaf Nodes with Uniform Distributions
  for(int loopA=0;loopA<nodeList.size();loopA++){
    if(nodeList[loopA]->nodeFactors.size() == 1){
      if(!nodeList[loopA]->isDownstreamFactor[0]){                
        printf("Initializing Node %d\n",nodeList[loopA]->nodeID);
        // Get Incoming Message from previous run and Assign it as samples for the downstream node
        nodeList[loopA]->varSamples = ntUtils::copySamples(nodeList[loopA]->marginal->msg);
      }
    }
  }

  // Delete all incoming messages on nodes
  printf("Deleting all Node Messages and Marginals...\n");
  for(int loopA=0;loopA<nodeList.size();loopA++){
    delete nodeList[loopA]->marginal;
    nodeList[loopA]->marginal = NULL;
    nodeList[loopA]->hasMarginal = false;
    for(int loopB=0;loopB<nodeList[loopA]->inMsgs.size();loopB++){
      delete nodeList[loopA]->inMsgs[loopB];
      nodeList[loopA]->inMsgs[loopB] = NULL;
    }
    nodeList[loopA]->inMsgs.clear();
  }

  // Delete all incoming messages on factors
  printf("Deleting all Factor Messages...\n");
  for(int loopA=0;loopA<factorList.size();loopA++){
    for(int loopB=0;loopB<factorList[loopA]->inMsgs.size();loopB++){
      delete factorList[loopA]->inMsgs[loopB];
      factorList[loopA]->inMsgs[loopB] = NULL;
    }
    factorList[loopA]->inMsgs.clear();
  }

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
  bool foundEvidence = false;
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    for(size_t loopB=0;loopB<nodeList[loopA]->evidenceVarID.size();loopB++){
      if(!foundEvidence){
        printf("\n");  
        printf("#####################\n");  
        printf("### NODE EVIDENCE ###\n");  
        printf("#####################\n");  
        printf("%10s %10s %15s %15s\n","NodeID","VarID","Average","Std");  
        foundEvidence = true;
      }
      printf("%10d %10d %15.3e %15.3e\n",nodeList[loopA]->nodeID,
                                         nodeList[loopA]->evidenceVarID[loopB],
                                         nodeList[loopA]->evidenceVarAvg[loopB],
                                         nodeList[loopA]->evidenceVarStd[loopB]);
    }
  }
  if(!foundEvidence){
    printf("No Evidence Found!\n");
  }else{
    printf("#####################\n");
    printf("\n");
  }
}

void ntNet::assignProbabilisticNodesInputs(){

  // Assign Additional Information to Probabilistic Nodes
  // int totalInputs;
  // stdIntVec inputSubdiv;
  // stdStringVec inputNames;

  stdIntVec inputIdx;

  int totInputs = 0;
  for(size_t loopA=0;loopA<nodeList.size();loopA++){
    totInputs = 0;
    if(nodeList[loopA]->nodeType == ntProbabilistic){
      inputIdx.clear();

      // int totalInputs;
      for(size_t loopB=0;loopB<edgeList.size();loopB++){
        if(edgeList[loopB]->nodes[1] == loopA){
          totInputs += nodeList[loopA]->numVariables;
          inputIdx.push_back(edgeList[loopB]->nodes[0]);
        }
      }
      nodeList[loopA]->totalInputs = totInputs;

      // stdIntVec inputSubdiv;
      for(int loopB=0;loopB<inputIdx.size();loopB++){
        if(nodeList[inputIdx[loopB]]->varSubdiv.size() > 0){
          // Assign subdivisions if they have been defined
          for(int loopC=0;loopC<nodeList[inputIdx[loopB]]->varSubdiv.size();loopC++){
            nodeList[loopA]->inputSubdiv.push_back(nodeList[inputIdx[loopB]]->varSubdiv[loopC]);
          }
          
        }else{
          // Binary variables if not otherwise specified
          for(int loopC=0;loopC<nodeList[inputIdx[loopB]]->numVariables;loopC++){
            // Binary variables if not otherwise stated
            nodeList[loopA]->inputSubdiv.push_back(2);
          }
        }
      }

      // stdStringVec inputNames;
      for(int loopB=0;loopB<inputIdx.size();loopB++){
        for(int loopC=0;loopC<nodeList[inputIdx[loopB]]->varNames.size();loopC++){
          nodeList[loopA]->inputNames.push_back(nodeList[inputIdx[loopB]]->varNames[loopC]);
        }
      }
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
    // Add to the list of Nodes
    nodeList.push_back(node);
  }

  // Create Edges
  for(size_t loopA=0;loopA<netInfo->edgeNode1.size();loopA++){
    // Add root node
    edge = new ntEdge(netInfo->edgeNode1[loopA],netInfo->edgeNode2[loopA]);
    // Add to the list of Edges
    edgeList.push_back(edge);
  }

  // Gather Additional Information For Probabilistic Nodes
  assignProbabilisticNodesInputs();

  // printf("PROBABILISTIC NODE INFOS\n");  
  // for(int loopA=0;loopA<nodeList.size();loopA++){
  //   if(nodeList[loopA]->nodeType == ntProbabilistic){
  //     // Print the variables names
  //     printf("VAR NAMES\n");
  //     for(int loopB=0;loopB<nodeList[loopA]->varNames.size();loopB++){
  //       printf("%s ",nodeList[loopA]->varNames[loopB].c_str());  
  //     }
  //     printf("\n");
  //     printf("VAR SUBDIVS\n");
  //     for(int loopB=0;loopB<nodeList[loopA]->varSubdiv.size();loopB++){
  //       printf("%d ",nodeList[loopA]->varSubdiv[loopB]);  
  //     }
  //     printf("\n");
  //     printf("Total Inputs: %d\n",nodeList[loopA]->totalInputs);
  //     printf("INPUT NAMES\n");
  //     for(int loopB=0;loopB<nodeList[loopA]->inputNames.size();loopB++){
  //       printf("%s ",nodeList[loopA]->inputNames[loopB].c_str());  
  //     }
  //     printf("\n");
  //     printf("INPUT SUBDIVS\n");
  //     for(int loopB=0;loopB<nodeList[loopA]->inputSubdiv.size();loopB++){
  //       printf("%d ",nodeList[loopA]->inputSubdiv[loopB]);  
  //     }
  //     printf("\n");
  //     printf("\n");
  //   }
  // }

  // Assign Node Evidence
  stdIntVec evVars;
  stdVec evAvg;
  stdVec evStd;
  int currNode = 0;
  if(netInfo->evidenceNodeID.size() > 0){
    for(size_t loopA=0;loopA<netInfo->evidenceNodeID.size();loopA++){
      currNode = netInfo->evidenceNodeID[loopA];
      evVars.clear();
      evAvg.clear();
      evStd.clear();
      for(size_t loopB=0;loopB<netInfo->evidenceVarID[loopA].size();loopB++){
        evVars.push_back(netInfo->evidenceVarID[loopA][loopB]);
        evAvg.push_back(netInfo->evidenceVarAvg[loopA][loopB]);
        evStd.push_back(netInfo->evidenceVarStd[loopA][loopB]);
      }
      // Add Evidence to node
      assignEvidence(currNode,evVars,evAvg,evStd);
    }    
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
}

void ntNet::printAllMessages(){
  
  printf("\n");
  printf("#################################\n");
  printf("#### Messages In the Network ####\n");
  printf("#################################\n");

  // Propagate messages from nodes to factors
  for(int loopA=0;loopA<nodeList.size();loopA++){
    // Propagate nodes that are not processed
    nodeList[loopA]->printMessages();        
  }

  // Propagate messages from factors to nodes
  for(int loopA=0;loopA<factorList.size();loopA++){
    // Propagate nodes that are not processed
    factorList[loopA]->printMessages();
  }
}

void ntNet::writeAllMessages(){
  for(int loopA=0;loopA<nodeList.size();loopA++){
    for(int loopB=0;loopB<nodeList[loopA]->inMsgs.size();loopB++){
      nodeList[loopA]->inMsgs[loopB]->writeToFile(string("msg_node_" + to_string(nodeList[loopA]->nodeID) + "_from_factor_" + to_string(nodeList[loopA]->inMsgs[loopB]->sourceID) + ".txt"));
    }
  }
  for(int loopA=0;loopA<factorList.size();loopA++){
    for(int loopB=0;loopB<factorList[loopA]->inMsgs.size();loopB++){
      factorList[loopA]->inMsgs[loopB]->writeToFile(string("msg_factor_" + to_string(factorList[loopA]->factorID) + "_from_node_" + to_string(factorList[loopA]->inMsgs[loopB]->sourceID) + ".txt"));
    }
  }
}

// Perform Belief Propagation
int ntNet::runBP(bool init){

  bool sentOK;

  printf("\n");
  printf("--- Running BP Iterations\n");
  printf("\n");

  if(init){
    assignUniformSamplesToLeaves();
  }else{
    reassignRootAndLeafSamplesFromPreviousRun();
  }

  // Initialize messages on root and leaf nodes
  printf("- Initializing Root Factors and Leaf Nodes...\n");
  initMsgsOnRootFactorsLeafNodes();    

  // Check Evidence for this run
  printEvidence();

  // Iterative complete message passing for the other nodes
  bool finished = false;
  int iterCount = 0;  
  while(!finished){

    printf("\n");
    printf("- Running BP Iteration Number %d\n",iterCount+1);
    printf("\n");

    // Propagate messages from nodes to factors
    for(int loopA=0;loopA<nodeList.size();loopA++){
      
      // Check if the node has sent all its messages
      // Propagate nodes that are not processed
      if(!nodeList[loopA]->hasProcessedAllMsgs()){
        printf("\n");
        printf("+ Processing Node %d\n",nodeList[loopA]->nodeID);
        printf("\n");
        nodeList[loopA]->sendMsgToFactors();
        // Print Messages in the whole network
        if(printLevel > 0){
          printAllMessages();  
        }
        
      }      
    }

    // Propagate messages from factors to nodes
    for(int loopA=0;loopA<factorList.size();loopA++){

      // Send Messages from Factors to Nodes
      if(!factorList[loopA]->hasProcessedAllMsgs()){
        printf("\n");
        printf("+ Processing Factor %d\n",factorList[loopA]->factorID);
        printf("\n");
        factorList[loopA]->sendMsgToNodes();
        // Print Messages in the whole network
        if(printLevel > 0){
          printAllMessages();
        }
      }
    }

    // Update finished: is finished when no more nodes or edges are to be processed
    finished = true;

    printf("\n");
    printf("################################\n");
    printf("#### Node and Factor Status ####\n");
    printf("################################\n");

    for(int loopA=0;loopA<nodeList.size();loopA++){
      finished = finished && nodeList[loopA]->hasProcessedAllMsgs();
      if(nodeList[loopA]->hasProcessedAllMsgs()){
        printf("Node %d completed!\n",nodeList[loopA]->nodeID);
      }
    }
    for(int loopA=0;loopA<factorList.size();loopA++){
      finished = finished && factorList[loopA]->hasProcessedAllMsgs();
      if(factorList[loopA]->hasProcessedAllMsgs()){
        printf("Factor %d completed!\n",factorList[loopA]->factorID);
      }      
    }
    printf("################################\n");

    // Update Iteration Count
    iterCount++;
    if(iterCount > 10){
      printf("\n");
      printf("WARNING: Maximum number of BP iterations reached. Exiting BP Loop.\n");
      printf("\n");
      break;
    }
    string pbFile("BP_it_"+to_string(iterCount)+".txt");
    checkFactorGraphMessages(pbFile);

    // TEST: Print Message 
    //nodeList[0]->inMsgs[0]->show();
    //if(iterCount == 2){
    //  exit(-1);
    //}


  }

  // BP Iterations competed: need to aggregate node incoming messages 
  printf("Computing Node Marginals...");
  for(int loopA=0;loopA<nodeList.size();loopA++){
    nodeList[loopA]->computeMarginal();
    // Compute outside
    // nodeMarginal->show();
    nodeList[loopA]->marginal->writeToFile(string("node_" + to_string(nodeList[loopA]->nodeID) + "_marginal.txt"));
  }
  printf("DONE.\n");

  // Print all messages in the converged BP to file
  printf("Export All Messages...");
  writeAllMessages();
  printf("DONE.\n");

  // !!! COMPLETE !!!
  printf("BP COMPLETED!\n");

}

