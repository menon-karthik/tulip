# include "ntNode.h"

using namespace boost::algorithm;

ntNode::ntNode(int nodeIndex,ntNetIO* netIO){

  int numVariables;
  int numSamples;
  stdStringVec varNames;
  stdVec varSTD;
  stdVec limits;
  stdMat varSamples;
  modelTypes detVarType;
  string detModelName;

  // Init Gaussian sampler
  uqPDF* nSampler = new uqGaussianPDF();
  // Init Uniform sampler
  uqPDF* uSampler = new uqUniformPDF();

  // Assign Node ID
  this->nodeID = netIO->nodeID[nodeIndex];
  // Look at the node Type 
  this->nodeType = netIO->nodeType[nodeIndex];

  // Read Node Information based on type
  if(this->nodeType == ntRoot){

    netIO->readRootNodeFile(netIO->nodeFile[nodeIndex],
                            numVariables,
                            numSamples,
                            varNames,
                            varSTD,
                            limits,
                            varSamples);

    // Assign to Node Variable
    this->numVariables = numVariables;
    this->numSamples = numSamples;
    this->varNames = varNames;
    this->varSTD = varSTD;
    this->limits = limits;
    this->varSamples = varSamples;

  }else if(this->nodeType == ntDeterministic){

    netIO->readDeterministicNodeFile(netIO->nodeFile[nodeIndex],
                                     numVariables,
                                     numSamples,
                                     varNames,
                                     varSTD,
                                     limits,
                                     detVarType,
                                     detModelName);
    // Assign to Node Variable
    this->numVariables = numVariables;
    this->numSamples = numSamples;
    this->varNames = varNames;
    this->varSTD = varSTD;
    this->limits = limits;
    this->detVarType = detVarType;
    this->detModelName = detModelName;

    // Assign Node Model and retrieve limits
    cmModel* mdl;
    if(detVarType == mtModel){
      // These are just the Darpa model, other can be added
      if(to_upper_copy(detModelName) == string("DARPASIMPLECODE1")){

        mdl = new cm_darpaSimple_code1();

      }else if(to_upper_copy(detModelName) == string("DARPASIMPLECODE2")){

        mdl = new cm_darpaSimple_code2();

      }else if(to_upper_copy(detModelName) == string("DARPASIMPLECODE3")){

        mdl = new cm_darpaSimple_code3();

      }else{
        throw ntException("ERROR: Model not supported in ntNode Constructor.");
      }

    }else if(detVarType == mtApproximant){
        
      mdl = new cmApproximant(detModelName);

    }else{
      throw ntException("ERROR: Invalid node model type in ntNode Constructor.");
    }
    
    // Add model to node
    nodeModel = mdl;

  }else if(this->nodeType == ntProbabilistic){

    netIO->readProbabilisticNodeFile(netIO->nodeFile[nodeIndex],
                                     numVariables,
                                     numSamples,
                                     varNames,
                                     varSTD,
                                     detVarType,
                                     detModelName);

  }else{
    throw ntException("ERROR: Invalid node type in ntNode Constructor.");
  }
}

ntNode::~ntNode(){
  delete nSampler;
  delete uSampler;
}

ntMessage* ntNode::copyInMsg(int factorID){
  bool found = false;
  int count = 0;
  while(!found){
    found = (inMsgs[count]->sourceID == factorID) && (inMsgs[count]->targetID == this->nodeID);
    if(!found){
      count += 1;  
    }
  }
  // Return the sample table
  ntMessage* newMsg = new ntMessage(inMsgs[count]);
  return newMsg;
}

// Check if the node has evidence
bool ntNode::hasEvidence(){
  if(evidenceVarID.size() > 0){
    return true;
  }else{
    return false;
  }
}

void ntNode::appendToNodeFactors(const vector<ntFactor*>& factors,const stdBoolVec& isDownFactor){
  if(factors.size() != isDownFactor.size()){
    throw ntException("ERROR: Invalid factor list in appendToNodeFactors.\n");
  }
  // Append nodes
  for(size_t loopA=0;loopA<factors.size();loopA++){
    this->nodeFactors.push_back(factors[loopA]);
  }
  // Append downstream nodes
  for(size_t loopA=0;loopA<isDownFactor.size();loopA++){
    this->isDownstreamFactor.push_back(isDownFactor[loopA]);
  }
}

// Propagate messages from nodes to factors
void ntNode::sendMsgToFactors(){

  int    varID      = 0;
  double avg        = 0.0;
  double std        = 0.0;
  double noise      = 0.0;
  bool   okToSend   = true;
  ntMessage* currMgs;

  printf("++ Sending message from node %d\n",this->nodeID);

  // Loop on all factor connected to the node
  for(int loopA=0;loopA<nodeFactors.size();loopA++){

    printf("Sending to factor %d...",nodeFactors[loopA]->factorID);

    // Check all other factors are ready
    okToSend = messagesAreReadyFor(nodeFactors[loopA]->factorID);

    // Continue to process the message only if all factors are ready
    if(okToSend){

      printf("OK\n");

      // Get Messages from all factors but loopA and aggegate
      vector<ntMessage*> msgs;
      for(int loopB=0;loopB<nodeFactors.size();loopB++){
        if(loopB != loopA){
          // Get message from node
          //printf("Gather message from neighbor factor %d\n",nodeFactors[loopB]->factorID);
          currMgs = copyInMsg(nodeFactors[loopB]->factorID);
          // Chenge the message information so it is consistent
          currMgs->sourceID = this->nodeID;
          currMgs->targetID = nodeFactors[loopA]->factorID;
          currMgs->messageType = mtNodeToFactor;
          msgs.push_back(currMgs);
        }
      }

      // Create a New Message by aggregating the other
      ntMessage* newMsg = new ntMessage();
      newMsg->aggregateFromList(msgs);

      // Add evidence to the message if specified
      stdVec tmp;
      if(evidenceVarID.size() > 0){        
        // If there is evidence for certain variables, replace these variables with the evidence
        for(int loopA=0;loopA<evidenceVarID.size();loopA++){
          tmp.clear();
          varID = evidenceVarID[loopA];
          avg = evidenceVarAvg[loopA];
          std = evidenceVarStd[loopA];
          for(int loopB=0;loopB<newMsg->msg.size();loopB++){
            tmp.push_back(avg + nSampler->sample(0.0,std));
          }
          newMsg->addEvidence(varID,tmp);
        }
      }
      // Update Message
      nodeFactors[loopA]->updateMsg(nodeID,newMsg);
    }else{
      printf("NO\n");
    }
  }
}

void ntNode::updateMsg(int factorID,ntMessage* currMsg){
  
  bool found = false;
  int count = 0;

  while((!found)&&(count<inMsgs.size())){
    found  = (inMsgs[count]->messageType == mtFactorToNode)&&(inMsgs[count]->sourceID == factorID);
    if(!found){
      count++;
    }
  }
  
  if(!found){
    inMsgs.push_back(currMsg);
  }else{
    delete inMsgs[count];
    inMsgs[count] = currMsg;
  }
}

stdIntVec matchInputID_First(cmModel* model,const stdStringVec& upNames){
  stdIntVec res;
  string currVarName;
  for(int loopA=0;loopA<model->getParameterTotal();loopA++){
    currVarName = model->getParamName(loopA);
    for(int loopB=0;loopB<upNames.size();loopB++){
      if(currVarName == upNames[loopB]){
        res.push_back(loopB);
      }
    }
  }
  if(res.size() == 0){
    throw ntException("ERROR: No model input found in getUpInputID.\n");
  }
  return res;
}

stdIntVec matchInputID_Second(cmModel* model,const stdStringVec& upNames){
  stdIntVec res;
  string currVarName;
  for(int loopA=0;loopA<upNames.size();loopA++){
    for(int loopB=0;loopB<model->getParameterTotal();loopB++){
      currVarName = model->getParamName(loopB);
      if(upNames[loopA] == currVarName){
        res.push_back(loopB);
      }
    }
  }
  if(res.size() == 0){
    throw ntException("ERROR: No model input found in getUpInputID.\n");
  }
  return res;
}


stdMat extractColumns(const stdIntVec& upID,const stdMat& mat){
  stdMat res;
  stdVec tmp;
  for(int loopA=0;loopA<mat.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<upID.size();loopB++){
      tmp.push_back(mat[loopA][upID[loopB]]);
    }
    res.push_back(tmp);
  }
  return res;
}

ntMessage* ntNode::forwardUQ(ntMessage* currMsg){

  // Get Column index for the sub-set of the inputs you need to propagate upwards
  stdIntVec upID = matchInputID_First(nodeModel,currMsg->varLabels);
  // Extract Columns according to IDs
  stdMat msgMat = extractColumns(upID,currMsg->msg);

  // Create Samples
  uqSamples* samples = new uqSamples(msgMat);

  // Initialize 
  acActionUP_MC* mc = new acActionUP_MC(samples);
  mc->setModel(nodeModel);
  mc->opts.numberOfRepeats = 1;
  mc->opts.sampleGroups.clear();
  mc->opts.sampleGroups.push_back(currMsg->msg.size());
  mc->opts.storeSamples = true;

  // Process Model
  mc->go();
  // Return resulting samples
  stdMat res = mc->all_outputs;
  // Get the labels, stds and limits from node
  ntMessage* resMsg = new ntMessage(currMsg->messageType,currMsg->sourceID,currMsg->targetID,varNames,varSTD,limits,res);
  // Free memory
  delete mc;
  delete currMsg;
  // Return
  return resMsg;
}

stdVec getInputLimits(cmModel* model,
                      const stdStringVec& upNames,
                      const stdVec& upLimits,
                      ntMessage* currMsg){
  stdVec res;
  string currVarName;
  for(int loopA=0;loopA<model->getParameterTotal();loopA++){
    currVarName = model->getParamName(loopA);
    printf("currVarName: %s\n",currVarName.c_str());

    // Look in the upVariables
    for(int loopB=0;loopB<upNames.size();loopB++){
      //printf("upNames: %s\n",upNames[loopB].c_str());      
      if(currVarName == upNames[loopB]){
        res.push_back(upLimits[loopB*2]);
        res.push_back(upLimits[loopB*2+1]);
      }
    }

    // Look in the message variables
    for(int loopB=0;loopB<currMsg->varLabels.size();loopB++){
      //printf("currMsg->varLabels: %s\n",currMsg->varLabels[loopB].c_str());      
      if(currVarName == currMsg->varLabels[loopB]){
        res.push_back(currMsg->varLimits[loopB*2]);
        res.push_back(currMsg->varLimits[loopB*2+1]);
      }
    }
  }
  return res;
}

ntMessage* ntNode::inverseUQ(const stdStringVec& upNames,
                             const stdVec& upSTD,
                             const stdVec& upLimits,
                             ntMessage* currMsg){

  // Create a New Model that returns both the inputs and the outputs
  cmModel* ioModel = new cmCombineInputOutput(nodeModel);

  int showNode = 3;

  // if(nodeID == showNode){
  //   printf("upNames\n");
  //   for(int loopA=0;loopA<upNames.size();loopA++){
  //     printf("%s\n",upNames[loopA].c_str());  
  //   }
  //   printf("\n");
  //   printf("msgNames\n");
  //   for(int loopA=0;loopA<currMsg->varLabels.size();loopA++){
  //     printf("%s\n",currMsg->varLabels[loopA].c_str());  
  //   }
  //   uqSamples* samples = new uqSamples(currMsg->msg);
  //   samples->printToFile("debug_samples_out.txt",true);
  // }

  // Create data object
  int columnID = 0;
  // Multiple measurements per result quantity
  bool useSingleColumn = false;
  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
  
  printf("Number of results: %d, Samples: %d, Dimensions: %d\n",int(currMsg->varLabels.size()),int(currMsg->msg.size()),int(currMsg->msg[0].size()));

  // Assign sample to data object
  ((daData_multiple_Table*)data)->assignFromLabelsAndMat(currMsg->varLabels,currMsg->msg);
  ((daData_multiple_Table*)data)->overwriteStandardDeviations(currMsg->varLabels,currMsg->varSTDs);

  // if(nodeID == showNode){
  //   data->printToScreen();
  // }


  //data->printUserSTDs();
  //printf("This is the model for Node %d\n",nodeID);

  // Assign Data to Model
  ioModel->setData(data);

  // Assign Model Limits based on upstream variables
  stdVec inputLimits = getInputLimits(nodeModel,upNames,upLimits,currMsg);
  printf("Limits\n");
  for(int loopA=0;loopA<inputLimits.size()/2;loopA++){
    printf("Min: %f, Max: %f\n",inputLimits[loopA*2],inputLimits[loopA*2+1]);
  }
  ioModel->setParameterLimits(inputLimits);

  // Set MCMC parameters
  int totChains           = max(int(currMsg->msg[0].size()), 5); // Min of Number of variables and 3
  int totGenerations      = 1000;
  int totalCR             = 3;
  int totCrossoverPairs   = 5;
  double dreamGRThreshold = 1.2;
  int dreamJumpStep       = 10;
  int dreamGRPrintStep    = 10;

  string dreamChainFileName("chain_GR_000000.txt");
  string dreamGRFileName("gr_GR.txt");
  
  string dreamRestartReadFileName("");
  string dreamRestartWriteFileName("restart_write_GR.txt");

  bool usePriorFromFile = false;
  string priorFileName("");
  int priorModelType = 0;

  // Create Dream Sampler
  acActionDREAMseq* mcmc = new acActionDREAMseq(totChains,totGenerations,totalCR,totCrossoverPairs,
                                                dreamChainFileName,dreamGRFileName,dreamGRThreshold,
                                                dreamJumpStep,dreamGRPrintStep,
                                                dreamRestartReadFileName,dreamRestartWriteFileName,
                                                usePriorFromFile, priorFileName, priorModelType);
  mcmc->setModel(ioModel);
  // Run MCMC Simulation
  mcmc->go();

  // Post process chains
  bool debugMode = false;
  double burnInPercent = 50.0;
  mcmc->postProcess(debugMode,burnInPercent);
  
  // Extract Parameter Samples
  stdMat res;
  stdIntVec resIDX;
  int mcmcSamples = currMsg->msg.size();
  cmUtils::subSampleTableData("paramTraces.txt",mcmcSamples,2,2 + nodeModel->getParameterTotal()-1,res,resIDX);
  if(mcmcSamples < currMsg->msg.size()){
    throw ntException("ERROR: Not enough samples from MCMC.");
  }

  // Get Column index for the sub-set of the inputs you need to propagate upwards
  stdIntVec upID = matchInputID_Second(nodeModel,upNames);
  // Extract Columns according to IDs
  stdMat msgMat = extractColumns(upID,res);
  // Create the message 
  ntMessage* resMsg = new ntMessage(currMsg->messageType,currMsg->sourceID,currMsg->targetID,upNames,upSTD,upLimits,msgMat);

  // if(nodeID == showNode){
  //   uqSamples* samples = new uqSamples(msgMat);
  //   samples->printToFile("debug_samples_in.txt",true);
  //   exit(-1);
  // }

  // Free memory
  delete mcmc;
  delete currMsg;
  delete ioModel;
  // Return Message
  return resMsg;
}

bool ntNode::varIsUnobserved(int varID){
  bool res = false;
  for(int loopA=0;loopA<evidenceVarID.size();loopA++){
    res = res && (varID == evidenceVarID[loopA]);
  }
  return !res;
}

stdMat ntNode::uniformSampleUnobserved(const stdMat& msg){
  stdMat res = msg;
  double currMin = 0.0;
  double currMax = 0.0;

  for(int loopA=0;loopA<msg[0].size();loopA++){
    if(varIsUnobserved(loopA)){
      
      // Get Limits
      currMin = limits[2*loopA + 0];
      currMax = limits[2*loopA + 1];

      for(int loopB=0;loopB<msg.size();loopB++){
        res[loopB][loopA] = uSampler->sample(currMin,currMax);
      }
    }
  }
}

bool ntNode::findInMsg(int factorID){
  bool found = false;
  int count = 0;
  while((!found) && (count<inMsgs.size())){
    found = (inMsgs[count]->sourceID == factorID) &&(inMsgs[count]->targetID == nodeID);
    if(!found){
      count++;
    }
  }
  return found;  
}

// Check that all messages to factors have been sent
bool ntNode::hasProcessedAllMsgs(){
  bool foundAll = true;
  for(int loopA=0;loopA<nodeFactors.size();loopA++){
    foundAll = foundAll && nodeFactors[loopA]->findInMsg(nodeID);
  }
  return foundAll;
}

bool ntNode::messagesAreReadyFor(int factorID){
  bool msgReady = true;
  for(int loopA=0;loopA<nodeFactors.size();loopA++){
    if(nodeFactors[loopA]->factorID != factorID){
      msgReady = msgReady && findInMsg(nodeFactors[loopA]->factorID);  
    }else{
      msgReady = msgReady && (!nodeFactors[loopA]->findInMsg(nodeID));
    }
  }
  return msgReady;
}

void ntNode::printMessages(){
  if(inMsgs.size()>0){
    printf("%-7s %-18s %-10s %-10s %-10s %-10s\n","Node","Type","SourceID","TargetID","n. Samples","n. Dims");  
  }  
  for(int loopA=0;loopA<inMsgs.size();loopA++){
    printf("%-7d %-18s %-10d %-10d %-10d %-10d\n",nodeID,
                                                inMsgs[loopA]->getMsgTypeString().c_str(),
                                                inMsgs[loopA]->sourceID,
                                                inMsgs[loopA]->targetID,
                                                int(inMsgs[loopA]->msg.size()),
                                                int (inMsgs[loopA]->msg[0].size()));
  }
}

ntMessage* ntNode::computeMarginal(){
  vector<ntMessage*> msgList;
  for(int loopA=0;loopA<inMsgs.size();loopA++){
    msgList.push_back(inMsgs[loopA]);
  }
  ntMessage* marginal = new ntMessage();
  marginal->aggregateMarginals(msgList);
  return marginal;
}

