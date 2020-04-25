#ifndef ACACTIONUP_MWBCS_H
#define ACACTIONUP_MWBCS_H

# include <math.h>

# include "acActionUP.h"
# include "acException.h"
# include "uqMWMatrix.h"
# include "uqConstants.h"
# include "uqAlgorithmBCS.h"
# include "uqMWMatrix.h"
# include "uqPartitionBinaryTree.h"
# include "uqUtils.h"
// Response Approximant
# include "uqApproximant.h"
# include "uqApproximant_SE.h"
# include "uqApproximant_ME.h"
// Density Approximant
# include "uq1DApproximant.h"
# include "uq1DApproximant_SE.h"
# include "uq1DApproximant_ME.h"

# include "cmUtils.h"

// Execution Modes
enum inputModeTypes{imRandom,imGrid,imTable,imCVSimVTK,imFromMCMC};
enum randomSamplingTypes{stRandom,stHalton};
enum measureInputModes{miUnit,miFromSamples,miFromBetaMarginals};

struct mwbcsOptions{
  // Number of Initial Samples In Domain
  int numIniSamples;
  // Option to Refine The space
  bool doRefineSpace;
  // Option to refine the Samples
  bool doRefineSamples;

  // Input Sample Mode
  inputModeTypes inputMode;

  // Type of Sampling for Random Sampling
  randomSamplingTypes samplingType;
  
  // Read Input and Outputs from table
  string tableInputFileName;
  string tableOutputFileName;

  // Different Ways to specify an input measure
  measureInputModes measureInputMode;
  string measureInputFileName;

  // Use Beta Factor for Arbitrary dependent inputs 
  bool   useBetaFactors;
  bool   readBetaFactorsFromFile;
  string betaFactorFileName;

  // Add Boundary Points to integrate probability distributions
  bool addBoundaryPoints;
  // Number of points to add in every dimension
  int boundaryPointOrder;

  // Maximum order of interpolation
  int maxMWOrder;
  int maxGridOrder;

  // Multiwavelet Quadrature Order
  int mwQuadOrder;

  // ID of the data column containing the outputs
  int outputColumn;

  // Refinement criteria variables
  double thresholdSize;
  double minSamplesInPartition;
  double maxPartitionSizeRatio;
  double maxMWVarianceRatio;

  // Node to Extract for VTK
  int vtkNodeToExtract;

  // Force RVM solution with integration grid
  // instead of using numerical quadrature
  bool forceRVMSolution;

  // Ratio between residual norm and standard
  // deviation in current partition
  double resToleranceRatio;

  // Compute the l1,l2 and lInf errors
  bool computeErrorMetrics;

  // File Printing
  bool printPartitionTree;
  bool printInputOutputSamples;
  bool printPartitionSamples;
  bool printAndEvalSurrogate;  
  bool printPartitionMonitor;
  bool printOverallStats;
  string simulPrefix;

  // Message Printing
  bool printProgress;
  bool printDBGMessages;
};

// GENERIC CLASS FOR SAMPLES
class acActionUP_MWBCS: public acActionUP{
  private:      
    // Store Volume
    double globalVolume;

  public:  	
    // MW_BCS Options
    mwbcsOptions opts;

    // Store the binary tree to recover the representation
    uqPartitionBinaryTree* partitionTree;

    // Final Statistics To Store
    double globalAVG;
    double globalSTD;

    // Beta Factors for dependent inputs
    stdVec betaFactors;

    // 1D Approximants storing the marginal distributions
    vector<uq1DApproximant*> marginalMeasures;

    // Constructor
  	acActionUP_MWBCS(uqSamples* locInputs=NULL,uqSamples* locOutputs=NULL);
    // Arbitrary and dependent samples
    acActionUP_MWBCS(uqSamples* locInputs,uqSamples* locOutputs,stdVec betaCoeffs,vector<uq1DApproximant*> marginals);
    //Destructor
    ~acActionUP_MWBCS();

    void evalSurrogate(stdMat params,stdVec& surrogate);
    double getVolume();
    void evalMarginals(uqSamples* inputs,stdMat& marginals);
    void setDefaultOptions();
    uq1DApproximant_ME* generate1DMEApproximant(bool normalize = true);    

    // Construct stochastic approximation or evaluate samples
    virtual int go();
    // Get Approximant
    virtual uqApproximant* getApproximant();
    // Get Statistics
    virtual stdMat getStatistics();
};

#endif //ACACTIONUP_MWBCS_H
