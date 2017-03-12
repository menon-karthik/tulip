# include "odeRCR2.h"

// ===========
// CONSTRUCTOR
// ===========
odeRCR2::odeRCR2(stdMat& flowRate,double heartCycleTime,int totalStepsOnSingleCycle,int numHeartCycles){
  // Model Parameters
  data = NULL;
  this->heartCycleTime = heartCycleTime;
  this->totalStepsOnSingleCycle = totalStepsOnSingleCycle;
  this->numHeartCycles = numHeartCycles;
  // Copy flow Rate Table
  stdVec temp;
  for(int loopA=0;loopA<flowRate.size();loopA++){
    temp.clear();
    for(int loopB=0;loopB<flowRate[0].size();loopB++){
      temp.push_back(flowRate[loopA][loopB]);
    }
    inletFlowRate.push_back(temp);
  }
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void rcrODE(double tn, double* Xn, double* params, double* Xn1,double QIN,double* out){

  // INITIALIZE VARIABLES
  // STORE LOCAL COPIES OF PARAMETERS
  double totRes       = params[0];
  double resRatio     = params[1];
  double Cp           = params[2];
  double distPressure = params[3];
  
  // Compute Local Quantities
  double R1 = resRatio * totRes;
  double R2 = (1.0 - resRatio) * totRes;
  
  // Define distal Pressure
  double P2 = distPressure;

  // UNPACK STATE VARIABLES
  double P1 = Xn[0]; // Pressure in 1

  // Eval other quantities
  double P0 = P1 + QIN*R1;
  double Q2 = (P1 - P2)/R2;
  // Eval Derivatives
  double P1p = (QIN-Q2)/(double)Cp;
  
  // STORE TIME DERIVATIVES
  Xn1[0] = P1p;

  // STORE SECONDARY OUTPUT
  out[0] = P0;
  out[1] = P2;
  out[2] = QIN;
  out[3] = Q2;
}

// ====================
// PRINT VECTOR TO FILE
// ====================
void printPressureToFile(string fileName,int totalSteps,double timeStep,double* press){
  FILE* file;
  file = fopen(fileName.c_str(),"w");
  for(int loopA=0;loopA<totalSteps;loopA++){
    fprintf(file,"%f %f\n",loopA*timeStep,press[loopA]/1333.22);
  }
  // Close State File
  fclose(file);
}

// EVAL FORCING TERM
void evalFlowRate(double currTime,stdMat inletFlowRate,double &currQ){
  double flowRateDuration = inletFlowRate[inletFlowRate.size()-1][0];
  double perTime = fmod(currTime,flowRateDuration);
  currQ = linInterp(inletFlowRate,0,1,perTime);
}

// =============================
// EVAL MODEL FOR RK4 ITERATIONS
// =============================
void evalRCRModel(double t,int totalStates,
                  double* Xk,double* params,double* DXk,
                  double QIN,double* auxOut){
  // SOLVE HEART MODEL
  rcrODE(t,Xk,params,DXk,QIN,auxOut);
}

// ==================
// SOLVE ODE WITH RK4
// ==================
int PerformRK4Steps(int totalSteps,double timeStep,int totalStates,int totAuxStates,
                    double* iniVals,int totalParams, double* params,
                    stdMat inletFlowRate,
                    double** outVals,double** auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = 0;
  const bool printIC = false;
  bool isValidDouble;

  // Allocate Temporary Variables
  double k1[totalStates];
  double k2[totalStates];
  double k3[totalStates];
  double k4[totalStates];
  double k1AuxOut[totAuxStates];
  double k2AuxOut[totAuxStates];
  double k3AuxOut[totAuxStates];
  double k4AuxOut[totAuxStates];
  double Xk2[totalStates];
  double Xk3[totalStates];
  double Xk4[totalStates];
  // Forcing Terms
  double currQ1;
  double currQ2;
  double currQ4;

  // Initialize State Vectors and Copy Initial Conditions
  double Xn[totalStates];
  double Xn1[totalStates];
  for(int loopA=0;loopA<totalStates;loopA++){
    Xn[loopA] = iniVals[loopA];
  }  

  // TIME LOOP
  for(int loopA=0;loopA<totalSteps;loopA++){
    // Increment Time Step
    stepId++;    

    // Update Current Time
    if(loopA>0){
      currTime += timeStep;
    }

    // Eval K1
    evalFlowRate(currTime,inletFlowRate,currQ1);
    evalRCRModel(currTime,totalStates,Xn,params,k1,currQ1,k1AuxOut);

    // Eval K2
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk2[loopB] = Xn[loopB] + (0.5*timeStep) * k1[loopB];
    }
    evalFlowRate(currTime + 0.5 * timeStep,inletFlowRate,currQ2);
    evalRCRModel(currTime + 0.5 * timeStep,totalStates,Xk2,params,k2,currQ2,k2AuxOut);
    // Eval K3
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk3[loopB] = Xn[loopB] + (0.5*timeStep) * k2[loopB];
    }
    evalRCRModel(currTime + 0.5 * timeStep,totalStates,Xk3,params,k3,currQ2,k3AuxOut);
    // Eval K4
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk4[loopB] = Xn[loopB] + timeStep * k3[loopB];
    }
    evalFlowRate(currTime + timeStep,inletFlowRate,currQ4);
    evalRCRModel(currTime + timeStep,totalStates,Xk4,params,k4,currQ4,k4AuxOut);

    // Eval Xn1
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn1[loopB] = Xn[loopB] + (1.0/6.0)*timeStep*(k1[loopB] + 2.0 * k2[loopB] + 2.0 * k3[loopB] + k4[loopB]);
    }

    // CHECK IF SOLUTION IS VALID
    if((loopA % 100) == 0){
      for(int loopB=0;loopB<totalStates;loopB++){
        isValidDouble = (Xn[loopB] != Xn[loopB]);
        if(isValidDouble){
          return 1;
        }
      }
    }

    // Update Xn
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn[loopB] = Xn1[loopB];     
    }

    // Copy back k1AuxOut for each time step
    for(int loopB=0;loopB<totAuxStates;loopB++){
      auxOutVals[loopA][loopB] = k1AuxOut[loopB];
    }

    // Copy The solution back for each time step
    for(int loopB=0;loopB<totalStates;loopB++){
      outVals[loopA][loopB] = Xn1[loopB];
    }
  }
  // RETURN OK
  return 0;
}

// ================================
// MAIN FUNCTION FOR HEART SUBMODEL
// ================================
int odeRCR2::solveRCR(double timeStep, int totalSteps, int totalParams, double* params, double* results){

  // TIME STEP ARRAY
  double t[totalSteps];
  t[0] = 0.0;
  for(int loopA=1;loopA<totalSteps;loopA++){
    t[loopA] = t[loopA-1] + timeStep;
  }  

  // Initial Condition on pressure in P1
  double* iniVals;
  iniVals = new double[1];
  //iniVals[0] = params[4];
  // Test: Use always the distal pressure as initial pressure
  iniVals[0] = params[3];

  // NUMBER OF AUXILIARY VARIABLES
  int totAuxStates = 4;

  // NUMBER OF RESULTS
  int totalResults = getResultTotal();
  int totalStates = getStateTotal();

  // State Variables
  double** outVals = NULL;
  outVals = new double*[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    outVals[loopA] = new double[totalStates];
  }

  // Auxiliary Outputs
  double** auxOutVals;
  auxOutVals = new double*[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    auxOutVals[loopA] = new double[totAuxStates];
  }

  // Solve All Cycles
  int error = PerformRK4Steps(totalSteps,timeStep,totalStates,totAuxStates,iniVals,
                              totalParams,params,
                              inletFlowRate,
                              outVals,auxOutVals);
  if(error !=0){
    printf("ERROR - SOLUTION DIVERGED\n");
    // FREE MEMORY
    for(int loopA=0;loopA<totalSteps;loopA++){
      delete [] outVals[loopA];
      delete [] auxOutVals[loopA];
    }
    delete [] outVals;
    delete [] auxOutVals;     
    delete [] iniVals;
    // RETURN
    return 1;
  }

  // COPY FINAL RESULT: P0 Pressure
  double inletPressure[totalSteps];
  //for(int loopA=(totalSteps-totalStepsOnSingleCycle-1);loopA<totalSteps;loopA++){
  for(int loopA=0;loopA<totalSteps;loopA++){
    //inletPressure[loopA-(totalSteps-totalStepsOnSingleCycle-1)] = auxOutVals[loopA][0];
    inletPressure[loopA] = auxOutVals[loopA][0];
  }

  string pressureFileName("pressOutput.dat");
  //printPressureToFile(pressureFileName,totalStepsOnSingleCycle,timeStep,inletPressure);
  printPressureToFile(pressureFileName,totalSteps,timeStep,inletPressure);

  // GET STATISTICS
  results[0] = getMin( totalSteps-totalStepsOnSingleCycle, totalSteps, inletPressure);
  results[1] = getMean(totalSteps-totalStepsOnSingleCycle, totalSteps, inletPressure);
  results[2] = getMax( totalSteps-totalStepsOnSingleCycle, totalSteps, inletPressure);

  // FREE MEMORY
  for(int loopA=0;loopA<totalSteps;loopA++){
    delete [] outVals[loopA];
    delete [] auxOutVals[loopA];
  }
  delete [] outVals;
  delete [] auxOutVals;
  delete [] iniVals;  

  // Solution Successful
  return 0;
}

int odeRCR2::getParameterTotal(){
  return 5;
}

string odeRCR2::getParamName(int parID){
  if(parID == 0){
    return string("ResTot");
  }else if(parID == 1){
    return string("ResRatio");
  }else if(parID == 2){
    return string("Capacitance");
  }else if(parID == 3){
    return string("DistPressure");
  }else{
    throw cmException("ERROR: Invalid Parameter ID in getParamName.\n");
  }
}

string odeRCR2::getResultName(int resID){
  if(resID == 0){
    return string("Min Inlet Pressure");
  }else if(resID == 1){
    return string("Mean Inlet Pressure");
  }else if(resID == 2){
    return string("Max Inlet Pressure");
  }else{
    throw cmException("ERROR: Invalid Parameter ID in getResultName.\n");
  }
}

// ==========================
// GET TOTAL NUMBER OF STATES
// ==========================
int odeRCR2::getStateTotal(){
  return 1;
}

// ================
// GET RESULT TOTAL
// ================ 
int odeRCR2::getResultTotal(){
  // Min,Av,Max Inlet Pressures
  return 3;
}

// ====================
// GET PARAMETER LIMITS
// ====================
void odeRCR2::getParameterLimits(stdVec& limits){
  limits.resize(8);
  limits[0] = 0.01;  limits[1] = 1000.0;  // TotRes
  limits[2] = 0.001; limits[3] = 1.0;  // resRatio
  limits[4] = 0.0000001;  limits[5] = 1.0; // Capacitance
  limits[6] = 0.0;   limits[7] = 50.0*1333.22;  // distPressure
}

// =========================
// GET DEFAULT PARAMETER SET
// =========================
void odeRCR2::getDefaultParams(stdVec& params){
  params.resize(4);
  params[0] = 100.0; // ResTot 
  params[1] = 0.8;// ResRatio
  params[2] = 0.001; // Capacitance 
  params[3] = 50.0*1333.22; // DistPressure 
}

// =================
// GET PRIOR MAPPING
// =================
void odeRCR2::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping not implemented.\n"); 
}

// ==========
// EVAL MODEL
// ==========
double odeRCR2::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
  // Read Input Quantities
  // Total Resistance
  double totRes      = inputs[0];
  // Ratio of proximal to distal resistance
  double resRatio    = inputs[1];
  // Model capacitance
  double capacitance = inputs[2];
  // Distal Pressure
  double rcrDistalPressure = inputs[3];

  // Time Step Settings
  double timeStep = heartCycleTime/(double)totalStepsOnSingleCycle;
  double totalSteps = totalStepsOnSingleCycle*numHeartCycles;
  
  // Total Parameters
  int totalParams = getParameterTotal();
  int totalResults = getResultTotal();

  // Set Parameters
  double* params = new double[totalParams];
  double* results = new double[totalResults];

  // Total Resistance
  params[0] = totRes;
  // Resistance Ratio
  params[1] = resRatio;
  // Capacitance
  params[2] = capacitance;
  // RCR Distal Pressure
  params[3] = rcrDistalPressure;

  // Solve MODEL
  int error = solveRCR(timeStep,totalSteps,totalParams,params,results);
  if(error != 0){
    throw cmException("ERROR: Cannot Solver RCR ODE Model.\n"); 
  }  

  // Return the results
  outputs.clear();
  for(int loopA=0;loopA<getResultTotal();loopA++){
    outputs.push_back(results[loopA]);
  }
  // Store Error
  errorCode.push_back(error);

  // SET RESULTS
  vector<string> keys;
  keys.push_back(string("DBP"));
  keys.push_back(string("mBP"));
  keys.push_back(string("SBP"));
  
  stdVec computedValues;
  computedValues.push_back(outputs[0]/1333.22);
  computedValues.push_back(outputs[1]/1333.22);
  computedValues.push_back(outputs[2]/1333.22);

  double stdRatio = 0.05;
  stdVec stdValues;
  stdValues.push_back(1.0);
  stdValues.push_back(1.0);
  stdValues.push_back(1.0);

  stdVec weigths;  
  weigths.push_back(1.0); // DBP
  weigths.push_back(1.0); // mBP
  weigths.push_back(1.0); // SBP

  // Print and compare
  double result = 0.0;
  if(data != NULL){
    
    // Print Info
    data->printAndCompare(datasetColumn,keys,computedValues,weigths);
  
    // Evaluate Objective Function
    result = data->evalLogLikelihood(datasetColumn,keys,computedValues,stdValues,weigths);
    // result = data->evalOBJ(datasetColumn,keys,computedValues,weigths);
  }

  // Delete Arrays
  delete [] params;
  delete [] results;

  // Return 
  return result;
}
