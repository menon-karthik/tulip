
# include "acAction_OBJProfile.h"

// CONSTRUCTOR
acAction_OBJProfile::acAction_OBJProfile(int locTotalProfileSteps,
                                         bool locInitialPointFromFile,
                                         bool locStartFromCenter,
                                         string locInitialPointFile){
  totalProfileSteps = locTotalProfileSteps;
  initialPointFromFile = locInitialPointFromFile;
  startFromCenter = locStartFromCenter;
  initialPointFile = locInitialPointFile;
}

// FUNCTION TO RUN THE MODEL
double acAction_OBJProfile::runModel(double* params){
  
  // SET HEART MODEL PARAMETERS
  int par_num = model->getParameterTotal();

  // Copy Inputs
  stdVec inputs;
  stdVec outputs;
  double valToInsert = 0.0;
  double ynewlo = 0.0;
  for(int loopA=0;loopA<par_num;loopA++){
    inputs.push_back(params[loopA]);
  }

  stdIntVec errorCodes;
  try{
    ynewlo = model->evalModelError(inputs,outputs,errorCodes);
  }catch(exception& e){
  	ynewlo = -1000.0;
  }

  if(errorCodes[0] == 0){
    printf("   LL =  %f\n",ynewlo);
    return ynewlo;  
  }else{
    printf("   ERROR!");
    return 10000.0;
  }
}

// PRINT RESULT MATRIX TO FILE
void printResultMatrix(string fileName,stdMat matrix){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  for(int loopA=0;loopA<matrix.size();loopA++){
  	for(int loopB=0;loopB<matrix[loopA].size();loopB++){
      fprintf(f,"%f ",matrix[loopA][loopB]);
  	}
  	fprintf(f,"\n");
    
  }
  fclose(f);	
}


// PERFORM ACTION
int acAction_OBJProfile::OBJ_Profile(){

  // CHECK LIMITS
  stdVec limits;
  model->getParameterLimits(limits);

  // SET HEART MODEL PARAMETERS
  int par_num = model->getParameterTotal();

  // Allocate
  double start[par_num];    

  // GET STARTING PARAMETER SET
  if(initialPointFromFile){
  	// Read Parameters From File
  	printf("\n");
  	printf(" READING PARAMETERS FROM FILE: %s\n",initialPointFile.c_str());
  	int error = cmUtils::readParamsFromFile(initialPointFile,start);
    if(error != 0){
      exit(1);
    }
  }else if(startFromCenter){
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

  // Loop on the main variables
  double currParams[par_num];
  stdMat resultMatrix;
  stdVec tempRes;
  double currParValue = 0.0;
  for(int loopA=0;loopA<par_num;loopA++){

  	printf("PROFILING DIMENSION: %d\n",loopA+1);

    // Copy the parameter Vector 
  	for(int loopB=0;loopB<par_num;loopB++){
      currParams[loopB] = start[loopB];
  	}  	

  	// Run Simulations
    tempRes.clear();
  	for(int loopB=0;loopB<totalProfileSteps;loopB++){
      currParValue = limits[2*loopA + 0] + ((double)loopB/(double)(totalProfileSteps-1))*(limits[2*loopA + 1] - limits[2*loopA + 0]);
      currParams[loopA] = currParValue;      
      tempRes.push_back(runModel(currParams));      
  	}
    resultMatrix.push_back(tempRes);
  }

  // Print Result Matrix
  string fileName("OBJ_profiling.dat");
  printResultMatrix(fileName,resultMatrix);

  // Return
  return 0;
}

// =====================
// MAIN PROGRAM FUNCTION
// =====================
int acAction_OBJProfile::go(){

  timestamp ( );
  cout << "\n";
  cout << "OBJ_Profiling:\n";

  // SET OPTIONS
  printf("\n");
  printf("  OBJECT FUNCTION PROFILING\n");
  printf("  -------------------\n");
  printf("  Total Steps: %d\n",totalProfileSteps);
  printf("  Initial Point From File: %s\n",initialPointFromFile ? "True" : "False");
  printf("  Initial Point File Name: %s\n",initialPointFile.c_str());
  printf("  Start From Center: %s\n",startFromCenter ? "True" : "False");
  printf("  -------------------\n");

  // RUN OPTIMIZATION CODE
  OBJ_Profile();  
  
  // PRINT END OF EXECUTION
  cout << "\n";
  cout << "OBJ_Profiling:\n";
  cout << "  Normal end of execution.\n";
  cout << "\n";
  timestamp ( );

  return 0;
}

