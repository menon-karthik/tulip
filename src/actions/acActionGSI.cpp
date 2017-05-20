// ========================================
// EVALUATION OF GLOBAL SENSITIVITY INDEXES
// Daniele Schiavazzi, 2014
// ========================================

# include "acActionGSI.h"

using namespace std;

// Define the type of polynomial interpolation
const int ipHermite  = 0;
const int ipLegendre = 1;

// DEFINE LSI OPTIONS
struct gsiOptions{
  bool writeDebugData;
  int maxOrder;
};

// ===========
// CONSTRUCTOR
// ===========
acActionGSI::acActionGSI(){
  // Init Parameters
  useDefaultRange = true;
  priorParamFile = "";
}

// ================================
// PRINT SENSITIVITY TABLES - LATEX
// ================================
void acActionGSI::printSensitivityTablesToLatex(int index, int res_num,int par_num,vector<vector<double> > dirTable,vector<vector<double> > totTable){
  FILE* stateFile;
  string resName;
  string parName; 
  char idx[15];
  sprintf(idx, "%d", index);
  string fileName = "sensTables_" + string(idx) + ".txt";
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
void acActionGSI::printSensitivityTables(int index, int res_num,int par_num,vector<vector<double> > dirTable,vector<vector<double> > totTable){
  FILE* outFile;
  char idx[15];
  sprintf(idx, "%d", index);
  string fileName = "sensTables_" + string(idx) + ".txt";
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
      fprintf(outFile,"%20.3f ",dirTable[loopB][loopA]);
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
      fprintf(outFile,"%20.3f ",totTable[loopB][loopA]);
    }
    fprintf(outFile,"\n");
  }

  // Close State File
  fclose(outFile);		
}

// ================================
// PRINT SIGMA COEFFICIENTS TO FILE
// ================================
void acActionGSI::printSigmaCoeffs(int index, int res_num,int basis_num,vector<vector<double> > sigmaCoeffs){
  FILE* outFile;
  char idx[15];
  sprintf(idx, "%d", index);
  string fileName = "sigmaCoeffs_" + string(idx) + ".txt";
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
void printMultiIndexToFile(int basis_num,int par_num,vector<vector<int> > multiIndex){
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
bool doesNotHaveIndex(int index,int row,vector<vector<int> > table){  
  return (table[row][index] == 0);
}

// ============================================================
// CHECK IF MULTI-INDEX IS ZERO FOR A GIVEN DIMENSION AND BASIS
// ============================================================
bool doesHaveIndex(int index,int row,vector<vector<int> > table){  
  return (table[row][index] != 0);
}

// ============================================================
// CHECK IF MULTI-INDEX IS ZERO FOR A GIVEN DIMENSION AND BASIS
// ============================================================
bool hasOnlyIndex(int index,int row,int par_num,vector<vector<int> > table){
  // ALL DIMENSIONS BUT INDEX WITH ZEROS
  bool result = true;
  for(int loopA=0;loopA<par_num;loopA++){
    if(loopA != index){
      result = (result && (table[row][loopA] == 0));
    }
  }

  // CHECK LAST CONDITION THAT INDEX SHOULD BE NOT ZERO
  result = (result && (table[row][index] != 0));
  return result;
}

// ================================================================
// EXTRACT GSI COEFFICIENT FROM THE TABLE OF POLYCHAOS COEFFICIENTS
// ================================================================
void extractGSICoefficients(int basis_num,int res_num,int par_num,
                            vector<vector<int> > multiIndex,vector<vector<double> > sigmaCoeffs,
                            vector<vector<double> > &dirTable,vector<vector<double> > &totTable){
  // DIRECT COEFFICIENTS
  // dirTable 
  // - Columns GSI for a single Result Quantity.
  // - Rows GSI for a single Parameter and multiple result quantities.
  double currVariance = 0.0;
  double currFirstOrderCoeffDir = 0.0;
  double currFirstOrderCoeffTot = 0.0;
  vector<double> tempDirTable;
  vector<double> tempTotTable;

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
        //if(hasOnlyIndex(loopB,loopC,par_num,multiIndex)){
        if(doesHaveIndex(loopB,loopC,multiIndex)){
          //printf("DIRECT: PARAM %d BASIS %d\n",loopB,loopC);
          currFirstOrderCoeffDir = currFirstOrderCoeffDir + sigmaCoeffs[loopA][loopC]*sigmaCoeffs[loopA][loopC];
        }
        // Total Coefficent
        if(doesNotHaveIndex(loopB,loopC,multiIndex)){
          //printf("TOTAL: PARAM %d BASIS %d\n",loopB,loopC);
          currFirstOrderCoeffTot = currFirstOrderCoeffTot + sigmaCoeffs[loopA][loopC]*sigmaCoeffs[loopA][loopC];
        }
      }
      // Store Table Value
      if(abs(currVariance)>1.0e-3){
        tempDirTable.push_back((currFirstOrderCoeffDir/currVariance)*100.0);
        tempTotTable.push_back((1.0-(currFirstOrderCoeffTot/currVariance))*100.0);
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
void evalMultiDPoly(int polyType,int current_basis,int totalNodes,double* nodes,int par_num,vector<vector<int> > multiIndex,double* results){
  
  // Declare
  double nodeResult;
  double xValue;
  int currPolyOrder;
  double currBaseValue;

  // Create polynomial interpolator
  uqPolyBasis* myInterp = new uqPolyBasis(kPolyHermite,3);

  // Loop on the grid points
  for(int loopA=0;loopA<totalNodes;loopA++){

    // Loop through the dimensions
    nodeResult = 1.0;
    for(int loopB=0;loopB<par_num;loopB++){
      // Copy X Value
      xValue = nodes[par_num*loopA + loopB];
      // Eval Order from multi-index
      currPolyOrder = multiIndex[current_basis][loopB];
      // Eval function Value
      currBaseValue = myInterp->evaluate(xValue,currPolyOrder);
      // Fill Constraint Mat
      nodeResult = nodeResult*currBaseValue;
    }
    results[loopA] = nodeResult;
  }
}

// =================================================================
// TRANSFORM GRID POINT TO SAMPLE FROM PARAMETER NORMAL DISTRIBUTION
// =================================================================
void transformToSampleGaussian(int curr_sample, int par_num, double* nodes,vector<double> prAv,vector<double> prSd,stdVec& currParams){
  double currValue = 0.0;
  for(int loopA=0;loopA<par_num;loopA++){
    currValue = nodes[curr_sample*par_num + loopA];
    if(prSd[loopA]>1.0e-8){
      // Sample from a Normal Distribution
      currParams[loopA] = currValue*prSd[loopA] + prAv[loopA];
    }else{
      // Return with Average Value	
      currParams[loopA] = prAv[loopA];
    }
  }  
}

// =======================================================
// GENERATE THE MULTI-INDEX FOR THE SELECTED MAXIMUM ORDER
// =======================================================
void generateMultiIndex(int dim, int order,int &basis_num,vector<vector<int> > &multiIndex){

  // Allocate
  vector<int> vecStore;
  int x[dim];
  for(int loopA=0;loopA<dim;loopA++){
    x[loopA] = 0;
  }
  vecStore.clear();
  for(int loopB=0;loopB<dim;loopB++){
    vecStore.push_back(x[loopB]);
  }
  multiIndex.push_back(vecStore);

  // Get the number of Monomials
  basis_num = mono_upto_enum(dim,order);

  // Loop to generatre the multi-index
  for(int loopA=0;loopA<(basis_num-1);loopA++){
    mono_next_grlex(dim,x);
    vecStore.clear();
    for(int loopB=0;loopB<dim;loopB++){
      vecStore.push_back(x[loopB]);
    }
    multiIndex.push_back(vecStore);
  }
}

// =======================================
// COMPUTATION OF GLOBAL SENSITIVITY INDEX
// =======================================
int acActionGSI::go(){

  // Set Options
  gsiOptions options;
  options.writeDebugData = true;


  vector<double> prAv;
  vector<double> prSd;  

  // Get Number Of Parameters for this Model
  int par_num = model->getParameterTotal();
  int res_num = model->getResultTotal();
  stdVec currParams;
  stdVec lpnRes;
  currParams.resize(par_num);
  lpnRes.resize(res_num);

  // Read Marginal Statistic (First and Second Order) from file or limits
  double currRange = 0.0;

  printf("\n");
  printf("--- PROCESSING PARAMETERS\n");

  if(useDefaultRange){
  	printf("PARAMETER RANGES FROM MODEL LIMITS\n");
    stdVec limits;
    model->getParameterLimits(limits);
    for(int loopA=0;loopA<par_num;loopA++){
      prAv.push_back(0.5*(limits[loopA*2+1] + limits[loopA*2+0]));
      prSd.push_back((1.0/sqrt(12))*(limits[loopA*2+1] - limits[loopA*2+0]));
    }
  }else{
    // Read Marginal Statistics From File
    int prior_num = 0;
    int error = readPriorFromFile(priorParamFile,prior_num,prAv,prSd);
    if((error != 0)||(prior_num != par_num)){
      printf("ERROR: INVALID FILE WITH PRIOR PARAMETERS.\n");
      exit(1);
    }
  }  

  // PRINT ADOPTED PARAMETER RANGES
  printf("%10s %10s %10s\n","NUM","MEAN","STD");
  for(int loopA=0;loopA<par_num;loopA++){
    printf("%10d %10f %10f\n",loopA,prAv[loopA],prSd[loopA]);
  }

  // Set Maximum Interpolation Order
  options.maxOrder = 3;

  // Get Start Indexes
  int startIDX[options.maxOrder];
  int endIDX[options.maxOrder];

  // Eval Sparse Grid Point Locations and Weights
  int totalNodes = nwspgr_size (gqn_order, par_num, options.maxOrder);
  int totalNodesAfterMerge = 0;
  double* nodes = new double[par_num*totalNodes];
  double* weights = new double[totalNodes];
  double polyResult[totalNodes];
  vector<vector<double> > modelResult;
  vector<vector<double> > sigmaCoeffs;
  vector<double> currModelRes;
  vector<double> oneResCoeff;
  vector<vector<double> > dirTable;
  vector<vector<double> > totTable;

  
  // Generate Multi-index for the maximum order
  printf("\n");
  printf("FORMING MULTI-INDEX...");
  int basis_num = 0;
  vector<vector<int> > multiIndex;
  generateMultiIndex(par_num,options.maxOrder,basis_num,multiIndex);
  printf("Done.\n");

  // DEBUG: Write Multi-index
  if(options.writeDebugData){
  	printMultiIndexToFile(basis_num,par_num,multiIndex);
  }
  
   // Main Loop on the Order of the polynomial Surrogate
  int startSampleId = 0;
  int endSampleId = 0;
  for(int loopA=1;loopA<=options.maxOrder;loopA++){

    // Find the Start and end index for the Sparse Grid
    totalNodes = nwspgr_size (gqn_order, par_num, loopA);
    basis_num = mono_upto_enum(par_num,loopA-1);
    nwspgr (gqn, gqn_order, par_num, loopA, totalNodes, totalNodesAfterMerge, nodes, weights);
    startSampleId = 0;
    endSampleId = totalNodes;

    printf("\n");
    printf("EVALUATING INTEGRATION ORDER: %d\n",loopA);
    printf("TOTAL MODEL EVALUATIONS: %d\n",totalNodes);
    printf("\n");

    // DEBUG: Write Multi-inde
    if(options.writeDebugData){
      printGridToFile(loopA,totalNodes,par_num,nodes,weights);
    }

    // Model Evaluation Loop
    modelResult.clear();
    for(int loopB=startSampleId;loopB<endSampleId;loopB++){  
      printf("MODEL EVALUATION: %d\n",loopB);
      printf("\n");

      // Transform Samples
      transformToSampleGaussian(loopB,par_num,nodes,prAv,prSd,currParams);

      // Write Sample Point if Debug
      //if(options.writeDebugData){
     // 	printf("CURRENT PARAMETER SAMPLE\n");
     // 	for(int loopC=0;loopC<par_num;loopC++){
      //    printf("%d %f\n",loopC,currParams[loopC]);
     // 	}
    //  	printf("\n");
    //  }

      // Solve Model
      stdIntVec errorCode;
      double ll = model->evalModelError(currParams,lpnRes,errorCode);
      if(errorCode[0] != 0){
        printf("ERROR: INVALID GRID POINT EVALUATION.\n");
        exit(1);
      }
      printf("MODEL EVALUATION OK\n");
      // Store results
      currModelRes.clear();
      for(int loopC=0;loopC<res_num;loopC++){
        currModelRes.push_back(lpnRes[loopC]);
      }
      modelResult.push_back(currModelRes);
    }
  
    // Compute gPC Coefficients
    sigmaCoeffs.clear();
    for(int loopB=0;loopB<res_num;loopB++){
      // Clear Vector
      oneResCoeff.clear();
      for(int loopC=0;loopC<basis_num;loopC++){
        
        // Eval The polynomial at cardinality loopC and for all grid points
        evalMultiDPoly(ipHermite,loopC,totalNodes,nodes,par_num,multiIndex,polyResult);

        //printf("CHECK INTEGRATION: RES %d BASIS %d\n",loopB,loopC);
        //for(int loopD=0;loopD<totalNodes;loopD++){
        //  printf("%f %f %f \n",modelResult[loopD][loopB],polyResult[loopD],weights[loopD]);
        //}
        
        // wSPGrid ALREADY CONSTAINS THE GAUSSIAN WEIGHT FUNCTION VALUES
        double currValue = 0.0;
        for(int loopD=0;loopD<totalNodes;loopD++){
          currValue += modelResult[loopD][loopB]*polyResult[loopD]*weights[loopD];
        }
        // Increment the value of the Polynomial Chaos Expansion Coefficient
        oneResCoeff.push_back(currValue);
      }
      sigmaCoeffs.push_back(oneResCoeff);
    }

    // DEBUG: Write Sigma Coefficients
    if(options.writeDebugData){
      printSigmaCoeffs(loopA,res_num,basis_num,sigmaCoeffs);
    }

    // EXTRACT FIRST ORDER DIRECT AND TOTAL COEFFICIENTS
    extractGSICoefficients(basis_num,res_num,par_num,multiIndex,sigmaCoeffs,dirTable,totTable);

    // DEBUG: Write Sensitivity Tables
    if(options.writeDebugData){
      //printSensitivityTablesToLatex(loopA,res_num,par_num,dirTable,totTable);
      printSensitivityTables(loopA,res_num,par_num,dirTable,totTable);
    }
  }

  // Free Memory
  delete [] nodes;
  delete [] weights;

  // Completed
  printf("Completed.\n");
  return 0;
}