
# include "acActionOPT_RSSD.h"

using namespace std;

void acActionOPT_RSSD::updateGradient(int par_num, double* params, int numComponents, double stepSizeCoeff, double* grad, double& fval){

  // Eval Function at current parameter Value
  fval = runModel(params);

  // Get an ordered list of Grad absolute components
  double tempDouble = 0.0;
  int tempInt = 0;
  double firstEntry = 0.0;
  double secondEntry = 0.0;
  double orderedGrad[par_num];
  int orderComponents[par_num];
  for(int loopA=0;loopA<par_num;loopA++){
    orderedGrad[loopA] = grad[loopA];
    orderComponents[loopA] = loopA;
  }
  // Bubble Sort
  for(int loopA=0;loopA<par_num-1;loopA++){
    for(int loopB=loopA+1;loopB<par_num;loopB++){
      firstEntry = fabs(orderedGrad[loopA]);
      secondEntry = fabs(orderedGrad[loopB]);
      if(firstEntry < secondEntry){
        // Swap Doubles
        tempDouble = orderedGrad[loopA];
        orderedGrad[loopA] = orderedGrad[loopB];
        orderedGrad[loopB] = tempDouble;
        // Swap Order
        tempInt = orderComponents[loopA];
        orderComponents[loopA] = orderComponents[loopB];
        orderComponents[loopB] = tempInt;
      }
    }
  }

  // SET THE GRADIENT TO ZERO
  for(int loopA=0;loopA<par_num;loopA++){
    grad[loopA] = 0.0;
  }

  // Evaluate Full Gradient
  double increment = 0.0;
  int currComp = 0;
  stdVec limits;
  model->getParameterLimits(limits);
  double newParams[par_num];
  double res[2];
  for(int loopA=0;loopA<numComponents;loopA++){

    // Get maximum components in absolute value
    currComp = orderComponents[loopA];
    
    // Set The increment based on limits
    increment = stepSizeCoeff * (limits[2*currComp + 1] - limits[2*currComp + 0]);
    
    // Set the parameters to the starting point
    for(int loopB=0;loopB<par_num;loopB++){
      newParams[loopB] = params[loopB];
    }
    
    // Perturb the parameter of interest
    for(int loopB=0;loopB<2;loopB++){
      if(loopB == 0){
        newParams[currComp] += increment;
      }else{
        newParams[currComp] -= increment;
      }      
      // Run Model 
      res[loopB] = runModel(newParams);
    }
    
    // Eval Gradient Component
    if(fabs(increment)>kMathZero){
      grad[currComp] = (res[0] - res[1])/(2.0*increment);  
    }else{
      grad[currComp] = 0.0;
    } 
  }
}


void acActionOPT_RSSD::evalFullGradient(int par_num, double* params, double stepSizeCoeff, double* grad, double& fval){

  // Eval Function at current parameter Value
  fval = runModel(params);

  // Evaluate Full Gradient
  double increment = 0.0;
  stdVec limits;
  model->getParameterLimits(limits);
  double newParams[par_num];
  double res[2];
  for(int loopA=0;loopA<par_num;loopA++){
    
    // Set The increment based on limits
    increment = stepSizeCoeff * (limits[2*loopA + 1] - limits[2*loopA + 0]);
    
    // Set the parameters to the starting point
    for(int loopB=0;loopB<par_num;loopB++){
      newParams[loopB] = params[loopB];      
    }
    
    // Perturb the parameter of interest
    for(int loopB=0;loopB<2;loopB++){
      if(loopB == 0){
        newParams[loopA] += increment;
      }else{
        newParams[loopA] -= increment;
      }      
      
      // Run Model 
      res[loopB] = runModel(newParams);
    }
    
    // Eval Gradient Component
    if(fabs(increment)>kMathZero){
      grad[loopA] = (res[0] - res[1])/(2.0*increment);  
    }else{
      grad[loopA] = 0.0;
    }  
  }
}

void acActionOPT_RSSD::perform_RSSD(int par_num, double* startPoint, double* xmin, double& ynewlo){

  // Get Parameter Limits
  stdVec limits;
  model->getParameterLimits(limits);
  
  // Eval Full Gradient
  double fold = 0.0;
  double fnew = 0.0;
  double grad[par_num];
  double normGradVec[par_num];
  evalFullGradient(par_num,startPoint,stepSizeCoeff,grad,fold);

  // Iterative Loop
  bool finished = false;
  int itCount = 0;
  double distNorm = 0.0;
  double currRatio = 0.0;
  double endPoint[par_num];
  double normGrad = 0.0;
  while((!finished)&&(itCount<itMax)){

    // Normalize Gradient
    normGrad = 0.0;
    for(int loopA=0;loopA<par_num;loopA++){
      normGrad += grad[loopA] * grad[loopA];
    }
    normGrad = sqrt(normGrad);
    for(int loopA=0;loopA<par_num;loopA++){
      normGradVec[loopA] = grad[loopA] / normGrad;
    }

    printf("ITERATION: %d\n",itCount);

    // Update Parameter Location
    for(int loopA=0;loopA<par_num;loopA++){
      endPoint[loopA] = startPoint[loopA] - stepLength * normGradVec[loopA] * (limits[2*loopA + 1] - limits[2*loopA + 0]);
    }

    // Update Gradient Computation
    updateGradient(par_num,endPoint,numComponentsToUpdate,stepSizeCoeff,grad,fnew);

    // Check and update
    printf("fnew: %f, fold: %f\n",fnew,fold);
    if(fnew>fold){
      for(int loopA=0;loopA<par_num;loopA++){
        endPoint[loopA] = startPoint[loopA] + stepLength * normGradVec[loopA] * (limits[2*loopA + 1] - limits[2*loopA + 0]);        
      }
      stepLength = stepLength/2.0;
    }

    // Check If Finished 
    distNorm = 0.0;
    currRatio = 0.0;
    stdVec limits;
    model->getParameterLimits(limits);    
    for(int loopA=0;loopA<par_num;loopA++){
      if(fabs(limits[2*loopA + 1] - limits[2*loopA + 0])<kMathZero){
        currRatio = 0.0;
      }else{
        currRatio = (startPoint[loopA] - endPoint[loopA])/(limits[2*loopA + 1] - limits[2*loopA + 0]);
      }
      distNorm += currRatio * currRatio;
    }
    distNorm = sqrt(distNorm);
    finished = (distNorm < normTol);
    printf("DISTNORM: %f\n",distNorm);
    getchar();

    // Update Parameter Location
    if(!finished){
      for(int loopA=0;loopA<par_num;loopA++){
        startPoint[loopA] = endPoint[loopA];
      }
    }else{
      printf("FINISHED: distNorm: %f\n",distNorm);
    }

    // Update Function Value
    fold = fnew;

    // Update Iteration Count
    itCount++;
  }

  // Copy the Minimum Value
  for(int loopA=0;loopA<par_num;loopA++){
    xmin[loopA] = startPoint[loopA];
  }

  
  
}

// FUNCTION TO RUN THE MODEL
double acActionOPT_RSSD::runModel(double* params){
  
  // SET HEART MODEL PARAMETERS
  int par_num = model->getParameterTotal();

  // CHECK LIMITS
  stdVec limits;
  model->getParameterLimits(limits);

  // Copy Inputs
  stdVec inputs;
  stdVec outputs;
  double valToInsert = 0.0;
  for(int loopA=0;loopA<par_num;loopA++){
    // CHECK LIMITS
    if(params[loopA]<limits[2*loopA + 0]){
      // Smaller than Min
      valToInsert = limits[2*loopA + 0] + (limits[2*loopA + 0] - params[loopA]);
    }else if(params[loopA]>limits[2*loopA + 1]){
      // Bigger than Max
      valToInsert = limits[2*loopA + 1] - (params[loopA] - limits[2*loopA + 1]);
    }else{
      valToInsert = params[loopA];
    }
    if(fabs(limits[2*loopA + 1] - limits[2*loopA + 0])<kMathZero){
      valToInsert = limits[2*loopA + 1];
    }
    params[loopA] = valToInsert;
    // Add to Inputs
    //printf("Original %f, New %f\n",params[loopA],valToInsert);
    inputs.push_back(valToInsert);
  }

  stdIntVec errorCodes;
  double ynewlo = model->evalModelError(inputs,outputs,errorCodes);
  if(errorCodes[0] == 0){
    printf("   LL =  %f\n",ynewlo);
    return ynewlo;  
  }else{
    printf("   ERROR!");
    return 10000.0;
  }
}

// CONSTRUCTOR
acActionOPT_RSSD::acActionOPT_RSSD(double locStepSizeCoeff,
                                   int locItMax,
                                   double locStepLength,
                                   double locNormTol,
                                   int locNumComponentsToUpdate,
                                   bool locInitialPointFromFile,
                                   bool locStartFromCenter,
                                   string locInitialPointFile){
  // Copy Info
  stepSizeCoeff = locStepSizeCoeff;
  itMax = locItMax;
  stepLength = locStepLength;
  normTol = locNormTol;
  numComponentsToUpdate = locNumComponentsToUpdate;
  initialPointFromFile = locInitialPointFromFile;
  startFromCenter = locStartFromCenter;
  initialPointFile = locInitialPointFile;

}

// ===================================
// WRITE OPTIMAL PARAMETER SET TO FILE
// ===================================
void writeOptimumToFile_RSSD(string fileName,int par_num,double* opt){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  for(int loopA=0;loopA<par_num;loopA++){
    fprintf(f,"%f\n",opt[loopA]);
  }
  fclose(f);	
}

// ===========================
// WRITE OPTIMAL VALUE TO FILE
// ===========================
void writeOptimumValueToFile_RSSD(string fileName,double optVal){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  fprintf(f,"%e\n",optVal);
  fclose(f);  
}

// ============================================================
// OPTIMIZE LPN MODEL WITH RANDOMIZED SUBSPACE STEEPEST DESCENT
// ============================================================
void acActionOPT_RSSD::LPN_OPT_RSSD(){

  // Declare
  double reqmin;
  double *start;
  double *step;
  double *xmin;
  double ynewlo;

  // Get parameter number from model
  int par_num = model->getParameterTotal();

  // Allocate
  start = new double[par_num];
  step = new double[par_num];
  xmin = new double[par_num];

  // GET STARTING PARAMETER SET
  if(initialPointFromFile){
  	// Read Parameters From File
  	printf("\n");
  	printf(" READING PARAMETERS FROM FILE: %s\n",initialPointFile.c_str());
  	int error = cmUtils::readParamsFromFile(initialPointFile,start);
    if(error != 0){
      exit(1);
    }
  }else if (startFromCenter){
    // Start From the Center of the Parameter Space
    printf("\n");
    printf(" USING CENTRE PARAMETERS\n");
    stdVec currLimits;
    model->getParameterLimits(currLimits);
    for(int loopA=0;loopA<par_num;loopA++){
      start[loopA] = 0.5 * (currLimits[2*loopA + 0] + currLimits[2*loopA + 1]);
    }
  }else{
  	// Use Default Parameter Values    
  	printf("\n");
  	printf(" USING DEFAULT PARAMETERS\n");
    stdVec tempStart;
    model->getDefaultParams(tempStart);
    for(int loopA=0;loopA<par_num;loopA++){
      start[loopA] = tempStart[loopA];
    }
  }
  
  // PRINT STARTING PARAMETER SET
  cout << "\n";
  cout << "  Starting Parameter Set:\n";
  cout << "\n";
  for (int loopA=0;loopA<par_num;loopA++){
    cout << "  " << setw(14) << start[loopA] << "\n";
  }

  // ===========================
  // EVAL INITIAL FUNCTION VALUE
  // ===========================
  // Copy Inputs
  stdVec inputs;
  stdVec outputs;
  for(int loopA=0;loopA<par_num;loopA++){
    inputs.push_back(start[loopA]);
  }
  stdIntVec errorCodes;
  ynewlo = model->evalModelError(inputs,outputs,errorCodes);

  // PRINT FUNCTION VALUE FOR INITIAL POINT
  cout << "\n";
  cout << "  F(X) = " << ynewlo << "\n";

  // RUN NELDER-MEAD OPTIMIZATION
  perform_RSSD(par_num, start, xmin, ynewlo);

  // PRINT OUTPUT REPORT
  cout << "\n";
  cout << "  Estimate of minimizing value X*:\n";
  cout << "\n";
  for (int loopA=0;loopA<par_num;loopA++){
    cout << "  " << setw(14) << xmin[loopA] << "\n";
  }
  cout << "\n";
  cout << "  F(X*) = " << ynewlo << "\n";
  cout << "\n";

  // WRITE OPTIMAL PARAMETER SET TO FILE
  writeOptimumToFile_RSSD("optParams.txt",par_num,xmin);
  writeOptimumValueToFile_RSSD("optValue.txt",ynewlo);

  // FREE MEMORY
  delete [] start;
  delete [] step;
  delete [] xmin;

  return;
}

// =====================
// MAIN PROGRAM FUNCTION
// =====================
int acActionOPT_RSSD::go(){

  uqUtils::printTimestamp();
  cout << "\n";
  cout << "OPT_NM:\n";

  // SET OPTIONS
  printf("\n");
  printf("  RANDOMIZED SUBSPACE STEEPEST DESCENT\n");
  printf("  -------------------\n");
  printf("  Convergence Tolerance: %e\n",normTol);
  printf("  Number of components in subspace: %d\n",numComponentsToUpdate);
  printf("  Maximum Iterations: %d\n",itMax);
  printf("  Step Length Coefficient: %.5f\n",stepLength);
  printf("  Step Size Coefficient: %.5f\n",stepSizeCoeff);
  printf("  -------------------\n");
  printf("  Use Initial Location File: %s\n",initialPointFromFile ? "True":"False");
  printf("  Start From Center: %s\n",startFromCenter ? "True":"False");
  printf("  Initial Location File Name: %s\n",initialPointFile.c_str());

  // RUN OPTIMIZATION CODE
  LPN_OPT_RSSD();  
  
  // PRINT END OF EXECUTION
  cout << "\n";
  cout << "RSSD:\n";
  cout << "  Normal end of execution.\n";
  cout << "\n";
  uqUtils::printTimestamp();

  return 0;
}
