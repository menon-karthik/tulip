#ifndef ACACTIONGSI_H
#define ACACTIONGSI_H

# include "acAction.h"
# include "acException.h"
# include "cmUtils.h"
# include "uqSamples.h"
# include "uqMatrix.h"
# include "uqPolyMatrix.h"
# include "uqMultiIndex.h"
# include "uqAlgorithm.h"
# include "uqAlgorithmBCS.h"
# include "uqConstants.h"
# include "uqTypes.h"

// Define the type of sampling
const int ipUniformSampling  = 0;
const int ipGaussianSampling = 1;
// Define if a file or the parameter limits in the model need to be used
const int ipDistributionsFromLimits = 0;
const int ipDistributionsFromFile   = 1;
// Define how the coefficients are computed
const int algUseRegression = 0;
const int algUseQuadrature = 1;

// =========================================
// COMPUTATION OF GLOBAL SENSITIVITY INDEXES
// =========================================
class acActionGSI: public acAction{
  protected:
    // Data Members
    int    sampleType;
    int    distributionSource;
    string priorParamFile;
    bool   writeDebugData;
    int    maxOrder;
    int    coeffAlg;

    // Member Functions
    void printSensitivityTablesToLatex(int res_num,int par_num,const stdMat& dirTable,const stdMat& totTable);
    void printSensitivityTables(int res_num,int par_num,const stdMat& dirTable,const stdMat& totTable);
    void printSigmaCoeffs(const stdMat& sigmaCoeffs);
  
  public:
    // Constructor
    acActionGSI(int sampleType=ipUniformSampling,
                int distributionSource=ipDistributionsFromLimits,
                string priorParamFile="",
                bool writeDebugData=false,
                int maxOrder=3,
                int coeffAlg=algUseQuadrature);
    
    // Perform Action
    virtual int go();
};

#endif //ACACTION_H
