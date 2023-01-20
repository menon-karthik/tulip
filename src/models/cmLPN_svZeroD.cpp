#include "cmLPN_svZeroD.h"

using namespace std;

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

// ==========================
// READ TARGET DATA FROM FILE
// ==========================
void cmLPN_svZeroD::readTargetsFromFile(string targetFileName)
{
  std::ifstream read_file;
  read_file.open(targetFileName.c_str());
  
  std::string buffer;
  std::vector<std::string> tokens;
  if(!read_file.is_open())
    throw cmException("ERROR: Cannot open targets file");
  if(read_file.eof())
    throw cmException("ERROR: No targets found!");
  
  std::vector<double> targets;
  while(std::getline(read_file,buffer))
  {
    cmUtils::schSplit(buffer,tokens,",");
    targets.push_back(atof(tokens[2].c_str()));
  }
  
  read_file.close();
  
  this->targetPaoMin = targets[0];
  this->targetPaoMax = targets[2];
  this->targetESV = targets[12];
  this->targetEDV = targets[13];
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

// READ CORONARY PARAMETERS FROM FILE
void cmLPN_svZeroD::readParamsFromFile(stdVec& inputs, std::string param_path) {
   std::ifstream param_reader(param_path.c_str());
   std::string buffer;
   std::vector<std::string> tokens;
   if(!param_reader.is_open())
      throw cmException("ERROR: Cannot open coronaryParams.txt");
   if(param_reader.eof())
      throw cmException("ERROR: No parameters found!");
   inputs.resize(getParameterTotal());
   int count = 0;
   while(std::getline(param_reader,buffer)) {
      inputs[count] = atof(buffer.c_str());
      count++;
   }

   if(count != getParameterTotal()) {
      std::cout << "Error: Number of coronary parameters does "
         << "not match the expected number of " << getParameterTotal() << std::endl;
      exit(1);
   }

   param_reader.close();
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

void cmLPN_svZeroD::printResults(int totalResults, double* Xn) {
  printf("RESULT PRINTOUT\n");
  for(int loopA = 0; loopA < totalResults; loopA++) {
    string s = getResultName(loopA);
    printf("%20s : %f\n",s.c_str(),Xn[loopA]);
  }
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

// ==========================================
// MAIN FUNCTION FOR STAGE1-2BLOCKS LPN MODEL
// ==========================================
int cmLPN_svZeroD::solveLPN(double* params, double* results){

  int totalStates = getStateTotal();
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
  keys.push_back(string("PaoMin"));
  keys.push_back(string("PaoMin_conv"));
  keys.push_back(string("PaoMax"));
  keys.push_back(string("PaoMax_conv"));
  keys.push_back(string("PaoMean"));
  keys.push_back(string("PaoMean_conv"));
  keys.push_back(string("AorCorSplit"));
  keys.push_back(string("AbsQin"));
  keys.push_back(string("AbsQin_conv"));
  keys.push_back(string("Qsystole_perc"));
  keys.push_back(string("PpulMean"));
  keys.push_back(string("EFLV"));
  keys.push_back(string("ESV"));
  keys.push_back(string("EDV"));
  keys.push_back(string("QlaRatio"));
  keys.push_back(string("mitValveTime"));
  keys.push_back(string("aorValveTime"));
  keys.push_back(string("pulValveTime"));
  keys.push_back(string("PraMean"));
  keys.push_back(string("LCorMaxRatio"));
  keys.push_back(string("LCorTotRatio"));
  keys.push_back(string("LThirdFF"));
  keys.push_back(string("LHalfFF"));
  keys.push_back(string("LGradOK"));
  keys.push_back(string("RCorMaxRatio"));
  keys.push_back(string("RCorTotRatio"));
  keys.push_back(string("RThirdFF"));
  keys.push_back(string("RHalfFF"));
  keys.push_back(string("RGradOK"));

  stdVec computedValues;
  for(int i = 0; i < resultTotal; i++) {
     computedValues.push_back(results[i]);
  }


  stdVec stdFactors;
  stdFactors.push_back(8.1); // PaoMin
  stdFactors.push_back(8.1); // PaoMin_diff
  stdFactors.push_back(12.6); // PaoMax
  stdFactors.push_back(12.6); // PaoMax_diff
  stdFactors.push_back(9.6); // PaoMean
  stdFactors.push_back(9.6); // PaoMean_diff
  stdFactors.push_back(0.4); // AorCorSplit
  stdFactors.push_back(9.07); // AbsQin
  stdFactors.push_back(9.07); // AbsQin_diff
  stdFactors.push_back(0.5); // Qsystole_perc
  stdFactors.push_back(3.3); // PpulMean
  stdFactors.push_back(0.065); // EFLV
  stdFactors.push_back(4.0); // ESV
  stdFactors.push_back(10.0); // EDV
  stdFactors.push_back(0.236); // QlaRatio
  stdFactors.push_back(0.084); // mitValveTime
  stdFactors.push_back(0.051); // aorValveTime
  stdFactors.push_back(0.051); // pulValveTime
  stdFactors.push_back(1.2); // PraMean
  stdFactors.push_back(0.8); // LCorMaxRatio
  stdFactors.push_back(2.5337); // LCorTotRatio
  stdFactors.push_back(0.02); // LThirdFF
  stdFactors.push_back(0.03); // LHalfFF
  stdFactors.push_back(1.00); // LGradOK
  stdFactors.push_back(0.3); // RCorMaxRatio
  stdFactors.push_back(1.0816); // RCorTotRatio
  stdFactors.push_back(0.07); // RThirdFF
  stdFactors.push_back(0.07); // RHalfFF
  stdFactors.push_back(1.00); // RGradOK
  
  stdVec weigths;
  weigths.push_back(0.25);  // PaoMin
  weigths.push_back(0.25);  // PaoMin_diff
  weigths.push_back(0.25);  // PaoMax
  weigths.push_back(0.25);  // PaoMax_diff
  weigths.push_back(1.0); // PaoMean
  weigths.push_back(0.25); // PaoMean_diff
  weigths.push_back(1.0);  // AorCorSplit
  weigths.push_back(0.5);  // AbsQin
  weigths.push_back(0.25);  // AbsQin_diff
  if(this->useRigidSurrogate)
    weigths.push_back(999999999.0);  // Qsystole_perc
  else
    weigths.push_back(1.0);
  weigths.push_back(2.0); // PpulMean
  weigths.push_back(1.0);  // EFLV
  weigths.push_back(0.5); // ESV
  weigths.push_back(0.5); // EDV
  weigths.push_back(2.0);  // QlaRatio
  weigths.push_back(2.0);  // mitValveTime
  weigths.push_back(2.0);  // aorValveTime
  weigths.push_back(2.0);  // pulValveTime
  weigths.push_back(1.0); // PraMean
  weigths.push_back(0.5); // LCorMaxRatio
  weigths.push_back(0.5); // LCorTotRatio
  weigths.push_back(1.0); // LThirdFF
  weigths.push_back(1.0); // LHalfFF
  weigths.push_back(1.0); // LGradOK
  weigths.push_back(0.5); // RCorMaxRatio
  weigths.push_back(0.5); // RCorTotRatio
  weigths.push_back(1.0); // RThirdFF
  weigths.push_back(1.0); // RHalfFF
  weigths.push_back(1.0); // RGradOK

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

void cmLPN_svZeroD::getDefaultParameterLimits(stdVec& limits) {
  this->zeroDmodel->getDefaultParameterLimits(limits);
}
