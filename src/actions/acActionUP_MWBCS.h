#ifndef ACACTIONUP_MWBCS_H
#define ACACTIONUP_MWBCS_H

# include <math.h>
# include <chrono>

# include "acActionUP.h"
# include "acException.h"
# include "uqMWMatrix.h"
# include "uqConstants.h"
# include "uqAlgorithmBCS.h"
# include "uqMWMatrix.h"
# include "uqPartitionBinaryTree.h"
# include "uqUtils.h"
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
    
    // Constructor for arbitrarily and dependent samples
    acActionUP_MWBCS(uqSamples* locInputs,uqSamples* locOutputs,stdVec betaCoeffs,vector<uq1DApproximant*> marginals);

    //Destructor
    ~acActionUP_MWBCS();

  	// Perform Action
  	virtual int go();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Evaluate the surrogate at a number of parameter realizations

    **Discussion**
    None

    **Modified**
    Jan 2016 - DES
        
    **Reference**
    No Reference
    \endverbatim
    \param[in] params Matrix with multi-dimensional inputs.
    \param[out] surrogate Vector with surrogate evaluations.
    */
    void evalSurrogate(stdMat params,stdVec& surrogate);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Gather the best estimate of the function volume: the integration constant.

    **Discussion**
    None

    **Modified**
    Jan 2016 - DES
        
    **Reference**
    No Reference
    \endverbatim
    return Best estimate of the function volume.
    */
    double getVolume(){return globalVolume;};

    /*! 
    \verbatim embed:rst
    **Purpose**
    Evaluate the marginal distribution at the provided sampling locations.

    **Discussion**
    None

    **Modified**
    Jan 2016 - DES
        
    **Reference**
    No Reference
    \endverbatim
    \param[in] inputs The samples where to evaluate the marginal through regression. 
    \param[out] marginals A matrix with the marginals. The rows store the value of the marginal
                at the various sample locations (in increasing order), while the columns store the 
                various dimensions.
    */
    void evalMarginals(uqSamples* inputs,stdMat& marginals);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Set default values for the options

    **Discussion**
    None

    **Modified**
    Aug 2016 - DES
        
    **Reference**
    No Reference
    \endverbatim
    */
    void setDefaultOptions();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Generate A Multi-element approximant

    **Discussion**
    None

    **Modified**
    Aug 2016 - DES
        
    **Reference**
    No Reference
    \endverbatim
    */
    uq1DApproximant_ME* generate1DMEApproximant(bool normalize = true);
};

#endif //ACACTIONUP_MWBCS_H
