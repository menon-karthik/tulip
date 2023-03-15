#include "cmLPN_svZeroD.h"

using namespace std;

// ========================
// CONSTRUCTOR
// ========================
cmLPN_svZeroD::cmLPN_svZeroD(std::string model_path, svZeroDModel* model, std::string interface_lib){
  
  // Load shared library and get interface functions.
  //auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release/src/interface/libsvzero_interface_library.so");
  this->interface.load_library(interface_lib);
  this->interface.initialize(model_path);
  auto nUnknowns = this->interface.system_size_;

  // Save initial state
  this->init_state_y.resize(nUnknowns);
  this->init_state_ydot.resize(nUnknowns);
  this->interface.return_y(init_state_y);
  this->interface.return_ydot(init_state_ydot);
  
  // Set svZeroD model
  this->zeroDmodel = model;
  this->zeroDmodel->setupModel(interface);
}

// ========================
// DESTRUCTOR
// ========================
cmLPN_svZeroD::~cmLPN_svZeroD() {
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int cmLPN_svZeroD::getParameterTotal(){
  return this->zeroDmodel->getParameterTotal();
}

// ===================================
// GET NUMBER OF PARAMETERS (UNKNOWNS)
// ===================================
int cmLPN_svZeroD::getStateTotal(){
  return this->zeroDmodel->getStateTotal();
}

int cmLPN_svZeroD::getAuxStateTotal(){
  return this->zeroDmodel->getAuxStateTotal();
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int cmLPN_svZeroD::getResultTotal(){
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

  // Change the limits bases on the Fixed Parameter List
  int currParam = 0;
  for(size_t loopA=0;loopA<frozenParamIDX.size();loopA++){
    currParam = frozenParamIDX[loopA];
    // Assign the new lower and upper bounds to the center
    limits[currParam*2]     = frozenParamVAL[loopA];
    limits[currParam*2 + 1] = frozenParamVAL[loopA];
  }  
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
void cmLPN_svZeroD::printResults(const int totalResults, const stdVec results) {
  printf("RESULT PRINTOUT\n");
  for(int loopA = 0; loopA < totalResults; loopA++) {
    string s = getResultName(loopA);
    printf("%20s : %f\n",s.c_str(),results[loopA]);
  }
}

// ==========================================
// MAIN FUNCTION FOR STAGE1-2BLOCKS LPN MODEL
// ==========================================
int cmLPN_svZeroD::solveLPN(const stdVec& params, stdVec& results){

  int totalStates = getStateTotal();
  int totalAuxStates = getAuxStateTotal();
  int totOutputSteps = interface.num_output_steps_;
  
  // Initialize the outputs
  stdMat auxOutVals;
  stdMat outVals;
  outVals.resize(totalStates);
  auxOutVals.resize(totalAuxStates);
  for(int loopA=0;loopA<totalStates;loopA++){
    outVals[loopA].resize(totOutputSteps);
  }
  for(int loopA=0;loopA<totalAuxStates;loopA++){
    auxOutVals[loopA].resize(totOutputSteps);
  }
 
  // Update the model parameters based on input params argument
  this->zeroDmodel->setModelParams(interface, params);
  
  // Set up solution and time vectors, and run simulation
  std::vector<double> solutions(interface.system_size_*interface.num_output_steps_);
  std::vector<double> times(interface.num_output_steps_);
  int error_code = 0;
  interface.update_state(init_state_y, init_state_ydot);
  interface.run_simulation(0.0, times, solutions, error_code);

  // Parse the solution vector
  int state, step;
  stdVec t;
  t.resize(totOutputSteps);
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
    return 1;
  }

  // Postprocess raw simulation outputs (outVals) and compute target quantities (results)
  this->zeroDmodel->postProcess(interface, t, outVals, auxOutVals, results);

  // Solution Successful
  return 0;
}

// =========================
// EVAL MODEL ERROR FUNCTION
// =========================
double cmLPN_svZeroD::evalModelError(const stdVec& inputs, stdVec& outputs, stdIntVec& errorCode) {

  int model = 0;

  int totalParams = getParameterTotal();
  int resultTotal = getResultTotal();
  
  //TODO: Does this need to be done every time this fn is called?
  outputs.clear();
  outputs.resize(resultTotal);

  // Solve coronary model
  int error = 0;
  try{
    error = solveLPN(inputs,outputs);
  }catch(...){
    error = 1;
  }
  errorCode.push_back(error);

////TODO: Is this needed?
//stdVec computedValues;
//for(int i = 0; i < resultTotal; i++) {
//   computedValues.push_back(outputs[i]);
//}

  // Keys/names for each target quantity
  vector<string> keys;
  this->zeroDmodel->getResultKeys(keys);

  // Standard deviation for measured targets
  stdVec stdFactors;
  this->zeroDmodel->getDataStd(stdFactors);
  
  // Weights for target quantities in error function
  stdVec weights;
  this->zeroDmodel->getResultWeights(weights);

  // Print and compare
  double logLikelihood = 0.0;
  if(data != NULL){

    // Print Info
    data->printAndCompare(keys,outputs,weights);
    
    // Evaluate Log Likelihood
    logLikelihood = data->evalLogLikelihood(keys,outputs,stdFactors,weights);
  }

  return logLikelihood;
}

void cmLPN_svZeroD::getPriorMapping(int priorModelType,int* prPtr) {
  this->zeroDmodel->getPriorMapping(priorModelType, prPtr);
}

// ==================
// SAVE ALL DATA FILE
// ==================
void writeAllDataFile(int totalSteps,int totalStates,int totAuxStates,const stdMat& outVals,const stdMat& auxOutVals){
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
