#ifndef CMTABLEINTERPOLATOR_H
#define CMTABLEINTERPOLATOR_H

# define ARMA_DONT_USE_CXX11
# include <armadillo>

# include <string>
# include "uqTypes.h"
# include "uqConstants.h"
# include "uqPolyMatrix.h"
# include "uqAlgorithmBCS.h"
# include "cmModel.h"
# include "cmException.h"
# include "cmUtils.h"

using namespace std;

// Algorithm to determine the polynomial coefficients
const int kLeastSquares = 0;
const int kBayesCS = 1;
const int kSparseGrid = 2;
const int kNearestNighbor = 3;

// General Interpolator Using Table Data
class cmTableInterpolator: public cmModel{
  protected:
    int totInputs;
    stdIntVec inputCols;
    int totOutputs;
    stdIntVec outputCols;
    stdVec targetAV;
    stdVec targetSD;
    // Interpolation Method
    int interpMethod;
    // Polynomial Order
    int polyOrder;
    // Table with input/outputs
    stdMat tableValues;
    // Representation coefficients for each output
    stdMat outputCoeffs;

    int getCloserPointId(stdVec inputs);
  public:
  	// Constructor
  	cmTableInterpolator(string tableFile, string weightFile,
                        int interpolationMethod, int order,
                        int localTotInputs, stdIntVec &localInputCols, 
                        int localTotOutputs, stdIntVec &localOutputCols,
                        stdVec &localTargetAV, stdVec &localTargetSD);

  	// Functions to be implemented
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif // CMTABLEINTERPOLATOR_H
