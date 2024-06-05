// TEST CASE 7 - Tune parameters of svZeroD coronary closed-loop model using Nelder Mead
// Karthik Menon, Jan 2023

# include "daData_multiple_Table.h"
# include "acActionOPT_NM.h"
# include "cmLPN_svZeroD.h"
//# include "svZeroD_ClosedLoopCoronary.h"
# include "svZeroD_ClosedLoopCoronary_GSI.h"


using namespace std;

int main(int argc, char* argv[]){
  
  // Parse command-line inputs
  std::string model_path;
  if(argc < 1 || argc > 2){
    printf("Usage: optSvZeroD (optional: model_path)\n");
    printf("Terminated!\n");
    exit(1);
  }else{
    try{
      // Get svZeroD model json file
      if(argc == 2) {
        model_path = string(argv[1]);
      } else {
        model_path = "svzerod_tuning.json"; // Default
      }
    }catch(exception &e){
      // ERROR: TERMINATED!
      printf("Invalid Input Arguments\n");
      printf("TERMINATED!\n");
      return 0;
    }
  }
    
  // Create new data object
  int keyColumn = 0;
  int columnID = 1; // N-1 after column with keys
  bool useSingleColumn = true;
  string currPatientFile("coronary.csv");
  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
  data->readFromFile(currPatientFile);

  // Create new LPN model
  // svZeroDPlus interface library
  auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
  // Type of svZeroD model
  //svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary();
  svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary_GSI();
  cmLPN_svZeroD* lpnModel;
  lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib);

  // Assign Dataset to model
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

  try{

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
  std::ofstream fin_file("NM_fin_closedloop.txt");
  fin_file << "COMPLETE" << std::endl;
  fin_file.close();

  printf("\n");
  printf("COMPLETED!\n");
  return 0;
}





