// =========================
// TEST DREAM ON PROJECTILE MODEL
// Karthik Menon, 2023
// =========================

# include "daData_multiple_Table.h"
# include "acActionDREAMmpi.h"
# include "cmTutorial.h"

using namespace std;

int main(int argc, char* argv[]){

  // Init MPI
  int num_procs = 0;
  int id = 0;
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);

  if(argc != 2){
    printf("Usage: test_estimateDREAM <target_file> \n");
    printf("Terminated!\n");
    exit(1);
  }

  // Dataset
  std::string target_file;
  target_file = string(argv[1]);
  bool useSingleColumn = false;
  int columnID = 0;
  daData* data = new daData_multiple_Table(useSingleColumn,columnID);
  data->readFromFile(target_file);

  // Assign model
  cmModel* model;
  model = new cmTutorial();

  // Assign dataset
  model->setData(data);

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
  dream.setModel(model);


  // Run MCMC simulation
  dream.go();

  // Add Barrier
  MPI_Barrier(MPI_COMM_WORLD);

  // Postprocess results
  if(id == 0){
    bool debugMode = false;
    double burnInPercent = 50.0; // half of the samples discarded
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
