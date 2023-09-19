// =========================
// TEST DREAM ON SVZERODPLUS MODEL
// Karthik Menon, 2023
// =========================

# include "acActionDREAMmpi.h"
# include "cmLPN_svZeroD.h"
# include "svZeroD_distalResistance.h"

using namespace std;

int main(int argc, char* argv[]){

  // Init MPI
  int num_procs = 0;
  int id = 0;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);

  std::string model_path;
  std::string targets_file;
  std::string perfusion_volumes_file;
  if (argc < 3 || argc > 4) {
    printf("Usage: test_estimateDREAM_distalR <model_json_file> <target_flows> (optional: perfusion_volumes) \n");
    printf("Terminated!\n");
    exit(1);
  } else {
    try {
      // Get svZeroD model json file and perfusion volumes file (if provided)
      model_path = string(argv[1]);
      targets_file = string(argv[2]);
      if(argc == 4) {
        perfusion_volumes_file = string(argv[3]);
      } else {
        perfusion_volumes_file = "None";
      }
    } catch(exception &e) {
      // ERROR: TERMINATED!
      printf("Invalid Input Arguments\n");
      printf("TERMINATED!\n");
      return 0;
    }
  }

  // Create new LPN model
  // svZeroDPlus interface library
  auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
  // Type of svZeroD model
  svZeroDModel* zeroDmodel = new svZeroD_distalResistance(targets_file, perfusion_volumes_file);
  cmLPN_svZeroD* lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib, false, true);

  // DREAM Parameters
  int totChains = num_procs;
  int totGenerations = 10000;
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
    double burnInPercent = 10.0; // half of the samples discarded
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
