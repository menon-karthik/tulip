# include "uqApproximant_SE.h"

using namespace std;
using namespace boost::algorithm;

uqApproximant_SE::uqApproximant_SE(){
}

uqApproximant_SE::uqApproximant_SE(approxTypes approxType,
	                                 int polyType,
	                                 int order,
	                                 const stdMat& coeffs,
	                                 const stdVec& limits){
  this->approxType = approxType;
  // Type of Polynomials 
  this->polyType = polyType;
  // Underlying Basis
  this->order = order;
  // Coefficient Representation
  this->coeffs = coeffs;
  // Limits for translation/scaling
  this->limits = limits;
}

uqApproximant_SE::~uqApproximant_SE(){

}
   
stdMat uqApproximant_SE::evaluate(const stdMat& XVals){

  // Create Sample
  uqSamples* samples = new uqSamples(XVals);
  
  // Rescale to hypercube
  samples->rescaleOnHypercube(limits);

  // Allocate the parameters for the uqMatrix
  uqMatrix* mat;
  bool addLegendrePoly = true;
  bool addMW = true;
  bool useBinaryPartitions = false;
  int  mwMatType = kMWFixedColumns;
  bool mwMatIncludeNullColumns = false;
  bool useExactMW = false;
  int mwQuadOrder = 10;
  stdMat measure;
  // Specify the number of columns or depth!!!
  int maxColumns = 0;
  int maxDetailLevel = 0;

  switch(approxType){
    case atPoly:
      mat = new uqPolyMatrix(samples,
      	                     order,
      	                     polyType, 
      	                     kMIPartialOrder);
      break;
    case atMW:
      // Form measurement Matrix
      mat = new uqMWMatrix(order, 
  	                       samples,
                           addLegendrePoly,
                           addMW,
                           useBinaryPartitions,
                           mwMatType,
                           mwMatIncludeNullColumns,
                           useExactMW,
                           mwQuadOrder,
                           measure,
                           maxColumns, 
                           maxDetailLevel);
      break;
    default:
      throw uqException("ERROR: Invalid Approximant type.");
  }

  // Init result
  stdMat res;
  stdVec temp;
  for(int loopA=0;loopA<coeffs.size();loopA++){
    temp = mat->applyTo(coeffs[loopA]);
    res.push_back(temp);
  }

  // Delete unused
  delete samples;
  delete mat;

  // Return set of sample
  return res;
}

int uqApproximant_SE::importFromFile(string fileName, bool startFromTop, int startLine){
  // Open File
  ifstream myReadFile;
  string buffer;
  int numOutputs = 0;
  int numCoeff = 0;
  stdStringVec stringVector;
  stdVec tmpVec;

  // Open file and get to the point
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()){
    if(!startFromTop){
      for(int loopA=0;loopA<startLine;loopA++){
        getline(myReadFile,buffer);
      }    
    }

    // Check you started from a new model
    getline(myReadFile,buffer);
    trim(buffer);
    if(buffer != "START"){
      throw uqException("ERROR: Mismatch in reading approximant file.");
    }
    
    // Read type of approximantion
    getline(myReadFile,buffer);
    trim(buffer);
    approxType = approxTypes(atoi(buffer.c_str()));

    // Read type of polynomial
    getline(myReadFile,buffer);
    trim(buffer);
    polyType = atoi(buffer.c_str());

    // Read polynomial order
    getline(myReadFile,buffer);
    trim(buffer);
    order = atoi(buffer.c_str());

    // Read size of expansion coefficient matrix
    getline(myReadFile,buffer);
    trim(buffer);    
    split(stringVector,buffer,boost::is_any_of(" "));
    numOutputs = atoi(stringVector[0].c_str());
    numCoeff = atoi(stringVector[1].c_str());

    // Read expansion coefficients for all outputs
    this->coeffs.clear();
    for(int loopA=0;loopA<numOutputs;loopA++){
     
      getline(myReadFile,buffer);
      trim(buffer);    
      split(stringVector,buffer,boost::is_any_of(" "));
      
      tmpVec.clear();
      for(int loopB=0;loopB<numCoeff;loopB++){
        tmpVec.push_back(atof(stringVector[loopB].c_str()));        
      }
      this->coeffs.push_back(tmpVec);
    }

    // Read polynomial order
    getline(myReadFile,buffer);
    trim(buffer);
    boost::split(stringVector,buffer,boost::is_any_of(" "));
    limits.clear();
    for(int loopB=0;loopB<stringVector.size();loopB++){
        limits.push_back(atof(stringVector[loopB].c_str()));        
    }

  }else{
    throw uqException("ERROR: Cannot Open Parameter File.\n");
    myReadFile.close();
    return 1;
  }
  myReadFile.close();
  return 0;
}

void uqApproximant_SE::exportToFile(string fileName, bool append){
  FILE* outFile;
  if(append){
    outFile = fopen(fileName.c_str(),"a");
  }else{
    outFile = fopen(fileName.c_str(),"w");
  }

  // Save Approx Types: atPoly,atOrthoPoly,atMW
  fprintf(outFile,"START\n");

  // Save Approx Types: atPoly,atOrthoPoly,atMW
  fprintf(outFile,"%d\n",(int)approxType);

  // Save Base Polynomial Types
  fprintf(outFile,"%d\n",polyType);

  // Save Base Polynomial Types
  fprintf(outFile,"%d\n",order);

  // Save the Expansion Coefficients Matrix
  fprintf(outFile,"%ld %ld\n",coeffs.size(),coeffs[0].size());
  for(int loopA=0;loopA<coeffs.size();loopA++){
    for(int loopB=0;loopB<coeffs[loopA].size();loopB++){
      fprintf(outFile,"%e ",coeffs[loopA][loopB]);
    }
    fprintf(outFile,"\n");  
  }

  // Save the Limits of the single-element approximant
  for(int loopA=0;loopA<limits.size();loopA++){
    fprintf(outFile,"%e ",limits[loopA]);
  }
  fprintf(outFile,"\n");  
  
  // Close State File
  fclose(outFile);
}
 
stdVec uqApproximant_SE::getLimits(){
  return limits;
}