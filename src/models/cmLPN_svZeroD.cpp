#include "cmLPN_svZeroD.h"

using namespace std;

// ========================
// CONSTRUCTOR
// ========================
cmLPN_svZeroD::cmLPN_svZeroD(std::string model_path, svZeroDModel* model){
  
  //TODO: Maybe interface should be created in svZeroD model? Or it at least needs to be accessible by the model
  //TODO: Try to handle all interface bookkeeping here and pass interface to model for specific things
  // Load shared library and get interface functions.
  auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release/src/interface/libsvzero_interface_library.so");
  this->interface.load_library(interface_lib);
  this->interface.initialize(model_path);
  this->nUnknowns = this->interface.system_size_;

  // Save initial state
  this->init_state_y.resize(this->nUnknowns);
  this->init_state_ydot.resize(this->nUnknowns);
  this->interface.return_y(init_state_y);
  this->interface.return_ydot(init_state_ydot);
  
  // Set svZeroD model
  this->zeroDmodel = model;
  this->zeroDmodel->setup_model(interface);
}

// ========================
// DESTRUCTOR
// ========================
cmLPN_svZeroD::~cmLPN_svZeroD(){
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int cmLPN_svZeroD::getParameterTotal(){
  //return 38;
  return this->zeroDmodel->getParameterTotal();
}

// ===================================
// GET NUMBER OF PARAMETERS (UNKNOWNS)
// ===================================
int cmLPN_svZeroD::getStateTotal(){
  //return nUnknowns; 
  return this->zeroDmodel->getAuxTotal();
}

int cmLPN_svZeroD::getAuxStateTotal(){
  //return 50;
  return this->zeroDmodel->getAuxTotal();
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int cmLPN_svZeroD::getResultTotal(){
  //return 29;  
  return this->zeroDmodel->getResultTotal();
}

// ==================
// GET PARAMETER NAME
// ==================
string cmLPN_svZeroD::getParamName(int index){
  return this->zeroDmodel->getParamName(index);
}

// ===============
// GET RESULT NAME
// ===============
string cmLPN_svZeroD::getResultName(int index){
  return this->zeroDmodel->getResultName(index);
}

// ====================
// GET MODEL PARAMETERS
// ====================
void cmLPN_svZeroD::getDefaultParams(stdVec& zp){
     this->zeroDmodel->getDefaultParams(zp); 
}

// ====================
// GET PARAMETER RANGES
// ====================
void cmLPN_svZeroD::getParameterLimits(stdVec& limits){
  this->zeroDmodel->getParameterLimits(limits);
}

// ====================
// GET DEFAULT PARAMETER RANGES
// ====================
void cmLPN_svZeroD::getDefaultParameterLimits(stdVec& limits) {
  this->zeroDmodel->getDefaultParameterLimits(limits);
}

// ====================
// PRINT RESULTS
// ====================
// TODO: Xn = results should be std::vector?
void cmLPN_svZeroD::printResults(int totalResults, double* Xn) {
  printf("RESULT PRINTOUT\n");
  for(int loopA = 0; loopA < totalResults; loopA++) {
    string s = getResultName(loopA);
    printf("%20s : %f\n",s.c_str(),Xn[loopA]);
  }
}

// ==========================================
// MAIN FUNCTION FOR STAGE1-2BLOCKS LPN MODEL
// ==========================================
int cmLPN_svZeroD::solveLPN(double* params, double* results){

  int totalStates = getStateTotal();
  int totOutputSteps = interface.num_output_steps_;
  // TODO: Change outvals to stdMat type
  // State Variables
  double** outVals = NULL;
  outVals = new double*[totalStates];
  for(int loopA=0;loopA<totalStates;loopA++){
    outVals[loopA] = new double[totOutputSteps];
  }
  
  this->zeroDmodel->setModelParams(interface, params)
  
  // Set up solution and time vectors, and run simulation
  std::vector<double> solutions(interface.system_size_*interface.num_output_steps_);
  std::vector<double> times(interface.num_output_steps_);
  int error_code = 0;
  interface.update_state(init_state_y, init_state_ydot);
  interface.run_simulation(0.0, times, solutions, error_code);
  //std::cout << "[solveCoronaryLPN] error_code: " << error_code << std::endl;

  // Parse the solution vector
  int state, step;
  double t[interface.num_output_steps_];
  for (step = 0; step < totOutputSteps; step++) {
    t[step] = times[step];
  }
  int sol_idx;
  for (state = 0; state < totalStates; state++) {
    for (step = 0; step < totOutputSteps; step++) {
      sol_idx = step*totalStates + state;
      outVals[state][step] = solutions[sol_idx];
    }
  }

  if(error_code != 0) {
    for(int loopA=0;loopA<totalStates;loopA++){
      delete [] outVals[loopA];
    }
    delete [] outVals;
    return 1;
  }

  // TODO: results passed by ref/pointer
  this->zeroDmodel->postprocess(interface, outVals, auxOutVals, results)

  // FREE MEMORY
    for(int loopA=0;loopA<totalStates;loopA++){
      delete [] outVals[loopA];
    }
  delete [] outVals;

  //std::cout << "[solveCoronaryLPN] END " << std::endl;
  // Solution Successful
  return 0;
}

// =========================
// EVAL MODEL ERROR FUNCTION
// =========================
double cmLPN_svZeroD::evalModelError(const stdVec& inputs, stdVec& outputs, stdIntVec& errorCode) {

  int model = 0;

  int totalParams = getParameterTotal();
  int resultTotal   = getResultTotal();

  // Create local copies of input parameters
  double paramsVals[totalParams];
  for(int loopA=0;loopA<totalParams;loopA++){
    paramsVals[loopA] = inputs[loopA];
  }
  
  // Results
  double results[resultTotal];
  for(int loopA=0;loopA<resultTotal;loopA++){
    results[loopA] = 0.0;
  }

  // Solve coronary model
  int error = 0;
  try{
    error = solveLPN(paramsVals,results);
  }catch(...){
    error = 1;
  }
  errorCode.push_back(error);

  // Store Results
  outputs.clear();
  for(int loopA=0;loopA<getResultTotal();loopA++){
    outputs.push_back(results[loopA]);
  }

  //std::cout << "[evalModelError] 3 " << std::endl;
  // KEYS
  vector<string> keys;
  this->zeroDmodel->getResultKeys(keys);

  stdVec computedValues;
  for(int i = 0; i < resultTotal; i++) {
     computedValues.push_back(results[i]);
  }

  stdVec stdFactors;
  this->zeroDmodel->getDataStd(stdFactors);
  
  stdVec weights;
  this->zeroDmodel->getResultWeights(weights);

  // Print and compare
  double result = 0.0;
  if(data != NULL){

    // Print Info
    //data->printAndCompare(datasetColumn,keys,computedValues,weigths);
    data->printAndCompare(keys,computedValues,weigths);
    // Evaluate Log Likelihood

    //std::cout << "[evalModelError] 6 " << std::endl;
    //result = data->evalLogLikelihood(datasetColumn,keys,computedValues,stdFactors,weigths);
    result = data->evalLogLikelihood(keys,computedValues,stdFactors,weigths);
  }

  //std::cout << "[evalModelError] END " << std::endl;
  return result;
}

void cmLPN_svZeroD::getPriorMapping(int priorModelType,int* prPtr) {
  this->zeroDmodel->getProrMapping(priorModelType, prPtr);
}

// ==================
// SAVE ALL DATA FILE
// ==================
void writeAllDataFile(int totalSteps,int totalStates,int totAuxStates,double** outVals,double** auxOutVals){
  FILE* outFile;
  outFile = fopen("AllData","w");
  for(int loopA=0;loopA<totalSteps;loopA++){
    // Print State Variables
    for(int loopB=0;loopB<totalStates;loopB++){
      fprintf(outFile,"%e ",outVals[loopB][loopA]);
    }
    // Print Aux Variables
    for(int loopB=0;loopB<totAuxStates;loopB++){
      fprintf(outFile,"%e ",auxOutVals[loopB][loopA]);
    }
    fprintf(outFile,"\n");
  }
  // CLOSE THE FILE
  fclose(outFile);
}