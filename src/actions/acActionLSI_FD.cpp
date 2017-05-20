// =======================================
// EVALUATION OF LOCAL SENSITIVITY INDEXES
// Daniele Schiavazzi, 2014
// =======================================

# include "acActionLSI_FD.h"

using namespace std;

// ===========
// CONSTRUCTOR
// ===========
acActionLSI_FD::acActionLSI_FD(bool specifyParamLocation,string initialParamFile,double perturbFactor,bool evalDeriv){
  this->specifyParamLocation = specifyParamLocation;
  this->initialParamFile = initialParamFile;
  this->perturbFactor = perturbFactor;
  this->evalDeriv = evalDeriv;
}

// ===============================
// WRITE SENSITIVITY TABLE TO FILE
// ===============================
void acActionLSI_FD::WriteSentitivityTableToFile(string fileName,int par_num,int res_num,vector<vector<double> > &sensTable){
  // WRITE RESULTS TO FILE
  FILE* stateFile;
  stateFile = fopen(fileName.c_str(),"w");
  // PRINT THE COLUMN HEADER FOR THE RESULT FILE
  string parName;
  string resName;

  fprintf(stateFile,"%15s ","Param");
  for(int loopA=0;loopA<res_num;loopA++){
    resName = model->getResultName(loopA);
    fprintf(stateFile,"%15s ",resName.c_str());
  } 
  fprintf(stateFile,"\n");
  for(int loopA=0;loopA<par_num;loopA++){
    // First Column Parameter Name
    parName = model->getParamName(loopA);
    fprintf(stateFile,"%15s ",parName.c_str());
    for(int loopB=0;loopB<res_num;loopB++){
      fprintf(stateFile,"%15.3f ",sensTable[loopA][loopB]);
    }
    fprintf(stateFile,"\n");
  }

  // Close State File
  fclose(stateFile);
}


// =========================
// GENERIC CALL TO THE MODEL
// =========================
int acActionLSI_FD::solveModel(int par_num, double* allParams, int res_num, double* results){

  // Call to LPN function evaluation
  int error = 0;
  double ll = 0.0;
  stdVec inputs;
  stdVec outputs;
  stdIntVec errorCode;
  try{
    for(int loopA=0;loopA<par_num;loopA++){
      inputs.push_back(allParams[loopA]);
    }    
    ll = model->evalModelError(inputs,outputs,errorCode);
    for(int loopA=0;loopA<res_num;loopA++){
      results[loopA] = outputs[loopA];
    }
    error = errorCode[0];
  }catch(...){
    error = 1;
  }
  // Return
  return error;
}


// =======================================
// MAIN FUNCTION - LOCAL SENSITIVITY INDEX
// =======================================
int acActionLSI_FD::go(){

  // Get Model Limits
  int par_num = model->getParameterTotal();
  int res_num = model->getResultTotal();

  // Init Sensitivity Table
  vector<vector<double> > sensTable(par_num,vector<double>(res_num));
  for(int loopA=0;loopA<par_num;loopA++){
  	for(int loopB=0;loopB<res_num;loopB++){
      sensTable[loopA][loopB] = 0.0;
  	}
  }

  // Get Model Limits
  stdVec limits;
  model->getParameterLimits(limits);
  double paramRanges[par_num];
  for(int loopA=0;loopA<par_num;loopA++){
    paramRanges[loopA] = limits[loopA*2+1] - limits[loopA*2+0];
  }

  // Specify Starting Parameter Location
  double centreParams[par_num];
  stdVec refParams;
  double perturbParams[par_num];

  // Read Parameter Location from File if required
  if(specifyParamLocation){
    // Read Parameter Starting Location From File
    std::string inputFileName(initialParamFile);
    printf("READING PARAMETERS FROM FILE %s.\n",inputFileName.c_str());    
    ReadParamsFromFile(inputFileName,centreParams);    
  }else{
    // Start From The Default Parameters
    printf("USING DEFAULT PARAMETERS.\n");
    model->getDefaultParams(refParams);
    for(int loopA=0;loopA<par_num;loopA++){
      centreParams[loopA] = refParams[loopA];
      printf("Params %d, Value: %e.\n",loopA,centreParams[loopA]);
    }
  }

  // INIT RESULTS
  double centerRes[res_num];
  double perturbRes[res_num];
  
  // SOLVE THE MODEL FOR THE CENTRE POINT  
  int error = solveModel(par_num,centreParams,res_num,centerRes);
  if(error != 0){
    printf("Error: Invalid Central Solution Point. Exiting.\n");
    exit(1);
  }

  // LOOP THROUGH THE PARAMETERS
  for(int loopA=0;loopA<par_num;loopA++){

    // Copy Center Point
    for(int loopB=0;loopB<par_num;loopB++){
      perturbParams[loopB] = centreParams[loopB];
    }
    
    // PERTURB PARAM LOCATION
    // % of its value not range !!!!
    double currentPert = perturbFactor*perturbParams[loopA];
    if(fabs(currentPert)>1.0e-8){
      perturbParams[loopA] += currentPert;
    }else{
      perturbParams[loopA] += perturbFactor*paramRanges[loopA];
    }
    
    // SOLVE PERTURBED MODEL
    int error = solveModel(par_num,perturbParams,res_num,perturbRes);    
    if(error != 0){
      string paramName = model->getParamName(loopA);
      printf("Error: Invalid Solution Point for param %s. Exiting.\n",paramName.c_str());
      exit(1);
    }

    // SAVE ALL RESULTS FOR THIS PARAMETER PERTURBATION
    for(int loopB=0;loopB<res_num;loopB++){
      if(evalDeriv){
        if(fabs(currentPert) > 1.0e-8){
          sensTable[loopA][loopB] = ((perturbRes[loopB]-centerRes[loopB])/(currentPert));
        }else{
          sensTable[loopA][loopB] = 0.0;
        }
      }else{
        if(fabs(centerRes[loopB])>1.0e-8){
          sensTable[loopA][loopB] = fabs(((perturbRes[loopB]-centerRes[loopB])/(centerRes[loopB]))*100.0);  
        }else{
          sensTable[loopA][loopB] = 0.0;
        }
      }      
    }
  }

  if(!evalDeriv){
    // POST PROCESS SENSITIVITY TABLE AND PRINT
    // NORMALIZE COLUMNS
    double sum = 0.0;
    for(int loopA=0;loopA<res_num;loopA++){
      // GET SUM OF COLUMNS
      sum = 0.0;
      for(int loopB=0;loopB<par_num;loopB++){
        sum += sensTable[loopB][loopA];
      }
      // NORMALIZE
      for(int loopB=0;loopB<par_num;loopB++){
        if(fabs(sum)>1.0e-8){
          sensTable[loopB][loopA] = sensTable[loopB][loopA]*(100.0/(double)sum);  
        }else{
          sensTable[loopB][loopA] = 0.0;
        }   
      }
    }
  }

  // PRINT SENSITIVITY TABLE
  WriteSentitivityTableToFile("LSI_Table.txt",par_num,res_num,sensTable);

  // Return No Error
  return 0;
}
