# include "acActionUP_PC.h"

acActionUP_PC::acActionUP_PC(uqSamples* in, uqSamples* out, int order, int polyType, int completeOrderType){
  // Number of samples should be the same 
  if(in->getTotSamples() != out->getTotSamples()){
    throw acException("ERROR: Different number of input and output samples.");
  }
  inputs = new uqSamples(in);
  // Store sample limits
  inputs->getSampleLimits(limits);
  if(out != NULL){
    outputs = new uqSamples(out);  
  }else{
  	outputs = NULL;
  }
  this->order = order;
  this->polyType = polyType;
  this->completeOrderType = completeOrderType;

  //for(int loopA=0;loopA<inputs->getTotSamples();loopA++){
  //	for(int loopB=0;loopB<inputs->getTotDims();loopB++){
  //    printf("%f ",inputs->getValuesAt(loopA,loopB));
  //	}
  //	printf("\n");
  //}

}

acActionUP_PC::~acActionUP_PC(){
  delete inputs;
  delete outputs;
}

// Construct stochastic approximation or evaluate samples
int acActionUP_PC::go(){

  uqMatrix* mat;
  uqAlgorithmBCS* bcs;

  //  If there are no outputs need to compute from model
  if(outputs == NULL){
  	stdMat modelOutputs;
  	stdVec tempOutputs;
  	stdIntVec errorCode;
  	double ll;
  	for(int loopA=0;loopA<inputs->getTotSamples();loopA++){
  	  ll = model->evalModelError(inputs->getSample(loopA),tempOutputs,errorCode);
  	  modelOutputs.push_back(tempOutputs);
  	}
  	outputs = new uqSamples(modelOutputs);  
  }

  // Scale the samples using the limits found in the constructor
  inputs->rescaleOnHypercube(limits);

  // Loop over all the output quantities
  for(int loopA=0;loopA<outputs->getTotDims();loopA++){

    // Construct the measurement matrix
    mat = new uqPolyMatrix(inputs,order,polyType,completeOrderType);

    // Compute the coefficient using BCS
    stdVec tempCoeffs;
    stdMat coeffsCov;
    double noiseEstimate;
    uqAlgorithmBCS* bcs = new uqAlgorithmBCS();
    stdVec rhs = outputs->getColumn(loopA);
    stdMat matrix = mat->getMatrix();
    bcs->run(mat->getRowCount(),
  	         mat->getColCount(),
  	         rhs,
  	         matrix,
             tempCoeffs,
             coeffsCov,
             noiseEstimate);

    // Store coefficient representation
    coeffs.push_back(tempCoeffs);

    // free memory
    delete mat;
    delete bcs;
  }
  return 0;
}

// Get Approximant
uqApproximant* acActionUP_PC::getApproximant(){
  // Create Approximant
  uqApproximant* approx = new uqApproximant_SE(atPoly,polyType,order,coeffs,limits);
  return approx;
}

// Get Statistics
stdMat acActionUP_PC::getStatistics(){
  // Compute the Statistics through the coefficients
  stdMat res;
  stdVec tempStats;
  double currAvg = 0.0;
  double currStd = 0.0;
  for(int loopA=0;loopA<coeffs.size();loopA++){
  	tempStats.clear();
  	// Compute Average: first coefficients
  	currAvg = coeffs[loopA][0];
  	// Compute Std: sum of the squares of 1+ coeffcients
  	currStd = 0.0;
  	for(int loopB=1;loopB<coeffs.size();loopB++){
  	  currStd += coeffs[loopA][loopB]*coeffs[loopA][loopB];
  	}
  	currStd = sqrt(currStd);
  	// Add to temorary vector
  	tempStats.push_back(currAvg);
  	tempStats.push_back(currStd);
  	res.push_back(tempStats);
  }
  return res;
}
