# include <vector>
# include <limits>
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <string>
# define ARMA_DONT_PRINT_ERRORS
# define ARMA_DONT_USE_CXX11
# include <armadillo>

# include "uqAlgorithmBCS.h"

using namespace std;

// ===========
// CONSTRUCTOR
// ===========
uqAlgorithmBCS::uqAlgorithmBCS(){
  // INITIALIZE OPTIONS
  opts.estimateNoise = true;
  opts.printProgressToScreen = false;
  opts.printDBGMessages = false;
  opts.maxIteration = 1;
  opts.maxBetaIterations = 300;
}

// ===================================
// PRINT UTILITY FOR COVARIANCE MATRIX
// ===================================
void printCovariance(int size, const stdMat &mat){
  printf("Covariance Matrix, size %d\n",size);
  for(int loopA=0;loopA<size;loopA++){
    printf("%d ",loopA);
    for(int loopB=0;loopB<size;loopB++){
      printf("%.7f ",mat[loopA][loopB]);
    }
    printf("\n");
  }
  printf("\n");
}

// ================================
// PRINT UTILITY FOR AVERAGE VECTOR
// ================================
void printAverageCoeff(int size, const stdVec &vec){
  printf("Average Coefficient Vector, size %d\n",size);
  for(int loopA=0;loopA<size;loopA++){
    printf("%d %.7f\n",loopA,vec[loopA]);
  }
  printf("\n");
}

// ==========================
// MATRIX UTILITIES - PRODUCT
// ==========================
void matMatProd(int dim1,int dim2,int dim3,
                const stdMat &mat1,const stdMat &mat2,
                stdMat &resMat){

  // Allocate Result
  resMat.resize(dim1);
  for(int loopA=0;loopA<dim1;loopA++){
    resMat[loopA].resize(dim3);
  }

  // Multiply
  for(int loopA=0;loopA<dim1;loopA++){
  	for(int loopB=0;loopB<dim3;loopB++){
      resMat[loopA][loopB] = 0.0;
      for(int loopC=0;loopC<dim2;loopC++){
        resMat[loopA][loopB] += mat1[loopA][loopC]*mat2[loopC][loopB];
      }
  	}
  }
}

// ========================================
// MATRIX UTILITIES - MATRIX-VECTOR PRODUCT
// ========================================
void matVecProd(int dim1,int dim2,
                const stdMat &mat,const stdVec &vec,
                stdVec &res){
  // Allocate Result
  double prodValue = 0.0;
  res.resize(dim1);
  for(int loopA=0;loopA<dim1;loopA++){
    prodValue = 0.0;
    for(int loopB=0;loopB<dim2;loopB++){
      prodValue += mat[loopA][loopB]*vec[loopB];
    }
    res[loopA] = prodValue;
  }
}

// ======================================================
// MATRIX UTILITIES - PRODUCE A RANK 1 MATRIX FROM VECTOR
// ======================================================
void vecVecTProd(int dim,const stdVec &vec,
                 stdMat &resMat){
  // Allocate
  resMat.resize(dim);
  for(int loopA=0;loopA<dim;loopA++){
    resMat[loopA].resize(dim);
  }
  // Fill Matrix
  for(int loopA=0;loopA<dim;loopA++){
    for(int loopB=0;loopB<dim;loopB++){
      resMat[loopA][loopB] = vec[loopA]*vec[loopB];
    }
  }
}

// ===========================
// ERASE INDEX FROM DBL VECTOR
// ===========================
void eraseIndexFromDblVector(int index,stdVec &vec){
  vec.erase(vec.begin()+index);
}

// =======================================
// ERASE ROW AND COLUMN FROM DOUBLE MATRIX
// =======================================
void eraseIndexFromDblMatrix(int index,stdMat &mat){
  for(size_t loopA=0;loopA<mat.size();loopA++){
    mat[loopA].erase(mat[loopA].begin()+index);
  }
  mat.erase(mat.begin()+index);
}

// =============================
// ERASE ELEMENT FROM INT VECTOR
// =============================
void eraseElementFromIntVector(int value,stdIntVec &vec){
  bool found = false;
  int count = 0;
  while(!found){
    found = (vec[count] == value);
    if(found){
      vec.erase(vec.begin()+count);
      return;
    }else{
      count++;
    }
  }
}

// ==========================
// INITIALIZE NOISE PARAMETER
// ==========================
double initNoiseParam(const stdVec &yValues){
  double factor = 0.1;
  double sum = 0.0;
  double sumSquared = 0.0;
  for(size_t loopA=0;loopA<yValues.size();loopA++){
    sum += yValues[loopA];
  }
  double av = (1.0/(double)yValues.size())*sum;
  for(size_t loopA=0;loopA<yValues.size();loopA++){
    sumSquared += (yValues[loopA]-av)*(yValues[loopA]-av);
  }
  double stdt = sqrt((1.0/((double)yValues.size()-1))*sumSquared);
  stdt = max(1.0e-6,stdt);
  return 1.0/((stdt*factor)*(stdt*factor));
}

// =========================
// GET MOST CORRELATED BASIS
// =========================
void getFirstBasisIndex(const stdVec &yValues,const stdMat &basisMat,double beta,
                        int &maxCorrelatedBasis,double &alpha){
  // Get Matrix Size
  int totSamples = yValues.size();
  int totBasis = basisMat[0].size();
  // Init Values
  double maxCorrelation = -numeric_limits<double>::max();
  maxCorrelatedBasis = 0; 
  // Init
  double currCorrelation = 0.0;
  double compareNorm = 0.0;
  double currNorm = 0.0;
  // Loop on the basis
  for(int loopA=0;loopA<totBasis;loopA++){
    currCorrelation = 0.0;
    currNorm = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      currCorrelation += yValues[loopB]*basisMat[loopB][loopA];
      currNorm += basisMat[loopB][loopA]*basisMat[loopB][loopA];
    }    
    compareNorm = ((currCorrelation*currCorrelation)/currNorm);
    if(compareNorm>maxCorrelation){
      maxCorrelation = compareNorm;
      maxCorrelatedBasis = loopA;
      alpha = currNorm/(compareNorm - (1.0/beta));
      //alpha = currCorrelation*beta;
    }
  }
}

// =======================
// EVAL INITIAL COVARIANCE
// =======================
double evalInitialCovariance(double alpha,double beta){
  return (1.0/(alpha + beta));
}

// ====================
// EVAL INITIAL AVERAGE
// ====================
double evalInitialAverage(double beta,
	                        double SigmaCoeff, int basisID, 
	                        const stdMat &basisMat,const stdVec &yValues){

  double currValue = 0.0;
  // Eval Correlation of basis and measurements  
  for(size_t loopA=0;loopA<yValues.size();loopA++){
    currValue += basisMat[loopA][basisID]*yValues[loopA];
  }

  // Return value
  return beta*SigmaCoeff*currValue;

}

// =========================
// INITIAL COVARIANCE MATRIX
// =========================
void initCovariance(double initialCovariance,stdMat &SigmaMat){
  stdVec firstRow;
  firstRow.push_back(initialCovariance);
  SigmaMat.push_back(firstRow);
}

// =========================
// INITIAL COVARIANCE MATRIX
// =========================
void initAverageVector(double initialAverage,stdVec &nuVector){
  nuVector.push_back(initialAverage);
}

// ===================
// INITIALIZE S VECTOR
// ===================
void initS(int currBaseIdx,double currAlpha,double beta,
           const stdMat &SigmaMat,const stdMat &basisMat,
           stdVec &S_in,stdVec &S_out){
  // Get Totals
  int totSamples = basisMat.size();
  int totBasis = basisMat[0].size();

  // Loop over the Basis
  for(int loopA=0;loopA<totBasis;loopA++){

    // Eval the first term
    double fistTerm = beta;
    
    // Eval the second term
    double secondTerm = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      secondTerm += basisMat[loopB][currBaseIdx]*basisMat[loopB][loopA];
    }
    secondTerm = (beta*secondTerm*SigmaMat[0][0]*beta*secondTerm);

    // Add to Vector
    S_in.push_back(fistTerm-secondTerm);
  }

  // Copy S_out
  for(size_t loopA=0;loopA<S_in.size();loopA++){
    S_out.push_back(S_in[loopA]);
  }

  // Modify the Element in the Model
  S_out[currBaseIdx] = (currAlpha*S_in[currBaseIdx])/(currAlpha - S_in[currBaseIdx]);
}

// ===================
// INITIALIZE Q VECTOR
// ===================
void initQ(int currBaseIdx,double currAlpha,double beta,
           const stdVec &yValues,const stdMat &SigmaMat,
           const stdMat &basisMat,const stdVec &S_in,
           stdVec &Q_in,stdVec &Q_out){
  // Get Totals
  int totSamples = basisMat.size();
  int totBasis = basisMat[0].size();

  // Loop over the Basis
  for(int loopA=0;loopA<totBasis;loopA++){

    // Eval the first term
    double fistTerm = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      fistTerm += basisMat[loopB][loopA]*yValues[loopB];
    }
    fistTerm = (fistTerm*beta);
    
    // Eval the second term
    double secondTerm = 0.0;
    double secondTerm1 = 0.0;
    double secondTerm2 = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      secondTerm1 += basisMat[loopB][loopA]*basisMat[loopB][currBaseIdx];
      secondTerm2 += basisMat[loopB][currBaseIdx]*yValues[loopB];
    }
    secondTerm = (secondTerm1*beta)*SigmaMat[0][0]*(secondTerm2*beta);

    // Add to Vector
    Q_in.push_back(fistTerm-secondTerm);
  }

  // Copy Q_out
  for(size_t loopA=0;loopA<Q_in.size();loopA++){
    Q_out.push_back(Q_in[loopA]);
  }

  // Modify the Element in the Model
  Q_out[currBaseIdx] = (currAlpha*Q_in[currBaseIdx])/(currAlpha - S_in[currBaseIdx]);
}

// ==================
// ADD BASIS TO MODEL
// ==================
void addBasisToModel(rvmOptionRecord opts,
                     int totSamples,int totBasis,int totBasisInModel,
                     int currBasisID,double beta,double newAlpha,
                     const stdIntVec &basisInModel,const stdMat &basisMat,
                     stdVec &alphaVector,
                     stdVec &S_in,stdVec &Q_in,
                     stdVec &muVector, stdMat &sigmaMat){

  // Eval PhiT
  stdMat phiT;
  stdMat phi;
  int basisGlobalID = 0;

  // Allocate phiT
  phiT.resize(totBasisInModel);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    phiT[loopA].resize(totSamples);
  }
  // Allocate phi
  phi.resize(totSamples);
  for(int loopA=0;loopA<totSamples;loopA++){
    phi[loopA].resize(totBasisInModel);
  }  

  // Assign phiT
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    basisGlobalID = basisInModel[loopA];
    for(int loopB=0;loopB<totSamples;loopB++){
      phiT[loopA][loopB] = basisMat[loopB][basisGlobalID];
    }
  }

  // Assign phi
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    basisGlobalID = basisInModel[loopA];
    for(int loopB=0;loopB<totSamples;loopB++){
      phi[loopB][loopA] = basisMat[loopB][basisGlobalID];
    }
  }

  // Eval SigmaPhiT
  stdMat sigmaPhiT;
  matMatProd(totBasisInModel,totBasisInModel,totSamples,sigmaMat,phiT,sigmaPhiT);

  // Eval PhiSigma
  stdMat phiSigma;
  matMatProd(totSamples,totBasisInModel,totBasisInModel,phi,sigmaMat,phiSigma);

  // Eval PhiSigmaPhiT
  stdMat phiSigmaPhiT;
  matMatProd(totSamples,totBasisInModel,totSamples,phi,sigmaPhiT,phiSigmaPhiT);

  // Add to global estimates
  alphaVector.push_back(newAlpha);

  double si = S_in[currBasisID];
  double qi = Q_in[currBasisID];

  // Update Coefficient Covariance
  double sigmaii = (1.0/(newAlpha + si));
  double mui = sigmaii * qi;

  // Copy Phi
  stdVec phiVector;
  phiVector.resize(totSamples);
  for(int loopA=0;loopA<totSamples;loopA++){
    phiVector[loopA] = basisMat[loopA][currBasisID];
  }

  // Compute ei
  stdVec eiTempVec;
  matVecProd(phiSigmaPhiT.size(),phiSigmaPhiT[0].size(),phiSigmaPhiT,phiVector,eiTempVec);
  stdVec eiVec;
  eiVec.resize(totSamples);
  for(int loopA=0;loopA<totSamples;loopA++){
    eiVec[loopA] = phiVector[loopA] - beta*eiTempVec[loopA];
  }


  // Update Sigma
  // Compute Extradiagonal term
  stdVec sigmaPhiTPhi;
  stdVec extraDiag;
  extraDiag.resize(totBasisInModel);
  matVecProd(totBasisInModel,totSamples,sigmaPhiT,phiVector,sigmaPhiTPhi);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    extraDiag[loopA] = -beta*sigmaii*sigmaPhiTPhi[loopA];
  }

  // Update Existing Covariance Matrix
  stdMat resMat;
  vecVecTProd(totBasisInModel,sigmaPhiTPhi,resMat);
  for(size_t loopA=0;loopA<resMat.size();loopA++){
    for(size_t loopB=0;loopB<resMat[0].size();loopB++){
      sigmaMat[loopA][loopB] += beta*beta*sigmaii*resMat[loopA][loopB];
    }
  }

  // Resize Covariance and add new terms
  totBasisInModel++;
  sigmaMat.resize(totBasisInModel);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    sigmaMat[loopA].resize(totBasisInModel);
  }
  for(int loopA=0;loopA<totBasisInModel-1;loopA++){
    sigmaMat[loopA][totBasisInModel-1] = extraDiag[loopA];
    sigmaMat[totBasisInModel-1][loopA] = extraDiag[loopA];
  }
  sigmaMat[totBasisInModel-1][totBasisInModel-1] = sigmaii;

  // UPDATE AVERAGE VECTOR
  // Update the other components
  for(size_t loopA=0;loopA<muVector.size();loopA++){
    muVector[loopA] -=  mui*beta*sigmaPhiTPhi[loopA];
  }
  // Add the last component
  muVector.push_back(mui);
  
  // UPDATE S AND Q
  for(int loopA=0;loopA<totBasis;loopA++){
    // Product of Basis function and ei
    double phimTei = 0.0;
    for(int  loopB=0;loopB<totSamples;loopB++){
      phimTei += eiVec[loopB]*basisMat[loopB][loopA];
    }
    // Update S Vector
    S_in[loopA] -= sigmaii*(beta*phimTei)*(beta*phimTei);
    // Update Q Vector
    Q_in[loopA] -= mui*(beta*phimTei);
  }

  // Print S and Q
  if(opts.printDBGMessages){
    printf("\n");
    printf("%15s %15s\n","Q_in","S_in");
    for(int loopA=0;loopA<totBasis;loopA++){
      printf("%15.3f %15.3f\n",Q_in[loopA],S_in[loopA]);
    }
    printf("\n");

    // Print Covariance and Average Vector
    printCovariance(totBasisInModel,sigmaMat);
    printAverageCoeff(totBasisInModel,muVector);
  }

}

// =================
// RE-ESTIMATE BASIS 
// =================
void reEstimateBasis(rvmOptionRecord opts,
                     int totSamples,int totBasis,int totBasisInModel,
                     int currBasisID,double beta, double newAlpha,
                     const stdIntVec &globalToLocalMap,
                     const stdIntVec &basisInModel,const stdMat &basisMat,
                     stdVec &alphaVector,
                     stdVec &S_in,stdVec &Q_in,
                     stdVec &muVector, stdMat &sigmaMat){

  // Get the local number of the basis in the model
  int currBasisLocalID = globalToLocalMap[currBasisID];
  if(currBasisLocalID<0){
    printf("Error: Invalid Mapping.\n");
    exit(1);
  }

  // Eval the old Alpha
  double oldAlpha = alphaVector[currBasisLocalID];
  alphaVector[currBasisLocalID] = newAlpha;

  // Eval muj
  double muj = muVector[currBasisLocalID];

  // Get The diagonal of the Covariance matrix
  double sigmajj = sigmaMat[currBasisLocalID][currBasisLocalID];

  double kj = (1.0/(sigmajj + (1.0/(newAlpha - oldAlpha))));

  // Create a matrix containing sigmaj
  stdMat sigmajMat;
  sigmajMat.resize(1);
  sigmajMat[0].resize(totBasisInModel);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    sigmajMat[0][loopA] = sigmaMat[loopA][currBasisLocalID];
  }

  // Update the average vector
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    muVector[loopA] -= kj*muj*sigmajMat[0][loopA];
  }
  
  // Update the covariance matrix
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    for(int loopB=0;loopB<totBasisInModel;loopB++){
      sigmaMat[loopA][loopB] -= kj*sigmajMat[0][loopA]*sigmajMat[0][loopB];
    }
  }

  // Create phiT
  stdMat phiT;
  int globalRowID = 0;
  phiT.resize(totBasisInModel);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    phiT[loopA].resize(totSamples);
  }
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    globalRowID = basisInModel[loopA];
    for(int loopB=0;loopB<totSamples;loopB++){
      phiT[loopA][loopB] = basisMat[loopB][globalRowID];
    }
  }
  
  // Compute sigmajPhiT
  stdMat sigmajPhiT;
  matMatProd(1,totBasisInModel,totSamples,sigmajMat,phiT,sigmajPhiT);

  // Loop on the basis
  double sigmajPhiTPhiM = 0.0;
  for(int loopA=0;loopA<totBasis;loopA++){
    
    // Get sigmajPhiTPhiM
    sigmajPhiTPhiM = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      sigmajPhiTPhiM += sigmajPhiT[0][loopB] * basisMat[loopB][loopA];
    } 

    // Update S
    S_in[loopA] += kj*(beta*sigmajPhiTPhiM)*(beta*sigmajPhiTPhiM);

    // Update Q
    Q_in[loopA] += kj*muj*(beta*sigmajPhiTPhiM);

  }

  // Print S and Q
  if(opts.printDBGMessages){
    printf("\n");
    printf("%15s %15s\n","Q_in","S_in");
    for(int loopA=0;loopA<totBasis;loopA++){
      printf("%15.3f %15.3f\n",Q_in[loopA],S_in[loopA]);
    }
    printf("\n");

    // Print Covariance and Average Vector
    printCovariance(totBasisInModel,sigmaMat);
    printAverageCoeff(totBasisInModel,muVector);
  }
  
}

// =======================
// DELETE BASIS FROM MODEL
// =======================
void deleteBasisFromModel(rvmOptionRecord opts,
                          int totSamples,int totBasis,int totBasisInModel,
                          int currBasisID,double beta,
                          const stdIntVec &globalToLocalMap,
                          const stdIntVec &basisInModel,const stdMat &basisMat,
                          stdVec &alphaVector,
                          stdVec &S_in,stdVec &Q_in,
                          stdVec &muVector, stdMat &sigmaMat){


  // Get the local number of the basis in the model
  int currBasisLocalID = globalToLocalMap[currBasisID];
  if(currBasisLocalID<0){
    printf("Error: Invalid Mapping.\n");
    exit(1);
  }

  // Get The diagonal of the Covariance matrix
  double sigmajj = sigmaMat[currBasisLocalID][currBasisLocalID];

  // Get the average value of the coefficient to delete
  double muj = muVector[currBasisLocalID];

  // Create a matrix containing sigmaj
  stdMat sigmajMat;
  sigmajMat.resize(1);
  sigmajMat[0].resize(totBasisInModel);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
   sigmajMat[0][loopA] = sigmaMat[loopA][currBasisLocalID];
  } 

  // Update the covariance matrix
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    for(int loopB=0;loopB<totBasisInModel;loopB++){
      sigmaMat[loopA][loopB] -= (1.0/sigmajj)*sigmaMat[loopA][currBasisLocalID]*sigmaMat[currBasisLocalID][loopB];
    }
  }

  // Update the average vector
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    muVector[loopA] -= (muj/sigmajj)*sigmaMat[loopA][currBasisLocalID];
  }

  // Update S and Q
  // Create phiT
  stdMat phiT;
  int localRowID = 0;
  phiT.resize(totBasisInModel);
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    phiT[loopA].resize(totSamples);  
  }  
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    localRowID = basisInModel[loopA];
    for(int loopB=0;loopB<totSamples;loopB++){
      phiT[loopA][loopB] = basisMat[loopB][localRowID];
    }
  }
  
  // Compute sigmajPhiT
  stdMat sigmajPhiT;
  matMatProd(1,totBasisInModel,totSamples,sigmajMat,phiT,sigmajPhiT);

  // Loop on the basis
  double sigmajPhiTPhiM = 0.0;
  for(int loopA=0;loopA<totBasis;loopA++){
    
    // Get sigmajPhiTPhiM
    sigmajPhiTPhiM = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      sigmajPhiTPhiM += sigmajPhiT[0][loopB] * basisMat[loopB][loopA];
    } 

    // Update S
    S_in[loopA] += (1.0/sigmajj)*(beta*sigmajPhiTPhiM)*(beta*sigmajPhiTPhiM);

    // Update Q
    Q_in[loopA] += (muj/sigmajj)*(beta*sigmajPhiTPhiM);

  }

}

// ================================
// UPDATE THE VARIANCE OF THE NOISE
// ================================
double updateNoiseVariance(int totSamples,int totBasis,int totBasisInModel,
                           double oldBeta,
                           const stdBoolVec &isInModel,const stdIntVec &globalToLocalMap,
                           const stdMat &basisMat,const stdVec &yValues,
                           const stdVec &muVector,const stdMat &sigmaMat,
                           const stdVec &alphaVector,
                           double &deltaLogBeta){


  // Crate Coefficient Vector
  stdVec scaledCoeffs;
  scaledCoeffs.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    scaledCoeffs[loopA] = 0.0;
  }
  int localRowID = 0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if(isInModel[loopA]){
      localRowID = globalToLocalMap[loopA];
      // Assign Coefficient Vector
      //scaledCoeffs[globalRowID] = muVector[loopA]/basisScales[globalRowID];
      scaledCoeffs[loopA] = muVector[localRowID];
    }
  }

  // Eval Model Measurements
  stdVec yValuesModel;
  matVecProd(totSamples,totBasis,basisMat,scaledCoeffs,yValuesModel);
  
  // Get Difference Norm between measurements and model predictions
  double diff2Norm = 0.0;
  double varY = 0.0;
  for(int loopA=0;loopA<totSamples;loopA++){
    diff2Norm += (yValues[loopA] - yValuesModel[loopA])*(yValues[loopA] - yValuesModel[loopA]);
    varY += yValues[loopA]*yValues[loopA];
  }

  // Get Other term with alpha and sigma
  double alphaSigmaTerm = 0.0;
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    alphaSigmaTerm += alphaVector[loopA]*sigmaMat[loopA][loopA];
  }

  // Return the estimate
  // CHECK !!!
  double beta = ((totSamples - alphaSigmaTerm)/diff2Norm);
  beta  = min(beta,1.0e6/varY);
  deltaLogBeta = log(beta) - log(oldBeta);

  return beta;

}

// ================================
// UPDATE THE VARIANCE OF THE NOISE
// ================================
double evalResidualNorm(int totSamples,int totBasis,
                        const stdMat &basisMat,const stdVec &yValues,
                        const stdBoolVec &isInModel,const stdIntVec &globalToLocalMap,
                        const stdVec &muVector,const stdVec &basisScales){

  // Crate Coefficient Vector
  stdVec scaledCoeffs;
  scaledCoeffs.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    scaledCoeffs[loopA] = 0.0;
  }
  int localRowID = 0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if(isInModel[loopA]){
      localRowID = globalToLocalMap[loopA];
      // Assign Coefficient Vector
      //scaledCoeffs[globalRowID] = muVector[loopA]/basisScales[globalRowID];
      scaledCoeffs[loopA] = muVector[localRowID];
    }
  }

  // Evaluate Model Measurements
  stdVec yValuesModel;
  matVecProd(totSamples,totBasis,basisMat,scaledCoeffs,yValuesModel);

  // Get Difference Norm between measurements and model predictions
  double diff2Norm = 0.0;
  for(int loopA=0;loopA<totSamples;loopA++){
    diff2Norm += (yValues[loopA] - yValuesModel[loopA])*(yValues[loopA] - yValuesModel[loopA]);
  }
  diff2Norm = sqrt(diff2Norm/(double)totSamples);

  // Return the residual Norm
  return diff2Norm;
}

// ======================
// NORMALIZE BASIS MATRIX
// ======================
void normalizeBasis(int totSamples, int totBasis, stdVec &basisScales, stdMat &basisMat){
  basisScales.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    // Compute Column Norm
    basisScales[loopA] = 0.0;
    for(int loopB=0;loopB<totSamples;loopB++){
      basisScales[loopA] += basisMat[loopB][loopA] * basisMat[loopB][loopA];
    }
    basisScales[loopA] = sqrt(basisScales[loopA]);
    // Normalize
    for(int loopB=0;loopB<totSamples;loopB++){
      basisMat[loopB][loopA] = basisMat[loopB][loopA]/basisScales[loopA];
    }
  }
}

// ========================
// ALGORITHM INITIALIZATION
// ========================
void Initialize_BayeCS(rvmOptionRecord opts,
                       int totBasis,const stdVec &yValues,stdMat &basisMat,
                       stdIntVec &basisInModel,stdBoolVec &isAligned,
                       stdMat &sigmaMat,stdVec &muVector,
                       stdIntVec &globalToLocalMap,stdBoolVec &isInModel,
                       stdVec &S_in,stdVec &Q_in,
                       stdVec &S_out,stdVec &Q_out,
                       stdVec &Theta,double &beta,
                       stdVec &alphaVector,
                       stdVec &basisScales){

  // Initialization Phase
  if(opts.printDBGMessages){
    printf("\n");
    printf("INITIALIZATION...\n");
    printf("\n");
  }

  // Global Basis Number
  int currBasisID = 0;
  double currAlpha = 0.0;
  int totBasisInModel = 1;

  // NORMALIZE BASIS MATRIX
  normalizeBasis(yValues.size(),totBasis,basisScales,basisMat);

  // Initialize Aligned Basis vector
  isAligned.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    isAligned[loopA] = false;
  }
  
  // Allocate and initialize globalToLocalMap
  globalToLocalMap.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    globalToLocalMap[loopA] = -1;
  }

  // Initialize isInModel Vector
  for(int loopA=0;loopA<totBasis;loopA++){
    isInModel.push_back(false);
  }

  // Initialize Noise Parameter
  beta = initNoiseParam(yValues);
  if(opts.printDBGMessages){
    printf("NOISE PARAMETER: %e\n",beta);
    printf("\n");
  }
  
  // Get Index of Basis that Best Explains Data
  getFirstBasisIndex(yValues,basisMat,beta,currBasisID,currAlpha);
  isInModel[currBasisID] = true;
  globalToLocalMap[currBasisID] = 0;
  basisInModel.push_back(currBasisID);
  alphaVector.push_back(currAlpha);
  if(opts.printDBGMessages){
    printf("FIRST BASIS SELECTED: %d; ALPHA: %e\n",currBasisID,currAlpha);
    printf("\n");
  }

  // Compute Initial Covariance
  double initialCovariance = evalInitialCovariance(currAlpha,beta);
  // Add to Covariance
  initCovariance(initialCovariance,sigmaMat);
  if(opts.printDBGMessages){
    printCovariance(totBasisInModel,sigmaMat);
  }

  // Compute Initial Average   
  double initialAverage = evalInitialAverage(beta,initialCovariance,currBasisID,basisMat,yValues);
  // Add to average vectors
  initAverageVector(initialAverage,muVector);
  if(opts.printDBGMessages){
    printAverageCoeff(totBasisInModel,muVector);
  }

  // Update S and Q Vectors
  initS(currBasisID,currAlpha,beta,sigmaMat,basisMat,S_in,S_out);
  initQ(currBasisID,currAlpha,beta,yValues,sigmaMat,basisMat,S_in,Q_in,Q_out);

  // Initialize Theta Vector
  for(int loopA=0;loopA<totBasis;loopA++){
    Theta.push_back(Q_out[loopA]*Q_out[loopA] - S_out[loopA]);
  }

  if(opts.printDBGMessages){    
    // Print S and Q
    printf("%15s %15s %15s %15s\n","Q_in","Q_out","S_in","S_out");
    for(int loopA=0;loopA<totBasis;loopA++){
      printf("%15.3f %15.3f %15.3f %15.3f\n",Q_in[loopA],Q_out[loopA],S_in[loopA],S_out[loopA]);
    }
  }

}

// ===========================
// GET STRING FOR GIVEN ACTION
// ===========================
string getActionString(int action){
  switch (action){
    case kActionReestimate: 
    {
      string outString("Re-estimate");
      return outString;
      break;
    }
    case kActionAdd: 
    {
      string outString("Add");
      return outString;
      break;
    }
    case kActionDelete: 
    {
      string outString("Delete");
      return outString;
      break;
    }
    case kActionFinish: 
    {
      string outString("Finish");
      return outString;
      break;
    }
    case kActionSkipForAlignment:
    {
      string outString("Align-skip");
      return outString;
      break;
    }    
    default:       
    {
      string outString("Error: Not Defined!");
      return outString;
      break;
    }
  }
}

// =================================================
// PERFORM DECISION ABOUT THE THREE POSSIBLE ACTIONS
// =================================================
void computeDecisionVectors(rvmOptionRecord opts,
                            int totBasis,const stdIntVec &globalToLocalMap,
                            const stdVec &Theta,const stdBoolVec &isInModel,const stdBoolVec &isBasisAligned,
                            const stdVec &S_in,const stdVec &S_out,const stdVec &Q_in,const stdVec &Q_out,
                            const stdVec &alphaVector,
                            int &maxLikeBasis,double &maxLikeChange,double &newAlpha,int &currAction){

  // Initialize DeltaLikelihood Vector
  stdVec DeltaML;
  DeltaML.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    DeltaML[loopA] = 0.0;
  }

  // Initialize Action Vector with all Re-estimation
  stdIntVec Action;
  Action.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    Action[loopA] = kActionReestimate;
  }

  // RE-ESTIMATION
  double tempAlpha  = 0.0;
  double tempDelta = 0.0;
  double tempValue = 0.0;
  int localBasisID = -1;
  double logValue = 0.0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if((Theta[loopA]>0.0)&&(isInModel[loopA])){
      // Get localBasisID for alpha
      localBasisID = globalToLocalMap[loopA];
      // Eval New Alpha
      tempAlpha = (S_out[loopA]*S_out[loopA])/(double)Theta[loopA];
      tempDelta = ((1.0/tempAlpha) - (1.0/alphaVector[localBasisID]));      
      // Eval Change in likelihood due to estimation
      logValue = 1.0 + S_in[loopA]*tempDelta;
      tempValue = (tempDelta*(Q_in[loopA]*Q_in[loopA])/(tempDelta*S_in[loopA] + 1.0) - log(logValue))/2.0;
      DeltaML[loopA] = tempValue;
    }
  }

  // DELETION
  for(int loopA=0;loopA<totBasis;loopA++){
    if((Theta[loopA]<=0.0)&&(isInModel[loopA])){
      // Get localBasisID for alpha
      localBasisID = globalToLocalMap[loopA];
      // Eval Change in likelihood due to Deletion
      DeltaML[loopA] = -(Q_out[loopA]*Q_out[loopA]/(S_out[loopA] + alphaVector[localBasisID]) - log(1.0 + S_out[loopA]/alphaVector[localBasisID]))/2.0;
      Action[loopA] = kActionDelete;
    }
  }

  // ADDITION
  for(int loopA=0;loopA<totBasis;loopA++){
    if(!isInModel[loopA]){
      // Eval Change in likelihood due to Addition
      tempDelta = (Q_in[loopA]*Q_in[loopA])/(double)S_in[loopA];
      if(Theta[loopA]>0.0){
        DeltaML[loopA]  = (tempDelta - 1.0 - log(tempDelta))/2.0;
      }else{
        DeltaML[loopA]  = 0.0;
      }
      
      Action[loopA] = kActionAdd;
    }
  }

  // GET INDEX WITH MAXIMUM CHANGE IN LIKELIHOOD
  maxLikeChange = -numeric_limits<double>::max();
  maxLikeBasis = 0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if((DeltaML[loopA]>maxLikeChange)&&(!isBasisAligned[loopA])){
      maxLikeChange = DeltaML[loopA];
      maxLikeBasis = loopA;
      currAction = Action[loopA];
    }
  }

  // Assign New Alpha Value
  newAlpha = ((S_out[maxLikeBasis]*S_out[maxLikeBasis])/Theta[maxLikeBasis]);

  // Compute the increment in alpha in case of re-estimation
  double alphaIncr = 0.0;
  if(currAction == kActionReestimate){
    localBasisID = globalToLocalMap[maxLikeBasis];
    alphaIncr = fabs(log(newAlpha)-log(alphaVector[localBasisID]));
  }

  // Count the number of deletions
  int delCount = 0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if(Action[loopA] == kActionDelete){
      delCount++;
    }
  }

  // Check if the work is finished
  if((maxLikeChange<1.0e-6)||(((currAction==kActionReestimate)&&(alphaIncr<1.0e-1))&&(delCount == 0))){
    maxLikeChange = 0.0;
    maxLikeBasis = 0;
    currAction = kActionFinish;
  }

  // Write Outcome
  string actionSelected;
  if(opts.printDBGMessages){
    // Print Decisions
    string currActionString;
    printf("LIST OF ACTIONS\n");
    printf("%10s %10s %10s %10s\n","Var","DL","Theta","Action");
    for(int loopA=0;loopA<totBasis;loopA++){
      currActionString = getActionString(Action[loopA]);
      printf("%10d %10.3f %10.3f %10s\n",loopA,DeltaML[loopA],Theta[loopA],currActionString.c_str());
    }
    // Write Decision Taken
    printf("\n");
    printf("Decision Phase\n");
    printf("---------------\n");
    printf("Candidate Basis: %d\n",maxLikeBasis);
    printf("Maximum Likelihood Change %e\n",maxLikeChange);
    actionSelected = getActionString(Action[maxLikeBasis]);
    printf("Action: %s\n",actionSelected.c_str());
    printf("---------------\n");
    printf("\n");
    // Print S and Q
    printf("%15s %15s %15s %15s\n","Q_in","Q_out","S_in","S_out");
    for(int loopA=0;loopA<totBasis;loopA++){
      printf("%15.3f %15.3f %15.3f %15.3f\n",Q_in[loopA],Q_out[loopA],S_in[loopA],S_out[loopA]);
    }
    printf("\n");
  }

}

// ========================================
// CHECK ADDITION CANDIDATE BASIS ALIGNMENT
// ========================================
void checkBasisAlignment(int currBasisID,int totSamples,int totBasis,
                         const stdMat &basisMat,const stdBoolVec &isInModel,
                         bool printDBGMessages,
                         int &currAction,stdIntMat areAlignedTo){

  if(printDBGMessages){
    printf("CHECKING BASIS ALIGNMENT\n");
  }

  // Scan the bases in the model
  double norm = 0.0;
  double prod = 0.0;

  // Eval norm for the current basis
  double currNorm = 0.0;
  for(int loopB=0;loopB<totSamples;loopB++){
    currNorm += basisMat[loopB][currBasisID]*basisMat[loopB][currBasisID];
  }
  currNorm = sqrt(currNorm);

  // Eval Norm for the other basis and product
  double maxAlignment = 0.0;
  double currBasisAlignment = 0.0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if((isInModel[loopA])&&(loopA!=currBasisID)){
      norm = 0.0;
      prod = 0.0;
      for(int loopB=0;loopB<totSamples;loopB++){
        norm += basisMat[loopB][loopA]*basisMat[loopB][loopA];
        prod += basisMat[loopB][currBasisID]*basisMat[loopB][loopA];
      }
      norm = sqrt(norm);
      
      // Calculate the alignment of the current Basis
      currBasisAlignment = prod/(currNorm*norm);

      // Store Information on the Alignment between bases
      if(currBasisAlignment>(1.0-1.0e-3)){
        if(printDBGMessages){
          printf("FOUND ALIGNMENT WITH BASIS %d\n",loopA);
        }
        areAlignedTo[loopA].push_back(currBasisID);
      }

      // Store Maximum Value
      if(currBasisAlignment>maxAlignment){
        maxAlignment = currBasisAlignment;
      }
    }
  }
  // CHECK ALIGNMENT
  if(maxAlignment>(1.0-1.0e-3)){
    currAction = kActionSkipForAlignment;
  }
}

// =================
// UPDATE STATISTICS
// =================
void updateStatistics(rvmOptionRecord opts, int totBasis, 
                      const stdVec &S_in,const stdVec &Q_in,
                      const stdBoolVec &isInModel,const stdIntVec &globalToLocalMap,
                      const stdVec &alphaVector,
                      stdVec &S_out,stdVec &Q_out,
                      stdVec &Theta){
  // Copy S and Q
  for(int loopA=0;loopA<totBasis;loopA++){
    S_out[loopA] = S_in[loopA];
    Q_out[loopA] = Q_in[loopA];
  }

  // Modify for basis that are in the model
  double tmp;
  int localBasisID = 0;
  for(int loopA=0;loopA<totBasis;loopA++){
    if(isInModel[loopA]){
      localBasisID = globalToLocalMap[loopA];
      tmp = alphaVector[localBasisID]/(alphaVector[localBasisID]-S_in[loopA]);
      S_out[loopA] = tmp * S_in[loopA];
      Q_out[loopA] = tmp * Q_in[loopA];
    }
    Theta[loopA] = (Q_out[loopA]*Q_out[loopA]-S_out[loopA]);
  }

  // PRINT S AND Q
  if(opts.printDBGMessages){    
    printf("%15s %15s %15s %15s\n","Q_in","Q_out","S_in","S_out");
    for(int loopA=0;loopA<totBasis;loopA++){
      printf("%15.3f %15.3f %15.3f %15.3f\n",Q_in[loopA],Q_out[loopA],S_in[loopA],S_out[loopA]);
    }
  }
}

// ===============================================
// UPDATE QUANTITIES FOR CHANGE IN NOISE PRECISION
// ===============================================
void updateQtyForBetaChange(int totSamples, int totBasis, int totBasisInModel,
                            double beta,
                            const stdIntVec &basisInModel,
                            const stdMat &basisMat,
                            const stdVec &alphaVector,
                            const stdVec &yValues,
                            stdMat &sigmaMat,stdVec &muVector,
                            stdVec &S_in,stdVec &Q_in,
                            stdVec &S_out,stdVec &Q_out,
                            stdVec &Theta){

  // Form Phi Matrix
  arma::mat phiMat(totSamples,totBasisInModel);
  
  // Fill Phi Matrix
  int GlobalBasisID = 0;
  for(int loopA=0;loopA<totSamples;loopA++){
    for(int loopB=0;loopB<totBasisInModel;loopB++){
      GlobalBasisID = basisInModel[loopB];
      phiMat(loopA,loopB) = basisMat[loopA][GlobalBasisID];
    }      
  }

  arma::mat armaBasisMat(totSamples,totBasis);
  for(int loopA=0;loopA<totSamples;loopA++){
    for(int loopB=0;loopB<totBasis;loopB++){
      armaBasisMat(loopA,loopB) = basisMat[loopA][loopB];
    }      
  }

  arma::mat armaYVals(totSamples,1);
  for(int loopA=0;loopA<totSamples;loopA++){
    armaYVals(loopA) = yValues[loopA];
  }

  // Product of Basis Functions
  arma::mat invCovA = phiMat.st()*phiMat*beta;
  // Finish Inverse Covariance
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    invCovA(loopA,loopA) = invCovA(loopA,loopA) + alphaVector[loopA];
  }

  // Compute Cholesky Factor
  arma::mat cholCovA = arma::chol(invCovA);  
  
  // Compute Matrix Inverse
  arma::mat invCholCovA = arma::inv(cholCovA);

  // Compute Covariance Matrix
  arma::mat upSigmaMat = invCholCovA*invCholCovA.st();

  //upSigmaMat.print("upSigmaMat: ");

  // Update Mean Vector
  arma::mat upMu = beta*(upSigmaMat*(phiMat.st()*armaYVals));

  //upMu.print("upMu: ");

  // Update sigmaMat and muVector
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    muVector[loopA] = upMu[loopA];
    for(int loopB=0;loopB<totBasisInModel;loopB++){
      sigmaMat[loopA][loopB] = upSigmaMat(loopA,loopB);
    }      
  }  

  // Update S_in, Q_in
  arma::mat tmp = phiMat*upSigmaMat*phiMat.st();
  arma::mat tmpS_in(totBasis,1);
  arma::mat tmpQ_in(totBasis,1);
  double temp1;
  double temp2;
  for(int loopA=0;loopA<totBasis;loopA++){
    temp1 = as_scalar(beta - beta*beta*armaBasisMat.col(loopA).st()*tmp*armaBasisMat.col(loopA));
    tmpS_in[loopA] = temp1;
    temp2 = as_scalar(beta*armaBasisMat.col(loopA).st()*armaYVals - beta*beta*armaBasisMat.col(loopA).st()*tmp*armaYVals);
    tmpQ_in[loopA] = temp2;
  }

  // Update S_out, Q_out
  arma::mat tmpS_out = tmpS_in;
  arma::mat tmpQ_out = tmpQ_in;

  // Update S_out, Q_out for basis in model
  int globalRowID = 0;
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    globalRowID = basisInModel[loopA];
    tmpS_out[globalRowID] = (alphaVector[loopA]*tmpS_in[globalRowID])/(alphaVector[loopA]-tmpS_in[globalRowID]);
    tmpQ_out[globalRowID] = (alphaVector[loopA]*tmpQ_in[globalRowID])/(alphaVector[loopA]-tmpS_in[globalRowID]);
  }

  // Copy Back S and Q Vectors
  for(int loopA=0;loopA<totBasis;loopA++){
    S_in[loopA]  = tmpS_in[loopA];
    S_out[loopA] = tmpS_out[loopA];
    Q_in[loopA]  = tmpQ_in[loopA];
    Q_out[loopA] = tmpQ_out[loopA];
  }
  
  // Update Theta Vector
  for(int loopA=0;loopA<totBasis;loopA++){
    Theta[loopA] = (Q_out[loopA]*Q_out[loopA]-S_out[loopA]);
  }

}

// ============================================
// RESET ALIGNMENT CONDITIONS FOR DELETED BASIS
// ============================================
void resetAlignmentConditions(int CurrentBasisID,bool printDBGMessages,stdIntMat &areAlignedTo,stdBoolVec &isAligned){

  // SET THE VARIABLES TO REUSE
  int currBasis = 0;
  for(size_t loopA=0;loopA<areAlignedTo[CurrentBasisID].size();loopA++){
    currBasis = areAlignedTo[CurrentBasisID][loopA];
    isAligned[currBasis] = false;
  }

  // PRINT MESSAGE OF FOR WHAT BASIS THE ALIGNMENT IS REMOVED
  if(printDBGMessages){
    printf("RESTORE ALIGNMENT FOR BASIS\n");
    for(size_t loopA=0;loopA<areAlignedTo[CurrentBasisID].size();loopA++){
      printf("%d\n",areAlignedTo[CurrentBasisID][loopA]);
    }
    areAlignedTo[CurrentBasisID].erase(areAlignedTo[CurrentBasisID].begin(),areAlignedTo[CurrentBasisID].end());
  }

  // Delete the aligned basis stored
  areAlignedTo[CurrentBasisID].erase(areAlignedTo[CurrentBasisID].begin(),areAlignedTo[CurrentBasisID].end());
}

// ============================
// INVERT THE COVARIANCE MATRIX
// ============================
void computeCoefficientsCovariance(bool print, int size,
                                   const stdMat &invMat,
                                   stdMat &coeffCov){
  // Write Message
  if(print){
    printf("\n");
    printf("COMPUTING INVERSE...\n");
    printf("\n");
  }
  
  // Init Arma Matrix
  arma::mat armaInvMat(size,size);
  // Fill Matrix
  for(int loopA=0;loopA<size;loopA++){
    for(int loopB=0;loopB<size;loopB++){
      armaInvMat(loopA,loopB) = invMat[loopA][loopB];
    }
  }
  
  // Compute the Pseudo-Inverse (Just for cases where it is singular)
  arma::mat armaCov = arma::pinv(armaInvMat);
  
  // Write the standard deviation out
  coeffCov.clear();
  stdVec temp;
  for(int loopA=0;loopA<size;loopA++){
    temp.clear();
    for(int loopB=0;loopB<size;loopB++){
      temp.push_back(armaCov(loopA,loopB));
    }
    coeffCov.push_back(temp);
  }
}

// =======================================
// MAIN ROUTINE - RELEVANCE VECTOR MACHINE
// =======================================
double uqAlgorithmBCS::run(int totSamples, int totBasis, 
                           stdVec yValues,stdMat basisMat,
                           stdVec &coeffs,stdMat &coeffsCov, 
                           double &noiseEstimate){

  // Noise Precision
  double iniBeta;
  double beta;
  int totBasisInModel = 1;

  // Norm of the Basis
  stdVec basisScales;

  // Storage of basis
  stdIntVec selectedBasis;
  stdIntVec basisInModel;
  stdIntVec globalToLocalMap;
  stdBoolVec isInModel;

  // Exclude Basis that are found Aligned with other basis
  stdBoolVec isBasisAligned;

  // Estimated Coefficient Covariance Matrix (Only elements in basis)
  stdMat sigmaMat;
  // Estimated Coefficient Average Values (Only elements in basis)
  stdVec muVector;

  // Quality and Sparsity Vectors in and out of basis
  stdVec S_in;
  stdVec Q_in;
  stdVec S_out;
  stdVec Q_out;

  // Factor
  stdVec Theta;

  // Vector Containing the coefficients precisions
  stdVec alphaVector;

  // Declarations for Selection Loop
  bool converged = false;
  bool selectionIterationsFinished = false;
  int currIt = 0;
  double maxLikeChange = 0.0;
  int maxLikeBasis = 0;
  double oldAlpha = 0.0;
  double newAlpha = 0.0;
  int localBasisID = 0;
  int currAction = 0;    
  string actionString;
  
  // Start Estimation Loop
  bool betaConverged = false;
  stdIntMat areAlignedTo;
  areAlignedTo.resize(totBasis);
  int betaCurrIt = 0;
  double deltaLogBeta = 0.0;
  double newbeta = 0.0;
  string iterationString;
  double resNorm = 0.0;

  // Loop on noise updates
  while(!betaConverged){ 

    // Increment Beta Iterations
    betaCurrIt++;
    
    // If First Iteration than use initialization value
    if(betaCurrIt == 1){
      // Re-Initialize
      totBasisInModel = 1;
      selectedBasis.clear();
      basisInModel.clear();
      globalToLocalMap.clear();
      isInModel.clear();
      isBasisAligned.clear();
      sigmaMat.clear();
      muVector.clear();
      S_in.clear();
      Q_in.clear();
      S_out.clear();
      Q_out.clear();
      Theta.clear();
      alphaVector.clear();

      // Perform Initialization
      Initialize_BayeCS(opts,totBasis,
                        yValues,basisMat,basisInModel,isBasisAligned,
                        sigmaMat,muVector,
                        globalToLocalMap,isInModel,
                        S_in,Q_in,S_out,Q_out,
                        Theta,iniBeta,alphaVector,basisScales);
      // Assign Beta
      beta = iniBeta;
    }

    // Print Message
    if(opts.printDBGMessages){    
      printf("Noise precision: %f\n",beta);    
    }

    // Initialize Variables
    converged = false;
    selectionIterationsFinished = false;
    currIt = 0;
    maxLikeChange = 0.0;
    maxLikeBasis = 0;
    oldAlpha = 0.0;
    newAlpha = 0.0;
    localBasisID = 0;
    currAction = 0;    

    // Print Iteration Header
    if(betaCurrIt == 1){
      if(opts.printProgressToScreen){
        printf("\n");
        printf("%15s %15s %15s %15s %15s %15s\n","IT","LH Change","Basis","Action","Residual","Beta");
      }
    }

    // RVM ITERATIONS
    while(!converged){
      
      // Increment Current Iteration
      currIt++;

      // DECISION
      if(opts.printDBGMessages){
        printf(" --- RVM DECISION\n");
      }

      // PERFORM DECISION      
      computeDecisionVectors(opts,totBasis,globalToLocalMap,
                             Theta,isInModel,isBasisAligned,
                             S_in,S_out,Q_in,Q_out,
                             alphaVector,maxLikeBasis,maxLikeChange,
                             newAlpha,currAction);

      // ACTION
      if(opts.printDBGMessages){
        printf(" --- RVM ACTION\n");
      }

      // CHECK CANDIDATE ALIGNMENT WITH BASIS 
      if(currAction == kActionAdd){
        checkBasisAlignment(maxLikeBasis,totSamples,totBasis,
                            basisMat,isInModel,
                            opts.printDBGMessages,
                            currAction,areAlignedTo);  
      }
      
      // CASES
      if(currAction == kActionReestimate){
        // Re-estimate the Current Expansion Coefficient
        reEstimateBasis(opts,
                        totSamples,totBasis,totBasisInModel,
                        maxLikeBasis,beta,newAlpha,
                        globalToLocalMap,basisInModel,basisMat,
                        alphaVector,S_in,Q_in,
                        muVector,sigmaMat);

      }else if(currAction == kActionAdd){
        // Add current basis to the model
        addBasisToModel(opts,
                        totSamples,totBasis,totBasisInModel,
                        maxLikeBasis,beta,newAlpha,
                        basisInModel,basisMat,
                        alphaVector,
                        S_in,Q_in,
                        muVector,sigmaMat);

        isInModel[maxLikeBasis] = true;
        totBasisInModel++;
        globalToLocalMap[maxLikeBasis] = totBasisInModel-1;
        basisInModel.push_back(maxLikeBasis);

      }else if(currAction == kActionDelete){        
        // Delete current basis from the model
        deleteBasisFromModel(opts,
                             totSamples,totBasis,totBasisInModel,
                             maxLikeBasis,beta,
                             globalToLocalMap,basisInModel,basisMat,
                             alphaVector,
                             S_in,Q_in,
                             muVector,sigmaMat);        

        isInModel[maxLikeBasis] = false;
        totBasisInModel--;
        localBasisID = globalToLocalMap[maxLikeBasis];
        globalToLocalMap[maxLikeBasis] = -1;
        for(int loopA=0;loopA<totBasis;loopA++){
          if(globalToLocalMap[loopA]>localBasisID){
            globalToLocalMap[loopA]--;
          }
        }

        // ERASE COMPONENT FOR DELETED BASIS
        eraseElementFromIntVector(maxLikeBasis,basisInModel);        
        eraseIndexFromDblVector(localBasisID,alphaVector);
        eraseIndexFromDblVector(localBasisID,muVector);
        eraseIndexFromDblMatrix(localBasisID,sigmaMat);  

        // RESET ALIGNMENT CONDITIONS
        resetAlignmentConditions(maxLikeBasis,opts.printDBGMessages,areAlignedTo,isBasisAligned);

        // Print S and Q
        if(opts.printDBGMessages){
          printf("\n");
          printf("%15s %15s\n","Q_in","S_in");
          for(int loopA=0;loopA<totBasis;loopA++){
            printf("%15.3f %15.3f\n",Q_in[loopA],S_in[loopA]);
          }
          printf("\n");

          // Print Covariance and Average Vector
          printCovariance(totBasisInModel,sigmaMat);
          printAverageCoeff(totBasisInModel,muVector);
        }
        
      }else if(currAction == kActionSkipForAlignment){
        // ALIGNED BASIS: SKIP ACTION
        isBasisAligned[maxLikeBasis] = true;
      }   

      if(opts.printDBGMessages){
        printf("\n");
        printf("ALPHA VECTOR\n");
        for(size_t loopA=0;loopA<alphaVector.size();loopA++){
          printf("%e\n",alphaVector[loopA]);
        }
        printf("\n");
      }


      // UPDATE STATISTICS AND RESIDUALS IF PROPER ACTION
      if(currAction == kActionSkipForAlignment){
        // Perform Another Selection Iterations
        converged = false;
      }else{

      // UPDATE
      if(opts.printDBGMessages){
        printf(" --- RVM UPDATE\n");
      }

      // Update Statistics
      updateStatistics(opts,totBasis,S_in,Q_in,
                       isInModel,globalToLocalMap,alphaVector,
                       S_out,Q_out,Theta);    

      // Check Residual Norm
      resNorm = evalResidualNorm(totSamples,totBasis,basisMat,yValues,isInModel,globalToLocalMap,muVector,basisScales);  

      // Check Convergence      
      converged = (currAction == kActionFinish)or(currIt >= opts.maxIteration);
      selectionIterationsFinished = (currAction == kActionFinish);
      if(converged){
        if(opts.printDBGMessages){   
          printf("\n");
          printf("Iterations CONVERGED at Iteration %d\n",currIt);
        }
      }


      }

      // Print Iteration Info
      actionString = getActionString(currAction);
      iterationString = to_string(betaCurrIt)+"-"+to_string(currIt);
      if(opts.printProgressToScreen){
        printf("%15s %15.3f %15d %15s %15.3f %15e\n",iterationString.c_str(),maxLikeChange,maxLikeBasis,actionString.c_str(),resNorm,beta);
      }
      
    }

    // Update Estimate of the Noise Vector
    beta = updateNoiseVariance(totSamples,totBasis,totBasisInModel,
                               beta,
                               isInModel,globalToLocalMap,
                               basisMat,yValues,
                               muVector,sigmaMat,
                               alphaVector,deltaLogBeta);
    
    if(opts.printDBGMessages){
      printf("UPDATED NOISE VALUE: %e\n",beta);  
    }

    // Check Beta Convergence
    if((!opts.estimateNoise)||
       (betaCurrIt>opts.maxBetaIterations)||
       ((fabs(deltaLogBeta)<1.0e-6)&&(selectionIterationsFinished))){ 

      // Set Beta Convergence
      betaConverged = true;
      if(opts.printDBGMessages){
        if(betaCurrIt >= opts.maxBetaIterations){
          printf("Maximum Noise Iterations Exceeded.\n");  
        }else{
          printf("Noise Iterations CONVERGED, deltaLogBeta: %e\n",deltaLogBeta);  
        }
      }

    }else{

      // Set Beta Convergence
      betaConverged = false;
      if(opts.printDBGMessages){
        printf("\n");
        printf("Additional Iteration due to change in noise precision %e\n",deltaLogBeta);
      }      

      // Update quantities and perform another iteration
      updateQtyForBetaChange(totSamples,totBasis,totBasisInModel,
                             beta,basisInModel,basisMat,
                             alphaVector,yValues,
                             sigmaMat,muVector,
                             S_in,Q_in,S_out,Q_out,
                             Theta);

      // PRINT S AND Q
      if(opts.printDBGMessages){
        printf("\n");
        printf("Updated Noise Precision: %f\n",beta);
        printf("\n");
        printf("%15s %15s %15s %15s\n","Q_in","Q_out","S_in","S_out");
        for(int loopA=0;loopA<totBasis;loopA++){
          printf("%15.3f %15.3f %15.3f %15.3f\n",Q_in[loopA],Q_out[loopA],S_in[loopA],S_out[loopA]);
        }
        // Print Covariance and Average Vector
        printCovariance(totBasisInModel,sigmaMat);
        printAverageCoeff(totBasisInModel,muVector);
      }
    }
  }

  // INVERT COVARIANCE MATRIX
  // stdMat coeffsCovTemp;
  // computeCoefficientsCovariance(opts.printDBGMessages,totBasisInModel,sigmaMat,coeffsCovTemp);
  
  // FINAL COPY OF COEFFICIENTS AND COVARIANCES
  // Allocate Coefficient Vector and Covariance Matrix
  coeffs.resize(totBasis);
  coeffsCov.resize(totBasis);
  for(int loopA=0;loopA<totBasis;loopA++){
    coeffsCov[loopA].resize(totBasis);
    for(int loopB=0;loopB<totBasis;loopB++){
      coeffsCov[loopA][loopB] = 0.0;
    }
  }
  // Copy Final Version of Coefficients and Associated Covariance
  if(opts.printDBGMessages){
    printf("FINAL BASIS IN MODEL\n");
    printf("%8s %15s %15s\n","Index","Coefficient","Std");
  }
  int globalRowID = 0;
  int globalColID = 0;
  double covScaleFactor = 0;
  for(int loopA=0;loopA<totBasisInModel;loopA++){
    globalRowID = basisInModel[loopA];
    // Assign Coefficient Vector
    coeffs[globalRowID] = muVector[loopA]/basisScales[globalRowID];
    //coeffPrec[globalRowID] = alphaVector[loopA]/(basisScales[globalRowID]*basisScales[globalRowID]);
    for(int loopB=0;loopB<totBasisInModel;loopB++){
      globalColID = basisInModel[loopB];
      covScaleFactor = (basisScales[globalRowID]*basisScales[globalColID]);
      // coeffsCov[globalRowID][globalColID] = coeffsCovTemp[loopA][loopB]/(covScaleFactor*covScaleFactor);
      coeffsCov[globalRowID][globalColID] = sigmaMat[loopA][loopB]/(covScaleFactor*covScaleFactor);
      
      //coeffsCov[globalRowID][globalColID] = coeffsCovTemp[loopA][loopB]/(covScaleFactor);
      //coeffsCov[globalRowID][globalColID] = coeffsCovTemp[loopA][loopB];
    }
    // Plot Basis In Model
    if(opts.printDBGMessages){
    printf("%8d %15.6e %15.6e\n",globalRowID,coeffs[globalRowID],coeffsCov[globalRowID][globalRowID]);
    }
  }

  // Assign Noise Standard Deviation Estimate
  noiseEstimate = sqrt(1.0/beta);

  // Return No Errors
  return sqrt(1.0/beta);
}
