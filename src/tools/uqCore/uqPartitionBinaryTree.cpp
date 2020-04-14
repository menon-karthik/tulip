# include "uqPartitionBinaryTree.h"
# include "uqSamples.h"
# include "uqException.h"

// Default Constructor
uqPartitionNode::uqPartitionNode(){
  // Init left and right node
  left = NULL;
  right = NULL;
  scalar = 0.0;
  isConverged = false;
  stats[0] = 0.0;
  stats[1] = 0.0;
  hasZeroSamples = false;
  maxSampleIdx = 0;
}

// Other Constructor
uqPartitionNode::uqPartitionNode(uqSamples* sample){

  // Declare
  double currAVG = 0.0;
  double currSTD = 0.0;
  
  // Init left and right node
  left = NULL;
  right = NULL;
  scalar = 0.0;
  isConverged = false;
  stats[0] = 0.0;
  stats[1] = 0.0;  
  double minVal = 0.0;
  double maxVal = 0.0;
  double currVal = 0.0;

  // Set Limits
  limits.clear();
  if(sample->getTotRVS() > 0){
    // Random Variables Defined by Distribution
    for(int loopA=0;loopA<sample->getTotRVS();loopA++){
      switch(sample->getRVS(loopA).type){
        case kSAMPLEUniform:
          limits.push_back(sample->getRVS(loopA).parameter1);
          limits.push_back(sample->getRVS(loopA).parameter2);
          break;
        case kSAMPLEGaussian:
          limits.push_back(sample->getRVS(loopA).parameter1 - 5.0 * sample->getRVS(loopA).parameter2);
          limits.push_back(sample->getRVS(loopA).parameter1 + 5.0 * sample->getRVS(loopA).parameter2);
          break;
        case kSAMPLELognormal:
          limits.push_back(0.0);
          limits.push_back(sample->getRVS(loopA).parameter1 + 5.0 * sample->getRVS(loopA).parameter2);
          break;
      }
    }
  }else{
    // Only Samples Are available: from Table
    // Compute Average and Standard Deviation
    for(int loopA=0;loopA<sample->getTotDims();loopA++){

      minVal = std::numeric_limits<double>::max();
      maxVal = -std::numeric_limits<double>::max();

      // Compute Average, Min and Max Value
      currAVG = 0.0;
      for(int loopB=0;loopB<sample->getTotSamples();loopB++){
        currVal = sample->getValuesAt(loopB,loopA);
        currAVG += currVal;
        if(currVal < minVal){
          minVal = currVal;
        }
        if(currVal > maxVal){
          maxVal = currVal;
        }
      }
      currAVG = currAVG/(double)sample->getTotSamples();
      // Compute Standard Deviation
      currSTD = 0.0;
      for(int loopB=0;loopB<sample->getTotSamples();loopB++){
        currSTD += (sample->getValuesAt(loopB,loopA) - currAVG) * (sample->getValuesAt(loopB,loopA) - currAVG);
      }
      currSTD = sqrt(currSTD/(double)(sample->getTotSamples() - 1));
      // Add to limits
      if(true){
        // Use Min and Max Value
        //limits.push_back(minVal - 0.01*(maxVal-minVal));
        //limits.push_back(maxVal + 0.01*(maxVal-minVal));
        limits.push_back(minVal);
        limits.push_back(maxVal);
      }else{
        // Use Average Value and Standard Deviation 
        limits.push_back(currAVG - 3.0 * currSTD);
        limits.push_back(currAVG + 3.0 * currSTD);
      }
    }
  }
}

// Get Limits
stdVec uqPartitionNode::getLimits(){
  stdVec result;
  for(int loopA=0;loopA<limits.size();loopA++){
    result.push_back(limits[loopA]);
  }
  return result;
}

// Refine partition node
void uqPartitionNode::refine(int dimToRefine){
  
  // Create two new nodes
  uqPartitionNode* newLeftNode = new uqPartitionNode();
  uqPartitionNode* newRightNode = new uqPartitionNode();

  // Resize Limits Array
  newLeftNode->limits.resize(limits.size());
  newRightNode->limits.resize(limits.size());
  
  // Set their limits
  for(int loopA=0;loopA<int(limits.size()/2);loopA++){
    if(loopA != dimToRefine){
      // Set Left node limits
      newLeftNode->limits[2*loopA + 0] = limits[2*loopA + 0];
      newLeftNode->limits[2*loopA + 1] = limits[2*loopA + 1];
      // Set Right node limits
      newRightNode->limits[2*loopA + 0] = limits[2*loopA + 0];
      newRightNode->limits[2*loopA + 1] = limits[2*loopA + 1];
    }else{
      // Set left node limits
      newLeftNode->limits[2*loopA + 0] = limits[2*loopA + 0];
      newLeftNode->limits[2*loopA + 1] = limits[2*loopA + 0] + 0.5*(limits[2*loopA + 1] - limits[2*loopA + 0]);
      // Set right node limits
      newRightNode->limits[2*loopA + 0] = limits[2*loopA + 0] + 0.5*(limits[2*loopA + 1] - limits[2*loopA + 0]);
      newRightNode->limits[2*loopA + 1] = limits[2*loopA + 1];
    }
  }

  // Make the current node to point to them 
  left = newLeftNode;
  right = newRightNode;
}

// Set Chaos Expansion Coefficients
void uqPartitionNode::setChaosCoefficients(stdVec coeffs){
  chaosCoeffs.clear();
  for(int loopA=0;loopA<coeffs.size();loopA++){
    chaosCoeffs.push_back(coeffs[loopA]);
  }
}

// Gather Expansion Coefficients
void uqPartitionNode::getChaosCoefficients(stdVec& coeffs){
  coeffs.clear();
  for(int loopA=0;loopA<chaosCoeffs.size();loopA++){
    coeffs.push_back(chaosCoeffs[loopA]);
  }
}

// Set Partition Error Metrics
void uqPartitionNode::setPartitionErrorMetrics(stdVec values,stdIntVec indexes){
  partErrorMetrics.clear();
  partErrorIdx.clear();
  // Metrics
  for(int loopA=0;loopA<values.size();loopA++){
    partErrorMetrics.push_back(values[loopA]);
  }
  // Indexes
  for(int loopA=0;loopA<indexes.size();loopA++){
    partErrorIdx.push_back(indexes[loopA]);
  }
}

// Get Partition Error Metrics
void uqPartitionNode::getPartitionErrorMetrics(stdVec& values,stdIntVec& indexes){
  values.clear();
  indexes.clear();
  // Metrics
  for(int loopA=0;loopA<partErrorMetrics.size();loopA++){
    values.push_back(partErrorMetrics[loopA]);
  }
  // Indexes
  for(int loopA=0;loopA<partErrorIdx.size();loopA++){
    indexes.push_back(partErrorIdx[loopA]);
  }  
}
 	
// Constructor
uqPartitionBinaryTree::uqPartitionBinaryTree(uqSamples* samples){
  // Get total number of dimensions
  totDims = samples->getTotDims();
  if(totDims == 0){
  	throw new uqException("ERROR: No Dimensions Defined in Samples.\n");
  }
  // Initialize the root node
  root = new uqPartitionNode(samples);
  // Add to allNodes
  allNodes.push_back(root);
}

// ==================================
// Access to the elements of allNodes
// ==================================
uqPartitionNode* uqPartitionBinaryTree::getAllNodes(int index){
  return allNodes[index];
}


// Get number of leaves
int uqPartitionBinaryTree::getLeafCount(){
  int totLeaves = 0;
  uqPartitionNode* currNode;
  for(int loopA=0;loopA<allNodes.size();loopA++){
  	currNode = allNodes[loopA];
    if((currNode->getLeft() == NULL)&&(currNode->getRight() == NULL)){
      totLeaves++;
    }
  }
  // Return
  return totLeaves;
}

// Get number of leaves
stdIntVec uqPartitionBinaryTree::getLeafIndex(){
  stdIntVec leafIndex;
  uqPartitionNode* currNode;
  for(int loopA=0;loopA<allNodes.size();loopA++){
  	currNode = allNodes[loopA];
    if((currNode->getLeft() == NULL)&&(currNode->getRight() == NULL)){
      leafIndex.push_back(loopA);
    }
  }
  // Return
  return leafIndex;
}

// GET MIN SIZE OF PARTITION
double uqPartitionNode::getMinSize(){
  int totDims = limits.size()/2;
  double result = limits[1] - limits[0];
  for(int loopB=0;loopB<totDims;loopB++){
    if((limits[2*loopB + 1] - limits[2*loopB + 0]) < result){
      result = (limits[2*loopB + 1] - limits[2*loopB + 0]);
    }
  }
  return result;
}

// GET MAX SIZE OF PARTITION AND ASSOCIATED DIMENSION
void uqPartitionNode::getMaxSizeAndDimension(double& maxPartSize,int& dimMaxSize){
  int totDims = limits.size()/2;
  maxPartSize = limits[1] - limits[0];
  dimMaxSize = 0;
  for(int loopB=0;loopB<totDims;loopB++){
    if((limits[2*loopB + 1] - limits[2*loopB + 0]) > maxPartSize){
      maxPartSize = (limits[2*loopB + 1] - limits[2*loopB + 0]);
      dimMaxSize = loopB;
    }
  }
}

// GET PARTITION VOLUME
double uqPartitionNode::getVolume(){
  return volume;
}

// SET THE VOLUME OF THE PARTITION
void uqPartitionNode::setVolume(double vol){
  this->volume = vol;
}


// GET PARTITION VOLUME
int uqPartitionNode::countSamplesOnPartition(uqSamples* samples){
  
  int result = 0;
  
  // Check dimension match
  if(samples->getTotDims() != limits.size()/2){
    throw uqException("ERROR in countSamplesOnPartition: Dimensions of partition and samples not compatible.\n");
  }

  // Get Input Samples
  stdMat allSamples;
  samples->getValues(allSamples);
    
  // Loop on existing samples
  bool useCurrSample = false;
  double currValue = 0.0;
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){
    // Init Current Sample
    useCurrSample = true;
    // Loop through the Samples
    for(int loopB=0;loopB<samples->getTotDims();loopB++){
      currValue = allSamples[loopA][loopB];
      // CAREFULL: EVAL REPEATED MODELS ON THE BORDER !!!
      //useCurrSample = (useCurrSample && ((currValue >= limits[2*loopB + 0])&&(currValue <= limits[2*loopB + 1])));
      useCurrSample = (useCurrSample && ((currValue > limits[2*loopB + 0])&&(currValue < limits[2*loopB + 1])));
    }

    // Add samples if it belongs to the domain
    if(useCurrSample){
      result++;
    }
  }
  // Return Values
  return result;
}

// PRINT PARTITION
void uqPartitionBinaryTree::printToScreen(){
  stdVec limits;
  double minSize = 0.0;
  for(int loopA=0;loopA<allNodes.size();loopA++){
    limits = allNodes[loopA]->getLimits();
    minSize = allNodes[loopA]->getMinSize();
    printf("Node %d\n",loopA+1);
    for(int loopB=0;loopB<limits.size()/2;loopB++){
      printf("Limits Dim %d: min %f, max %f\n",loopB+1,limits[2*loopB + 0],limits[2*loopB + 1]);
    }
  }
}

// Print leaf limits to visualize 
void uqPartitionBinaryTree::printLeavesToFile(string outFileName){
  stdVec currentLimits = allNodes[0]->getLimits();
  uqPartitionNode* currNode;
  int totDims = currentLimits.size()/2;
  FILE *f;
  f = fopen(outFileName.c_str(),"w");    
  for(int loopA=0;loopA<allNodes.size();loopA++){
    //printf("Nodo %d\n",loopA);
    currNode = allNodes[loopA];
    currentLimits = currNode->getLimits();
    if((currNode->getLeft() == NULL)&&(currNode->getRight() == NULL)){
      // Print Minima
      for(int loopB=0;loopB<totDims;loopB++){
        fprintf(f,"%f ",currentLimits[2 * loopB + 0]);
        //printf("%f ",currentLimits[2 * loopB + 0]);
        fflush(stdout);
      }
      // Print Maxima
      for(int loopB=0;loopB<totDims;loopB++){
        fprintf(f,"%f ",currentLimits[2 * loopB + 1]);
        //printf("%f ",currentLimits[2 * loopB + 1]);
        fflush(stdout);
      }
      fprintf(f,"%25.10e\n",currNode->getScalar());
    }
  }
  fclose(f);  
  //getchar();
}

// Get the underlying measure: product of marginals in current partition
void uqPartitionNode::getMeasure(stdMat& currMeasure){
  stdVec tmp;
  currMeasure.clear();
  for(int loopA=0;loopA<measure.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<measure[loopA].size();loopB++){
      tmp.push_back(measure[loopA][loopB]);
    }
    currMeasure.push_back(tmp);
  }
}

// Set the underlying measure: product of marginals in current partition
void uqPartitionNode::setMeasure(stdMat currMeasure){
  stdVec tmp;
  measure.clear();
  for(int loopA=0;loopA<currMeasure.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<currMeasure[loopA].size();loopB++){
      tmp.push_back(currMeasure[loopA][loopB]);
    }
    measure.push_back(tmp);
  }
}

// Get the multiIndex stored in the current element
void uqPartitionNode::getMultiIndex(stdIntMat& mi){
  stdIntVec tmp;
  mi.clear();
  for(int loopA=0;loopA<multiIndex.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<multiIndex[loopA].size();loopB++){
      tmp.push_back(multiIndex[loopA][loopB]);
    }
    mi.push_back(tmp);
  }  
}

// Set the multiIndex stored in the current element
void uqPartitionNode::setMultiIndex(stdIntMat mi){
  stdIntVec tmp;
  multiIndex.clear();
  for(int loopA=0;loopA<mi.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<mi[loopA].size();loopB++){
      tmp.push_back(mi[loopA][loopB]);
    }
    multiIndex.push_back(tmp);
  }  
}

// Get total number of multiscaling basis
void uqPartitionNode::getMSTotBasis(int& msBasis){
  msBasis = msTotBasis;
}

// Set total number of multiscaling basis
void uqPartitionNode::setMSTotBasis(int msBasis){
  msTotBasis = msBasis;
}

// Get partition residual norm
double uqPartitionNode::getResidualNorm(){
  return residualNorm;
}

// Set partition residual norm
void uqPartitionNode::setResidualNorm(double resNorm){
  residualNorm = resNorm;
}

bool uqPartitionNode::getHasZeroSamples(){
  return hasZeroSamples;
}

void uqPartitionNode::setHasZeroSamples(bool flag){
  hasZeroSamples = flag;
}

// Get and Set Quadrature Order
void uqPartitionNode::getMWQuadOrder(int& mwQuadOrder){
  mwQuadOrder = this->mwQuadOrder;
}

void uqPartitionNode::setMWQuadOrder(int mwQuadOrder){
  this->mwQuadOrder = mwQuadOrder;
}
