// TEST CASE 7 - Tune parameters of svZeroD coronary closed-loop model using Nelder Mead
// Karthik Menon, Jan 2023

# include "daData_multiple_Table.h"
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
    int columnID = 1; // N-1 after column with keys
    bool useSingleColumn = true;
    string currPatientFile("coronary.csv");
    daData* data = new daData_multiple_Table(useSingleColumn,columnID);
    data->readFromFile(currPatientFile);

    // Create new LPN model
    // svZeroDPlus interface library
    auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
    // svZeroD json file
    std::string model_path = "svzerod_tuning.json";
    // Type of svZeroD model
    svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary();
    cmLPN_svZeroD* lpnModel;
    lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib);

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





