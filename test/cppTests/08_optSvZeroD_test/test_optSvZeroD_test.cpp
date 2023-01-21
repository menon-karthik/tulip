// TEST CASE 7 - Tune parameters of svZeroD coronary closed-loop model using Nelder Mead
// Karthik Menon, Dec 2022

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
# include "odeNormalAdultSimplePA.h"

# include "odeIntegrator.h"
# include "odeIntegratorRK4.h"

# include "cmException.h"
# include "cmUtils.h"
# include "cmModel.h"
# include "cmLPNModel.h"

# include "daData.h"
# include "daData_multiple_Table.h"

# include "acAction.h"
# include "acActionOPT_NM.h"

#include "cmLPN_svZeroD.h"
#include "svZeroD_ClosedLoopCoronary.h"


using namespace std;

int main(int argc, char* argv[]){

  double ll = 0.0;
    
  try{

    // Create new data object
    int keyColumn = 0;
    int timeStampColumn = 0;
    int columnID = 1; //TODO: Not sure
    bool useSingleColumn = true;
    string currPatientFile("coronary.csv"); //TODO
    daData* data = new daData_multiple_Table(useSingleColumn,columnID);
    data->readFromFile(currPatientFile);

//  // Create a ODE Model
//  odeModel* ode = new odeNormalAdultSimplePA(0001,ipTargetConfig_IncludeAll);

//  // Create a ODE Model Integrator
//  double timeStep = 0.01;
//  int totalCycles = 5;
//  odeIntegrator* rk4 = new odeIntegratorRK4(ode,timeStep,totalCycles);

//  // Create new LPN model
//  cmModel* lpnModel;
//  lpnModel = new cmLPNModel(rk4);

    // Create new LPN model
    std::string model_path = "svzerod_tuning.json";
    svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary();
    //cmLPN_svZeroD_coronary* lpnModel(model_path);
    cmLPN_svZeroD* lpnModel;
    lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel);

    // Assign Dataset to model
    int currPatient = 0;
    lpnModel->setData(data);

    // Set Optimizer Parameters
    // Total Number of iterations
    int totIterations = 50;
    // Convergence Tolerance
    double convTol = 1.0e-1;
    // Check Convergence every convUpdateIt iterations
    int convUpdateIt = 10;
    // Maximum Iterations
    int maxOptIt = 200;
    // Coefficient for Step increments
    double stepCoefficient = 0.1;
    // File with initial starting point
    bool useStartingParameterFromFile = true;
    bool startFromCentre = false;
    string startParameterFile("coronaryParams.txt");

    // Construct Action
    acAction* nm = new acActionOPT_NM(convTol, 
                                      convUpdateIt,
                                      maxOptIt,
                                      stepCoefficient);
    
    // Set Model
    nm->setModel(lpnModel);

    // SET INITIAL GUESS
    ((acActionOPT_NM*)nm)->setInitialParamGuess(useStartingParameterFromFile,
                                                startFromCentre,
                                                startParameterFile);        

    for(int loopA=0;loopA<totIterations;loopA++){
      
      // PERFORM ACTION
      nm->go();

      if(loopA == 0){
        ((acActionOPT_NM*)nm)->setInitialPointFromFile(true);
        ((acActionOPT_NM*)nm)->setInitialPointFile(string("optParams.txt"));
      }
    }

  }catch(exception &e){
    // ERROR: TERMINATED!
    printf("\n");
    printf("TERMINATED!\n");
    return 0;
  }

  // COMPLETED
  printf("\n");
  printf("Obj: %f\n",ll);
  printf("COMPLETED!\n");
  return 0;
}





