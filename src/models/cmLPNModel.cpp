# include "cmLPNModel.h"

using namespace std;

// Constructor
cmLPNModel::cmLPNModel(odeIntegrator* integrator): cmModel(){
  this->integrator = integrator;
}
// Distuctor
cmLPNModel::~cmLPNModel(){
}
int cmLPNModel::getParameterTotal(){
  return this->integrator->ode->getParameterTotal();
}
int cmLPNModel::getStateTotal(){
  return this->integrator->ode->getStateTotal();
}
int cmLPNModel::getResultTotal(){
  stdStringVec keys;
  this->integrator->ode->getResultKeys(keys);
  return keys.size();
}
void cmLPNModel::getDefaultParameterLimits(stdVec& limits){
  this->integrator->ode->getDefaultParameterLimits(limits);
}
void cmLPNModel::getDefaultParams(stdVec& params){
  this->integrator->ode->getDefaultParams(params);
}
string cmLPNModel::getParamName(int parID){
  return this->integrator->ode->getParamName(parID);
}
string cmLPNModel::getResultName(int resID){
  stdStringVec keys;
  this->integrator->ode->getResultKeys(keys);
  return keys[resID];
}
void cmLPNModel::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: cmLPNModel::getPriorMapping Not Implemented.");
}
void writeAllDataToFile(string fileName,int totalSteps,const stdMat& outVals,const stdMat& auxOutVals){
  // Get totals
  int totalStates  = outVals.size();
  int totAuxStates = auxOutVals.size();
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
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

double cmLPNModel::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  // Get Heart Rate from inputs
  double HR = inputs[integrator->ode->getHRIndex()];
  // Get Number of states from ode model
  int totStates = integrator->ode->getStateTotal();
  // The Parameters Include the Initial conditions
  int totParams = integrator->ode->getParameterTotal();
  // Check compatibility between inputs, initial values and parameters
  if(inputs.size() != totParams){
    printf("Input size: %d, States+Parameters: %d\n",int(inputs.size()),int(totParams));
    throw cmException("ERROR: Input size is not compatible with ODE model.");
  }
  // Set the initial conditions
  stdVec iniVals(totStates,0.0);
  for(int loopA=0;loopA<totStates;loopA++){
    iniVals[loopA] = inputs[loopA];
  }
  // Set the model parameters
  stdVec params(totParams-totStates,0.0);
  for(int loopA=totStates;loopA<totParams;loopA++){
    params[loopA-totStates] = inputs[loopA];
  }
  // Set Steps per cycle
  double cycleDuration = 60.0/HR;
  int totalStepsOnSingleCycle = int(cycleDuration/(double)integrator->timeStep);
  int totalSteps = totalStepsOnSingleCycle*integrator->totalCycles;
  // Run the integrator and post-process
  stdMat outVals;
  stdMat auxOutVals;
  // Except if the solution crashes
  try{
    integrator->run(totalSteps,iniVals,params,outVals,auxOutVals);
    integrator->ode->postProcess(integrator->timeStep,totalStepsOnSingleCycle,totalSteps,inputs,outVals,auxOutVals,outputs);
  }catch (exception& e){
    throw;
  }
  // Write Results to File
  if(printLevel > 0){
    string outFile("allData.dat");
    writeAllDataToFile(outFile,totalSteps,outVals,auxOutVals);
  }
  // Recover Keys,outs,stds and weights
  stdStringVec keys;
  stdVec outs;
  stdVec stds;
  stdVec weights;
  integrator->ode->getResultKeys(keys);
  integrator->ode->getFinalOutputs(outputs,outs);
  integrator->ode->getDataSTD(stds);
  integrator->ode->getResultWeigths(weights);
  // Need to evaluate log-likelihood/posterior and return
  double result = 0.0;
  if(data != NULL){
    if(printLevel > 0){
      // Print Info
      data->printAndCompare(keys,outs,weights); 
    }

    // Evaluate Objective Function
    result = data->evalLogLikelihood(keys,outs,stds,weights);
    //result = data->evalOBJ(keys,computedValues,weigths);
  }
  // Sync Outputs
  outputs.clear();
  for(size_t loopA=0;loopA<outs.size();loopA++){
    outputs.push_back(outs[loopA]);
  }
  // Return 
  errorCode.clear();
  errorCode.push_back(0);
  return result;
}