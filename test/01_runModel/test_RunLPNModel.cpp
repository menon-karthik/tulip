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
# include "cmException.h"
# include "cmUtils.h"
# include "cmModel.h"
# include "cmLPN_NA_S_PA.h"
# include "cmLPN_NA_S_PD.h"
# include "daData.h"
# include "daData_Variable_SinglePatient.h"

using namespace std;

int main(int argc, char* argv[]){
    
  try{

    // Create new data object
    daData* data = new daData_Variable_SinglePatient(keyColumn,timeStampColumn);
    data->readFromFile(currPatientFile);

    // Create a ODE Model
    odeModel* ode = new odeNormalAdult();

    // Create a ODE Model Integrator
    odeIntegrator* rk4 = new odeIntegratorRK4(ode,timeStep,totalSteps);

    // Create new LPN model
    cmModel* lpnModel;
    lpnModel = new cmLPNModel(rk4);

    // Assign Dataset to model
    lpnModel->setData(data,currPatient);

    // Get Default parameter set
    lpnModel->getDefaultParams(inputs);

    // Solve Model
    stdVec outputs;
    stdIntVec errorCodes;    
    ll = lpnModel->evalModelError(inputs,outputs,errorCodes);
    
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





