// =========================
// TEST GSI ON SVZERODPLUS MODEL
// Karthik Menon, 2023
// =========================

# include "acActionGSI.h"
# include "cmLPN_svZeroD.h"
//# include "svZeroD_ClosedLoopCoronary.h"
# include "svZeroD_ClosedLoopCoronary_GSI.h"

using namespace std;

int main(int argc, char* argv[]){

  if((argc < 2) || (argc > 3)){
    printf("Usage: test_estimateDREAM_svZeroD <model_json_file> (optional: <input_distribution_parameters_file>)>\n");
    printf("Terminated!\n");
    exit(1);
  }

  // Assign model
  std::string model_path = string(argv[1]);
  auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-master/src/interface/libsvzero_interface_library.so");
  //auto interface_lib = std::string("/home/users/kmenon13/svZeroDPlus/Release-sbi/src/interface/libsvzero_interface.so");
  svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary_GSI(true);
  //svZeroDModel* zeroDmodel = new svZeroD_ClosedLoopCoronary_GSI();
  cmLPN_svZeroD* lpnModel;
  lpnModel = new cmLPN_svZeroD(model_path, zeroDmodel, interface_lib);

  // GSI parameters
  // Type of samples for inputs: ipUniformSampling or ipGaussianSampling
  int sampleType = ipUniformSampling;
  // Input distributions: ipDistributionsFromLimits or ipDistributionsFromFile
  int distributionSource = ipDistributionsFromLimits;
  std::string distributionsFileName("");
  if (argc == 3) {
    distributionSource = ipDistributionsFromFile;
    distributionsFileName = string(argv[2]);
  }
  // Order of PCE
  int maxOrder = 2;
  // Method to compute PCE coeffs: algUseRegression or algUseQuadrature
  int coeffAlg = algUseQuadrature;
  // Write debug data?
  bool writeDebugData = true;

  // If ipDistributionsFromFile, specify file name to read from
  if ((distributionSource == ipDistributionsFromFile) && (distributionsFileName == "")) {
    throw std::runtime_error("Need to specify a file to read input distribution parameters from");
  }

  // Initialize GSI action
  acActionGSI gsi(sampleType, distributionSource, distributionsFileName, writeDebugData, maxOrder, coeffAlg);

  // Set model
  gsi.setModel(lpnModel);

  // Run MCMC simulation
  gsi.go();

  printf("\n");
  printf("COMPLETED!\n");

  return 0;
}
