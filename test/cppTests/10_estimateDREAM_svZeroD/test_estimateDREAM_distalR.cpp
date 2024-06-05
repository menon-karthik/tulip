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
  std::string perfusion_covariance_file;
  if (argc < 3 || argc > 5) {
    printf("Usage: test_estimateDREAM_distalR <model_json_file> <target_flows> (optional: perfusion_volumes perfusion_covariance) \n");
    printf("Terminated!\n");
    exit(1);
  } else {
    try {
      // Get svZeroD model json file and perfusion volumes file (if provided)
      model_path = string(argv[1]);
      targets_file = string(argv[2]);
      if(argc >= 4) {
        perfusion_volumes_file = string(argv[3]);
        if(argc == 5) {
          perfusion_covariance_file = string(argv[4]);
        } else {
          perfusion_covariance_file = "None";
        }
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
  //auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-sbi/src/interface/libsvzero_interface.so");
  //auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-test/src/interface/libsvzero_interface.so");
  // Type of svZeroD model
  svZeroDModel* zeroDmodel = new svZeroD_distalResistance(targets_file, perfusion_volumes_file, perfusion_covariance_file);
  cmLPN_svZeroD* lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib, false, true);

  // DREAM Parameters
  int totChains = num_procs;
  int totGenerations = 10000;
  //int totGenerations = 10;
  int totalCR = 3;
  //int totalCR = 5;
  //int totCrossoverPairs = 5;
  int totCrossoverPairs = 10;
  //double dreamGRThreshold = 1.2;
  double dreamGRThreshold = 1.1;
  //double dreamGRThreshold = 1.05;
  //int dreamJumpStep = 10;
  int dreamJumpStep = 5;
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

  // Print RScaling parameter
  std::vector<double> R_scaling_history;
  int dummy_int  = 0;
  double dummy_dbl  = 0;
  std::string specifier = "RScaling_history";
  zeroDmodel->getSpecifiedParameter(specifier,dummy_dbl,dummy_int,R_scaling_history);
  double* R_scaling_dbl = new double[totGenerations];
  for (int i = 0; i < R_scaling_history.size(); i++) {
    R_scaling_dbl[i] = R_scaling_history[i];
  }
  double* R_scaling_all = new double[totGenerations*num_procs];
  MPI_Gather(R_scaling_dbl,totGenerations,MPI_DOUBLE,R_scaling_all,totGenerations,MPI_DOUBLE,0,MPI_COMM_WORLD);
  if (id == 0) {
    FILE* f;
    f = fopen("R_scaling.txt", "w");
    for (int i = 0; i < totGenerations*num_procs; i++) {
      fprintf(f,"%f\n",R_scaling_all[i]);
    }
    fclose(f);
  }

  if(id == 0){
    printf("\n");
    printf("COMPLETED!\n");
  }

  // Finalize MPI
  MPI_Finalize();
  return 0;
}
