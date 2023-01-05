// TEST CASE 1 - RUN LPN Model with RK4 Integrator
// DES, March 2017

# include <stdlib.h>
# include <stdio.h>
# include <iostream>
# include <fstream>
# include <math.h>
# include <string>
# include <vector>

# include "uqTypes.h"
# include "uqConstants.h"

# include "odeModel.h"
# include "odeRCR.h"

# include "odeIntegrator.h"
# include "odeIntegratorRK4.h"

# include "cmException.h"
# include "cmUtils.h"
# include "cmModel.h"
# include "cmLPNModel.h"

# include "daData.h"
# include "daData_multiple_Table.h"

using namespace std;

int main(int argc, char* argv[]){

  double ll = 0.0;
    
  try{

    stdMat matForcing;
    string fileName = string("inflow.flow");
    cmUtils::readTableFromFile(fileName, matForcing);
    
    // Create a ODE Model
    odeModel* ode = new odeRCR();

    // Create a ODE Model Integrator
    double timeStep = 0.01;
    int totalCycles = 5;
    // TODO: Add a constructor to odeIntegratorRK4 to be able to provide the forcing... odeINtegratorRK4.cpp
    odeIntegrator* rk4 = new odeIntegratorRK4(ode,timeStep,totalCycles, matForcing);

    // Create new LPN model
    cmModel* lpnModel;
    lpnModel = new cmLPNModel(rk4);

    // Get Default parameter set
    stdVec inputs;
    lpnModel->getDefaultParams(inputs);
    //fileName = string("optParams.txt");
    //readVectorFromFile(fileName,0,inputs); // first column of file contains inputs, store in vector inputs

    for(int loopA=0;loopA<inputs.size();loopA++){
      printf("Compoment %d : %.3f\n",loopA,inputs[loopA]);
    }
    
    // Solve Model
    stdVec outputs;
    stdIntVec errorCodes;    
    ll = lpnModel->evalModelError(inputs,outputs,errorCodes);

    // TODO: Print the results
    printf("Outputs:\n");
    fflush(stdout);
    
    // ...
    for (int i =0; i < outputs.size(); i++){
        std::cout << outputs[i] << ", ";
    }
    
    
  }catch(exception& e){
    // ERROR: TERMINATED!
    printf("\n");
    printf("Msg: %s\n",e.what());
    printf("TERMINATED!\n");
    return 0;
  }

  // COMPLETED
  printf("\n");
  printf("Obj: %f\n",ll);
  printf("COMPLETED!\n");
  return 0;
}





