// TEST CASE 1 - RUN LPN Model with RK4 Integrator
// DES, March 2017

# include <stdlib.h>
# include <stdio.h>
# include <iostream>
# include <fstream>
# include <math.h>
# include <string>
# include <vector>

//# include "uqTypes.h"
//# include "uqConstants.h"

//# include "odeModel.h"
//# include "odeNormalAdultSimplePA.h"

//# include "odeIntegrator.h"
//# include "odeIntegratorRK4.h"

# include "cmException.h"
# include "cmUtils.h"
//# include "cmModel.h"
//# include "cmLPNModel.h"

# include "daData.h"
# include "daData_multiple_Table.h"

# include "cmLPN_svZeroD.h"
# include "svZeroD_ClosedLoopCoronary_GSI.h"

using namespace std;

int main(int argc, char* argv[]){

  double ll = 0.0;
  
  if(argc != 3){
    printf("Usage: test_runSvZeroD <model_json_file> <parameters_file>\n");
    printf("Terminated!\n");
    exit(1);
  }

  // Assign model
  std::string model_path = string(argv[1]);
  //auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
  auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-sbi/src/interface/libsvzero_interface.so");
  //svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary_GSI(true);
  svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary_GSI();
  cmLPN_svZeroD* lpnModel;
  lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib);
    
  // Create new data object
  int keyColumn = 0;
  int columnID = 1; // N-1 after column with keys
  bool useSingleColumn = true;
  string currPatientFile("coronary.csv");
  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
  data->readFromFile(currPatientFile);
  
  // Assign Dataset to model
  lpnModel->setData(data);
    
  // Get parameter set
  stdVec inputs;
  std::string params_path = string(argv[2]);
  
  // Get parameter number from model
  int par_num = lpnModel->getParameterTotal();

  // Setup input parameters
  double* input_params = new double[par_num];
  std::cout<<"READING PARAMETERS FROM FILE: "<<params_path<<std::endl;
  int error = cmUtils::readParamsFromFile(params_path,input_params);
  for(int loopA=0;loopA<par_num;loopA++){
    inputs.push_back(input_params[loopA]);
  }

  // Solve Model
  stdVec outputs;
  stdIntVec errorCodes; 
  ll = lpnModel->evalModelError(inputs,outputs,errorCodes);
  printf("Obj: %f\n",ll);
  printf("COMPLETED!\n");
  return 0;
  
//try{

//  // Create new data object
//  int keyColumn = 0;
//  int timeStampColumn = 0;
//  int columnID = 0;
//  bool useSingleColumn = true;
//  string currPatientFile("heartFailure.dat");
//  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
//  data->readFromFile(currPatientFile);

//  // Create a ODE Model
//  int mode = 0; // ipTargetConfig_IncludeAll
//  odeModel* ode = new odeNormalAdultSimplePA(0001,mode);

//  // Create a ODE Model Integrator
//  double timeStep = 0.01;
//  int totalCycles = 5;
//  odeIntegrator* rk4 = new odeIntegratorRK4(ode,timeStep,totalCycles);

//  // Create new LPN model
//  cmModel* lpnModel;
//  lpnModel = new cmLPNModel(rk4);

//  // Assign Dataset to model
//  int currPatient = 0;
//  lpnModel->setData(data);

//  // Get Default parameter set
//  stdVec inputs;
//  lpnModel->getDefaultParams(inputs);

//  // Solve Model
//  stdVec outputs;
//  stdIntVec errorCodes;    
//  ll = lpnModel->evalModelError(inputs,outputs,errorCodes);
//  
//}catch(exception& e){
//  // ERROR: TERMINATED!
//  printf("\n");
//  printf("Msg: %s\n",e.what());
//  printf("TERMINATED!\n");
//  return 0;
//}

//// COMPLETED
//printf("\n");
//printf("Obj: %f\n",ll);
//printf("COMPLETED!\n");
//return 0;
}





