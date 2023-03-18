// TEST CASE 8 - Tune outlet/distal reistrances of svZeroD model using Nelder Mead
// Karthik Menon, Mar 2023

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

# include "daData_multiple_Table.h"

# include "acActionOPT_NM.h"

#include "cmLPN_svZeroD.h"
#include "svZeroD_distalResistance.h"

using namespace std;

//const int modelPA = 0;
//const int modelPD = 1;

//const int keyColumn = 2;
//const int timeStampColumn = 1;

// =============
// MAIN FUNCTION
// =============
int main(int argc, char* argv[]){

  // Parse command-line inputs
  std::string model_path;
  std::string targets_file;
  std::string perfusion_volumes_file;
  if(argc < 3 || argc > 4){
    printf("Usage: optSvZeroD_distalR model_path target_flows (optional: perfusion_volumes)\n");
    printf("Terminated!\n");
    exit(1);
  }else{
    try{
      // Get Model ID and Patient Number
      model_path = string(argv[1]);
      targets_file = string(argv[2]);
      if(argc == 4) {
        perfusion_volumes_file = string(argv[3]);
      } else {
        perfusion_volumes_file = "None";
      }
    }catch(exception &e){
      // ERROR: TERMINATED!
      printf("Invalid Input Arguments\n");
      printf("TERMINATED!\n");
      return 0;
    }
  }
  
  //try{

    // DATASET
    //daData* data = new daData_Scalar_MultiplePatients();
    //data->readFromFile(currPatientFile);

//  // LPN MODEL
//  cmLPN_svZeroD_distalR* lpnModel;
//  //std::string model_path = "svzerod_tuning.json";
//  lpnModel = new cmLPN_svZeroD_distalR(model_path,targets_file,perfusion_volumes_file); 

    // ASSIGN DATASET
    //lpnModel->setData(data,2);
    //lpnModel->readTargetsFromFile(currPatientFile);

    // Create new LPN model
    // svZeroDPlus interface library
    auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
    // Type of svZeroD model
    svZeroDModel* zeroDmodel = new svZeroD_distalResistance(targets_file, perfusion_volumes_file);
    cmLPN_svZeroD* lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib, true);

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
    bool useStartingParameterFromFile = false;
    string startParameterFile("");
    bool startFromCentre = false;

    // INIT ACTION
    acAction* nm = new acActionOPT_NM(convTol, 
                                      convUpdateIt,
                                      maxOptIt,
                                      stepCoefficient);
    
    // SET THE MODEL
    nm->setModel(lpnModel);

    // SET INITIAL GUESS
    ((acActionOPT_NM*)nm)->setInitialParamGuess(useStartingParameterFromFile,
                                                startFromCentre,
                                                startParameterFile);        

  try{

    for(int loopA=0;loopA<totIterations;loopA++){

      //std::cout << "[main] loopA " << loopA << std::endl;
      
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
  std::ofstream fin_file("NM_fin.txt");
  fin_file << "COMPLETE" << std::endl;
  fin_file.close();

  // Write RScaling parameter to end of optParams.txt
  FILE *f;
  double RScaling = 0.0;
  int dummy  = 0;
  std::string specifier = "RScaling";
  //f = fopen(fileName.c_str(), "w");
  f = fopen("optParams.txt", "a");
  //double test = lpnModel->getRScaling;
  //lpnModel->getRScaling();
//fprintf(f,"%f\n",lpnModel->getRScaling());
  zeroDmodel->getSpecifiedParameter(specifier,RScaling,dummy);
  fprintf(f,"%f\n",RScaling);
  fclose(f);

  printf("\n");
  printf("COMPLETED!\n");
  return 0;
}
