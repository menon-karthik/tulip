// ========================================
// EVALUATION OF GLOBAL SENSITIVITY INDEXES
// Daniele Schiavazzi, 2014
// ========================================

# include "acActionGSI.h"

using namespace std;

// ===========
// CONSTRUCTOR
// ===========
acActionGSI::acActionGSI(int sampleType,int distributionSource,string priorParamFile,bool writeDebugData,int maxOrder,int coeffAlg){
  this->sampleType = sampleType;
  this->distributionSource = distributionSource;
  this->priorParamFile = priorParamFile;
  this->writeDebugData = writeDebugData;
  this->maxOrder = maxOrder;
  this->coeffAlg = coeffAlg;
}

// ================================
// PRINT SENSITIVITY TABLES - LATEX
// ================================
void acActionGSI::printSensitivityTablesToLatex(int res_num,int par_num,const stdMat& dirTable,const stdMat& totTable){
  FILE* stateFile;
  string resName;
  string parName; 
  string fileName = "sensTables.txt";
  stateFile = fopen(fileName.c_str(),"w");

  // DIRECT SENSITIVITY TABLE

  fprintf(stateFile,"\\begin{table}[h!]\n");
  fprintf(stateFile,"\\centering\n");
  fprintf(stateFile,"\\begin{tabular}{l ");
  // Table Header
  for(int loopA=0;loopA<res_num;loopA++){
    fprintf(stateFile,"c ");
  } 
  fprintf(stateFile,"}\n");

  fprintf(stateFile,"\\toprule\n");
  fprintf(stateFile,"Param ");
  for(int loopA=0;loopA<res_num;loopA++){
    resName = model->getResultName(loopA);
    fprintf(stateFile,"& %s ",resName.c_str());
  } 
  fprintf(stateFile,"\\\\\n");
  fprintf(stateFile,"\\midrule\n");
  for(int loopA=0;loopA<par_num;loopA++){
    // First Column Parameter Name
    parName = model->getParamName(loopA);
    fprintf(stateFile,"%s ",parName.c_str());
    for(int loopB=0;loopB<res_num;loopB++){
      fprintf(stateFile,"& %20.3f ",dirTable[loopB][loopA]);
    }
    fprintf(stateFile,"\\\\\n");
  }
  fprintf(stateFile,"\\bottomrule\n");
  fprintf(stateFile,"\\end{tabular}\n");
  fprintf(stateFile,"\\caption{Full model (Stage12Blocks) Direct GSI results for patient \\patient}\n");
  fprintf(stateFile,"\\end{table}\n");

  // TOTAL SENSITIVITY TABLE

  fprintf(stateFile,"\\begin{table}[h!]\n");
  fprintf(stateFile,"\\centering\n");
  fprintf(stateFile,"\\begin{tabular}{l ");
  // Table Header
  for(int loopA=0;loopA<res_num;loopA++){
    fprintf(stateFile,"c ");
  } 
  fprintf(stateFile,"}\n");

  fprintf(stateFile,"\\toprule\n");
  fprintf(stateFile,"Param ");
  for(int loopA=0;loopA<res_num;loopA++){
    resName = model->getResultName(loopA);
    fprintf(stateFile,"& %s ",resName.c_str());
  } 
  fprintf(stateFile,"\\\\\n");
  fprintf(stateFile,"\\midrule\n");
  for(int loopA=0;loopA<par_num;loopA++){
    // First Column Parameter Name
    parName = model->getParamName(loopA);
    fprintf(stateFile,"%s ",parName.c_str());
    for(int loopB=0;loopB<res_num;loopB++){
      fprintf(stateFile,"& %20.3f ",totTable[loopB][loopA]);
    }
    fprintf(stateFile,"\\\\\n");
  }
  fprintf(stateFile,"\\bottomrule\n");
  fprintf(stateFile,"\\end{tabular}\n");
  fprintf(stateFile,"\\caption{Full model (Stage12Blocks) Total GSI results for patient \\patient}\n");
  fprintf(stateFile,"\\end{table}\n");

  // Close State File
  fclose(stateFile);    
}

// ========================
// PRINT SENSITIVITY TABLES
// ========================
void acActionGSI::printSensitivityTables(int res_num,int par_num,const stdMat& dirTable,const stdMat& totTable){
  FILE* outFile;
  string fileName = "sensTables.txt";
  outFile = fopen(fileName.c_str(),"w");

  fprintf(outFile,"\n");
  fprintf(outFile,"DIRECT COEFFICIENT TABLE\n");
  fprintf(outFile,"\n");

  fprintf(outFile,"%20s ","Param ");
  for(int loopA=0;loopA<res_num;loopA++){ 
  	fprintf(outFile,"%20s ",model->getResultName(loopA).c_str());
  }
  fprintf(outFile,"\n");

  for(int loopA=0;loopA<par_num;loopA++){
  	fprintf(outFile,"%20s ",model->getParamName(loopA).c_str());
    for(int loopB=0;loopB<res_num;loopB++){
      fprintf(outFile,"%20.10f ",dirTable[loopB][loopA]);
    }
    fprintf(outFile,"\n");
  }

  fprintf(outFile,"\n");
  fprintf(outFile,"TOTAL COEFFICIENT TABLE\n");
  fprintf(outFile,"\n");

  fprintf(outFile,"%20s ","Param ");
  for(int loopA=0;loopA<res_num;loopA++){ 
  	fprintf(outFile,"%20s ",model->getResultName(loopA).c_str());
  }
  fprintf(outFile,"\n");

  for(int loopA=0;loopA<par_num;loopA++){
  	fprintf(outFile,"%20s ",model->getParamName(loopA).c_str());
    for(int loopB=0;loopB<res_num;loopB++){
      fprintf(outFile,"%20.10f ",totTable[loopB][loopA]);
    }
    fprintf(outFile,"\n");
  }

  // Close State File
  fclose(outFile);		
}

// ================================
// PRINT SIGMA COEFFICIENTS TO FILE
// ================================
void acActionGSI::printSigmaCoeffs(const stdMat& sigmaCoeffs){

  int res_num   = sigmaCoeffs.size();
  int basis_num = sigmaCoeffs[0].size();

  FILE* outFile;
  string fileName = "sigmaCoeffs.txt";
  outFile = fopen(fileName.c_str(),"w");

  // Header
  fprintf(outFile,"%15s ","Basis ");
  for(int loopA=0;loopA<res_num;loopA++){ 
  	fprintf(outFile,"%15s ",model->getResultName(loopA).c_str());
  }
  fprintf(outFile,"\n");

  // Print Table
  for(int loopA=0;loopA<basis_num;loopA++){
  	fprintf(outFile,"%15d ",loopA);
    for(int loopB=0;loopB<res_num;loopB++){
      fprintf(outFile,"%15.3f ",sigmaCoeffs[loopB][loopA]);
    }
    fprintf(outFile,"\n");
  }
  // Close State File
  fclose(outFile);		
}

// =========================
// PRINT SPARSE GRID TO FILE
// =========================
void printGridToFile(int order,int grid_size,int par_num,double* nodes,double* weights){
  FILE* outFile;
  outFile = fopen("gridPrintOut.txt","w");
  for(int loopA=0;loopA<grid_size;loopA++){
    for(int loopB=0;loopB<par_num;loopB++){
      fprintf(outFile,"%f ",nodes[loopA*par_num + loopB]);
    }
    fprintf(outFile,"%f \n",weights[loopA]);
  }
  // Close State File
  fclose(outFile);	
}

// =========================
// PRINT MULTI-INDEX TO FILE
// =========================
void printMultiIndexToFile(int basis_num,int par_num,const stdIntMat& multiIndex){
  FILE* outFile;
  outFile = fopen("multiIndex.txt","w");
  for(int loopA=0;loopA<basis_num;loopA++){
    for(int loopB=0;loopB<par_num;loopB++){
      fprintf(outFile,"%d ",multiIndex[loopA][loopB]);
    }
    fprintf(outFile,"\n");
  }
  // Close State File
  fclose(outFile);
}

// ============================================================
// CHECK IF MULTI-INDEX IS ZERO FOR A GIVEN DIMENSION AND BASIS
// ============================================================
bool hasOnlyIndex(int index,int row,int par_num,uqMultiIndex* mi){
  // ALL DIMENSIONS BUT INDEX WITH ZEROS
  bool result = true;
  for(int loopA=0;loopA<par_num;loopA++){
    if(loopA != index){
      result = (result && (mi->getMultiIndexAt(row,loopA) == 0));
    }
  }

  // CHECK LAST CONDITION THAT INDEX SHOULD BE NOT ZERO
  result = (result && (mi->getMultiIndexAt(row,index) != 0));
  return result;
}

// ================================================================
// EXTRACT GSI COEFFICIENT FROM THE TABLE OF POLYCHAOS COEFFICIENTS
// ================================================================
void extractGSICoefficients(int basis_num,int res_num,int par_num,
                            uqMultiIndex* mi,
                            const stdMat& sigmaCoeffs,
                            stdMat& dirTable,
                            stdMat& totTable){
  // DIRECT COEFFICIENTS
  // dirTable 
  // - Columns GSI for a single Result Quantity.
  // - Rows GSI for a single Parameter and multiple result quantities.
  double currVariance = 0.0;
  double currFirstOrderCoeffDir = 0.0;
  double currFirstOrderCoeffTot = 0.0;
  stdVec tempDirTable;
  stdVec tempTotTable;

  dirTable.clear();
  totTable.clear();
  for(int loopA=0;loopA<res_num;loopA++){
    
    // Compute total Variance
    // CARE: the last index is the average!!!
    // Note that sigmaCoeffs(nResults,nBasis)
    currVariance = 0.0;
    for(int loopB=1;loopB<basis_num;loopB++){
      currVariance = currVariance + sigmaCoeffs[loopA][loopB]*sigmaCoeffs[loopA][loopB];
    }
   
    // Clear Temporary Arrays
    tempDirTable.clear();
    tempTotTable.clear();

    for(int loopB=0;loopB<par_num;loopB++){
      // Sum all variances with same Coefficients in multi-index
      currFirstOrderCoeffDir = 0.0;
      currFirstOrderCoeffTot = 0.0;
      for(int loopC=1;loopC<basis_num;loopC++){
        // Verify multi-index does not start from 0
        // Direct Coefficient
        if(hasOnlyIndex(loopB,loopC,par_num,mi)){
          currFirstOrderCoeffDir = currFirstOrderCoeffDir + sigmaCoeffs[loopA][loopC]*sigmaCoeffs[loopA][loopC];
        }
        // Total Coefficent
        if(mi->getMultiIndexAt(loopC,loopB) != 0){
          currFirstOrderCoeffTot = currFirstOrderCoeffTot + sigmaCoeffs[loopA][loopC]*sigmaCoeffs[loopA][loopC];
        }
      }
      // Store Table Value
      if(abs(currVariance)>1.0e-5){
        tempDirTable.push_back(currFirstOrderCoeffDir/currVariance);
        tempTotTable.push_back(currFirstOrderCoeffTot/currVariance);
      }else{
        tempDirTable.push_back(0.0);
        tempTotTable.push_back(0.0);              
      }
    }

    // Accumulate in table
    dirTable.push_back(tempDirTable);
    totTable.push_back(tempTotTable);
  }
}

// =================================================================
// TRANSFORM GRID POINT TO SAMPLE FROM PARAMETER NORMAL DISTRIBUTION
// =================================================================
void transformGridToSample(int sampleType,const stdVec& gridSample,const stdVec& prStat1,const stdVec& prStat2,stdVec& currParams){
  double currValue = 0.0;
  for(int loopA=0;loopA<gridSample.size();loopA++){
    currValue = gridSample[loopA];
    if(sampleType == ipUniformSampling){
      // Uniform Sample
      currParams[loopA] = prStat1[loopA] + currValue*(prStat2[loopA] - prStat1[loopA]);
    }else{
      // Gaussian Sample
      currParams[loopA] = currValue*prStat2[loopA] + prStat1[loopA];
    }
  }  
}

// =======================================
// COMPUTATION OF GLOBAL SENSITIVITY INDEX
// =======================================
int acActionGSI::go(){

  // Init Parameter Gaussian Statistics
  stdVec prStat1;
  stdVec prStat2;  

  // Get Number Of Parameters for this Model
  int par_num = model->getParameterTotal();
  int res_num = model->getResultTotal();
  stdVec currParams;
  stdVec modelRes;
  stdVec currGridSample;
  stdVec currModelRes;
  stdMat sigmaCoeffs;  
  stdVec oneResCoeff;
  stdVec rhs;
  currParams.resize(par_num);
  modelRes.resize(res_num);
  stdMat dirTable;
  stdMat totTable;

  // Read Marginal Statistic (First and Second Order) from file or limits
  double currRange = 0.0;
  stdVec limits;

  printf("\n");
  printf("--- Processing Parameters\n");

  if(distributionSource == ipDistributionsFromLimits){
    if(sampleType == ipUniformSampling){    
      printf("Independent Uniform Parameters from Model Limits\n");
      model->getParameterLimits(limits);
      for(int loopA=0;loopA<par_num;loopA++){
        prStat1.push_back(limits[loopA*2+0]);
        prStat2.push_back(limits[loopA*2+1]);
      }
    }else{
      printf("Independent Gaussian Parameters from Model Limits\n");
      model->getParameterLimits(limits);
      for(int loopA=0;loopA<par_num;loopA++){
        prStat1.push_back(0.5*(limits[loopA*2+1] + limits[loopA*2+0]));
        prStat2.push_back((1.0/sqrt(12))*(limits[loopA*2+1] - limits[loopA*2+0]));
      }
    }
  }else{
    // Read Marginal Statistics From File
    int prior_num = 0;
    int error = readPriorFromFile(priorParamFile,prior_num,prStat1,prStat2);
    if((error != 0)||(prior_num != par_num)){
      printf("ERROR: Invalid file with prior parameters.\n");
      exit(1);
    }
  }

  // PRINT ADOPTED PARAMETER RANGES
  if(sampleType == ipUniformSampling){    
    printf("%10s %10s %10s\n","n.","Min","Max");
  }else{
    printf("%10s %10s %10s\n","n.","Mean","Std");
  }
  for(int loopA=0;loopA<par_num;loopA++){
    printf("%10d %10f %10f\n",loopA,prStat1[loopA],prStat2[loopA]);
  }

  // Generate Independent Gaussian Samples
  uqSamples* samples = new uqSamples(par_num);
  
  // Add Variables
  int varType = 0;
  if(sampleType == ipUniformSampling){
    varType = kSAMPLEUniform;      
  }else{
    varType = kSAMPLEGaussian;
  }
  for(int loopA=0;loopA<par_num;loopA++){
    samples->addVariable("var_" + to_string(loopA),varType,prStat1[loopA],prStat2[loopA]);
  }

  // Generate a Sparse Grid of a Certain Order
  // Need to get a list with starting and ending points of the various orders
  samples->generateSparseGrid(maxOrder);

  // Print Grid to file if required
  if(writeDebugData){
    string outGridFile("sampleGrid.txt");
    bool printTitle = true;
    samples->printToFile(outGridFile,printTitle);
  }

  // Check the weight sum at each order
  double weightSum = 0.0;
  for(int loopA=0;loopA<maxOrder;loopA++){
    weightSum = 0.0;
    for(int loopB=0;loopB<samples->getTotSamples();loopB++){
      weightSum += samples->getWeightAt(loopB,loopA);
    }
    printf("Order %d, Integration Weight Sum: %f\n",loopA,weightSum);
  }
  

   // EVAL ALL MODELS
  int startSampleId = 0;
  int endSampleId = 0;
  int orderStart = 0;
  int orderEnd = 0;

  printf("Total Model Evaluations: %d\n",samples->getTotSamples());

  // Clear Model Results
  stdMat modelResult;
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){

    // Transform Samples
    currGridSample.clear();
    for(int loopB=0;loopB<samples->getTotDims();loopB++){
      currGridSample.push_back(samples->getValuesAt(loopA,loopB));
    }
    //transformGridToSample(sampleType,currGridSample,prStat1,prStat2,currParams);

    // Write Sample Point if debug
    if(writeDebugData){
      printf("CURRENT PARAMETER SAMPLE\n");
      for(int loopC=0;loopC<par_num;loopC++){
        printf("%d %f\n",loopC,currGridSample[loopC]);
      }
      printf("\n");
    }

    // Solve Model
    stdIntVec errorCode;
    double ll = model->evalModelError(currGridSample,modelRes,errorCode);
    if(errorCode[0] != 0){
      printf("ERROR: INVALID GRID POINT EVALUATION.\n");
      exit(1);
    }
        
    // Store results
    currModelRes.clear();
    for(int loopC=0;loopC<res_num;loopC++){
      currModelRes.push_back(modelRes[loopC]);
    }
    modelResult.push_back(currModelRes);
  }

  if(writeDebugData){
    printf("Model Result Table\n");
    for(int loopA=0;loopA<modelResult.size();loopA++){
      for(int loopB=0;loopB<modelResult[loopA].size();loopB++){
        printf("%5.3f ",modelResult[loopA][loopB]);
      }
      printf("\n");
    }
  }

  // RESCALE SAMPLES TO EVALUATE POLYNOMIAL MATRIX
  stdVec currLimits;
  samples->getSampleLimits(currLimits);
  samples->rescaleOnHypercube(currLimits);

  if(writeDebugData){
    string outGridFile("sampleRescaledGrid.txt");
    bool printTitle = true;
    samples->printToFile(outGridFile,printTitle);
  }

  // EVAL POLYNOMIAL MATRIX AT INTEGRATION POINTS
  int matType = 0;
  if(sampleType == ipUniformSampling){
    matType = kPolyRescaledLegendre;      
  }else{
    matType = kPolyHermite;
  }
  uqPolyMatrix* polyMat = new uqPolyMatrix(samples,maxOrder,matType,kMIPartialOrder);

  // Explicitly Evaluate MultiIndex
  uqMultiIndex* mi = new uqMultiIndex(samples->getTotDims(),maxOrder,kMIPartialOrder);

  string outMiFile("multiIndexGSI.txt");
  mi->printToFile(outMiFile,true);

  printf("Computing Coefficients...\n");

  // EVAL COEFFICIENTS
  if(coeffAlg == algUseRegression){

    // USE REGRESSION

    // Use Bayesian CS for Random Samples
    uqAlgorithmBCS* bcs = new uqAlgorithmBCS();
    // Set Options
    bcs->opts.printProgressToScreen = false;
    bcs->opts.printDBGMessages      = false;

    stdVec coeffs;
    stdMat coeffPrec;
    double resNorm;
    double noiseStd;

    sigmaCoeffs.clear();
    for(int loopA=0;loopA<res_num;loopA++){

      // Fill RHS With Model Result Evaluations
      rhs.clear();
      for(int loopB=0;loopB<samples->getTotSamples();loopB++){
        rhs.push_back(modelResult[loopB][loopA]);
      }

      coeffs.clear();
      coeffPrec.clear();
      noiseStd = bcs->run(polyMat->getRowCount(),polyMat->getColCount(),rhs,polyMat->getMatrix(),coeffs,coeffPrec,resNorm);

      // Store in Sigma Coefficients
      sigmaCoeffs.push_back(coeffs);
    }
              
  }else if(coeffAlg == algUseQuadrature){

    // USE QUADRATURE

    // Compute gPC Coefficients
    sigmaCoeffs.clear();
    for(int loopB=0;loopB<res_num;loopB++){
      // Clear Vector
      oneResCoeff.clear();
      for(int loopC=0;loopC<polyMat->getColCount();loopC++){
         
        // wSPGrid ALREADY CONSTAINS THE GAUSSIAN WEIGHT FUNCTION VALUES
        double currValue = 0.0;
        for(int loopD=0;loopD<samples->getTotSamples();loopD++){
          currValue += modelResult[loopD][loopB]*polyMat->getMatrixAt(loopD,loopC)*samples->getWeightAt(loopD,maxOrder);
        }
        // Increment the value of the Polynomial Chaos Expansion Coefficient
        oneResCoeff.push_back(currValue);
      }
      sigmaCoeffs.push_back(oneResCoeff);
    }

  }else{
    throw acException("ERROR: Invalid Algorithm for GSI Coefficient Computation.\n");
  }

  // DEBUG: Write Sigma Coefficients
  if(writeDebugData){
    printSigmaCoeffs(sigmaCoeffs);
  }

  // EXTRACT FIRST ORDER DIRECT AND TOTAL COEFFICIENTS
  printf("Computing GSI...\n");
  extractGSICoefficients(polyMat->getColCount(),res_num,samples->getTotDims(),mi,sigmaCoeffs,dirTable,totTable);

  // Print Resulting Global Sensitivities
  // printSensitivityTablesToLatex(loopA,res_num,par_num,dirTable,totTable);
  printf("Printing GSI...\n");
  printSensitivityTables(res_num,samples->getTotDims(),dirTable,totTable);

  // Completed
  printf("Completed!\n");

  // Return
  return 0;
}