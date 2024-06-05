// =========================
// TEST DREAM ON SVZERODPLUS MODEL
// Karthik Menon, 2023
// =========================

# include "daData_multiple_Table.h"
# include "acActionDREAMmpi.h"
# include "cmLPN_svZeroD.h"
# include "svZeroD_ClosedLoopCoronary_GSI.h"

using namespace std;

int main(int argc, char* argv[]){

  // Init MPI
  int num_procs = 0;
  int id = 0;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);

  if(argc != 3 && argc != 4){
    printf("Error! Terminated!\n");
    throw std::runtime_error("Usage: test_estimateDREAM_svZeroD <model_json_file> <target_file> (optional: <frozen_parameters_file>)");
  }

  // Dataset
  std::string target_file;
  target_file = string(argv[2]);
  bool useSingleColumn = true;
  int columnID = 1;
  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
  data->readFromFile(target_file);

  // Assign model
  std::string model_path = string(argv[1]);
  auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
  svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary_GSI();
  cmLPN_svZeroD* lpnModel;
  lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib);

  // Assign dataset
  lpnModel->setData(data);

  // Frozen parameters
  stdIntVec frozen_param_idxs;
  stdVec frozen_param_values;
  if(argc == 4){
    // Read list and values of frozen parameters
    cmUtils::getFrozenParametersFromCSVFile(argv[3], frozen_param_idxs, frozen_param_values);
    // Set the model frozen parameters
    lpnModel->freezeModelParameters(frozen_param_idxs, frozen_param_values);
  }

  // DREAM Parameters
  int totChains = num_procs;
  int totGenerations = 10000;
  //int totGenerations = 20;
  int totalCR = 3;
  int totCrossoverPairs = 5;
  double dreamGRThreshold = 1.2;
  int dreamJumpStep = 10;
  int dreamGRPrintStep = 10;

  // Output files
  string dreamChainFileName("chain_GR_000000.txt");
  string dreamGRFileName("gr_GR.txt");

  // Restart files
  string dreamRestartReadFileName = ""; // No restart simulation
  //string dreamRestartReadFileName = "restart_read_GR.txt";
  
  // Write restart file just in case
  string dreamRestartWriteFileName = "restart_write_GR.txt";
  
  // SET prior information
  bool usePriorFromFile = false;
  string priorFileName("");
  int priorModelType = 0;

  // Initialize DREAM action
  acActionDREAMmpi dream(totChains,
                      totGenerations,
                      totalCR,
                      totCrossoverPairs,
                      dreamChainFileName,
                      dreamGRFileName,
                      dreamGRThreshold,
                      dreamJumpStep,
                      dreamGRPrintStep,
                      dreamRestartReadFileName,
                      dreamRestartWriteFileName,
                      usePriorFromFile,
                      priorFileName,
                      priorModelType);

  // Set model
  dream.setModel(lpnModel);


  // Run MCMC simulation
  dream.go();

  // Add Barrier
  MPI_Barrier(MPI_COMM_WORLD);

  // Postprocess results
  if(id == 0){
    bool debugMode = false;
    double burnInPercent = 10.0; // samples discarded
    dream.postProcess(debugMode,burnInPercent);
  }

  if(id == 0){
    printf("\n");
    printf("COMPLETED!\n");
  }

  // Finalize MPI
  MPI_Finalize();
  return 0;
}
