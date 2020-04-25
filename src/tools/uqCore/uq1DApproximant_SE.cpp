# include "uq1DApproximant_SE.h"

// Default Constructor
uq1DApproximant_SE::uq1DApproximant_SE(){
  basis = NULL;
}

uq1DApproximant_SE::uq1DApproximant_SE(approxTypes approxType,int order,int quadOrder,stdVec probMeasure,stdVec locCoeffs,stdVec limits){

  // Define Approx and Polynomial Type
  this->approxType = approxType;

  // Type of Polynomials: Not Considered in this Case
  polyType = 0;

  // Copy translation and scaling
  this->limits.clear();
  this->limits.push_back(limits[0]);
  this->limits.push_back(limits[1]);
  
  // Check consistency between basis order and number of expansion coefficient (equal for 1D)
  if(2*order != (int)locCoeffs.size()){
    throw uqException("ERROR: Basis size and number of expansion coefficients are different.\n");
  }
  
  // Build MW Basis
  if(approxType == atOrthoPoly){
    this->basis = new uqOrthoPolyBasis(order,quadOrder,probMeasure);
  }else if(approxType == atMW){
    this->basis = new uqMWBasis(order,quadOrder,probMeasure);
  }
  
  // Store the coefficient 
  coeff.clear();
  for(int loopA=0;loopA<locCoeffs.size();loopA++){
    coeff.push_back(locCoeffs[loopA]);
  }
}

uq1DApproximant_SE::uq1DApproximant_SE(int polyType,int order,stdVec locCoeffs,stdVec limits){

  // Define Approx and Polynomial Type
  this->approxType = atPoly;

  // Type of Polynomials: Not Considered in this Case
  polyType = polyType;

  // Copy translation and scaling
  this->limits.clear();
  this->limits.push_back(limits[0]);
  this->limits.push_back(limits[1]);
  
  // Build Polynomials Basis
  basis = new uqPolyBasis(polyType,order);

  // Store the coefficient 
  coeff.clear();
  for(int loopA=0;loopA<locCoeffs.size();loopA++){
    coeff.push_back(locCoeffs[loopA]);
  }
}

// Destructor
uq1DApproximant_SE::~uq1DApproximant_SE(){
  coeff.clear();
  limits.clear();
  delete basis;
}

// EVALUATION FUNCTION
double uq1DApproximant_SE::evaluate(double XValue){

  //Scale and shift to [0,1]
  double currVal = (XValue - limits[0])/(limits[1] - limits[0]);

  // For the vector of function evaluations
  stdVec mwVals;
  int currOrder = basis->basisOrder;

  // Add multiscaling
  for(int loopA=0;loopA<currOrder;loopA++){
    mwVals.push_back(((uqMWBasis*) basis)->EvalMS(currVal,0,0,loopA));
  }

  // Add multiwavelets
  for(int loopA=0;loopA<currOrder;loopA++){
    mwVals.push_back(((uqMWBasis*) basis)->EvalMotherMW(currVal,loopA));
  }

  // Eval Result
  double result = 0.0;
  for(int loopA=0;loopA<(int)coeff.size();loopA++){
    result += mwVals[loopA] * coeff[loopA];
  }
  return result;
}

// Export Single-element Approximant to File
void uq1DApproximant_SE::exportToTextFile(string fileName,bool append){
  FILE* outFile;
  if(append){
    outFile = fopen(fileName.c_str(),"a");
  }else{
    outFile = fopen(fileName.c_str(),"w");
  }
  
  // Save Approx Types: atPoly,atOrthoPoly,atMW
  fprintf(outFile,"%d\n",(int)approxType);

  // Save Base Polynomial Types
  fprintf(outFile,"%d\n",basis->getPolynomialType());

  // Save Base Max Order
  fprintf(outFile,"%d\n",basis->basisOrder);


  // Save Base Quadrature Order
  fprintf(outFile,"%d\n",basis->quadratureOrder);

  // Save the Probability Measure of the Basis
  for(int loopA=0;loopA<basis->probabilityMeasure.size();loopA++){
    fprintf(outFile,"%e ",basis->probabilityMeasure[loopA]);
  }
  fprintf(outFile,"\n");

  // Save the Expansion Coefficients
  for(int loopA=0;loopA<coeff.size();loopA++){
    fprintf(outFile,"%e ",coeff[loopA]);
  }
  fprintf(outFile,"\n");

  // Save the Limits of this Basis
  fprintf(outFile,"%e ",limits[0]);
  fprintf(outFile,"%e\n",limits[1]);
  
  // Close State File
  fclose(outFile);
}

// Import Multi-element Approximant from file
int uq1DApproximant_SE::importFromTextFile(string fileName,bool startFromTop,int startLine){
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  int order = 0;
  int quadOrder = 0;
  stdStringVec stringVector;
  stdVec probMeasure;
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()){
    // Got to the Requested Line Number
    if(!startFromTop){
      for(int loopA=0;loopA<startLine;loopA++){
        std::getline(myReadFile,buffer);
      }    
    }
    for(int loop0=0;loop0<7;loop0++){
      lineCount++;
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      boost::algorithm::trim(buffer);
      if(!buffer.empty()){
        switch(lineCount) {
          case 1 : // Save Approx Types: atPoly,atOrthoPoly,atMW
                   approxType = approxTypes(atoi(buffer.c_str()));
                   break;

          case 2 : // Polynomial Type
                   polyType = atoi(buffer.c_str());
                   break;

          case 3 : // Save Base Max Order
                   order = atoi(buffer.c_str());
                   break;

          case 4 : // Save Base Quadrature Order
                   quadOrder = atoi(buffer.c_str());
                   break;

          case 5 : // Save the Probability Measure of the Basis
                   boost::split(stringVector,buffer,boost::is_any_of(" "));
                   probMeasure.clear();
                   for(int loopA=0;loopA<stringVector.size();loopA++){
                     probMeasure.push_back(atof(stringVector[loopA].c_str()));
                   }
                   break;

          case 6 : // Save the Expansion Coefficients
                   boost::split(stringVector,buffer,boost::is_any_of(" "));
                   this->coeff.clear();
                   for(int loopA=0;loopA<stringVector.size();loopA++){
                     this->coeff.push_back(atof(stringVector[loopA].c_str()));
                   }        
                   break;

          case 7 : // Save the Limits of this Basis
                   boost::split(stringVector,buffer,boost::is_any_of(" "));
                   this->limits.clear();
                   this->limits.push_back(atof(stringVector[0].c_str()));
                   this->limits.push_back(atof(stringVector[1].c_str()));
                   break;
        }
      }
    }
  }else{
    throw uqException("ERROR: Cannot Open Parameter File.\n");
    myReadFile.close();
    return 1;
  }
  myReadFile.close();

  // Delete Basis if needed
  if(basis != NULL){
    delete basis;
  }

  // Allocate New Basis
  if(approxType == atPoly){
    basis = new uqPolyBasis(polyType,order);
  }else if(approxType == atOrthoPoly){
    basis = new uqOrthoPolyBasis(order,quadOrder,probMeasure);
  }else if(approxType == atMW){
    basis = new uqMWBasis(order,quadOrder,probMeasure);
  }

  // Return No Error
  return 0;
}

// Normalize by dividing the coefficients for a common constant
void uq1DApproximant_SE::normalizeByConstant(double normValue){
  for(int loopA=0;loopA<coeff.size();loopA++){
    coeff[loopA] /= normValue;
  }
}

// Get Extremes of the approximant
void uq1DApproximant_SE::getLimits(stdVec& res){
  res.clear();
  res.push_back(limits[0]);
  res.push_back(limits[1]);
}
