#ifndef UQSAMPLES_H
#define UQSAMPLES_H

# include <vector>
# include <string>
# include <math.h>
# include <fstream> 
# include <algorithm>

# define ARMA_DONT_USE_CXX11
# include <armadillo>

# include "pdflib.h"
# include "rnglib.h"
# include "uqTypes.h"
# include "uqConstants.h"
# include "uqPartitionBinaryTree.h"
# include "uqException.h"
# include "cmUtils.h"
# include "sparse_grid_cc.h"
# include "uq1DQuadratureRule.h"
# include "uq1DQuadratureRule_CC.h"
# include "uq1DQuadratureRule_Regular.h"

using namespace std;

// TYPES OF RANDOM EXTRACTION
const int kExtractRandom = 0;
const int kExtractRandomWithInitialBuffer = 1;
const int kExtractRandomWithFinalBuffer = 2;
const int kExtractRandomWithBothEndsBuffer = 3;
const int kExtractSequential = 4;

// TYPES OF VARIABLES
const int kSAMPLEUniform = 0;
const int kSAMPLEGaussian = 1;
const int kSAMPLELognormal = 2;
const int kSAMPLEConstant = 3;

// TYPES OF QUADRATURE GRID
const int kRegular = 0;
const int kCC = 1;
const int kDoubleCC = 2;

// TYPES OF LIMITS
const int kBoundingBox = 0;
const int kFourSTD = 1;

const int array[] = { 1, 2, 3 };

// QUASI RANDOM SAMPLES
enum quasiRandomTypes{rsHalton};
enum randomAdaptiveSamplingTypes{asRandom,asHalton};

// STRUCT CONTAINING VARIOUS ALL VARIABLES
struct variableRecord{
  string name;
  int type;
  double parameter1;
  double parameter2;
};

// AUX STRUCT FOR SORTING
struct inputSort{ 
  double coord;
  int idx;
};

// SORTING CRITERIA
struct by_idx { 
  bool operator()(inputSort const &a, inputSort const &b) { 
       return a.coord < b.coord;
  }
};

// Need Forward Declaration to Resolve Circular Reference
class uqPartitionBinaryTree;

// GENERIC CLASS FOR SAMPLES
class uqSamples{
  protected:
    // NUMBER OF SAMPLES AND DIMENSIONALITY
    int totSamples;
    int totDims;
    // NORMALIZED SAMPLES
    stdMat values;    
    // UNNORMALIZED AND CORRELATED SAMPLES
    stdMat corrValues;    
    // Grid Weights Including normalized distribution values
    // Careful: Index from 0 to maxOrder included !!!
    stdMat weights;    

  public:
    // Record of Random Variables
    bool areIndependent;
    vector<variableRecord> rvs;
    stdMat covCholFactor;
    stdMat corrCholFactor;
    
    // CONSTRUCTOR AND DISTRUCTOR
    uqSamples();
    uqSamples(int dimNum);
    uqSamples(stdVec onePoint);
    uqSamples(uqSamples* sample);
    virtual ~uqSamples();
    // GETTER AND SETTERS
    // GET
    int    getTotSamples(){return totSamples;}
    int    getTotDims(){return totDims;}
    void   getValues(stdMat& copyVals);
    stdMat getCorrelatedValues(){return corrValues;}
    double getValuesAt(int sample, int dim){return values[sample][dim];};
    double getCorrelatedValues(int sample, int dim){return corrValues[sample][dim];}
    stdMat getWeights(){return weights;};
    double getWeightAt(int nodeId, int order);
    int    getMaxWeightOrder();

    void AdjustWeights(int maxOrder);

    int getTotRVS(){return int(rvs.size());}
    variableRecord getRVS(int index){return rvs[index];}

    // EASY ACCESS TO VALUES
    double operator()(const int nRow, const int nCol);

    // SET DEPENDANCE TO USE VALUES OR CORRVALUES
    void setSampleIndependence(bool flag);

    // =======================================
    // SAMPLE ADDITION OR CREATION FROM MATRIX
    // =======================================
    void   addOneSample(stdVec sample);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Build samples from matrix of sample evaluations

    **Discussion**
    None

    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] params The STD Matrix where the rows are the samples to be added. 
    */
    void   buildFromMatrix(stdMat params);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Add random samples independently on all dimensions given a specific partition
    identified through the associated limits

    **Discussion**
    None

    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] samplesToAdd number of samples to be added.
    \param[in] limits 2*totDims vector storing the current partition limits.
    */
    void   AddUniformSamplesFromPartition(int samplesToAdd, stdVec limits);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Add quasi-random space filling samples in a selected partition.

    **Discussion**
    None

    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] samplesToAdd number of samples to be added.
    \param[in] limits 2*totDims vector storing the the current partition limits.
    \param[in] rootLimits 2*totDims vector storing the limits of the root node. This is needed as the space filling is computed on 
               the hypercube.
    \param[in][out] startSampleIdx in input it is the starting sample number in the Halton sequence while in output is the
                    sample with the highest index generated.
    */
    void   AddQuasiRandomSamplesFromPartition(int samplesToAdd, quasiRandomTypes type, bool onPartition, stdVec limits, stdVec rootLimits, int& startSampleIdx);
    stdMat normalizeColumns(bool symmetric);
    void   addPointsOnBoundary(int boundPointOrder);

    // NORMALIZE VARIABLES
    double denormalizeRV(double value, int dimId);
    void   rescaleOnHypercube(stdVec currLimits);
    void   rescaleFromHypercubeToPartition(stdVec currLimits);

    // ADD VARIABLE DEFINITION
    void AddVariable(string varName,int varType,double par1,double par2);
    // Add Covariance Matrix
    void AddCovariance(stdMat covariance);
    // Check if all variables are uniform
    bool areAllUniformVariables();
    // Correlate Variables
    void CorrelateVariables();
    // Decorrelate Variables
    void DecorrelateVariables();

    // GENERATE SAMPLES ACCORDING TO VARIABLE INFORMATION
    // Generate Random Samples
    void generateRandomSamples(int numSamples, int seed = 0);
    // Generate Samples on a sparse Grid
    void generateSparseGrid(int gridOrder);
    // Generate a Cartesian Grid 
    void generateCartesianGrid(int gridOrder, int gridType, int rangeType);
    // Transform Existing Inputs
    void readAndTransform(uqSamples* samples);

    // EXTRACT SAMPLE SUBSET
    uqSamples extractSamples(int size,int extractionType,int extractionTypeAux,stdIntVec &indexes);
    // EXTRACT SAMPLE WITH EXISTING INDEX
    uqSamples extractSamplesWithIndex(stdIntVec indexes);
    // EXTRACT RESULTS FOR ROW INDEX AND SPECIFIED COLUMN
    void extractSampleColumnWithIndex(stdIntVec Indexes,int outCol,stdVec& rhs);
    // EXTRACT SAMPLES FROM PARTITION LIMITS
    void extractPartitionSamples(uqSamples* samples,stdVec limits,stdIntVec& ind);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Extract samples the overlap the current partition along one dimension

    **Discussion**
    none
    
    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] inputs input samples,
    \param[in] limits limit vector (2 * nDims) for the current partition
    \param[in] dim dimension to be considered
    \return object with one dimensional samples
    */
    void extractSamplesWithOneOverlappingDimension(uqSamples* inputs,stdVec limits,int dim,stdIntVec& ind);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Extract one dimensional samples from multiD sample object

    **Discussion**
    This is use to evaluate the marginals after computing a MW surrogate
    
    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] samples
    \param[in] dim
    \return object with one dimensional samples
    */
    void extractOneDimensionalSamples(uqSamples* samples,int dim);

    // EVALUATE STANDARD PROBABILITY DISTRIBUTION ON HYPERCUBE
    double evalNormalizedProbabilityOnHypercube(int dim, double location);

    // I/O
    // Read From File
    void readFromFile(string outMatFile,bool skipFirstLine);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Read sub-samples from MCMC trace file.

    **Discussion**
    This is used to post-process the MCMC realizations, for example to
    estimate the integration constant, etc.
    
    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] mcmcTraceFile Name of the file containing the MCMC traces.
    \param[in] totSubSamples Sub-sample size to be read.    
    \param[in] startColumn Column (space separated, assumed contiguous) where input quantities start.
    \param[in] endColumn Column where input quantities end.
    */
    void readMCMCSubSamples(string mcmcTraceFile,int totSubSamples,int startColumn,int endColumn,stdIntVec& sampleIndexes);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Read sub-samples from MCMC trace file if you know the index

    **Discussion**
    This is used to post-process the MCMC realizations, for example to
    estimate the integration constant, etc.
    
    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] mcmcTraceFile Name of the file containing the MCMC traces.
    \param[in] totSubSamples Sub-sample size to be read.
    \param[in] sampleIndexes Vector with the indexes to sample.
    \param[in] startColumn Column (space separated, assumed contiguous) where input quantities start.
    \param[in] endColumn Column where input quantities end.
    */
    void readMCMCSubSamplesWithIndex(string mcmcTraceFile,int totSubSamples,stdIntVec sampleIndexes,int startColumn,int endColumn);

    // Write Samples To File
    void printToFile(string outMatFile,bool printTitle);
    // Write Weights To File
    void printWeightsToFile(string outMatFile,bool printTitle);
    // Read Additional dimension from column in File
    void addVariableFromFile(string fileName,int numColumn);
    
    // OPERATIONS ON SAMPLES
    // Get Sample Limits
    void getSampleLimits(stdVec& currLimits, int limitType = kBoundingBox);
    // Evaluate Sample Difference with given column
    void evalDifference(int refColumn);
    // Remove Initial Samples
    void removeFirstSamples(int samplesToRemove);
    // ADD VARIABLES FROM SAMPLE
    void addVariablesFromSample(uqSamples qVals);
    // REMOVE VARIABLES
    uqSamples removeVariable(int idx);
    // Add Adaptive Samples based on a binary tree partition
    void AddSamplesAdaptive(uqSamples* inputs,uqPartitionBinaryTree* tree,double metricThreshold,int samplesInPartition,randomAdaptiveSamplingTypes type=asRandom);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Modifies the sample object and takes the exp{} of all value entries

    **Discussion**
    This is used to go from the log-posterior fitness values from MCMC to the actual posterior
    
    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    void expTransform();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Normalize all samples such that each component has a maximum of one

    **Discussion**
    none
    
    **Modified**
    2 May 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    void normalizeToOne();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Evaluate a multivariate Gaussian at the locations provided using 
    the stored average vector and covariance Cholesky factor

    **Discussion**
    TO CHECK !!!
    
    **Modified**
    3 Aug 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    void evalMultiVariateGaussian(stdVec& mvg);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Returns the permutation that orders the dim-dimensional inputs in increasing order

    **Discussion**
    none
    
    **Modified**
    3 Aug 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    void evalSamplePermutation(int dim,stdIntVec& samplePermutation);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Multiplies all the sample values by an associated scalar
    This addresses the multiplication by the Beta factor
    for arbitrary dependent random inputs

    **Discussion**
    none
    
    **Modified**
    3 Aug 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    void multiplyByScalar(stdVec scalars);

};

#endif // UQSAMPLES_H
