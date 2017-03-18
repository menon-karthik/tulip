
# include "cmTableInterpolator.h"

void solveLSProblem(int totRows,int totCols, 
                    stdVec rhs,stdMat mat,
                    stdVec &coeffs,double &resNorm){

  // Copy MAT and RHS
  arma::mat armaMAT(totRows,totCols);
  arma::vec armaRHS(totRows);
  for(int loopA=0;loopA<totRows;loopA++){
    armaRHS(loopA) = rhs[loopA];
    for(int loopB=0;loopB<totCols;loopB++){
      armaMAT(loopA,loopB) = mat[loopA][loopB];
    }
  }
  
  // Solve Least Squares
  arma::vec lsSol = arma::solve(armaMAT,armaRHS);

  // Copy Coefficients
  for(int loopA=0;loopA<totCols;loopA++){
    coeffs.push_back(lsSol(loopA));
  }
  
  // Compute Residual Norm
  arma::vec resVec = armaRHS - armaMAT*lsSol;
  resNorm = 0.0;
  for(int loopA=0;loopA<totRows;loopA++){
    resNorm += (resVec(loopA))*(resVec(loopA));
  }
  resNorm = sqrt(resNorm/(double)totRows);
}

// ===========
// CONSTRUCTOR
// ===========
cmTableInterpolator::cmTableInterpolator(string tableFile, string weightFile,
                                         int interpolationMethod, int order,
                                         int localTotInputs, stdIntVec &localInputCols, 
                                         int localTotOutputs, stdIntVec &localOutputCols,
                                         stdVec &localTargetAV, stdVec &localTargetSD){

  // Store totals
  totInputs = localTotInputs;
  totOutputs = localTotOutputs;

  // Store local input columns
  for(int loopA=0;loopA<localInputCols.size();loopA++){
    inputCols.push_back(localInputCols[loopA]);
  }

  // Store local output columns
  for(int loopA=0;loopA<localOutputCols.size();loopA++){
    outputCols.push_back(localOutputCols[loopA]);
  }

  // Copy target quantity average and standard deviation
  for(int loopA=0;loopA<localTargetAV.size();loopA++){
    targetAV.push_back(localTargetAV[loopA]);
  }
  for(int loopA=0;loopA<localTargetSD.size();loopA++){
    targetSD.push_back(localTargetSD[loopA]);
  }

  // Store Interpolation Method
  interpMethod = interpolationMethod;

  // Store Polynomial Order
  polyOrder = order;

  // Read Table from file
  int error = readTableFromFile(tableFile,tableValues);
  if(error != 0){
    printf("ERROR: Reading Simulation Table.\n");
    exit(1);
  }

  // Form Samples
  stdVec limits;
  getParameterLimits(limits);
  // Print Limits for Debug
  //for(int loopA=0;loopA<totInputs;loopA++){
  //  printf("Min: %f, Max: %f\n",limits[2*loopA + 0],limits[2*loopA + 1]);
  //}
  double currValue = 0.0;
  uqSamples xValues(totInputs);
  stdVec sample;
  for(int loopA=0;loopA<tableValues.size();loopA++){
    sample.clear();
    for(int loopB=0;loopB<totInputs;loopB++){
      // Get Table Value
      currValue = tableValues[loopA][inputCols[loopB]];
      // Map to [-1,1]
      currValue = 2.0*(currValue - limits[loopB*2 + 0])/(limits[loopB*2 + 1] - limits[loopB*2 + 0]) - 1.0;
      sample.push_back(currValue);
    }
    xValues.addOneSample(sample);
  }

  // Print Samples For Debug Purposes
  xValues.printToFile("Samples.txt",true);
  
  // Form Polynomial Matrix
  uqPolyMatrix polyMat(xValues,order,kPolyLegendre,kMIPartialOrder);

  // Find Coefficients for every output
  stdVec coeffs;
  stdVec rhs;
  stdMat coeffsCov;
  double resNorm = 0.0;
  double currCoeff = 0.0;
  int lastWeightCol = 0;
  for(int loopA=0;loopA<totOutputs;loopA++){
    
    // Init Coeffs
    coeffs.clear();
    rhs.clear();
    coeffsCov.clear();
      
    // Assemble rhs vector
    for(int loopB=0;loopB<tableValues.size();loopB++){
      rhs.push_back(tableValues[loopB][outputCols[loopA]]);
    }

    // COMPUTE INTERPOLATION COEFFICIENTS
    if(interpolationMethod == kLeastSquares){
      
      // Find the coefficients for every output
      //polyMat.printToFile("PolyMat.txt",true);
      //printf("RHS Vector\n");
      //for(int loopC=0;loopC<rhs.size();loopC++){
      //  printf("rhs n. %d: %e\n",loopC,rhs[loopC]);
      //}

      solveLSProblem(polyMat.getRowCount(),polyMat.getColCount(),rhs,polyMat.getMatrix(),coeffs,resNorm);      

      printf("Output: %d, Res Norm: %e\n",loopA,resNorm);
      //for(int loopC=0;loopC<polyMat.getColCount();loopC++){
      //  printf("Coeff n. %d: %e\n",loopC,coeffs[loopC]);
      //}
      
      // Store the coefficient representation for each output quantity
      outputCoeffs.push_back(coeffs);
    }else if(interpolationMethod == kBayesCS){
      // COMPUTATION OF THE COEFFICIENTS WITH BAYES CS
      // Initialize a new algorithm
      uqAlgorithmBCS bcs;
      // Launch the new algorithm
      bcs.run(polyMat.getRowCount(),polyMat.getColCount(),rhs,polyMat.getMatrix(),
              coeffs,coeffsCov,resNorm);
      // Store the coefficient representation for each output quantity
      outputCoeffs.push_back(coeffs);      
    }else if(interpolationMethod == kSparseGrid){
      // COMPUTATION OF THE COEFFICIENTS WITH NUMERICAL INTEGRATION ON SPARSE GRIDS
      // Read Weight Matrix
      stdMat weightTable;
      int error = readTableFromFile(weightFile,weightTable);
      if(error != 0){
        throw new cmException("ERROR: Invalid Weight Matrix Files.\n");  
      }
      lastWeightCol = weightTable[0].size();
      // Determine the coefficients using numerical Integration
      coeffs.clear();
      for(int loopA=0;loopA<polyMat.getColCount();loopA++){
        currCoeff = 0.0;
        for(int loopB=0;loopB<polyMat.getRowCount();loopB++){
          currCoeff += rhs[loopB] + polyMat(loopB,loopA) *  weightTable[loopB][lastWeightCol];
        }
        // Store Coefficient
        coeffs.push_back(currCoeff);
      }
      // Add Current Coefficients to representation vector
      outputCoeffs.push_back(coeffs);
    }else if(interpolationMethod == kNearestNighbor){
      
    }else{
      throw new cmException("ERROR: Invalid Interpolation Type.\n");
    }
  }
}

// GET NUMBER OF PARAMETERS
int cmTableInterpolator::getParameterTotal(){
  return totInputs;
}

// GET PARAMETER NAME
string cmTableInterpolator::getParamName(int parID){
  return string("Parameter " + to_string(parID) + "\n");
}

// GET TOTAL STATES
int cmTableInterpolator::getStateTotal(){
  return 0;
}

// GET TOTAL RESULTS
int cmTableInterpolator::getResultTotal(){
  return totOutputs;
}

// GET RESULTS NAME
string cmTableInterpolator::getResultName(int resID){
  throw cmException("ERROR: cmTableInterpolator::getResultName not implemented.\n");
}

// GET PARAMETER LIMITS FROM TABLE
void cmTableInterpolator::getParameterLimits(stdVec& limits){
  limits.resize(2 * getParameterTotal());
  // Return maximum and minimum values from table
  double currValue = 0.0;
  for(int loopA=0;loopA<totInputs;loopA++){
    limits[loopA * 2 + 0] = tableValues[0][inputCols[loopA]];
    limits[loopA * 2 + 1] = tableValues[0][inputCols[loopA]];
    for(int loopB=0;loopB<tableValues.size();loopB++){
      // Get Current Value
      currValue = tableValues[loopB][inputCols[loopA]];
      if(currValue < limits[loopA * 2 + 0]){
        limits[loopA * 2 + 0] = currValue;
      }
      if(currValue > limits[loopA * 2 + 1]){
        limits[loopA * 2 + 1] = currValue;
      }
    }
  }
}
// Get Default Parameters: Centre of the admissible space
void cmTableInterpolator::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  // Return 
  stdVec limits;
  getParameterLimits(limits);
  for(int loopA=0;loopA<totInputs;loopA++){
    params[loopA] = 0.5 * (limits[loopA*2 + 0] + limits[loopA*2 + 1]);
  }
}

// Prior Mapping
void cmTableInterpolator::getPriorMapping(int priorModelType,int* prPtr){

}

// Eval distance between points
double evalPointDistance(stdVec inputs,stdVec currPoint){
  double result = 0.0;
  for(int loopA=0;loopA<inputs.size();loopA++){
    result = result + (inputs[loopA]-currPoint[loopA])*(inputs[loopA]-currPoint[loopA]);
  }
  return sqrt(result);
}

// Get closer point Id
int cmTableInterpolator::getCloserPointId(stdVec inputs){
  double currDistance = 0.0;
  stdVec currPoint;
  int result = -1;
  double minDistance = std::numeric_limits<double>::max();
  for(int loop0=0;loop0<tableValues.size();loop0++){
    // Get Current Point
    currPoint.clear();
    for(int loopA=0;loopA<totInputs;loopA++){
      currPoint.push_back(tableValues[loop0][inputCols[loopA]]);
    }
    // Eval distance
    currDistance = evalPointDistance(inputs,currPoint);
    // Store minimum distance point
    if(currDistance < minDistance){
      minDistance = currDistance;
      result = loop0;
    }
  }
  // Return minimum index
  return result;
}

// ================
// EVAL MODEL ERROR
// ================
double cmTableInterpolator::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){

  // Init Error
  errorCode.clear();
  errorCode.push_back(0);

  if(interpMethod != kNearestNighbor){
    // Convert Input to Samples
    uqSamples sample(totInputs);

    // Form Samples
    stdVec limits;
    getParameterLimits(limits);
    double currValue = 0.0;
    stdVec currSample;
    for(int loopB=0;loopB<totInputs;loopB++){
      // Get Table Value
      currValue = inputs[loopB];
      // Map to [-1,1]
      currValue = 2.0*(currValue - limits[loopB*2 + 0])/(limits[loopB*2 + 1] - limits[loopB*2 + 0]) - 1.0;
      currSample.push_back(currValue);
    }
    sample.addOneSample(currSample);
  
    // Form Polynomial Matrix
    uqPolyMatrix polyMat(sample,polyOrder,kPolyLegendre,kMIPartialOrder);
    int totRows = polyMat.getRowCount();
    int totCols = polyMat.getColCount();

    // Loop on the output quantities
    outputs.clear();
    double currOutput = 0.0;
    for(int loopA=0;loopA<totOutputs;loopA++){

      // Multiply the components to produce the synthetic output
      currOutput = 0.0;
      for(int loopB=0;loopB<totCols;loopB++){
        currOutput += polyMat(0,loopB) * outputCoeffs[loopA][loopB];
      }

      // Store output
      outputs.push_back(currOutput);
    }
 
  }else{

    // Find the Id of the closer point
    int closerPointId = getCloserPointId(inputs);

    // Loop on all the outputs
    outputs.clear();
    double currOutput = 0.0;
    for(int loopA=0;loopA<totOutputs;loopA++){
      // Store output
      outputs.push_back(tableValues[closerPointId][outputCols[loopA]]);
    }
  }
  
  // Eval Model Error Part
  double modelError = 0.0;
  double currSD = 0.0;
  int currCol = 0;
  for(int loopA=0;loopA<totOutputs;loopA++){
    currCol = outputCols[loopA]-totInputs;
    if(fabs(targetSD[currCol])>1.0e-6){
      currSD = targetSD[currCol];
    }else{
      currSD = 1.0;
    }
    //printf("Loop %d, Column  %d, Output %e, targetAV %e, targetSD %e\n",loopA,
    //  currCol,outputs[loopA],targetAV[currCol],currSD);
    modelError += (targetAV[currCol] - outputs[loopA])*(targetAV[currCol] - outputs[loopA])/(currSD*currSD);
  }
  // Return log-likelihood
  return sqrt(modelError);
}
