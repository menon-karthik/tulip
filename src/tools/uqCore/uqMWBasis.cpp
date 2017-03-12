# include "uqMWBasis.h"

// Constructor
uqMWBasis::uqMWBasis(int order)
{
  // Set Order
  basisOrder = order; 
  // set Default Quadrature Points
  quadratureOrder = 30;
  // Form Uniform Measure
  isMeasureUniform = true;
  formUniformMeasure(quadratureOrder);
  // Form Orthogonal Multiscaling Basis
  orthoPoly = new uqOrthoPolyBasis(order,quadratureOrder,probabilityMeasure);
  // Allocate MW Coefficients
  AllocateMWCoefficients();
  // form Multiwavelet Coefficients
  FormMultiWaveletCoeffs();
}

// Constructor 2
uqMWBasis::uqMWBasis(int order,int quadOrder)
{
  // Set Order
  basisOrder = order; 
  // set Quadrature Points
  quadratureOrder = quadOrder;
  // Form Uniform Measure
  isMeasureUniform = true;
  formUniformMeasure(quadratureOrder);
  // Form Orthogonal Multiscaling Basis
  orthoPoly = new uqOrthoPolyBasis(order,quadratureOrder,probabilityMeasure);
  // Allocate MW Coefficients
  AllocateMWCoefficients();
  // form Multiwavelet Coefficients
  FormMultiWaveletCoeffs();
}

// Constructor 3
uqMWBasis::uqMWBasis(int order,int quadOrder, stdVec probMeasure)
{
  // Set Order
  basisOrder = order; 
  // set Quadrature Points
  quadratureOrder = quadOrder;
  // Form Uniform Measure
  isMeasureUniform = false;
  // Store Measure
  probabilityMeasure.resize(probMeasure.size());
  for(int loopA=0;loopA<probMeasure.size();loopA++){
    probabilityMeasure[loopA] = probMeasure[loopA];
  }
  // Form Orthogonal Multiscaling Basis
  orthoPoly = new uqOrthoPolyBasis(order,quadratureOrder,probabilityMeasure);

  // Allocate MW Coefficients
  AllocateMWCoefficients();

  // form Multiwavelet Coefficients
  FormMultiWaveletCoeffs();

}

// Assemble a Constant Probability measure
void uqMWBasis::formUniformMeasure(int quadOrder){
  probabilityMeasure.resize(2*quadOrder);
  for(int loopA=0;loopA<2*quadOrder;loopA++){
    probabilityMeasure[loopA] = 1.0;
  }
}

// Destructor
uqMWBasis::~uqMWBasis(){
  // Deallocate quantities
  delete orthoPoly;
}

// Allocate All Coefficient Vectors
void uqMWBasis::AllocateMWCoefficients(){

  // Allocate
  monoCoeffs.resize(basisOrder);
  productMat.resize(basisOrder);
  gsCoeffs.resize(basisOrder-1);
  refPoly.resize(basisOrder);
  normCoeffs.resize(basisOrder);
  for(int loopA=0;loopA<basisOrder;loopA++){
    monoCoeffs[loopA].resize(basisOrder);
    productMat[loopA].resize(basisOrder);
    if(loopA<basisOrder-1){
      gsCoeffs[loopA].resize(basisOrder-1);
    }
  }
  
  // Initialize
  for(int loopA=0;loopA<basisOrder;loopA++){
    refPoly[loopA] = 0;
    normCoeffs[loopA] = 0.0;
    for(int loopB=0;loopB<basisOrder;loopB++){
      monoCoeffs[loopA][loopB] = 0.0;
      productMat[loopA][loopB] = 0.0;
      if((loopA<basisOrder-1)&&(loopB<basisOrder-1)){
        gsCoeffs[loopA][loopB] = 0.0;
      }
    }
  }
}

// Eval Traditional Monomials for a given approximation Order
double EvalBasic_P_Polynomials(double XValue, int Order){
  double result = 1.0;
  for(int loopA=0;loopA<Order;loopA++){
    result *= XValue;
  }
  return result;
}

// Eval QNabla Polynomials
double EvalBasic_QNabla_Polynomials(double XValue, double MinX, double MaxX, int Order)
{
  double halfValue = 0.5*(MinX+MaxX);
  double result = 0.0;
  bool isRight = (XValue>(halfValue));
  // Eval Function
  if (isRight) {
    result = EvalBasic_P_Polynomials(XValue,Order);
  }else{
    result = -EvalBasic_P_Polynomials(XValue,Order);
  }
  return result;
}

// Eval Q Polynonials
double EvalBasic_Q_Polynomials(double XValue, int  Index, int MaxOrder, double MinX, double MaxX, stdMat firstStepCoeff){
  // Eval Function
  double result = EvalBasic_QNabla_Polynomials(XValue,MinX,MaxX,Index);
  for(int loopA=0;loopA<MaxOrder;loopA++){
    result += firstStepCoeff[Index][loopA]*EvalBasic_P_Polynomials(XValue,loopA);
  }
  return result;
}

// Build Auxiliary Polynomials
double BuildAuxPoly(double XValue, int ConstructionStep, int CurrentOrder, int MaxOrder,
                    double MinX, double MaxX, stdMat MonoCoeff,stdIntVec RefPoly, stdMat productMat){
  
  // Allocate Auxiliary Vector
  double auxVector[MaxOrder];
  // Initialize
  for(int loopA=0;loopA<MaxOrder;loopA++){
    auxVector[loopA] = EvalBasic_Q_Polynomials(XValue,loopA,MaxOrder,MinX,MaxX,MonoCoeff);
  }
  // Combine
  for(int loopA=0;loopA<ConstructionStep;loopA++){
    for(int loopB=0;loopB<MaxOrder;loopB++){
      if (loopB != RefPoly[loopA]){
        auxVector[loopB] -= productMat[loopB][loopA] * auxVector[RefPoly[loopA]];
      }
    }
  }
  // result
  return auxVector[CurrentOrder];
}

// Eval Basic R Polynomials
double EvalBasic_R_Polynomials(double XValue, int currentJ, int maxOrder, stdMat firstStepCoeff, stdIntVec refPoly,
                               stdMat productMat, double minX, double maxX, stdMat coeffMat){
  // Initialize RPoly
  stdVec rPoly(maxOrder);
  for(int loopA=0;loopA<maxOrder;loopA++){
    rPoly[loopA] = 0.0;
  }
  // Evaluate RPoly
  for(int loopA=(maxOrder-1);loopA>=currentJ;loopA--){
    rPoly[loopA] = BuildAuxPoly(XValue,(maxOrder-1),loopA,maxOrder,minX,maxX,firstStepCoeff,refPoly,productMat);
    for(int loopB=(loopA+1);loopB<maxOrder;loopB++){
      rPoly[loopA] = rPoly[loopA] + coeffMat[loopA][loopB-1] * rPoly[loopB];
    }
  }
  // Send Results Back
  return rPoly[currentJ];
}

// Eval Gram-Schmidt System Matrix
void EvalGSSystemMat(int currentJ, int order, 
                     int totalQuadraturePoints, stdVec ccPoints, stdVec ccWeights, stdVec measure,
                     stdMat firstStepCoeff, stdIntVec refPoly,
                     stdMat productMat, stdMat coeffMat, double minX, double maxX,
                     stdMat& SysMat){
                       
  // Assemble Matrix Terms
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double firstPoly = 0.0;
  double secondPoly = 0.0;
  for(int loopA=0;loopA<(order-(currentJ+1));loopA++){
    for(int loopB=0;loopB<(order-(currentJ+1));loopB++){
      SysMat[loopA][loopB] = 0.0;
      for(int loopC=0;loopC<totalQuadraturePoints;loopC++){
        currentPoint = ccPoints[loopC];
        currentWeight = ccWeights[loopC];
        // First Polynomial
        firstPoly = EvalBasic_R_Polynomials(currentPoint,currentJ+1+loopA,order,firstStepCoeff,refPoly,productMat,minX,maxX,coeffMat);
        // Second Polynomial
        secondPoly = EvalBasic_R_Polynomials(currentPoint,currentJ+1+loopB,order,firstStepCoeff,refPoly,productMat,minX,maxX,coeffMat);
        // Form Matrix
        SysMat[loopA][loopB] += firstPoly * secondPoly * measure[loopC] * currentWeight;
      }
    }
  }
}

// Eval Gram-Schmidt Right Hand Side
void EvalGSSystemRHS(int currentJ, int maxOrder, 
                     int totalQuadraturePoints, 
                     stdVec ccPoints, stdVec ccWeights, stdVec measure,
                     stdMat firstStepCoeff, stdIntVec refPoly,
                     stdMat productMat, stdMat coeffMat,
                     double minX, double maxX, 
                     stdVec& matRHS){
       
  // Initialize           
  for(int loopA=0;loopA<maxOrder;loopA++){
    matRHS[loopA] = 0.0;
  }

  // Assemble Matrix Terms
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double firstPoly = 0.0;
  double secondPoly = 0.0;
  for(int loopA=0;loopA<(maxOrder-(currentJ+1));loopA++){
    matRHS[loopA] = 0.0;
    for(int loopC=0;loopC<totalQuadraturePoints;loopC++){
      currentPoint = ccPoints[loopC];
      currentWeight = ccWeights[loopC];
      // First Polynomial
      firstPoly = EvalBasic_R_Polynomials(currentPoint,currentJ+1+loopA,maxOrder,firstStepCoeff,refPoly,productMat,minX,maxX,coeffMat);
      // Second Polynomial
      secondPoly = BuildAuxPoly(currentPoint,maxOrder-1,currentJ,maxOrder,minX,maxX,firstStepCoeff,refPoly,productMat);
      // Form Matrix
      matRHS[loopA] = matRHS[loopA] - firstPoly * secondPoly * measure[loopC] * currentWeight;
    }
  }
}

// Get a Coordinate Index Array from the global interval numbering in arbitrary dimensions
void GetMWShiftIndexes(int numberOfDimensions, int mwScaleIDX, int mwShiftIDX, stdIntVec& currentIDX){
    
  // Check If The Index is Too Big
  if((mwShiftIDX!=0)&&(mwShiftIDX>(int)(pow(2,numberOfDimensions*mwScaleIDX))-1)){
    throw "Error: Shift Index Too Big!";
  }
  // Check If Shift Index is Equal to Zero
  if(mwShiftIDX == 0){
    // All Zeros
    for(int loopA=0;loopA<numberOfDimensions;loopA++){
      currentIDX[loopA] = 0;
    }
  }else{
    // Initialize
    int currentInt = mwShiftIDX;
    int value = 0;

    // Find the indexes for all fathers
    int* fatherIDs = new int[mwScaleIDX];
    for(int loopA=0;loopA<mwScaleIDX;loopA++){
      fatherIDs[loopA] = (currentInt % (int)(pow(2,numberOfDimensions)));
      currentInt = (currentInt)/((int)(pow(2,numberOfDimensions)));
    }

    // Initialize
    for(int loopA=0;loopA<numberOfDimensions;loopA++){
      currentIDX[loopA] = 0;
    }

    // Get Coordinates
    for(int loopA=0;loopA<mwScaleIDX;loopA++){
      value = fatherIDs[loopA];
      for(int loopB=0;loopB<numberOfDimensions;loopB++){
        currentIDX[loopB] = currentIDX[loopB] + (value % 2)*((int)(pow(2,loopA-1)));
        value = (value / 2);
      }
    }
    delete [] fatherIDs;
  }
}

// Eval RHS Matrix for First Step
void EvalFirstStepRHSMatrix(int Order, int TotalQuadraturePoints, stdVec CCPoints, stdVec CCWeights,
                            stdVec measure, double MinX, double MaxX, stdMat& rhsMat){
  // Eval RHS
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double firstP = 0.0;
  double secondP = 0.0;
  for(int loopA=0;loopA<Order;loopA++){
    for(int loopB=0;loopB<Order;loopB++){
      rhsMat[loopA][loopB] = 0.0;
      for(int loopC=0;loopC<TotalQuadraturePoints;loopC++){
        currentPoint =  CCPoints[loopC];
        currentWeight = CCWeights[loopC];
        firstP = EvalBasic_P_Polynomials(currentPoint,loopA);
        secondP = EvalBasic_QNabla_Polynomials(currentPoint,MinX,MaxX,loopB);
        rhsMat[loopA][loopB] +=  firstP * secondP * measure[loopC] * currentWeight;
      }
    }
  }
}

// Eval The Coefficient Matrix to Solver the First Step
void EvalFirstStepCoefficientMatrix(int Order, int TotalQuadraturePoints, stdVec CCPoints, stdVec CCWeights,
                                    stdVec measure, stdMat& coeffMat){
  // Eval Coefficient Matrix
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double firstP = 0.0;
  double secondP = 0.0;
  for(int loopA=0;loopA<Order;loopA++){
    for(int loopB=0;loopB<Order;loopB++){
      coeffMat[loopA][loopB] = 0.0;
      for(int loopC=0;loopC<TotalQuadraturePoints;loopC++){
        currentPoint =  CCPoints[loopC];
        currentWeight = CCWeights[loopC];
        firstP =  EvalBasic_P_Polynomials(currentPoint,loopA);
        secondP = EvalBasic_P_Polynomials(currentPoint,loopB);
        coeffMat[loopA][loopB] += firstP * secondP * measure[loopC] * currentWeight;
      }
    }
  }
}

// Solve First Solver Step
void SolveFirstStep(int order, int TotalQuadraturePoints, stdVec CCPoints, stdVec CCWeights, 
                    stdVec measure, double MinX, double MaxX, stdMat& FirstStepCoeffs){
  
  // Allocate
  stdMat coeffMat;
  stdMat rhsMat;
  coeffMat.resize(order);
  rhsMat.resize(order);
  for(int loopA=0;loopA<order;loopA++){
    coeffMat[loopA].resize(order);
    rhsMat[loopA].resize(order);
  }
  stdVec currentRHS(order);
  stdVec sol(order);
  
  // Eval System Matrix
  EvalFirstStepCoefficientMatrix(order,TotalQuadraturePoints,CCPoints,CCWeights,measure,coeffMat);

  // Eval RHS Matrix
  EvalFirstStepRHSMatrix(order,TotalQuadraturePoints,CCPoints,CCWeights,measure,MinX,MaxX,rhsMat);

  // Create Coefficient Matrix
  for(int loopA=0;loopA<order;loopA++){
    // Copy RHS
    for(int loopB=0;loopB<order;loopB++){
      currentRHS[loopB] = -rhsMat[loopB][loopA];
    }    

    // Solve Dense Linear System Of Equations
    uqUtils::solveDenseLinearSystem(order,order,coeffMat,currentRHS,sol);

    // Store Coefficients: Stored in Rows
    for(int loopB=0;loopB<order;loopB++){
      FirstStepCoeffs[loopA][loopB] = sol[loopB];
    }
  }
}

// Solve Second Step 
void SolveSecondStep(int MaxOrder, int TotalQuadraturePoints, stdVec CCPoints, stdVec CCWeights, stdVec measure,
                     stdMat MonoCoeff, double MinX, double MaxX,stdIntVec& refPoly, stdMat& productMat){
                       
  // Allocate vectors
  stdBoolVec isActive(MaxOrder);
  for(int loopA=0;loopA<MaxOrder;loopA++){
    isActive[loopA] = true;
  }

  // Ciclo Su tutti i Livelli
  bool finished = false;
  int loopA = 0;
  int currentOrder = 0;
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double firstPoly = 0.0;
  double secondPoly = 0.0;
  while((!finished)&&(loopA<MaxOrder)){
    currentOrder = loopA + MaxOrder;
    for(int loopB=0;loopB<MaxOrder;loopB++){
      if(isActive[loopB]){
        productMat[loopB][loopA] = 0.0;
        for(int loopC=0;loopC<TotalQuadraturePoints;loopC++){
          currentPoint =  CCPoints[loopC];
          currentWeight = CCWeights[loopC];
          // First Polynomial
          firstPoly = BuildAuxPoly(currentPoint,loopA,loopB,MaxOrder,MinX,MaxX,MonoCoeff,refPoly,productMat);
          //FirstPoly:=EvalBasic_Q_Polynomials(CurrentPoint,LoopB,MaxOrder,MinX,MaxX,MonoCoeff,RefPoly,ProductMat);
          // Second Polynomial
          secondPoly = EvalBasic_P_Polynomials(currentPoint,currentOrder);
          // Form Matrix
          productMat[loopB][loopA] += firstPoly * secondPoly * measure[loopC] * currentWeight;
        }
      }
    }
    // Cerchi Sulla riga il valore diverso da Zero
    bool found = false;
    int count = 0;
    double diagValue = 0.0;
    while((!found)&&(count<MaxOrder)){
      if(isActive[count]){
        if (fabs(productMat[count][loopA])>kMathZero){
          // Segnati il Coefficiente
          refPoly[loopA] = count;
          // Dividi tutti i Coeff per questo valore
          diagValue = productMat[count][loopA];
          for(int loopC=0;loopC<MaxOrder;loopC++){
            productMat[loopC][loopA] = productMat[loopC][loopA]/diagValue;
          }
          // Rendi Inactive
          isActive[count] = false;
          // Found
          found = true;
        }
      }
      // Update
      count++;
    }
    if (!found){
      finished = true;
    }
    // Increment Construction Step
    loopA++;
  }
}

// Check if Samples are available for a given scale and shift IDs
bool CheckSamplesAvailability(int numberOfDimensions, 
                              int TotalSamples, stdMat sampleXValues,
                              int MWScaleIDX, stdIntVec CurrentIDX){
  
  // Check If There Are Samples Available
  if (TotalSamples<1){
    return false;
  }

  // Loop On Samples
  bool result = true;
  int totIntervals = 0;
  int singleDimensionShiftIDX = 0;
  double sampleValue = 0.0;
  double minIntevalLimit = 0.0;
  double maxIntevalLimit = 0.0;
  for(int loopA=0;loopA<TotalSamples;loopA++){
    // Initialize Result
    result = true;
    for(int loopB=0;loopB<numberOfDimensions;loopB++){
      sampleValue = sampleXValues[loopB][loopA];
      // Find Current Interval Limits for the Current Dimension
      totIntervals = pow(2,MWScaleIDX);
      // Find Shift Index For Current Dimension
      singleDimensionShiftIDX = CurrentIDX[loopB];
      minIntevalLimit = (singleDimensionShiftIDX/totIntervals);
      maxIntevalLimit = ((singleDimensionShiftIDX+1)/totIntervals);
      // Check If is In
      result = (result)&&((sampleValue>minIntevalLimit)&&(sampleValue<=maxIntevalLimit));
    }
    if(result){
      return result;
    } 
  }
  return result;
}

// Eval The total number of columns with and without samples
int EvalNumberOfColumns(bool includeNullColumns, 
                        int totalSamples, int basisSize, 
                        int numberOfDimensions, int maxDetailLevel,
                        stdMat XValues){
  // Intialize
  int totalCols = basisSize;
  bool finished = false;
  int currentMWScaleIndex = -1;
  int currentMWShiftIndex = 0;
  int maxShiftIndex = 0;
  bool areSamplesAvailable = false;
  // Allocate
  stdIntVec mwShiftIndex(numberOfDimensions);
  
  while(!finished){
    currentMWScaleIndex++;
    finished = (currentMWScaleIndex > maxDetailLevel);
    maxShiftIndex = (int)(pow(2,currentMWScaleIndex*numberOfDimensions))-1;
    currentMWShiftIndex = -1;
    while((currentMWShiftIndex<maxShiftIndex)&&(!finished)){
      currentMWShiftIndex++;
      // Get Shift Coordinates For Interval
      GetMWShiftIndexes(numberOfDimensions,currentMWScaleIndex,currentMWShiftIndex,mwShiftIndex);
      // Check If There is At Least One Sample Falling Within the Interval
      if(includeNullColumns){
        areSamplesAvailable = true;
      }else{
        areSamplesAvailable = CheckSamplesAvailability(numberOfDimensions,totalSamples,XValues,currentMWScaleIndex,mwShiftIndex);
      }
      // Increase Counter
      if(areSamplesAvailable){
        for(int loopC=0;loopC<basisSize;loopC++){
          totalCols++;
        }
      }
    }
  }
  return totalCols;
}

// Perform Gram-Schmidt Orthogonalization
void PerformGSOrthogonalization(int order, int totalQuadraturePoints, stdVec ccPoints, stdVec ccWeights, stdVec measure,
                                stdMat firstStepCoeff, stdIntVec refPoly, stdMat productMat,
                                double minX, double maxX, stdMat& gsCoefficients){
  
  // Initialize Coefficient Matrix
  for(int loopA=0;loopA<(order-1);loopA++){
    for(int loopB=0;loopB<(order-1);loopB++){
      gsCoefficients[loopA][loopB] = 0.0;
    }
  }

  // Allocate
  stdMat gsMat;
  gsMat.resize(order-1);
  for(int loopA=0;loopA<(order-1);loopA++){
    gsMat[loopA].resize(order-1);
  }
  stdVec gsRHS(order);
  stdVec gsSol(order-1);  

  // Initialize Order
  int currentJ = (order-1);
  while(currentJ > 0){
    // Update J
    currentJ--;

    // Eval System Matrix
    EvalGSSystemMat(currentJ,order,totalQuadraturePoints,ccPoints,ccWeights,measure,firstStepCoeff,refPoly,productMat,gsCoefficients,minX,maxX,gsMat);

    // DEBUG
    //uqUtils::PrintMatrixToFile("GSMat.txt",order-1,order-1,gsMat);

    // Eval System RHS
    EvalGSSystemRHS(currentJ,order,totalQuadraturePoints,ccPoints,ccWeights,measure,firstStepCoeff,refPoly,productMat,gsCoefficients,minX,maxX,gsRHS);
    
    // DEBUG
    //uqUtils::PrintVectorToFile("GSRHS.txt",order-1,gsRHS);

    // Solve For Coefficients
    uqUtils::solveDenseLinearSystem((order-1-currentJ),(order-1-currentJ),gsMat,gsRHS,gsSol);

    // Store Coefficients
    for(int loopA=currentJ;loopA<(order-1);loopA++){
      gsCoefficients[currentJ][loopA] = gsSol[loopA-currentJ];
    }    
  }
}

// Eval Normalization Coefficients
void EvalNormalizationCoeffs(int order, int totalQuadraturePoints, stdVec ccPoints, stdVec ccWeights, stdVec measure,
                             stdMat firstStepCoeff, stdIntVec refPoly, stdMat productMat,
                             stdMat coeffMat, double minX, double maxX, stdVec& normCoeffs){
  // Assemble Matrix Terms
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double firstPoly = 0.0;
  double secondPoly = 0.0;
  for(int loopA=0;loopA<order;loopA++){
    normCoeffs[loopA] = 0.0;
    for(int loopC=0;loopC<totalQuadraturePoints;loopC++){
      currentPoint =  ccPoints[loopC];
      currentWeight = ccWeights[loopC];
      // First Polynomial
      firstPoly = EvalBasic_R_Polynomials(currentPoint,loopA,order,firstStepCoeff,refPoly,productMat,minX,maxX,coeffMat);
      // Second Polynomial
      secondPoly = EvalBasic_R_Polynomials(currentPoint,loopA,order,firstStepCoeff,refPoly,productMat,minX,maxX,coeffMat);
      // Form Matrix
      normCoeffs[loopA] += firstPoly * secondPoly * measure[loopC] * currentWeight;
    }
  }

  // Apply Square Root
  for(int loopA=0;loopA<order;loopA++){
    normCoeffs[loopA] = sqrt(normCoeffs[loopA]);
  }
}

// Form Coefficient Matrixes for MultiWavelets
void uqMWBasis::FormMultiWaveletCoeffs(){
  // Init Result
  double minX = -1.0;
  double maxX = 1.0;

  // Allocate
  uq1DQuadratureRule_CCDouble* quadRule = new uq1DQuadratureRule_CCDouble(quadratureOrder,kLegendreRange);
  quadRule->generatePointsAndWeights();
  //quadRule->printToFile("DoubleCCPoints.txt");
  
  // Print Vector
  //uqUtils::PrintVectorToFile("QuadPoints.txt",quadRule->getTotalPoints(),quadRule->getPoints());
  //uqUtils::PrintVectorToFile("QuadWeight.txt",quadRule->getTotalPoints(),quadRule->getWeights());

  // First Step - Making Functions Orthogonal to Monomials
  SolveFirstStep(basisOrder,quadRule->getTotalPoints(),quadRule->getPoints(),quadRule->getWeights(),probabilityMeasure,minX,maxX,monoCoeffs);
  
  // Second Step - Making Functions Orthogonal to Higher Monomials: ORIGINAL ALPERT CONSTRUCTION!!!
  SolveSecondStep(basisOrder,quadRule->getTotalPoints(),quadRule->getPoints(),quadRule->getWeights(),probabilityMeasure,monoCoeffs,minX,maxX,refPoly,productMat);
  
  // Print To File For Debug
  //uqUtils::PrintMatrixToFile("ProductMat.txt",basisOrder,basisOrder,productMat);  

  // Third Step - Making functions Orthogonal to each other
  PerformGSOrthogonalization(basisOrder,quadRule->getTotalPoints(),quadRule->getPoints(),quadRule->getWeights(),probabilityMeasure,monoCoeffs,refPoly,productMat,minX,maxX,gsCoeffs);

  // Fourth Step - Eval Normalization Factors
  EvalNormalizationCoeffs(basisOrder,quadRule->getTotalPoints(),quadRule->getPoints(),quadRule->getWeights(),probabilityMeasure,monoCoeffs,refPoly,productMat,gsCoeffs,minX,maxX,normCoeffs);
}

// Evaluating 1D Mother MultiWavelet of index mwIndex
double uqMWBasis::EvalMotherMW(double XValue, int mwIndex){
  // First Polynomial
  double result = sqrt(2.0)*EvalBasic_R_Polynomials(2.0*XValue-1,mwIndex,basisOrder,monoCoeffs,refPoly,productMat,-1.0,1.0,gsCoeffs);
  return result/normCoeffs[mwIndex];
}

// Evaluating 1D MultiWavelet of any order
double uqMWBasis::EvalMW(double XValue, int scaleIndex, int shiftIndex, int polyOrderIndex){
  if(!isMeasureUniform){
    if(scaleIndex > 0){
      throw uqException("ERROR: Cannot refine Multiwavelet Functions with non uniform measure.\n");
    }
  }
  // Find The Limits
  double minLimit = (1.0/pow(2.0,scaleIndex))*(0.0+shiftIndex);
  double maxLimit = (1.0/pow(2.0,scaleIndex))*(1.0+shiftIndex);
  double centerLimit = 0.5*(minLimit + maxLimit);
  // Check If Point is Different From Zero
  // CAREFULL !!! Check
  bool isOutSide = false;
  if(XValue>(maxLimit+kMathZero)){
    isOutSide = true;
  }
  if(XValue<(minLimit+kMathZero)){
    isOutSide = true;
  }
  if((fabs(XValue)<kMathZero)&&(XValue>(minLimit-kMathZero))){
    isOutSide = false;
  }
  /*bool isOutSide = false;
  if((XValue>=minLimit)&&(XValue<=maxLimit)){
    isOutSide = false;
  }else{
    isOutSide = true;
  }*/
  // Check If Outside
  if(isOutSide){
    return 0.0;
  }else{
    double newCoord = pow(2.0,scaleIndex)*XValue - shiftIndex;
    return pow(2.0,0.5*scaleIndex)*EvalMotherMW(newCoord,polyOrderIndex);
  }
}

// Evaluating Scaled Legendre Polynomial MW
double uqMWBasis::EvalMS(double XValue, int scaleIndex, int shiftIndex, int polyOrderIndex){
  double res = 0.0;
  if(!isMeasureUniform){
    if(scaleIndex > 0){
      throw uqException("ERROR: Cannot refine Multiscaling Functions with non uniform measure.\n");
    }
  }
  // Check Limits
  double minLimit = (1.0/pow(2.0,scaleIndex))*(0.0+shiftIndex);
  double maxLimit = (1.0/pow(2.0,scaleIndex))*(1.0+shiftIndex);
  // Check If it is Inside or Outside
  bool isOutSide = false;
  if(XValue>(maxLimit + kMathZero)){
    isOutSide = true;
  }
  if(XValue<(minLimit + kMathZero)) {
    isOutSide = true;
  }
  if((fabs(XValue)<kMathZero)&&(XValue>(minLimit - kMathZero))){
    isOutSide = false;
  }
  // Transform Coord
  if(isOutSide){
    return 0.0;
  }else{
    double newCoord = pow(2.0,scaleIndex)*XValue-shiftIndex;    
    return pow(2.0,0.5*scaleIndex)*orthoPoly->eval(newCoord,polyOrderIndex);
  }
}

// Eval 
double EvalExactSingleMW(int basisOrder, int polyOrder, double XValue){
  double result = 0.0;
  switch(basisOrder){
    case 0:
      result = (1.0/sqrt(2.0));
      break;
    case 1:
      switch(polyOrder){
        case 0: 
          result = sqrt(3.0/2.0)*(-1.0+2.0*XValue);
          break;
        case 1: 
          result = sqrt(1.0/2.0)*(-2.0+3.0*XValue);
          break;
      }
      break;
    case 2:
      switch(polyOrder){
        case 0: 
          result = (1.0/3.0)*sqrt(1.0/2.0)*(1.0-24.0*XValue+30.0*XValue*XValue);
          break;
        case 1: 
          result = (1.0/2.0)*sqrt(3.0/2.0)*(3.0-16.0*XValue+15.0*XValue*XValue);
          break;
        case 2:
          result = (1.0/3.0)*sqrt(5.0/2.0)*(4.0-15.0*XValue+12.0*XValue*XValue);
          break;
      }
      break;
    case 3:
      switch(polyOrder){
        case 0: 
          result = (1.0/1.0)*sqrt(15.0/34.0)*( 1.0+4.0*XValue-30.0*XValue*XValue+28.0*XValue*XValue*XValue);
          break;
        case 1: 
          result = (1.0/1.0)*sqrt(1.0/42.0)*( -4.0+105.0*XValue-300.0*XValue*XValue+210.0*XValue*XValue*XValue);
          break;
        case 2: 
          result = (1.0/2.0)*sqrt(35.0/34.0)*(-5.0+48.0*XValue-105.0*XValue*XValue+64.0*XValue*XValue*XValue);
          break;
        case 3: 
          result = (1.0/2.0)*sqrt(5.0/42.0)*(-16.0+105.0*XValue-192.0*XValue*XValue+105.0*XValue*XValue*XValue);
          break;
      }
      break;
    case 4:
      switch(polyOrder){
        case 0: 
          result = (1.0/1.0)*sqrt(1.0/186.0)*(1.0+30.0*XValue+210.0*XValue*XValue-840.0*XValue*XValue*XValue+630.0*XValue*XValue*XValue*XValue);
          break;
        case 1: 
          result = (1.0/2.0)*sqrt(1.0/38.0)*(-5.0-144.0*XValue+1155.0*XValue*XValue-2240.0*XValue*XValue*XValue+1260.0*XValue*XValue*XValue*XValue);
          break;
        case 2: 
          result = (1.0/1.0)*sqrt(35.0/14694.0)*(+22.0-735.0*XValue+3504.0*XValue*XValue-5460.0*XValue*XValue*XValue+2700.0*XValue*XValue*XValue*XValue);
          break;
        case 3: 
          result = (1.0/8.0)*sqrt(21.0/38.0)*(+35.0-512.0*XValue+1890.0*XValue*XValue-2560.0*XValue*XValue*XValue+1155.0*XValue*XValue*XValue*XValue);
          break;
        case 4: 
          result = (1.0/2.0)*sqrt(7.0/158.0)*(+32.0-315.0*XValue+960.0*XValue*XValue-1155.0*XValue*XValue*XValue+480.0*XValue*XValue*XValue*XValue);
          break;
      }
      break;
  }
  return sqrt(2.0)*result;
}

// Eval Single Exact MW
double uqMWBasis::EvalExactMotherMW(double XValue, int polyOrder, int basisOrder){

  // Check Max Order
  if (basisOrder>4){ 
    throw uqInterpolationException("Internal: MW Order Cannot be Bigger than 4 for Exact Evaluation.\n");
  }
  // Eval MW
  double currentXValue = XValue;
  currentXValue = 2.0 * currentXValue - 1.0;
  if(currentXValue >= 0.0){
    return EvalExactSingleMW(basisOrder,polyOrder,currentXValue);
  }else{
    if(((basisOrder + polyOrder + 1)%2)!=0){
      return -EvalExactSingleMW(basisOrder,polyOrder,-currentXValue);
    }else{
      return EvalExactSingleMW(basisOrder,polyOrder,-currentXValue);
    }
  }
}

// Eval Single Exact MW
double uqMWBasis::EvalSingleExactMW(double XValue, int scaleIndex, int shiftIndex, int polyOrder, int basisOrder){
  // Scale and Shift
  double newCoord = pow(2.0,scaleIndex) * XValue - shiftIndex;
  if(newCoord > 1.0){
    return 0.0;
  }else if (newCoord < 0.0){
    return 0.0;
  }else{
    return pow(2.0,0.5*scaleIndex) * EvalExactMotherMW(newCoord,polyOrder,basisOrder);
  }
}

// Virtual Evaluation Function
double uqMWBasis::eval(double XValue, int order){
  throw uqException("ERROR: Eval Not implemented for uqMWBasis.\n");
}

int uqMWBasis::getPolynomialType(){
  return -1;
}
