# include "ntNode.h"

using namespace boost::algorithm;

ntNode::ntNode(int nodeIndex,ntNetIO* netIO){

  int numVariables;
  int numSamples;
  int totalInputs;
  stdStringVec varNames;
  stdVec varSTD;
  stdVec limits;
  stdMat varSamples;
  modelTypes detVarType;
  string detModelName;
  stdIntVec subdiv;
  stdIntVec inputSubdiv;
  stdMat cpt;

  stdVec tmp;

  // Init Gaussian sampler
  nSampler = new uqGaussianPDF();
  // Init Uniform sampler
  uSampler = new uqUniformPDF();

  // Assign Node ID
  this->nodeID = netIO->nodeID[nodeIndex];
  // Look at the node Type 
  this->nodeType = netIO->nodeType[nodeIndex];

  // Set the marginal properties
  hasMarginal = false;

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
                                     limits,
                                     subdiv,
                                     cpt);

    // Assign to Node Variable
    this->numVariables = numVariables;
    this->numSamples = numSamples;
    this->varNames = varNames;
    this->varSTD = varSTD;
    this->limits = limits;
    // printf("Limits\n");
    // for(int loopA=0;loopA<numVariables;loopA++){
    //   printf("lb %f, ub %f\n",limits[2*loopA],limits[2*loopA+1]);
    // }
    this->detVarType = detVarType;
    this->detModelName = detModelName;
    this->varSubdiv = subdiv;
    // Store Conditional Probability Matrix
    for(int loopA=0;loopA<cpt.size();loopA++){
      tmp.clear();
      for(int loopB=0;loopB<cpt[loopA].size();loopB++){
        tmp.push_back(cpt[loopA][loopB]);
      }
      this->CPT.push_back(tmp);
    }
    
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
    okToSend = messagesAreReadyFor_2(nodeFactors[loopA]->factorID);

    // Continue to process the message only if all factors are ready
    if(okToSend){

      printf("OK\n");

      // Get Messages from all factors but loopA and aggegate
      vector<ntMessage*> msgs;
      for(int loopB=0;loopB<nodeFactors.size();loopB++){
        if((loopB != loopA)&&(findInMsg(nodeFactors[loopB]->factorID))){
          // Get message from node
          //printf("Gather message from neighbor factor %d\n",nodeFactors[loopB]->factorID);
          currMgs = copyInMsg(nodeFactors[loopB]->factorID);
          // Chenge the message information so it is consistent
          currMgs->sourceID = this->nodeID;
          currMgs->targetID = nodeFactors[loopA]->factorID;
          currMgs->messageType = mtNodeToFactor;
          msgs.push_back(currMgs);

          // printf("Samples in original message\n");
          // for(int loopB=0;loopB<currMgs->msg.size();loopB++){
          //   for(int loopC=0;loopC<currMgs->msg[loopB].size();loopC++){
          //     printf("%f ",currMgs->msg[loopB][loopC]);
          //   }
          //   printf("\n");
          // }

        }
      }

      // If messages are more than 1 then exit
      if(msgs.size() > 1){
        printf("AGGREGATING MULTIPLE MESSAGES AT NODES\n");
      }

      // These are incoming messages for a node, therefore contain the same variable
      // Need to aggregate as you do with marginals
      ntMessage* newMsg = new ntMessage();
      newMsg->aggregateMarginals(msgs);

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

  // Need to add noise to the results as every variable is affected by it
  // Loop throug the samples
  for(int loopA=0;loopA<res.size();loopA++){
    // Loop through the dimensions
    for(int loopB=0;loopB<res[loopA].size();loopB++){
      res[loopA][loopB] += nSampler->sample(0.0, varSTD[loopB]);
    }
  }

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
  bool printDebugMsgs = true;

  if(printDebugMsgs){
    printf("upNames\n");
    for(int loopA=0;loopA<upNames.size();loopA++){
      printf("%s\n",upNames[loopA].c_str());  
    }
    printf("\n");
    printf("msgNames\n");
    for(int loopA=0;loopA<currMsg->varLabels.size();loopA++){
      printf("%s\n",currMsg->varLabels[loopA].c_str());  
    }
    uqSamples* samples = new uqSamples(currMsg->msg);
    samples->printToFile("debug_samples_incomingMsg.txt",true);
  }

  // Create data object
  int columnID = 0;

  // Multiple measurements per result quantity
  bool useSingleColumn = false;
  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
  
  printf("Number of results: %d, Samples: %d, Dimensions: %d\n",int(currMsg->varLabels.size()),int(currMsg->msg.size()),int(currMsg->msg[0].size()));

  // Assign sample to data object
  // Use only a single measurment equal to the mean
  bool useMean = true;
  ((daData_multiple_Table*)data)->assignFromLabelsAndMat(currMsg->varLabels,currMsg->msg,useMean);
  // For inverse analysis you need to compute the standard deviations directly from the message
  // this will allow you to preserve the incoming messages when computing the outcoming one
  stdVec msgSampleSTD = currMsg->getSTDFromSamples();
  ((daData_multiple_Table*)data)->overwriteStandardDeviations(currMsg->varLabels,msgSampleSTD);

  // Assign Data to Model
  ioModel->setData(data);

  // Assign Model Limits based on upstream variables
  stdVec inputLimits = getInputLimits(nodeModel,upNames,upLimits,currMsg);
  ioModel->setParameterLimits(inputLimits);  

  if(printDebugMsgs){
    data->printToScreen();
    data->printUserSTDs();
    printf("This is the model for Node %d\n",nodeID);
    printf("######\n");
    printf("Limits\n");
    for(int loopA=0;loopA<inputLimits.size()/2;loopA++){
      printf("Min: %f, Max: %f\n",inputLimits[loopA*2],inputLimits[loopA*2+1]);
    }
  }

  // Set MCMC parameters
  int totChains           = max(int(currMsg->msg[0].size()), 5); // Min of Number of variables and 3
  int totGenerations      = 5000;
  int totalCR             = 3;
  int totCrossoverPairs   = 5;
  double dreamGRThreshold = 1.2;
  int dreamJumpStep       = 10;
  int dreamGRPrintStep    = 10;

  printf("Total Number of chains used: %d\n",totChains);

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
  stdMat res_filtered;
  stdIntVec resIDX;
  int mcmcSamples = currMsg->msg.size();

  // Subsample and discard the samples that are outside the variable limits
  // cmUtils::subSampleTableData("paramTraces.txt",mcmcSamples,2,2 + nodeModel->getParameterTotal()-1,res,resIDX,inputLimits);
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

  if(printDebugMsgs){
    uqSamples* samples = new uqSamples(res);
    samples->printToFile("debug_samples_outcomingMsg.txt",true);
  }

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
  return res;
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
  // If it's a leaf node you can resend the message you received from the unique factor
  if((nodeFactors.size() == 1)&&(nodeFactors[0]->factorID == factorID)){
    return findInMsg(nodeFactors[0]->factorID);
  }
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

// DEBUG VERSION
bool ntNode::messagesAreReadyFor_2(int factorID){
  // If you find at least one incoming message excluding this factor then send
  int count = 0;
  for(int loopA=0;loopA<nodeFactors.size();loopA++){
    if(nodeFactors[loopA]->factorID != factorID){
      if(findInMsg(nodeFactors[loopA]->factorID)){
        count++;
      }
    }else{
      // Check if the factor already got a message from this node
      if(nodeFactors[loopA]->findInMsg(nodeID)){
        return false;
      }
    }
  }
  return count > 0;
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

void ntNode::computeMarginal(){
  vector<ntMessage*> msgList;
  for(int loopA=0;loopA<inMsgs.size();loopA++){
    msgList.push_back(inMsgs[loopA]);
  }
  marginal = new ntMessage();
  marginal->aggregateMarginals(msgList);
  // marginal->aggregateMarginals_fake(msgList);
}

// Find CPT row which corresponds to the given sample
void ntNode::getPMFSfromCPT(const stdVec& sample,stdMat& pmfs){

  // Print Node CPT
  // printf("Node CPT\n");
  // for(int loopA=0;loopA<CPT.size();loopA++){
  //   for(int loopB=0;loopB<CPT[loopA].size();loopB++){
  //     printf("%f ",CPT[loopA][loopB]);
  //   }
  //   printf("\n");
  // }

  // printf("Sample in getPMFSfromCPT ");
  // for(int loopA=0;loopA<sample.size();loopA++){
  //   printf("%f ",sample[loopA]);
  // }
  // printf("\n");

  stdVec tmpPMF;
  stdVec currState;
  bool found = false;
  int count = 0;
  while((!found)&&(count<CPT.size())){
    // Extract state from CPT row
    currState.clear();
    for(int loopA=0;loopA<totalInputs;loopA++){
      currState.push_back(CPT[count][loopA]);
    }
    // Compare sample and currState componentwise
    found = ntUtils::sameArray(sample,currState);
    if(!found){
      count++;
    }
  }
  if(!found){
    throw ntException("ERROR: Cannot find row in probabilistic node CPT.");
  }else{

    pmfs.clear();
    for(int loopA=0;loopA<numVariables;loopA++){
      tmpPMF.clear();
      for(int loopB=0;loopB<varSubdiv[loopA];loopB++){
        // printf("Count: %d\n",count);
        // printf("Index: %d\n",totalInputs + loopA*varSubdiv[loopA] + loopB);
        // printf("CPT component: %f\n",CPT[count][totalInputs + loopA*varSubdiv[loopA] + loopB]);
        tmpPMF.push_back(CPT[count][totalInputs + loopA*varSubdiv[loopA] + loopB]);

      }
      pmfs.push_back(tmpPMF);
    }
  }
}

ntMessage* ntNode::forwardCPT(ntMessage* currMsg){

  stdMat pmfs;
  stdVec tmp;
  stdVec discrSample;
  double lb = 0.0;
  double ub = 0.0;
  stdMat res;
  stdVec bins;
  int varID = 0;
  double avg = 0.0;

  // Loop through the samples 
  for(int loopA=0;loopA<currMsg->msg.size();loopA++){
    
    // Collect Sample
    tmp.clear();
    // printf("Sample: ");
    for(int loopB=0;loopB<currMsg->msg[loopA].size();loopB++){
      tmp.push_back(currMsg->msg[loopA][loopB]);
      // printf("%f ",currMsg->msg[loopA][loopB]);
    }
    // printf("\n");
    
    // Make sample discrete using input subdivisions
    discrSample.clear();
    for(int loopB=0;loopB<tmp.size();loopB++){
      lb = currMsg->varLimits[2*loopB];
      ub = currMsg->varLimits[2*loopB+1];
      // printf("Bounds: %f %f\n",lb,ub);
      // printf("Subdivs: %d\n",inputSubdiv[loopB]);
      discrSample.push_back(ntUtils::discretizeSample(tmp[loopB],inputSubdiv[loopB],lb,ub));
    }

    // printf("Discrete sample: ");
    // for(int loopB=0;loopB<tmp.size();loopB++){
    //   printf("%f ",discrSample[loopB]);
    // }
    // printf("\n");

    // Find CPT row
    getPMFSfromCPT(discrSample,pmfs);

    // printf("PMF from CPT: ");
    // for(int loopB=0;loopB<pmfs[0].size();loopB++){
    //  printf("%f ",pmfs[0][loopB]);
    // }
    // printf("\n");

    // Generate sample from PMF for each outputs variable
    tmp.clear();
    // printf("BINS\n");
    for(int loopB=0;loopB<pmfs.size();loopB++){
      // Determine the bins for this variable
      lb = currMsg->varLimits[2*loopB];
      ub = currMsg->varLimits[2*loopB+1];
      bins = ntUtils::getBins(inputSubdiv[loopB],lb,ub);
      
      // printf("BINS: ");
      // for(int loopC=0;loopC<bins.size();loopC++){
      //   printf("%f ",bins[loopC]);
      // }
      // printf("\n");

      // Generate Sample From PMF
      tmp.push_back(ntUtils::genSampleFromPMF(bins,pmfs[loopB],1,uSampler)[0]);

      // printf("PMF from CPT: ");
      // for(int loopC=0;loopC<pmfs[loopB].size();loopC++){
      //  printf("%f ",pmfs[loopB][loopC]);
      // }
      // printf("\n");
    }

    // Add sample to outgoing message sample matrix
    res.push_back(tmp);    

  }

  // Create a new message
  ntMessage* resMsg = new ntMessage(currMsg->messageType,currMsg->sourceID,currMsg->targetID,varNames,varSTD,limits,res);

  
  // I DON'T THINK I CAN SIMPLY ADD THE EVIDENCE HERE!!!!
  // // Add evidence to the message if needed
  // if(evidenceVarID.size() > 0){        
  //   // If there is evidence for certain variables, replace these variables with the evidence
  //   for(int loopA=0;loopA<evidenceVarID.size();loopA++){
  //     tmp.clear();
  //     varID = evidenceVarID[loopA];
  //     avg = evidenceVarAvg[loopA];
  //     for(int loopB=0;loopB<resMsg->msg.size();loopB++){
  //       tmp.push_back(avg);
  //     }
  //     resMsg->addEvidence(varID,tmp);
  //   }
  // }  

  // Return message
  return resMsg;
}

int ntNode::findOutputIDX(string output){
  int count = 0;
  bool found = false;
  while((!found)&&(count<varNames.size())){
    found = (output == varNames[count]);
    if(!found){
      count++;
    }
  }
  if(found){
    return count;
  }else{
    throw ntException("ERROR: Cannot find output variable name in ntNode::findOutputIDX.");
  }
}

int ntNode::getoutputColumn(string output, double sample){
  // Find the output number 
  int outIDX = findOutputIDX(output);
  // Sum all discrete states up to outIDX-1
  int colStartIDX = totalInputs;
  for(int loopA=0;loopA<outIDX;loopA++){
    colStartIDX += varSubdiv[loopA];
  }
  // Get limits for the current output
  double lb = limits[2*outIDX];
  double ub = limits[2*outIDX+1];
  // Get the discrete state which corresponds to sample
  colStartIDX += ntUtils::getSampleState(sample, varSubdiv[outIDX],lb,ub);
  // Return index
  return colStartIDX;
}

void ntNode::extractCPTColumnPMFFromSample(const stdVec& sample, const stdStringVec& msgVarNames, const stdBoolVec& isOutCol, const stdIntVec& inColIdx, stdVec& rowIDX, stdVec& pmf){

  stdIntVec inIdx;
  stdVec inVal;

  // Get input index and value
  for(int loopA=0;loopA<sample.size();loopA++){
    if(!isOutCol[loopA]){
      inIdx.push_back(inColIdx[loopA]);
      inVal.push_back(sample[loopA]);
      // printf("Input idx: %d, Sample value: %f\n",inColIdx[loopA],sample[loopA]);
    }
  }

  // Find all row indices in CPT which correspond to inIdx and inVal
  int count = 0;
  bool found = false;
  rowIDX.clear();
  while(count<CPT.size()){
    found = true;
    for(int loopA=0;loopA<inIdx.size();loopA++){
      found = ((found) && (fabs(CPT[count][inIdx[loopA]]-inVal[loopA]) < 1.0e-5));
    }
    if(found){
      rowIDX.push_back(count);
    }
    // Advance search    
    count++;
  }

  // printf("Rows found in the CPT matrix\n");
  // for(int loopA=0;loopA<rowIDX.size();loopA++){
  //   printf("%d\n",int(rowIDX[loopA]));
  // }

  // For every row make a list by multipying the column value for each output
  double prod = 1.0;
  int discrIdx = 0;
  pmf.clear();
  for(int loopA=0;loopA<rowIDX.size();loopA++){
    prod = 1.0;
    for(int loopB=0;loopB<sample.size();loopB++){
      if(isOutCol[loopB]){
        // Determine the columns of this output
        discrIdx = getoutputColumn(msgVarNames[loopB],sample[loopB]);        
        // Multiply its value
        prod *= CPT[rowIDX[loopA]][discrIdx];  
      }      
    }
    // printf("Prod: %f\n",prod);
    pmf.push_back(prod);
  }

  // Normalize PMF
  double sum = 0.0;
  for(int loopA=0;loopA<pmf.size();loopA++){
    sum += pmf[loopA];
  }
  for(int loopA=0;loopA<pmf.size();loopA++){
    pmf[loopA] = pmf[loopA]/sum;
  }

  // printf("Resulting PMF\n");
  // for(int loopA=0;loopA<pmf.size();loopA++){
  //   printf("%f\n",pmf[loopA]);
  // }
}

bool ntNode::isNodeVariableName(string name){
  int count = 0;
  bool found = false;
  while((!found)&&(count<varNames.size())){
    found = (name == varNames[count]);
    if(!found){
      count++;
    }
  }
  return found;
}

int ntNode::getNodeInputVariableNameIdx(string name){
  int count = 0;
  bool found = false;
  while((!found)&&(count<inputNames.size())){
    found = (name == inputNames[count]);
    if(!found){
      count++;
    }
  }
  return count;
}

void ntNode::prepareIO(ntMessage* currMsg, stdBoolVec& isOutCol, stdIntVec& inColIdx){
  string currVarName;
  // Get a boolean vector if a variable is an output
  isOutCol.clear();
  for(int loopA=0;loopA<currMsg->varLabels.size();loopA++){
    currVarName = currMsg->varLabels[loopA];
    isOutCol.push_back(isNodeVariableName(currVarName));
  }
  // Find the indices of the input variables
  inColIdx.clear();
  for(int loopA=0;loopA<currMsg->varLabels.size();loopA++){
    currVarName = currMsg->varLabels[loopA];
    inColIdx.push_back(getNodeInputVariableNameIdx(currVarName));
  }
}

void getMsgTargetVarIDX(const stdStringVec& upNames, const stdStringVec& inputNames, stdIntVec& msgTargetVarIDX){
  msgTargetVarIDX.clear();
  for(int loopA=0;loopA<inputNames.size();loopA++){
    // See if you can find it in upNames
    if(ntUtils::isInStringVec(inputNames[loopA],upNames)){
      msgTargetVarIDX.push_back(loopA);
    }
  }
}

ntMessage* ntNode::InverseCPT(const stdStringVec& upNames,
                              const stdVec& upSTD,
                              const stdVec& upLimits,
                              ntMessage* currMsg){

  int dim1 = 0;
  int dim2 = 0;
  int cptRow = 0;
  double prod = 1.0;
  double sumPMF = 0.0;
  double lb = 0.0;
  double ub = 0.0;
  stdVec sample;
  stdVec discrSample;
  stdVec tmpCol;
  stdVec pmf;
  stdVec bins;
  stdVec tmp;
  stdMat cptCols;
  stdMat res;

  stdBoolVec isOutCol;
  stdIntVec inColIdx;
  stdVec rowIDX;

  int varID = 0;
  double avg = 0.0;

  //currMsg->show();  

  printf("\n");

  // Analyze the incoming message and odentify inputs/outputs
  prepareIO(currMsg, isOutCol, inColIdx);

  // Loop through the samples 
  for(int loopA=0;loopA<currMsg->msg.size();loopA++){

    // Collect Sample
    sample.clear();
    for(int loopB=0;loopB<currMsg->msg[loopA].size();loopB++){
      sample.push_back(currMsg->msg[loopA][loopB]);
    }
    
    // Make sample discrete using input subdivisions
    discrSample.clear();
    //printf("Sample Inverse discr.\n");
    for(int loopB=0;loopB<sample.size();loopB++){
      lb = currMsg->varLimits[2*loopB];
      ub = currMsg->varLimits[2*loopB+1];
      //printf("Bounds: %f %f\n",lb,ub);
      //printf("Subdivs: %d\n",inputSubdiv[loopB]);
      discrSample.push_back(ntUtils::discretizeSample(sample[loopB],inputSubdiv[loopB],lb,ub));
      //printf("Sample: %f, Discrete sample: %f\n",sample[loopB],discrSample[loopB]);
    }
    //printf("\n");

    // Extract CPT columns corresponding to each sample
    extractCPTColumnPMFFromSample(discrSample, currMsg->varLabels, isOutCol, inColIdx, rowIDX, pmf);
  
    // Sample from PMF and return corresponding input state
    cptRow = int(ntUtils::genSampleFromPMF(rowIDX,pmf,1,uSampler)[0]);
    // Find index of target ID variable on node inputs
    stdIntVec msgTargetVarIDX;
    getMsgTargetVarIDX(upNames,inputNames,msgTargetVarIDX);
    // Add samples from message target variable
    tmp.clear();
    for(int loopB=0;loopB<msgTargetVarIDX.size();loopB++){
      // printf("Selected Up Value: %f\n",CPT[cptRow][msgTargetVarIDX[loopB]]);
      tmp.push_back(CPT[cptRow][msgTargetVarIDX[loopB]]);
    }
    res.push_back(tmp);
  }

  // Create outgoing message
  ntMessage* resMsg = new ntMessage(currMsg->messageType,currMsg->sourceID,currMsg->targetID,upNames,upSTD,upLimits,res);


  // if(upNames[0] == string("rainy")){
  //   printf("ECCOLO!!!!\n");
  //   exit(-1);  
  // }


  // Return message
  return resMsg;  
}


