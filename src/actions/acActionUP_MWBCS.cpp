# include "acActionUP_MWBCS.h"

acActionUP_MWBCS::~acActionUP_MWBCS(){
}

void performMWOrthogonalityTest(uqSamples* grid, uqMatrix* mwMat, stdMat measure){

  // Get Maximum Weight Order
  int maxOrder = grid->getMaxWeightOrder();

  // Assumes the matrix is evaluated at a grid of integration points
  double integral    = 0.0;
  double firstBasis  = 0.0;
  double secondBasis = 0.0;
  double currMeasure = 0.0;
  double intWeigth   = 0.0;
  int    currIDX     = 0;
  int    testIDX     = 0;
  printf("\n");
  printf("MW ORTHOGONALITY TEST\n");
  printf("Number of Basis: %d\n",mwMat->getColCount());
  printf("Number of Samples: %d\n",mwMat->getRowCount());
  for(int loopA=0;loopA<mwMat->getColCount();loopA++){    
    for(int loopB=0;loopB<mwMat->getColCount();loopB++){
      integral = 0.0; 
      for(int loopC=0;loopC<mwMat->getRowCount();loopC++){
        firstBasis  = mwMat->getMatrixAt(loopC,loopA);
        secondBasis = mwMat->getMatrixAt(loopC,loopB);
        intWeigth   = grid->getWeightAt(loopC,maxOrder);
        currMeasure = 1.0;
        currIDX = loopC;        
        for(int loopD=0;loopD<grid->getTotDims();loopD++){
          testIDX = currIDX % grid->getTotSamples();
          currMeasure *= measure[loopD][testIDX];
          currIDX = currIDX / grid->getTotSamples();
        }
        integral += firstBasis * secondBasis * currMeasure * intWeigth;
      }
      printf("%15.6e ",integral);
    }
    printf("\n");
  }
  printf("\n");
}

string getTimeStamp(){
  std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
  return string(to_string(ms.count()));
}

// ===================
// SET DEFAULT OPTIONS
// ===================
void acActionUP_MWBCS::setDefaultOptions(){
  
  // Set Default Values for options
  opts.numIniSamples   = 100;
  opts.doRefineSpace   = false;
  opts.doRefineSamples = false;
  
  opts.inputMode           = imRandom;
  opts.samplingType        = stHalton;
  opts.tableInputFileName  = "";
  opts.tableOutputFileName = "";

  // Boundary Points
  opts.addBoundaryPoints = false;
  opts.boundaryPointOrder = 0;

  opts.maxMWOrder   = 2;
  opts.maxGridOrder = 2;
  opts.mwQuadOrder  = 30;
  opts.outputColumn = 0;

  opts.thresholdSize         = 0.01;
  opts.minSamplesInPartition = 10;
  opts.maxPartitionSizeRatio = 1.0;
  opts.maxMWVarianceRatio    = 1.0e-5;

  // Ways to specify a probability measure
  opts.measureInputMode = miUnit;
  opts.measureInputFileName = "";

  // Beta factors for arbitrary dependent inputs
  // Default is with Beta Factors equal to 1.0
  opts.useBetaFactors = false;
  opts.readBetaFactorsFromFile = false;
  opts.betaFactorFileName = "";

  // Option for VTK node value extraction
  opts.vtkNodeToExtract  = 0;

  opts.forceRVMSolution  = false;

  opts.resToleranceRatio = 0.1;

  // Do not computed error metrics by default
  opts.computeErrorMetrics = false;

  // Init Global Statistics
  globalAVG = 0.0;
  globalSTD = 0.0;

  // File Printing
  opts.printPartitionTree      = false;
  opts.printInputOutputSamples = false;
  opts.printPartitionSamples   = false;
  opts.printAndEvalSurrogate   = false;
  opts.printPartitionMonitor   = false;
  opts.printOverallStats       = false;
  opts.simulPrefix             = "";

  // Message printing
  opts.printProgress = true;
  opts.printDBGMessages = false;
}

// ===========
// CONSTRUCTOR
// ===========
acActionUP_MWBCS::acActionUP_MWBCS(uqSamples* locInputs,uqSamples* locOutputs): acActionUP(locInputs,locOutputs){
  // Set Default Values for options
  setDefaultOptions();
}

// =========================================
// ALTERNATIVE CONSTRUCTOR FOR ARBITRARY 
// PROBABILITY MEASURES AND DEPENDENT INPUTS
// COMING FROM MCMC SIMULATIONS
// =========================================
acActionUP_MWBCS::acActionUP_MWBCS(uqSamples* locInputs,uqSamples* locOutputs,stdVec locBetaFactors,vector<uq1DApproximant*> locMarginals): acActionUP(locInputs,locOutputs){
  
  // ASSIGN BETA COEFFICIENTS
  betaFactors.clear();
  for(int loopA=0;loopA<locBetaFactors.size();loopA++){
    betaFactors.push_back(locBetaFactors[loopA]);
  }
  
  // ASSIGN MARGINALS
  marginalMeasures.clear();
  for(int loopA=0;loopA<locMarginals.size();loopA++){
    uq1DApproximant* marginal = locMarginals[loopA];
    marginalMeasures.push_back(marginal);
  }

  // Set Default Options
  setDefaultOptions();

  // Set appropriate options for dependent
  // inputs with arbitrary marginals from MCMC
  opts.doRefineSpace = true;
  
  // Set input mode
  opts.inputMode = imFromMCMC;

  // Ways to specify a probability measure
  opts.measureInputMode = miFromBetaMarginals;

  // Beta factors for arbitrary dependent inputs
  // Default is with Beta Factors equal to 1.0
  opts.useBetaFactors = true;
  opts.readBetaFactorsFromFile = false;
}

// ========================================
// CHECK MULTI-INDEX FOR DIRECT SENSITIVITY
// ONLY ON MULTIWAVELET BASIS
// ========================================
bool hasOnlyIndex(int index,stdIntVec multiIndex){
  // ALL DIMENSIONS BUT INDEX WITH ZEROS
  bool result = true;
  for(size_t loopA=0;loopA<multiIndex.size();loopA++){
    if((int)loopA != index){
      result = (result && (multiIndex[loopA] == 0));
    }
  }
  // CHECK LAST CONDITION THAT INDEX SHOULD BE NOT ZERO
  // This conditions should not be considered as zero index 
  // multiwavelets are non constant functions!!!
  // result = (result && (multiIndex[index] != 0));
  // Return
  return result;
}

// ========================================
// ALL OTHER INDEXES ARE MULTISCALING ZEROS
// ========================================
bool allOthersAreMultiscalingZeros(int index,stdIntVec multiIndex,int mwDim){
  // ALL DIMENSIONS BUT INDEX WITH ZEROS
  bool result = true;
  for(size_t loopA=0;loopA<multiIndex.size();loopA++){
    if((int)loopA != index){
      result = (result && (multiIndex[loopA] == 0));
    }
  }
  // Careful if you are in a multiwavelet basis, 0 may be a valid multiwavelet instead
  // of the constant function
  if(mwDim > 0){
    result = (result && (index == (mwDim-1)));
  }
  return result;
}

// ========================================
// ALL OTHER INDEXES ARE MULTISCALING ZEROS
// ========================================
int getMWDimension(int basisID,int msTotBasis){
  return basisID/(int)msTotBasis;
}

// INIT FILE WITH GLOBAL STATISTICS
void initPartitionMonitorFile(string prefix){
  FILE *f;
  f = fopen(string(prefix + "partitionMonitor.dat").c_str(),"w");    
  fprintf(f,"%15s %15s %15s %15s %15s %15s %15s %15s %15s %15s\n","Iteration","Partition","MS Variance","MW Variance", "Refined", "Dimension", "Description", "Min", "Max","Volume");
  fclose(f);
}

// INIT FILE WITH GLOBAL STATISTICS
void saveToPartitionMonitorFile(string prefix,int itCount,int partNum,double msVar,double mwVar, bool doRef,int refDim, const char* desc,double min, double max,double volume){
  FILE *f;
  f = fopen(string(prefix + "partitionMonitor.dat").c_str(),"a");    
  fprintf(f,"%15d %15d %15e %15e %15s %15d %15s %15f %15f %15f\n",itCount,partNum,msVar,mwVar,doRef ? "true" : "false",refDim,desc,min,max,volume);
  fclose(f);
}

// ================================================================
// CHECK IF THE PARTITION NEEDS TO BE REFINED AND IN WHAT DIRECTION
// ================================================================
bool evalPartitionRefinement(int itCount, int partNumber, stdVec limits,int partTotSamples,
                             int totDims, uqMWMatrix* mwMat, 
                             stdVec coeffs, stdMat coeffPrec, double resNorm,
                             double minSize, double maxPartSize, int dimMaxSize, double maxPartitionSizeRatio,
                             double thresholdSize, double partitionVolume, int minSamplesInPartition,
                             bool printPartitionMonitor,double resToleranceRatio,string simulPrefix, bool doRefineSamples,
                             bool printDBGMessages,
                             int &dimToRefine, double& metric, bool& needToAddSamples, 
                             double& constTerm,double& msVariance,double& mwVariance, double maxMWVarianceRatio,
                             double& currPartAV,double& currPartSD){

  //printf("final threshold %f\n",thresholdSize);

  // Init
  bool doRefine = true;
  needToAddSamples = false;
  
  // Allocate the contribution of separate dimensions
  stdVec mwVarianceSingleDim;
  mwVarianceSingleDim.resize(totDims);
  for(int loopA=0;loopA<totDims;loopA++){
    mwVarianceSingleDim[loopA] = 0.0;
  }

  // ==============================
  // COMPUTE VARIANCE DECOMPOSITION
  // ==============================
  msVariance = 0.0;
  mwVariance = 0.0;
  constTerm = coeffs[0];
  double mwRatio = 0.0;  
  // Start From One to Neglect the Constant Function Coefficient
  for(size_t loopA=1;loopA<coeffs.size();loopA++){
    if(mwMat->isMW[loopA]){
      for(int loopB=0;loopB<totDims;loopB++){
        // Eval Direct Sensitivities on Multiwavelets
        if(hasOnlyIndex(loopB,mwMat->multiIndex[loopA])){
          mwVarianceSingleDim[loopB] += coeffs[loopA] * coeffs[loopA];
        }      
      }
      // Multiwavelet
      mwVariance += coeffs[loopA] * coeffs[loopA];
    }else{
      // Multiscaling 
      msVariance += coeffs[loopA] * coeffs[loopA];
    }
  }
  if(printDBGMessages){
    printf("msVar %f, mwVar %f\n",msVariance,mwVariance);  
  }
  
  double totVariance = msVariance + mwVariance; 
  if(fabs(totVariance) > 0.0e0){
    mwRatio = mwVariance/(double)totVariance;
  }else{
    mwRatio = 0.0;
  }

  // VARIOUS METRICS TO CHECK MW CONTRIBUTION
  //metric = mwVariance * partitionVolume;
  metric = mwRatio * partitionVolume;
  //metric = mwVariance;

  // Write partition metric and statistics  
  currPartAV = coeffs[0];
  currPartSD = sqrt(totVariance);

  // NOT ENOUGH SAMPLES
  if(partTotSamples < minSamplesInPartition){
    // Need to add samples but not to refine the partition...
    doRefine = false;
    if(doRefineSamples){
      needToAddSamples = true;
    }else{
      needToAddSamples = false;
    }
    if(printPartitionMonitor){
      saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,msVariance,mwVariance,doRefine,0,"Too Few Samples",limits[0],limits[1],partitionVolume);
    }
    return doRefine;
  }

  // ELEMENT IS TOO SMALL - DO NOT REFINE
  if(minSize <= thresholdSize){
    doRefine = false;
    if(printPartitionMonitor){
      saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,0.0,0.0,doRefine,0,"Small Part",limits[0],limits[1],partitionVolume);      
    }
    return doRefine;
  }

  // ASPECT RATIO IS TOO LARGE - REFINE
  double partAspect = maxPartSize/minSize;
  if(partAspect > maxPartitionSizeRatio){
    doRefine = true;
    dimToRefine = dimMaxSize;
    if(printPartitionMonitor){
      saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,msVariance,mwVariance,doRefine,0,"Large Aspect",limits[0],limits[1],partitionVolume);
    }
    return doRefine;
  }
  
  // Set Variance Ratio
  //if(fabs(totVariance)<kMathZero){
  //  doRefine = false;
  //  if(printPartitionMonitor){
  //    saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,0.0,0.0,doRefine,0,"Zero Var",limits[0],limits[1],partitionVolume);
  //  }
  //  metric = 0.0;
  //  return doRefine;
  //}
  
  // Eval Variance for every subdivision term
  stdVec dirVariance;  

  // Check the dimension to refine 
  //printf("mwVarianceVolume: %f, maxMWVarRatio: %f\n",mwVariance * partitionVolume,maxMWVarianceRatio);
  //printf("resNorm: %f, resNormTolerance: %f\n",resNorm,currPartSD * resToleranceRatio);
  //getchar();
  
  // if((mwVariance * partitionVolume < maxMWVarianceRatio)&&(resNorm < currPartSD * resToleranceRatio)){
  //if(mwVariance / partitionVolume < maxMWVarianceRatio){
  if(metric < maxMWVarianceRatio){
    // The contribution of the Wavelet basis is small
    doRefine = false;
    if(printPartitionMonitor){
      saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,msVariance,mwVariance,doRefine,0,"Small MWVar",limits[0],limits[1],partitionVolume);
    }
    return doRefine;

  }else{      
      
    // Eval the variances for every direction
    dimToRefine = 0;
    double maxSens = 0.0;
    for(int loopA=0;loopA<totDims;loopA++){
      if(mwVarianceSingleDim[loopA]/mwVariance > maxSens){
        maxSens = mwVarianceSingleDim[loopA]/mwVariance;
        dimToRefine = loopA;
      }
    }      
    // Refine Partition
    doRefine = true;  
  }

  // Case of negligible residual
  //if(resNorm < kMathZero){
  //  // You do not need to refine anymore  
  //  // The residual norm is very small
  //  doRefine = false;
  //  if(printPartitionMonitor){
  //    saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,msVariance,mwVariance,doRefine,0,"Small Norm",limits[0],limits[1],partitionVolume);
  //  }
  //  return doRefine;
  //}

  // Print to partition monitor file
  if(printPartitionMonitor){
    saveToPartitionMonitorFile(simulPrefix,itCount,partNumber,msVariance,mwVariance,doRefine,dimToRefine,"OK",limits[0],limits[1],partitionVolume);
  }

  // Result
  return doRefine;
}

// INIT CURRENT PARTITION FILE
void initSurrogateSampleFile(string prefix, int step){
  FILE *f;
  string fileName(prefix + "surrogate_" + to_string(step) + ".dat");
  f = fopen(fileName.c_str(),"w");    
  fclose(f);
}

// PRINT CURRENT PARTITION SAMPLES
void appendToSurrogateSampleFile(string prefix, int step, uqSamples* samples,stdVec rhs){
  FILE *f;
  string fileName(prefix + "surrogate_" + to_string(step) + ".dat");
  f = fopen(fileName.c_str(),"a");    
  // Print Basis Matrix
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){
    for(int loopB=0;loopB<samples->getTotDims();loopB++){
      fprintf(f,"%f ",samples->getValuesAt(loopA,loopB));  
    }
    fprintf(f,"%f\n",rhs[loopA]);
  }
  fclose(f);
}

// PRINT CURRENT PARTITION SAMPLES
void initCurrentPartitionSampleFile(string fileName){
  FILE *f;
  f = fopen(fileName.c_str(),"w");
  fclose(f);
}

// PRINT CURRENT PARTITION SAMPLES
void printCurrentPartitionSamples(string fileName, uqSamples* samples,stdVec rhs){
  FILE *f;
  f = fopen(fileName.c_str(),"a");    
  // Print Basis Matrix
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){
    for(int loopB=0;loopB<samples->getTotDims();loopB++){
      fprintf(f,"%f ",samples->getValuesAt(loopA,loopB));  
    }
    fprintf(f,"%f\n",rhs[loopA]);
  }
  fclose(f);
}

// ==============================================
// EVAL AND PRINT SURROGATE FOR CURRENT PARTITION
// ==============================================
void printAndEvalSurrogate(string simulPrefix,int itCount,int maxOrder,int dims,stdVec coeffs,
                           stdVec partLimits,int totMWIntPoints,stdMat measure){

  // Build multiwavelet approximant on current partition
  bool addLegendrePoly = true;
  bool addMW = true;
  bool useBinPartitions = true;
  int mwMatType = kMWFixedMaxDetailLevel;
  int maxDetailLevel = 0;
  bool mwMatIncludeNullColumns = true;
  bool useExactMW = false;
  int maxColumns = 0;

  // Create Samples on Regular Grid in [0,1]^2
  uqSamples* reSamples = new uqSamples(dims);
  int gridOrder = 2;
  int gridType = kRegular;
  int rangeType = kHaarRange;
  reSamples->generateCartesianGrid(gridOrder,gridType,rangeType);    

  // Resample Multiwavelet Matrix
  uqMWMatrix* mwResampledMat = new uqMWMatrix(maxOrder,reSamples,
                                              addLegendrePoly,addMW,useBinPartitions,
                                              mwMatType,mwMatIncludeNullColumns,
                                              useExactMW,totMWIntPoints,measure,
                                              maxColumns,maxDetailLevel);

  // Set To Zero the multiwavelet Coefficients
  if(false){
    for(int loopA=0;loopA<coeffs.size();loopA++){
      if(mwResampledMat->isMW[loopA]){
        coeffs[loopA] = 0.0;  
      }    
    }
  }

  // Find Surrogate Measurements
  stdVec surrogateVals = mwResampledMat->applyTo(coeffs);

  // Scale Back to original Size before saving
  reSamples->rescaleFromHypercubeToPartition(partLimits);

  // Print Values
  appendToSurrogateSampleFile(simulPrefix,itCount,reSamples,surrogateVals);
}

// ==============================================
// EVALUATE SURROGATE ERRORS AT CURRENT PARTITION
// ==============================================
void evalSurrogateError(uqSamples* checkPointInputs,uqSamples* checkPointOutputs,
                        int maxOrder,stdVec coeffs,stdVec partLimits,int totMWIntPoints,stdMat measure,int outputColumn,
                        stdVec& returnErrors, stdIntVec& sampleIndexes){

  // Set MW Approximant Options
  bool addLegendrePoly = true;
  bool addMW = true;
  bool useBinPartitions = true;
  int mwMatType = kMWFixedMaxDetailLevel;
  int maxDetailLevel = 0;
  bool mwMatIncludeNullColumns = true;
  bool useExactMW = false;
  int maxColumns = 0;
  stdVec trueOutputs;

  // SubSample Error Locations
  uqSamples* currPartCheckPointInputs  = new uqSamples();        
  uqSamples* currPartCheckPointOutputs = new uqSamples();        

  // Extract Samples for Current Partition
  currPartCheckPointInputs->extractPartitionSamples(checkPointInputs,partLimits,sampleIndexes);

  // Evaluate Errors Only if there are checkpoint samples
  if(currPartCheckPointInputs->getTotSamples() > 0){
    // Rescale Samples on Hypercube
    currPartCheckPointInputs->rescaleOnHypercube(partLimits);

    // Get Corresponding Outputs
    checkPointOutputs->extractSampleColumnWithIndex(sampleIndexes,outputColumn,trueOutputs);

    // Resample Multiwavelet Matrix
    uqMWMatrix* mwResampledMat = new uqMWMatrix(maxOrder,currPartCheckPointInputs,
                                                addLegendrePoly,addMW,useBinPartitions,
                                                mwMatType,mwMatIncludeNullColumns,
                                                useExactMW,totMWIntPoints,measure,
                                                maxColumns,maxDetailLevel);

    // Find Surrogate Measurements
    stdVec surrogateVals = mwResampledMat->applyTo(coeffs);

    // Evaluate Error Vector
    returnErrors.clear();
    for(int loopA=0;loopA<sampleIndexes.size();loopA++){
      returnErrors.push_back(trueOutputs[loopA] - surrogateVals[loopA]);
    }
    //getchar();
    // Free Matrix
    delete mwResampledMat;
  }
  // Free Quantities
  delete currPartCheckPointInputs;
  delete currPartCheckPointOutputs;
}

// GET VECTOR l2 NORM
double getVectorTwoNorm(stdVec rhs){
  double result = 0.0;
  for(size_t loopA=0;loopA<rhs.size();loopA++){
    result += rhs[loopA] * rhs[loopA];
  }
  return sqrt(result);
}

// GET ERROR l2 NORM
double getErrorTwoNorm(stdVec rhs){
  double result = 0.0;
  for(size_t loopA=0;loopA<rhs.size();loopA++){
    result += rhs[loopA] * rhs[loopA];
  }
  return sqrt(result/(double)rhs.size());
}

// GET ERROR l1 NORM
double getErrorOneNorm(stdVec rhs){
  double result = 0.0;
  for(int loopA=0;loopA<rhs.size();loopA++){
    result += fabs(rhs[loopA]);
  }
  return result/(double)rhs.size();
}

// GET ERROR INF NORM
double getErrorInfNorm(stdVec rhs){
  double result = 0.0;
  for(size_t loopA=0;loopA<rhs.size();loopA++){
    if(fabs(rhs[loopA]) > result){
      result = fabs(rhs[loopA]);
    }
  }
  return result;
}

// EVAL GLOBAL STATISTICS
void evalGlobalStatistics(stdVec partAV,stdVec partSD, stdVec partVolume, stdIntVec partSampleCount,int& totSamples,double& totAV,double& totSD){
  // Evaluate Global Average
  totAV = 0.0;
  totSamples = 0;
  double totVol = 0.0;
  for(int loopA=0;loopA<partAV.size();loopA++){
    totVol += partVolume[loopA];
    totAV += partVolume[loopA] * partAV[loopA];
    totSamples += partSampleCount[loopA];
  }
  totAV = totAV/totVol; 
  if(false){
    for(int loopA=0;loopA<partAV.size();loopA++){
      printf("VOLUME OF PARTITION %d: %f\n",loopA,partVolume[loopA]);    
    }
    printf("TOTAL VOLUME OF THE PARTITIONS: %f\n",totVol);
    //getchar();
  }
  // Evaluate Global Standard Deviation
  double totVariance = 0.0;
  for(int loopA=0;loopA<partSD.size();loopA++){
    totVariance += (partSD[loopA] * partSD[loopA] + (partAV[loopA] - totAV) * (partAV[loopA] - totAV)) * partVolume[loopA];
  }
  // THIS SHOULD WORK...
  //totSD = sqrt(totVariance);
  totSD = sqrt(totVariance/totVol);
}

// INIT FILE WITH GLOBAL STATISTICS
void initOverallStatistics(string prefix){
  FILE *f;
  f = fopen(string(prefix + "globalStatistics.dat").c_str(),"w");    
  fprintf(f,"%15s %15s %25s %25s\n","Iteration","Samples","AV","SD");
  fclose(f);
}

// INIT FILE WITH ERROR METRIC
void initErrorMetricFile(string prefix){
  FILE *f;
  f = fopen(string(prefix + "errorMetric.dat").c_str(),"w");    
  fprintf(f,"%15s %15s %15s %15s %15s\n","Iteration","Samples","l1","l2","lInf");
  fclose(f);
}

// INIT FILE WITH ERROR METRIC
void saveErrorMetricFile(string prefix,int itCount,int totSamples,double l2Norm,double l1Norm,double lInfNorm){
  FILE *f;
  f = fopen(string(prefix + "errorMetric.dat").c_str(),"a");    
  fprintf(f,"%15d %15d %25.10e %25.10e %25.10e\n",itCount,totSamples,l1Norm,l2Norm,lInfNorm);
  fclose(f);
}

// SAVE STATICTICS TO FILE
void saveOverallStatistics(string prefix,int itCount,int totSamples,double totAV,double totSD){
  FILE *f;
  f = fopen(string(prefix + "globalStatistics.dat").c_str(),"a");
  fprintf(f,"%15d %15d %25.10e %25.10e\n",itCount,totSamples,totAV,totSD);
  fclose(f);
}

// INIT FILE WITH VOLUME INFO
void initVolumeInfo(string prefix){
  FILE *f;
  f = fopen(string(prefix + "volumeInfo.dat").c_str(),"w");    
  fprintf(f,"%15s %15s %15s\n","Iteration","Samples","Volume");
  fclose(f);
}

// SAVE VOLUME INFO
void saveVolumeInfo(string prefix,int itCount,int totSamples,double volume){
  FILE *f;
  f = fopen(string(prefix + "volumeInfo.dat").c_str(),"a");
  fprintf(f,"%15d %15d %15f\n",itCount,totSamples,volume);
  fclose(f);  
}


// =============================
// SCALE 1D SAMPLES TO PARTITION
// =============================
void scale1DGridOnPartition(uqSamples* points,double lb,double ub,stdVec& outLocs){
  double sampleValue = 0.0;
  for(int loopA=0;loopA<points->getTotSamples();loopA++){
    sampleValue = points->getValuesAt(loopA,0);
    outLocs[loopA] = lb + sampleValue * (ub-lb);
  }
}

// ======================
// PERFORM ADAPTIVE MW UP
// ======================
int acActionUP_MWBCS::go(){
  
  // Initialize Results
  int result = 0;

  // Clear Statistics
  stats.clear();

  // Init model outputs and errorCodes
  stdMat     modelRes;
  stdMat     newValues;
  stdIntVec  errorCodes;
  stdVec     modelErrors;
  stdVec     rhs;
  stdVec     tempVec;
  stdIntVec  sampleIndexes; 
  stdVec     measureLocations;
  stdVec     returnErrorMetrics;
  stdVec     surrogateErrors;
  stdBoolVec errorWasComputed;
  stdIntVec  errorMetricSampleIdX;

  // Grid Parameters
  int gridOrder = 0;
  int gridType = 0;
  int rangeType = 0;
  int rootQuasiRandomID = 0;

  // INIT
  if(opts.printDBGMessages){
    printf("\n");
    printf("--- INITIALIZATION\n");
  }

  // Create inputs/outputs if not defined  
  if(inputs == NULL){
    inputs = new uqSamples();   
  }
  if(outputs == NULL){
    outputs = new uqSamples(1);   
  }

  // Read Or Generate Results
  if(opts.inputMode == imTable){
    
    // Read From File
    if(inputs != NULL){
      // In table file name is empty 
      if(!opts.tableInputFileName.empty()){
        bool skipFirstLine = false;
        inputs->readFromFile(opts.tableInputFileName,skipFirstLine);  
        // Confirm that Input File has been read
        if(opts.printDBGMessages){
          printf("Input table file %s read.\n",opts.tableInputFileName.c_str());
          fflush(stdout);
        }
      }
    }
        
    // Read From File
    if(outputs != NULL){
      if(!opts.tableOutputFileName.empty()){
        bool skipFirstLine = false;
        outputs->readFromFile(opts.tableOutputFileName,skipFirstLine);
        // Confirm that Output File has been read
        if(opts.printDBGMessages){
          printf("Output table file %s read.\n",opts.tableOutputFileName.c_str());
          fflush(stdout);
        }      
      }
    }
    
  }else if(opts.inputMode == imRandom){
    // Generate the Results
    // Start with uniform random sampling
    if(inputs == NULL){
      throw acException("ERROR: Uninitialized Input Variables.\n");
    }else{
      if(opts.samplingType == stRandom){
        inputs->generateRandomSamples(opts.numIniSamples);          
        //inputs->printToFile(string("test_Inputs_Gauss.txt"),false);
        //getchar();
      }else if(opts.samplingType == stHalton){
        inputs->addQuasiRandomSamplesFromPartition(opts.numIniSamples,rsHalton,false,tempVec,tempVec,rootQuasiRandomID);  
      }
    }

    // Evaluate Results
    if(opts.printDBGMessages){
      printf("Evaluating Model...");
      fflush(stdout);
    }
    modelRes.clear();
    errorCodes.clear();
    inputs->getValues(newValues);
    modelErrors = model->evalModelError(newValues,modelRes,errorCodes);      
    if(opts.printDBGMessages){
      printf("Done.\n");
    }

    // Create Output Samples From results
    for(int loopA=0;loopA<modelRes.size();loopA++){
      tempVec.clear();
      tempVec.push_back(modelRes[loopA][0]);
      outputs->addOneSample(tempVec);
    }
  
  }else if(opts.inputMode == imGrid){   

    // Create Sample for quadrature points 
    gridOrder = opts.maxGridOrder;
    gridType = kDoubleCC; //kRegular kCC kDoubleCC
    rangeType = kHaarRange;
    inputs->generateCartesianGrid(gridOrder,gridType,rangeType);

    // Evaluate Results with SG Nodes    
    if(opts.printDBGMessages){
      printf("Evaluating Model...");
      fflush(stdout);
    }
    modelRes.clear();
    errorCodes.clear();
    inputs->getValues(newValues);
    modelErrors = model->evalModelError(newValues,modelRes,errorCodes);
    if(opts.printDBGMessages){
      printf("Done.\n");  
    }
    
    // Create Output Samples From results
    for(int loopA=0;loopA<modelRes.size();loopA++){
      tempVec.clear();
      tempVec.push_back(modelRes[loopA][0]);
      outputs->addOneSample(tempVec);
    }

  }else if(opts.inputMode == imCVSimVTK){
    // Numerical Simulations stored in VTK format
    // Create Inputs
    stdMat existingInputs;
    stdVec tempExisting;
    for(int loopA=0;loopA<inputs->getTotSamples();loopA++){
      tempExisting.clear();
      tempExisting.push_back(loopA);
      tempExisting.push_back(opts.vtkNodeToExtract);
      existingInputs.push_back(tempExisting);
    }

    // Evaluate Model Solutions
    if(opts.printDBGMessages){
      printf("Evaluating Model for Existing Samples...");
      fflush(stdout);
    }
    modelRes.clear();
    errorCodes.clear();
    if(true){
      modelErrors = model->evalModelError(existingInputs,modelRes,errorCodes);  
    }else{
      stdMat tmpSamples;
      inputs->getValues(tmpSamples);
      modelErrors = model->evalModelError(tmpSamples,modelRes,errorCodes);
    }
    
    if(opts.printDBGMessages){
      printf("Done.\n");
    }

    // Create Output Samples From results
    //printf("Outputs\n");
    for(int loopA=0;loopA<modelRes.size();loopA++){
      tempVec.clear();
      tempVec.push_back(modelRes[loopA][0]);
      //printf("%f ",modelRes[loopA][0]);
      outputs->addOneSample(tempVec);
    }
    //printf("\n");
  }    

  // Add Boundary Points if Required
  // CAREFUL: WORKS IN 2D. NEED TO GENERALIZE TO ARBITRARY DIMENSIONS
  if(opts.addBoundaryPoints){
    // Get limits for the current partition
    inputs->addPointsOnBoundary(opts.boundaryPointOrder);
    // Add Zero Outputs
    for(int loopA=0;loopA<inputs->getTotDims()*2*opts.boundaryPointOrder;loopA++){
      tempVec.clear();
      tempVec.push_back(0.0);
      outputs->addOneSample(tempVec);
    }
  }

  // Need to set the points where to evaluate the error metrics
  // And to evaluate the corresponding model solutions
  uqSamples* checkPointInputs;
  uqSamples* checkPointOutputs;
  if(opts.computeErrorMetrics){
    
    // Create New Random Samples with copy constructor
    checkPointInputs  = new uqSamples(inputs);
    checkPointOutputs = new uqSamples(1);
    
    // Generate Random Samples
    checkPointInputs->generateRandomSamples(1000,5);
    modelRes.clear();
    errorCodes.clear();
    checkPointInputs->getValues(newValues);
    modelErrors = model->evalModelError(newValues,modelRes,errorCodes);      
    
    // Create Output Samples From results
    for(int loopA=0;loopA<modelRes.size();loopA++){
      tempVec.clear();
      tempVec.push_back(modelRes[loopA][0]);
      checkPointOutputs->addOneSample(tempVec);
    }

    // Print To File
    checkPointInputs->printToFile(string(opts.simulPrefix + "checkPointsInput.txt"),false);
    checkPointOutputs->printToFile(string(opts.simulPrefix + "checkPointsOutput.txt"),false);

    // Allocate surrogateErrors
    surrogateErrors.resize(1000);
    returnErrorMetrics.resize(1000);
    errorWasComputed.resize(1000);
  }

  // Initialize the partition tree associated to this reconstruction
  partitionTree = new uqPartitionBinaryTree(inputs);
  
  // Set Current Partition
  uqPartitionNode* currPartition;  

  // Set the maximum number of samples fro quasi-Random Sequence
  if((opts.inputMode == imRandom)&&(opts.samplingType == stHalton)){
    partitionTree->getAllNodes(0)->setQuasiRandomSampleId(rootQuasiRandomID);
  } 

  // =======================================================
  // INITIALIZE PROBABILITY MEASURE BUT DO NOT ASSIGN VALUES
  // =======================================================
  stdMat measure;
  uqSamples* measure1DGridPoints;
  int totMWIntPoints = 0;
  // Create a 1D integration grid in [0,1]
  measure1DGridPoints = new uqSamples();
  measure1DGridPoints->addVariable("grid1D",kSAMPLEUniform,0.0,1.0);
  measure1DGridPoints->generateCartesianGrid(opts.mwQuadOrder,kDoubleCC,kHaarRange);
  // Allocate 1D locations for arbitrary measure
  measureLocations.resize(measure1DGridPoints->getTotSamples());
  totMWIntPoints = measure1DGridPoints->getTotSamples();    
  // Allocate the probability Measure
  int numberOfDimensions = inputs->getTotDims();
  measure.resize(inputs->getTotDims());
  for(int loopA=0;loopA<inputs->getTotDims();loopA++){
    measure[loopA].resize(totMWIntPoints);
  }

  // ===========================================
  // BETA FACTORS FOR ARBITRARY DEPENDENT INPUTS
  // ===========================================
  // This handles arbitrary distributed and dependent samples  
  if(((opts.inputMode == imTable)||(opts.inputMode == imCVSimVTK))&&(opts.useBetaFactors)){

    // Check if the size is compatible with the available model outputs
    if(betaFactors.size() != outputs->getTotSamples()){
      throw acException("ERROR: Number of Beta Coefficients is different from the available output samples.\n");
    }

    // Multiply the outputs by the Beta factor    
    outputs->multiplyByScalar(betaFactors);
  }
  
  // Start Adaptation Loop
  bool finished = false;
  int itCount = 0;
  int totPartitions = 0;
  stdIntVec leafIndex;
  double bcsReturn = 0.0;
  int currLeafIndex = 0;
  stdVec currLimits;
  double modelError = 0.0;
  int totAddedSamples = 0;
  int totSamples = 0;
  double partVarianceMetric = 0.0;
  double totAV = 0.0;
  double totSD = 0.0;
  double currPartAV = 0.0;
  double currPartSD = 0.0;
  double currPartIntMeasure = 0.0;
  double dimIntMeasure = 0.0;
  double dimSize = 0.0;
  double marginalAVG = 0.0;
  double marginalSTD = 0.0;

  // Init multiwavelet matrix options
  bool addLegendrePoly = true;
  bool addMW = true;
  bool useBinPartitions = true;
  //bool addMW = false;
  int  mwMatType = kMWFixedMaxDetailLevel;
  int  maxDetailLevel = 0;
  bool mwMatIncludeNullColumns = false;
  bool useExactMW = false;
  int  maxColumns = 0;
  int partTotSamples = 0;
  int partDims = 0;
  double constTerm = 0.0;
  double msVariance = 0.0;
  double mwVariance = 0.0;
  double coeffNorm = 0.0; 
  double currVolume = 0.0;

  // Error Metrics
  double errorTwoNorm = 0.0;
  double errorOneNorm = 0.0;
  double errorInfNorm = 0.0;
 
  // Additional Samples
  bool samplesAreNeeded;
  uqSamples* AddedSamples;

  // Coefficients and representations
  stdVec coeffs;
  stdMat coeffPrec;
  stdVec gridRhs;
  double resNorm = 0.0;
  double currCoeff = 0.0;
  double currCoeffWeight = 0.0;

  bool doRefine = false;
  int dimToRefine = -1;
  double rhsNorm = 0.0;
  // Max and Min size, dim and Volume
  double minPartSize = 0.0;
  double minRootPartitionSize = 0.0;
  double partitionVolume = 0.0;
  double partArea = 0.0;
  double maxPartSize = 0.0;
  int dimMaxSize = 0;
  double tempConst = 0.0;

  stdVec partAV;
  stdVec partSD;
  stdVec partVolume;
  stdIntVec partSampleCount;

  bool areThereRefinedPartitions = false;  
  bool needToAddSamples = false;

  // Global partition information
  double avgVarianceRatio = 0.0;
  double maxRedisualNorm = 0.0;
  double currResNorm = 0.0;

  // Init File With Overall Statistics
  if(opts.printOverallStats){
    initOverallStatistics(opts.simulPrefix);  
    initVolumeInfo(opts.simulPrefix);
  }
  
  if(opts.printPartitionMonitor){
    initPartitionMonitorFile(opts.simulPrefix);
  }

  if(opts.computeErrorMetrics){
    initErrorMetricFile(opts.simulPrefix); 
  }


  // Print Current Iteration Header
  if(opts.printProgress){
    printf("%15s %15s %15s %15s %15s %15s %15s\n","IT","Partitions","AVG Var Ratio","Max Residual","AVG","STD","Volume");
  }
  
  // Approximation Loop
  while(!finished){
    
    // Update Iteration Count
    itCount++;
    if(opts.printDBGMessages){
      printf("\n");
      printf("--- CURRENT ITERATION: %d\n",itCount);
    }
    if(opts.printAndEvalSurrogate){
      initSurrogateSampleFile(opts.simulPrefix,itCount);  
    }

    // Reset Error Computation Bookkeeper
    if(opts.computeErrorMetrics){
      for(int loopA=0;loopA<errorWasComputed.size();loopA++){
        errorWasComputed[loopA] = false;
      }
      for(int loopA=0;loopA<surrogateErrors.size();loopA++){
        surrogateErrors[loopA] = 0.0;
      }      
    }
    
    // Read Or Generate Results
    if((opts.inputMode != imTable)&&(opts.inputMode != imCVSimVTK)&&(opts.inputMode != imFromMCMC)&&(totAddedSamples > 0)){

      if(opts.printDBGMessages){
        printf("Added samples: %d, dims %d...\n",AddedSamples->getTotSamples(),AddedSamples->getTotDims());
        fflush(stdout);
      }

      // Create Matrix of New Inputs
      AddedSamples->getValues(newValues);

      //printf("New Values Size %d\n",(int)newValues.size());

      // Add Samples to inputs
      for(int loopA=0;loopA<newValues.size();loopA++){
        tempVec.clear();
        for(int loopB=0;loopB<newValues[loopA].size();loopB++){
          tempVec.push_back(newValues[loopA][loopB]);
        }        
        inputs->addOneSample(tempVec);
      }
      
      // Evaluate Response Function for Additional Samples
      // Evaluate Results
      if(opts.printDBGMessages){
        printf("Evaluating Model...");
        fflush(stdout);
      }
      modelRes.clear();
      errorCodes.clear();
      modelErrors = model->evalModelError(newValues,modelRes,errorCodes);      

      if(opts.printDBGMessages){
        printf("Done.\n");
      }

      // Add To Output Samples
      for(int loopA=0;loopA<modelRes.size();loopA++){
        tempVec.clear();
        tempVec.push_back(modelRes[loopA][0]);
        outputs->addOneSample(tempVec);
      }

      // Delete Additional Samples
      if(AddedSamples != NULL){
        delete AddedSamples; 
        AddedSamples = NULL;
      }      
    }  

    // Print Partition Tree
    if(opts.printPartitionTree){
      string partLeavesOutput(opts.simulPrefix + "part_Leaves_" + to_string(itCount) + ".dat");
      partitionTree->printLeavesToFile(partLeavesOutput);    
    }

    // Write Samples To File
    if(opts.printInputOutputSamples){
      string inputSamplesFileName(opts.simulPrefix + "inputSamples.dat");
      inputs->printToFile(inputSamplesFileName,false);
      string outputSamplesFileName(opts.simulPrefix + "outputSamples.dat");
      outputs->printToFile(outputSamplesFileName,false);
    }

    // Get total partitions
    totPartitions = partitionTree->getLeafCount();
    
    if(opts.printDBGMessages){
      printf("Number of partitions: %d\n",totPartitions);    
    }
    
    // Get list with the partition limits
    leafIndex = partitionTree->getLeafIndex();

    // Loop on all the partition tree leaves
    areThereRefinedPartitions = false;
    needToAddSamples = false;

    // Init Partition File
    string currPartSampleFile;
    if(opts.printPartitionSamples){
      currPartSampleFile = opts.simulPrefix + "partSamples_" + to_string(itCount) + ".dat";
      initCurrentPartitionSampleFile(currPartSampleFile);
    }
      
    // Set To Zero Average and Standard Deviation
    partAV.resize(totPartitions);
    partSD.resize(totPartitions);
    partVolume.resize(totPartitions);
    partSampleCount.resize(totPartitions);

    // LOOP ON PARTITIONS
    for(int loopA=0;loopA<totPartitions;loopA++){           

      // Print Current Partition Info
      if(opts.printDBGMessages){
        printf("--- CURRENT PARTITION: %d/%d\n",loopA+1,totPartitions);
      }

      // Get Leaf Index
      currLeafIndex = leafIndex[loopA];

      // Get Current Partition
      currPartition = partitionTree->getAllNodes(currLeafIndex);      

      // Compute the quantities or use the stored statistics
      if(!currPartition->hasConverged()){

        // Get limits for the current partition
        currLimits = currPartition->getLimits();
        if(opts.printDBGMessages){
          for(int loopB=0;loopB<inputs->getTotDims();loopB++){
            printf("Limits for Partition %3d, Dimension %3d: [%10f, %10f]\n",loopA+1,loopB+1,currLimits[2*loopB + 0],currLimits[2*loopB + 1]);  
          }
        }

        uqSamples* currPartSamples;      

        // Extract Sample or use Numerical Quadrature for each partition
        if((opts.inputMode == imRandom)||(opts.inputMode == imTable)||(opts.inputMode == imCVSimVTK)||(opts.inputMode == imFromMCMC)){

          // Get Samples on Current Partition
          currPartSamples = new uqSamples();        

          // Extract Samples for Current Partition
          currPartSamples->extractPartitionSamples(inputs,currLimits,sampleIndexes);

          // Get Corresponding Outputs
          outputs->extractSampleColumnWithIndex(sampleIndexes,opts.outputColumn,rhs);
          
          // Write RHS to file
          if(opts.printDBGMessages){
            writeVectorToFile("RHS.txt",rhs);
          }

        }else{

          // Get Samples on Current Partition
          currPartSamples = new uqSamples(inputs);

          // Scale Sample to the Current Limits
          currPartSamples->rescaleFromHypercubeToPartition(currLimits);

          // Evaluate Results
          modelRes.clear();
          errorCodes.clear();
          rhs.clear();

          if(opts.printDBGMessages){
            printf("Evaluating Model...");
            fflush(stdout);
          }

          currPartSamples->getValues(newValues);
          modelErrors = model->evalModelError(newValues,modelRes,errorCodes);      

          if(opts.printDBGMessages){
            printf("Done.\n");
          }

          // Add To Output Samples
          for(int loopB=0;loopB<modelRes.size();loopB++){
            rhs.push_back(modelRes[loopB][0]);
          }
        }

        //printf("Size of RHS: %d\n",rhs.size());
        //for(int loopB=0;loopB<rhs.size();loopB++){
        //  printf("RHS: %d %f\n",loopB,rhs[loopB]);
        //}

        // Get RHS Norm
        rhsNorm = getVectorTwoNorm(rhs);

        // Print Current Partition Samples
        if(opts.printPartitionSamples){
          printCurrentPartitionSamples(currPartSampleFile,currPartSamples,rhs);  
        }
        
        // Get Totals
        partDims = currPartSamples->getTotDims();      
        if(opts.printDBGMessages){
          printf("Partition Dimensionality: %d\n",partDims);
        }

        partTotSamples = currPartSamples->getTotSamples();
        if(opts.printDBGMessages){
          printf("Total Samples in Partition: %d\n",partTotSamples);
        }
        // Store Number of Samples in partition
        currPartition->setInteger(partTotSamples);

        // Process only Partitions with Samples
        if(partTotSamples > 0){       

          // Get minimum partition size
          minPartSize = currPartition->getMinSize();
          currPartition->getMaxSizeAndDimension(maxPartSize,dimMaxSize);
          minRootPartitionSize = partitionTree->getAllNodes(0)->getMinSize();

          if(opts.printDBGMessages){
            printf("Partition Min Size: %f, Max size %f\n",minPartSize,maxPartSize);
            printf("Refinement Level %d, Partition number: %d\n",itCount,loopA);
          }

          // Rescale Samples on Hypercube - Except Grids
          currPartSamples->rescaleOnHypercube(currLimits);

          // ==============================================================================
          // DETERMINE THE PROBABILITY MEASURE IN THIS PARTITION TO BUILD THE MULTIWAVELETS
          // ==============================================================================

          currPartIntMeasure = 1.0; 
          partArea = 1.0;
          stdVec tmpLimits;         
          for(int loopB=0;loopB<inputs->getTotDims();loopB++){
            // Scale Grid on partition
            scale1DGridOnPartition(measure1DGridPoints,currLimits[2*loopB + 0],currLimits[2*loopB + 1],measureLocations);
            dimSize = fabs(currLimits[2*loopB + 1] - currLimits[2*loopB + 0]);
            partArea *= dimSize;

            //printf("Extremes of the Element: %f %f\n",currLimits[2*loopB + 0],currLimits[2*loopB + 1]);
            //((uq1DMEApproximant*)marginalMeasures[loopB])->getExtremes(tmpLimits);
            //printf("Extremes of the interpolant: %f %f\n",tmpLimits[0],tmpLimits[1]);

            dimIntMeasure = 0.0;            
            for(int loopC=0;loopC<measureLocations.size();loopC++){
              // Use Uniform Measure if not otherwise specified
              if(opts.measureInputMode == miUnit){
                measure[loopB][loopC] = 1.0;
              }else if(opts.measureInputMode == miFromSamples){
                measure[loopB][loopC] = inputs->evalNormalizedProbabilityOnHypercube(loopB,measureLocations[loopC]);  
                //printf("Value: %f, Result: %f\n",measureLocations[loopC],measure[loopB][loopC]);
              }else if(opts.measureInputMode == miFromBetaMarginals){
                //printf("Called with Value: %f\n",measureLocations[loopC]);
                measure[loopB][loopC] = marginalMeasures[loopB]->evaluate(measureLocations[loopC]);
                //printf("Value: %f, Result: %f\n",measureLocations[loopC],measure[loopB][loopC]);
                // getchar();
              }else{
                throw acException("ERROR: Invalid modality to specify an input measure.\n");
              }              
              // NOTE: The Measure Must Integrate to One on the Hypercube!
              dimIntMeasure += measure[loopB][loopC] * measure1DGridPoints->getWeightAt(loopC,measure1DGridPoints->getMaxWeightOrder());              
            }
            // Scale Measure
            for(int loopC=0;loopC<measureLocations.size();loopC++){
              measure[loopB][loopC] /= dimIntMeasure;
            }
            //getchar();

            if(opts.printDBGMessages){
              // Determine the mean and standard deviation of the marginal
              marginalAVG = 0.0;
              marginalSTD = 0.0;
              for(int loopC=0;loopC<measureLocations.size();loopC++){
                marginalAVG += measureLocations[loopC] * measure[loopB][loopC] * measure1DGridPoints->getWeightAt(loopC,measure1DGridPoints->getMaxWeightOrder());
                marginalSTD += measureLocations[loopC] * measureLocations[loopC] * measure[loopB][loopC] * measure1DGridPoints->getWeightAt(loopC,measure1DGridPoints->getMaxWeightOrder());
              }
              marginalSTD = sqrt(marginalSTD - marginalAVG*marginalAVG);
              printf("Marginal AVG Dimension %d: %f\n",loopB,marginalAVG);
              printf("Marginal STD Dimension %d: %f\n",loopB,marginalSTD);
              fflush(stdout);
            }

            if(opts.printDBGMessages){
              printf("Current 1D Integral of the Measure on [0,1]: %f\n",dimIntMeasure);
              fflush(stdout);
            }
            currPartIntMeasure *= dimIntMeasure;
            if(opts.printDBGMessages){
              printf("Current Partition Marginal Integral on [0,1]^d: %f\n",dimIntMeasure);  
            }
            
          }

          if(opts.printDBGMessages){
            printf("Current Partition Integral Value: %f\n",currPartIntMeasure);
            fflush(stdout);
            //getchar();
          }

          // Set the volume in probability for this partition
          // CAREFULL: MEASURED SHOULD BE NORMALIZED BEFORE BEING PASSED TO MWBCS
          // So it should have a unit integral on the original size of the parameter space.
          partitionVolume = partArea * currPartIntMeasure;

          if(opts.printDBGMessages){
            printf("Current Partition Area: %f\n",partArea);
            printf("Current Partition Volume: %f\n",partitionVolume);
            fflush(stdout);
          }

          // ========================
          // FORM MULTIWAVELET MATRIX
          // ========================          
          // Print the Samples used for MW
          //string currPartFile("currPartSamples.dat");
          //currPartSamples->printToFile(currPartFile,true);
          uqMWMatrix* mwMat = new uqMWMatrix(opts.maxMWOrder,currPartSamples,
                                             addLegendrePoly,addMW,useBinPartitions,
                                             mwMatType,mwMatIncludeNullColumns,
                                             useExactMW,opts.mwQuadOrder,measure,
                                             maxColumns,maxDetailLevel);

          if(false){
            string outMatFile("mwPrintout.txt");
            mwMat->printToFile(outMatFile,false);

            //string outInpFile("mwInputs.txt");
            //currPartSamples->printToFile(outInpFile,false);

            //string outMeasFile("mwMeasure.txt");
            //writeTableToFile(outMeasFile,measure);

            //performMWOrthogonalityTest(currPartSamples,mwMat,measure);
            //getchar();
          }

          // Form Multiwavelet Matrix
          if(opts.printDBGMessages){
            printf("Multiwavelet Matrix Created. Rows %d, Columns %d\n",mwMat->getRowCount(),mwMat->getColCount());
            fflush(stdout);
          }

          // Evaluate Expansion Coefficients
          if((opts.inputMode == imGrid)&&(!opts.forceRVMSolution)){
            // Use Numerical Quadrature for Grid Samples
            coeffs.clear();
            for(int loopB=0;loopB<mwMat->getColCount();loopB++){
              // Add mwMat Row entries multiplied by weights
              currCoeff = 0.0;
              for(int loopC=0;loopC<mwMat->getRowCount();loopC++){
                currCoeffWeight = currPartSamples->getWeightAt(loopC,currPartSamples->getMaxWeightOrder());
                currCoeff += mwMat->getMatrixAt(loopC,loopB) * rhs[loopC] * currCoeffWeight;
              }
              // Add coefficient
              coeffs.push_back(currCoeff);
            }
            // Get the Residual Norm
            gridRhs = mwMat->applyTo(coeffs);
            resNorm = 0.0;
            for(int loopB=0;loopB<mwMat->getRowCount();loopB++){
              resNorm += (gridRhs[loopB] - rhs[loopB]) * (gridRhs[loopB] - rhs[loopB]);
            }
            resNorm = sqrt(resNorm/(double)mwMat->getRowCount());
          }else{
            // Use Bayesian CS for Random Samples
            uqAlgorithmBCS* bcs = new uqAlgorithmBCS();

            // Run Bayesian Compressed Sensing
            if(rhsNorm > kMathZero){
              if(opts.printDBGMessages){
                printf("Running RVM...\n");
                fflush(stdout);
              }

              // Print Progress to screen
              bcs->opts.printProgressToScreen = false;
              bcs->opts.printDBGMessages      = false;

              coeffs.clear();
              try{
                bcsReturn = bcs->run(partTotSamples,mwMat->getColCount(), 
                                     rhs,mwMat->getMatrix(),
                                     coeffs,coeffPrec,
                                     resNorm);
                
                //tempConst = coeffs[0];
                //printf("Removing: %f\n",tempConst);
                //for(int loopA=0;loopA<rhs.size();loopA++){
                //  rhs[loopA] = rhs[loopA] - tempConst;
                //}

                //coeffs.clear();

                //bcsReturn = bcs->run(partTotSamples,mwMat->getColCount(), 
                //                     rhs,mwMat->getMatrix(),
                //                     coeffs,coeffPrec,
                //                     resNorm);
                
                //coeffs[0] += tempConst;


              }catch(...){
                printf("--- [Warning] RVM not converged. RHS Norm: %15.6e\n",rhsNorm);
                // The RHS is in the same order of the noise 
                // RHS is very small: so zero coefficients
                coeffs.clear();          
                for(int loopA=0;loopA<mwMat->getColCount();loopA++){
                  coeffs.push_back(0.0);
                }          
              }
            }else{ 
              if(opts.printDBGMessages){
                printf("\n");
                printf("BCS Not Used: Zero RHS.\n");
                printf("\n");
              }
              coeffs.clear();          
              for(int loopA=0;loopA<mwMat->getColCount();loopA++){
                coeffs.push_back(0.0);
              }          
            }         
            // Delete bcs alg
            delete bcs;
          }
        
          // Print Coefficient Norm
          coeffNorm = getVectorTwoNorm(coeffs);
          if(opts.printDBGMessages){
            printf("Coefficients\n");
            for(int loopA=0;loopA<coeffs.size();loopA++){
              printf("%8.4f ",coeffs[loopA]);                
            }
            printf("\n");
            printf("Coefficient Norm: %f\n",coeffNorm);        
            printf("Residual Norm: %f\n",resNorm);
          }

          // Eval and Print Surrogate
          if(opts.printAndEvalSurrogate){
            printAndEvalSurrogate(opts.simulPrefix,itCount,opts.maxMWOrder,currPartSamples->getTotDims(),coeffs,currLimits,opts.mwQuadOrder,measure);                            
          }

          // EVAL ERROR METRICS FOR CURRENT PARTITION
          if(opts.computeErrorMetrics){
            // Eval Error Norms for Surrogate
            evalSurrogateError(checkPointInputs,checkPointOutputs,
                               opts.maxMWOrder,coeffs,currLimits,opts.mwQuadOrder,measure,opts.outputColumn,
                               returnErrorMetrics,errorMetricSampleIdX);
            // Append Errors
            
            for(int loopC=0;loopC<errorMetricSampleIdX.size();loopC++){
              if(!errorWasComputed[errorMetricSampleIdX[loopC]]){
                surrogateErrors[errorMetricSampleIdX[loopC]] = returnErrorMetrics[loopC];  
                errorWasComputed[errorMetricSampleIdX[loopC]] = true;
              }              
            }     
            // Store Error Metrics In Current Partition
            currPartition->setPartitionErrorMetrics(returnErrorMetrics,errorMetricSampleIdX);
          }
          
          // Check refinement conditions for current partition
          //printf("threshold: %f, minRootPartitionSize: %f\n",opts.thresholdSize,minRootPartitionSize);
          //getchar();
          doRefine = evalPartitionRefinement(itCount,loopA,currLimits,partTotSamples,
                                             partDims,mwMat, 
                                             coeffs,coeffPrec,resNorm,
                                             minPartSize,maxPartSize,dimMaxSize,opts.maxPartitionSizeRatio,
                                             opts.thresholdSize * minRootPartitionSize,partitionVolume,opts.minSamplesInPartition,
                                             opts.printPartitionMonitor,opts.resToleranceRatio,opts.simulPrefix,opts.doRefineSamples,
                                             opts.printDBGMessages,
                                             dimToRefine,partVarianceMetric,samplesAreNeeded,
                                             constTerm,msVariance,mwVariance,opts.maxMWVarianceRatio,
                                             currPartAV,currPartSD);

          // Check if you can refine at all
          doRefine = (doRefine)&&(opts.doRefineSpace);

          // Sum contribution from current partition
          partAV[loopA] = currPartAV;
          partSD[loopA] = currPartSD;
          partVolume[loopA] = partitionVolume;            
          partSampleCount[loopA] = partTotSamples;

          // Assign Variance to Partition
          currPartition->setScalar(partVarianceMetric);
          if(opts.printDBGMessages){
            printf("Current partition metric: %f\n",partVarianceMetric);
          }

          // Store Refinement and statistics on partition
          currPartition->setConvergence((!doRefine)&&(!samplesAreNeeded));
          if(opts.printDBGMessages){
            printf("Partition Converged: %s\n",(!doRefine)&&(!samplesAreNeeded) ? "TRUE" : "FALSE");
          }
          
          // Store Statistics
          currPartition->setStats(0,currPartAV);
          currPartition->setStats(1,currPartSD);

          // Set total number of multiscaling basis
          currPartition->setMSTotBasis(mwMat->num_basis);

          // Set the quadrature Order for the multiwavelet Basis
          currPartition->setMWQuadOrder(opts.mwQuadOrder);

          // Set the volume of the partition
          currPartition->setVolume(partitionVolume);
          
          // Store Expansion Coefficients for this partition
          currPartition->setChaosCoefficients(coeffs);

          // Assign Measure to this partition
          currPartition->setMeasure(measure);

          // Assign MultiIndex to current partition
          currPartition->setMultiIndex(mwMat->multiIndex);

          // Set Partition Residual Norm
          currPartition->setResidualNorm(resNorm);

          // Write result
          if(opts.printDBGMessages){
            printf("Refine: %s, Dimension To Refine: %d\n",doRefine ? "true" : "false",dimToRefine+1);
          }

          if(doRefine){

            // Refine current partition
            currPartition->refine(dimToRefine);
  
            // Add to all Nodes
            partitionTree->AddToAllNodes(currPartition->getLeft());
            partitionTree->AddToAllNodes(currPartition->getRight());

            // Assign Variance to Partition
            currPartition->getLeft()->setScalar(partVarianceMetric);
            currPartition->getRight()->setScalar(partVarianceMetric);

            // Assign the minimum Sample ID to partitions for quasi random Sampling
            currPartition->getLeft()->setQuasiRandomSampleId(currPartition->getQuasiRandomSampleId());
            currPartition->getRight()->setQuasiRandomSampleId(currPartition->getQuasiRandomSampleId());

            // Set the flag to show that some partitions were refined
            areThereRefinedPartitions = true;
          }

          // Check if new samples are needed
          if(opts.printDBGMessages){
            printf("SamplesAreNeeded: %s\n",samplesAreNeeded ? "TRUE" : "FALSE"); 
          }
          needToAddSamples = (needToAddSamples || samplesAreNeeded);
              
          // Delete Samples
          delete currPartSamples;
          // Delete mwMat
          delete mwMat;

        }else{
          // ZERO SAMPLE IN PARTITION
          partAV[loopA] = 0.0;
          partSD[loopA] = 0.0;
          partVolume[loopA] = currPartition->getVolume();
          partSampleCount[loopA] = currPartition->getInteger();

          // Store the fact that the partition has zero samples
          currPartition->setHasZeroSamples(true);
        }

      }else{
        // PARTITION HAS CONVERGED
        
        // Read stored statistics for these partitions
        partAV[loopA]          = currPartition->getStats(0);
        partSD[loopA]          = currPartition->getStats(1);
        partVolume[loopA]      = currPartition->getVolume();
        partSampleCount[loopA] = currPartition->getInteger();
        
        // Print the Surrogate for these partitions
        if(opts.printAndEvalSurrogate){
          // Gather Expansion Coefficients for this partition
          currPartition->getChaosCoefficients(coeffs);
          currLimits = currPartition->getLimits();
          printAndEvalSurrogate(opts.simulPrefix,itCount,
                                opts.maxMWOrder,numberOfDimensions,
                                coeffs,currLimits,opts.mwQuadOrder,measure);                            
        }

        // Use Stored Error Metric 
        if(opts.computeErrorMetrics){
          // Get Partition Error Metrics
          currPartition->getPartitionErrorMetrics(returnErrorMetrics,errorMetricSampleIdX);
          // Pass errors
          for(int loopC=0;loopC<errorMetricSampleIdX.size();loopC++){            
            if(!errorWasComputed[errorMetricSampleIdX[loopC]]){
              surrogateErrors[errorMetricSampleIdX[loopC]] = returnErrorMetrics[loopC];  
              errorWasComputed[errorMetricSampleIdX[loopC]] = true;
            }            
          }     
        }
      }
    }    

    // Eval Volume
    currVolume = 0.0;
    for(int loopA=0;loopA<partAV.size();loopA++){
      currVolume += partAV[loopA] * partVolume[loopA];
    }
    // All partitions were computed: Eval Statistics
    // Save Current Volume As the volume estimate
    globalVolume = currVolume;

    evalGlobalStatistics(partAV,partSD,partVolume,partSampleCount,totSamples,totAV,totSD);
    if(opts.printOverallStats){
      saveOverallStatistics(opts.simulPrefix,itCount,totSamples,totAV,totSD);  
      // Save volume info to file
      saveVolumeInfo(opts.simulPrefix,itCount,totSamples,currVolume);  
    }

    // Store Statistics
    stats.push_back(constTerm);
    stats.push_back(msVariance);
    stats.push_back(mwVariance);

    // Compute the Error Metrics
    if(opts.computeErrorMetrics){
      // All Components of errorWasComputed should be True
      for(int loopC=0;loopC<errorWasComputed.size();loopC++){
        if(!errorWasComputed[loopC]){
          throw uqException("Error Metric Not Computed.\n");
        }
      }
      // Compute Error Norms
      errorTwoNorm = getErrorTwoNorm(surrogateErrors);
      errorOneNorm = getErrorOneNorm(surrogateErrors);
      errorInfNorm = getErrorInfNorm(surrogateErrors);
      //for(int loopC=0;loopC<surrogateErrors.size();loopC++){
      //  printf("%f\n",surrogateErrors[loopC]);
      //}
      // Save To File
      saveErrorMetricFile(opts.simulPrefix,itCount,totSamples,errorTwoNorm,errorOneNorm,errorInfNorm);
    }

    // Eval Convergence
    //printf("Need to Add Samples: %s\n",needToAddSamples ? "TRUE" : "FALSE"); 
    //getchar();
    finished = (areThereRefinedPartitions == false)&&(needToAddSamples == false);

    // If No space and sample refinement then finish
    if((!opts.doRefineSpace)&&(!opts.doRefineSamples)){
      finished = true;
    }

    if(needToAddSamples){
      // If you read from Table you cannot add samples
      if((opts.inputMode == imRandom)&&(opts.doRefineSamples)){
        // Eval Additional Samples
        AddedSamples = new uqSamples(inputs->getTotDims());
        AddedSamples->addSamplesAdaptive(inputs,partitionTree,opts.maxMWVarianceRatio,opts.minSamplesInPartition,asHalton);
        //AddedSamples->AddSamplesAdaptive(inputs,partitionTree,opts.maxMWVarianceRatio,opts.minSamplesInPartition);
        totAddedSamples = AddedSamples->getTotSamples();

        if(opts.printDBGMessages){
          printf("--- ADAPTIVITY\n");
          printf("--- ADAPTIVE SAMPLING: Added %d samples.\n",totAddedSamples);
        }        
      }
    }else{
      totAddedSamples = 0;
    }

    // Compute Maximum Residual Norm
    maxRedisualNorm = 0.0;
    for(int loopC=0;loopC<totPartitions;loopC++){
      // Get Leaf Index
      currLeafIndex = leafIndex[loopC];
      // Get Current Partition
      currPartition = partitionTree->getAllNodes(currLeafIndex);      
      // Get Current Residual Norm
      currResNorm = currPartition->getResidualNorm();
      if(currResNorm > maxRedisualNorm){
        maxRedisualNorm = currResNorm;
      }
    }

    // Compute Average Variance Ratio
    avgVarianceRatio = 0.0;    
    if(false){
      for(int loopC=0;loopC<totPartitions;loopC++){
        // Get Leaf Index
        currLeafIndex = leafIndex[loopC];
        // Get Current Partition
        currPartition = partitionTree->getAllNodes(currLeafIndex);      
        // Get Current MW/Sigma Ratio
        avgVarianceRatio += currPartition->getScalar();
      }
      avgVarianceRatio /= (double)totPartitions;
    }else{
      for(int loopC=0;loopC<totPartitions;loopC++){
        // Get Leaf Index
        currLeafIndex = leafIndex[loopC];
        // Get Current Partition
        currPartition = partitionTree->getAllNodes(currLeafIndex);      
        if(fabs(currPartition->getScalar()) > avgVarianceRatio)
        avgVarianceRatio = fabs(currPartition->getScalar());
      }
    }

    //for(int loopA=0;loopA<partAV.size();loopA++){
    //  printf("Partition AV: %e, Partition VOL: %e\n",partAV[loopA],partVolume[loopA]);
    //}

    // Print current iteration messages
    if(opts.printProgress){
      printf("%15d %15d %15.6e %15.6e %15.6e %15.6e %15.6e\n",itCount,totPartitions,avgVarianceRatio,maxRedisualNorm,totAV,totSD,currVolume);
    }
    //getchar();
  }

  // Save Total Statistics
  globalAVG = totAV;
  globalSTD = totSD;

  // Delete the outputs
  delete outputs;
  outputs = NULL;

  // Return 
  return result;
}

// Eval Surrogate at a number of pre-defined sampling locations
void acActionUP_MWBCS::evalSurrogate(stdMat params,stdVec& surrogate){
  
  // Set Options for MW Matrix
  bool addLegendrePoly = true;
  bool addMW = true;
  bool useBinPartitions = true;
  int mwMatType = kMWFixedMaxDetailLevel;
  int maxDetailLevel = 0;
  bool mwMatIncludeNullColumns = true;
  bool useExactMW = false;
  int maxColumns = 0;
  
  // Create Samples from stdMat
  uqSamples* sampleLocations = new uqSamples();
  uqSamples* partitionSamples;
  uqMWMatrix* mwResampledMat;
  stdIntVec leafIndex;
  int currLeafIndex;
  uqPartitionNode* currPartition;
  stdVec currLimits;
  stdIntVec sampleIndexes;
  stdVec coeffs;
  stdMat currMeasure;
  stdVec partSurrogate;
  stdBoolVec isLocationVisited;

  // Allocate Result
  surrogate.resize(params.size());

  // Init isLocationVisited
  isLocationVisited.resize(params.size());
  for(int loopA=0;loopA<isLocationVisited.size();loopA++){
    isLocationVisited[loopA] = false;
  }

  // Build Sample Locations from matrix
  sampleLocations->buildFromMatrix(params);

  // Get Total number of leaves
  int totPartitions = partitionTree->getLeafCount();

  // Get list with the partition limits
  leafIndex = partitionTree->getLeafIndex();

  // Loop on the leaves of the binary tree
  for(int loopA=0;loopA<totPartitions;loopA++){   
    
    // Get Leaf Index
    currLeafIndex = leafIndex[loopA];
    // Get Current Partition
    currPartition = partitionTree->getAllNodes(currLeafIndex);
    // Get associated limits
    currLimits = currPartition->getLimits();

    // Get Samples in current partition
    uqSamples* partitionSamples = new uqSamples();
    partitionSamples->extractPartitionSamples(sampleLocations,currLimits,sampleIndexes);    

    // Evaluate Errors Only if there are checkpoint samples
    if(partitionSamples->getTotSamples() > 0){            

      // Rescale Samples on Hypercube
      partitionSamples->rescaleOnHypercube(currLimits);

      // Get measure for current partition
      currPartition->getMeasure(currMeasure);

      // Resample Multiwavelet Matrix
      mwResampledMat = new uqMWMatrix(opts.maxMWOrder,partitionSamples,
                                      addLegendrePoly,addMW,useBinPartitions,
                                      mwMatType,mwMatIncludeNullColumns,
                                      useExactMW,opts.mwQuadOrder,currMeasure,
                                      maxColumns,maxDetailLevel);
      
      // Get chaos coefficients from current partition
      currPartition->getChaosCoefficients(coeffs);
      
      // Find Surrogate Measurements
      partSurrogate = mwResampledMat->applyTo(coeffs);
      
      // Delete Matrix
      delete mwResampledMat;
    }
    delete partitionSamples;
    partitionSamples = NULL;    
    // Store Results at sample Index
    for(int loopB=0;loopB<sampleIndexes.size();loopB++){
      isLocationVisited[sampleIndexes[loopB]] = true;
      surrogate[sampleIndexes[loopB]] = partSurrogate[loopB];
    }    
  }
  
  // MAKE SURE ALL SAMPLE LOCATIONS ARE VISITED !!!
  bool allIsFine = true;
  for(int loopA=0;loopA<isLocationVisited.size();loopA++){
    allIsFine = (allIsFine && isLocationVisited[loopA]);
    if(!isLocationVisited[loopA]){
      printf("Location %d has not been visited.\n",loopA);
      printf("Coords: ");
      for(int loopB=0;loopB<sampleLocations->getTotDims();loopB++){
        printf("%15.6e ",sampleLocations->getValuesAt(loopA,loopB));
      }
      printf("\n");
    }
  }
  if(!allIsFine){
    throw acException("Error: there are sample location not visited.");  
  }
  
  // Free Objects
  delete sampleLocations;
}

// Eval the marginal distribution once the regression has been performed
void acActionUP_MWBCS::evalMarginals(uqSamples* inputs,stdMat& marginals){

  // Set Options for MW Matrix
  bool   addLegendrePoly          = true;
  bool   addMW                    = true;
  bool   useBinPartitions         = true;
  int    mwMatType                = kMWFixedMaxDetailLevel;
  int    maxDetailLevel           = 0;
  bool   mwMatIncludeNullColumns  = true;
  bool   useExactMW               = false;
  int    maxColumns               = 0;
  double currPartSize             = 0.0;

  // Declare
  stdIntVec leafIndex;
  int currLeafIndex;
  uqPartitionNode* currPartition;
  stdVec currLimits;
  uqSamples* samplesAlongDimension;
  uqSamples* oneDSamples;
  uqMWMatrix* mwResampledMat;
  stdIntVec sampleIndexes;
  stdMat currMeasure;
  stdVec coeffs;
  stdVec oneDCoeffs;
  stdMat oneDMeasure;
  stdVec tmp;
  stdIntMat currMI;  
  int msTotBasis = 0;
  int mwDim = 0;
  stdVec oneDLimits;

  // Allocate resulting marginal matrix
  marginals.resize(inputs->getTotSamples());
  for(int loopA=0;loopA<inputs->getTotSamples();loopA++){
    marginals[loopA].resize(inputs->getTotDims());
    for(int loopB=0;loopB<inputs->getTotDims();loopB++){
      marginals[loopA][loopB] = 0.0;
    }
  }  
  oneDLimits.resize(2);

  // Get Total number of leaves
  int totPartitions = partitionTree->getLeafCount();    
  // Get list with the partition limits
  leafIndex = partitionTree->getLeafIndex();    
  
  // Loop on the leaves of the binary tree
  for(int loopA=0;loopA<totPartitions;loopA++){           
    
    // Get Leaf Index
    currLeafIndex = leafIndex[loopA];    
    // Get Current Partition
    currPartition = partitionTree->getAllNodes(currLeafIndex);
    // Get associated limits
    currLimits = currPartition->getLimits();

    // Proceed if at least one sample is present
    if((inputs->getTotSamples() > 0)&&(!currPartition->getHasZeroSamples())){
      
      // RETRIEVE INFO STORED IN PARTITIONS
      // Get Total number of Multiscaling basis
      currPartition->getMSTotBasis(msTotBasis);
      // Get measure for current partition
      currPartition->getMeasure(currMeasure);
      // Get chaos coefficients from current partition
      currPartition->getChaosCoefficients(coeffs);
      // Need the multiIndex used for current partition
      currPartition->getMultiIndex(currMI);      

      // Loop on the Dimension
      for(int loopB=0;loopB<inputs->getTotDims();loopB++){        

        // Find the size of this partition in dimension d-1
        currPartSize = 1.0;
        for(int loopC=0;loopC<inputs->getTotDims();loopC++){
          if(loopC != loopB){
            currPartSize *= (currLimits[2*loopC + 1] - currLimits[2*loopC + 0]);
          }
        }

        // Get The Samples with coordinate in this dimension belonging to the current partition
        uqSamples* samplesAlongDimension = new uqSamples();
        samplesAlongDimension->extractSamplesWithOneOverlappingDimension(inputs,currLimits,loopB,sampleIndexes);

        // Get limits across the loopB dimension
        oneDLimits[0] = currLimits[2*loopB + 0];
        oneDLimits[1] = currLimits[2*loopB + 1];

        // Check if samples are available
        if(samplesAlongDimension->getTotSamples() > 0){

          // Filter the coefficients only for this dimension
          // Careful Start from 1
          oneDCoeffs.clear();
          for(int loopC=0;loopC<coeffs.size();loopC++){
            // Based on LoopC it is easy to understand which dimension has the multiwavelet
            mwDim = getMWDimension(loopC,msTotBasis);
            //printf("Basis num: %d, msTotBasis: %d, mwDim: %d\n",loopC,msTotBasis,mwDim);
            //getchar();
            if(allOthersAreMultiscalingZeros(loopB,currMI[loopC],mwDim)){
              oneDCoeffs.push_back(coeffs[loopC]);
            }
          }

          // Create one-dimensional Samples
          oneDSamples = new uqSamples();

          // Extract One Dimensional Samples
          oneDSamples->extractOneDimensionalSamples(samplesAlongDimension,loopB);        

          // Need to map to the unit interval
          oneDSamples->rescaleOnHypercube(oneDLimits);
          //printf("LB: %15.6e, UB: %15.6e\n",oneDLimits[0],oneDLimits[1]);
          //printf("Sample List:\n");
          //for(int loopC=0;loopC<oneDSamples->getTotSamples();loopC++){
          //  printf("%15.6e\n",oneDSamples->getValuesAt(loopC,0));
          //}
          //getchar();

          // Copy 1D Measure for current dimension: Should be alway equal to 1.0
          oneDMeasure.clear();
          tmp.clear();
          for(int loopC=0;loopC<currMeasure[loopB].size();loopC++){
            tmp.push_back(currMeasure[loopB][loopC]);
          }
          oneDMeasure.push_back(tmp);

          // Evaluate a 1D MW basis at the sampling locations for this dimension
          try{
            mwResampledMat = new uqMWMatrix(opts.maxMWOrder,oneDSamples,
                                            addLegendrePoly,addMW,useBinPartitions,
                                            mwMatType,mwMatIncludeNullColumns,
                                            useExactMW,opts.mwQuadOrder,oneDMeasure,
                                            maxColumns,maxDetailLevel);
          }catch(...){
            printf("AHI...\n");
            printf("MW INTEGRATION ORDER: %d\n",opts.mwQuadOrder);
            printf("Measure Rows: %d\n",(int)currMeasure.size());
            printf("Measure Cols: %d\n",(int)currMeasure[0].size());
            oneDSamples->printToFile("1DSamples.txt",false);
            for(int loopC=0;loopC<oneDMeasure[0].size();loopC++){
              printf("%e \n",oneDMeasure[0][loopC]);
            }
            exit(-1);
          }

          // Plot Points and Basis
          oneDSamples->printToFile("oneDSamples.txt",false);
          mwResampledMat->printToFile("oneDBasis.txt",false);
          //getchar();

          // Check the agreement between MW Matrix Column Size and 1D Coefficients
          if(oneDCoeffs.size() != mwResampledMat->getColCount()){
            printf("1DCoeff Size: %d\n",(int)oneDCoeffs.size());
            printf("MW Matrix Size: %d\n",mwResampledMat->getColCount());
            throw acException("ERROR: 1D coefficient size is not compatible with 1D MW matrix cardinality.\n");
          }

          // Evaluate the product of coefficients times 1D basis        
          for(int loopC=0;loopC<mwResampledMat->getRowCount();loopC++){
            //marginals[sampleIndexes[loopC]][loopB] = 0.0;
            // Careful: the coefficients has -1 as the constant term is not there
            for(int loopD=0;loopD<mwResampledMat->getColCount();loopD++){
              marginals[sampleIndexes[loopC]][loopB] += currPartSize * oneDCoeffs[loopD] * mwResampledMat->getMatrixAt(loopC,loopD);
            }
          }          

          // Free one-dimensional samples
          delete oneDSamples;
          // Free Matrix        
          delete mwResampledMat;    
          // Free Free sampling with overlapping coordinates
          delete samplesAlongDimension;
        }
      }
    }
  }

  // Normalize marginals with trapezoidal rule
  double currInt = 0.0;
  int currNode = 0;
  int nextNode = 0;
  stdIntVec samplePermutation;
  uqSamples* tempSamples;
  for(int loopA=0;loopA<inputs->getTotDims();loopA++){

    // Create a new set of one-dimensional samples
    tempSamples = new uqSamples();

    // Init sum
    currInt = 0.0;

    // Get inputs along the current dimension
    tempSamples->extractOneDimensionalSamples(inputs,loopA);

    // Determine input permutation
    tempSamples->evalSamplePermutation(0,samplePermutation);

    // Compute integral of marginal along current dimension
    for(int loopB=0;loopB<tempSamples->getTotSamples()-1;loopB++){
      currNode = samplePermutation[loopB];
      nextNode = samplePermutation[loopB+1];
      currInt += 0.5*(marginals[currNode][loopA] + marginals[nextNode][loopA])*(tempSamples->getValuesAt(nextNode,0) - tempSamples->getValuesAt(currNode,0));
    }

    // Normalize Marginal by computed area
    for(int loopB=0;loopB<tempSamples->getTotSamples();loopB++){
      marginals[loopB][loopA] /= (double)currInt;
    }

    // Free the memory for these samples
    delete tempSamples;
  }
}

// NORMALIZE THE INTEGRAL OF THE APPROXIMANT TO 1.0
void normalizeApproximant(uq1DApproximant_ME* meApprox){
  // Get an integration grid

  // Set Quadrature Order and type of support
  int quadOrder = 30;
  int suppType = kHaarRange;
  uq1DQuadratureRule* rule = new uq1DQuadratureRule_CC(quadOrder,suppType);
  rule->generatePointsAndWeights();
  stdVec quadPoints = rule->getPoints();
  stdVec quadWeights = rule->getWeights();

  // Loop over the approximants
  double currIntegral = 0.0;
  double minEl = 0.0;
  double maxEl = 0.0;
  double elSize = 0.0;
  double currPoint = 0.0;
  double currVal = 0.0;
  for(int loopA=0;loopA<meApprox->approx.size();loopA++){
    // Get the limits of the current approximant
    minEl = meApprox->approx[loopA]->limits[0];
    maxEl = meApprox->approx[loopA]->limits[1];
    elSize = fabs(maxEl - minEl);      
    for(int loopB=0;loopB<rule->getTotalPoints();loopB++){
      // Scale Current Point
      currPoint = minEl + quadPoints[loopB] * (maxEl - minEl);
      currVal = meApprox->approx[loopA]->evaluate(currPoint);
      currIntegral += currVal * quadWeights[loopB] * elSize;
    }
  }
  if(true){
    printf("Current Integral from approximant normalization: %f\n",currIntegral);
  }  
  meApprox->normalizeByConstant(currIntegral);
}

// If the inputs are one-dimensional, generate a multi-element approximant
uq1DApproximant_ME* acActionUP_MWBCS::generate1DMEApproximant(bool normalize){

  // Return error if input has more than one dimension
  if(inputs->getTotDims() != 1){
    uqException("ERROR: The Approximant is multi-variate.\n");
  }

  // Get total partitions
  int totPartitions = partitionTree->getLeafCount();
  
  // Get list with the partition limits
  stdIntVec leafIndex;
  leafIndex = partitionTree->getLeafIndex();

  // Loop on Partitions
  uqPartitionNode* currPartition;
  vector<uq1DApproximant_SE*> seApprox;
  int currLeafIndex = 0;
  for(int loopA=0;loopA<totPartitions;loopA++){

    // Get Leaf Index
    currLeafIndex = leafIndex[loopA];

    // Get Current Partition
    currPartition = partitionTree->getAllNodes(currLeafIndex);      

    // Get all the quantities required
    int locBasisOrder = 0;
    int locQuadOrder = 0;
    stdVec locCoeffs;
    stdVec locLimits;
    stdMat locMeasure;
    currPartition->getMSTotBasis(locBasisOrder);
    currPartition->getMWQuadOrder(locQuadOrder);
    currPartition->getMeasure(locMeasure);
    currPartition->getChaosCoefficients(locCoeffs);
    locLimits = currPartition->getLimits();

    if(false){
      printf("Basis Order: %d\n",locBasisOrder);
      printf("Basis Quadrature Order: %d\n",locQuadOrder);
      printf("Size of Coeff: %d\n",(int)locCoeffs.size());
      printf("Size of Limits: %d\n",(int)locLimits.size());
      printf("Size of Measure: %d\n",(int)locMeasure.size());
      printf("\n");
    }

    // Construct each single-element approximant
    uq1DApproximant_SE* currApprox = new uq1DApproximant_SE(atMW,locBasisOrder,locQuadOrder,
                                                            locMeasure[0],locCoeffs,locLimits);
    // Add to the array of SE approximants
    seApprox.push_back(currApprox);
  }

  // Create Multiwavelet approximant from array for SE
  uq1DApproximant_ME* result = new uq1DApproximant_ME(seApprox);

  // Normalize to have a unit integral
  if(normalize){
    normalizeApproximant(result);  
  }
  
  // Return Multi-element approximant
  return result;
}