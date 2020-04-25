# include "uqApproximant_SE.h"


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

int uqApproximant_SE::importFromTextFile(string fileName, bool startFromTop,int startLine){
}

void uqApproximant_SE::exportToTextFile(string fileName, bool append){
  FILE* outFile;
  if(append){
    outFile = fopen(fileName.c_str(),"a");
  }else{
    outFile = fopen(fileName.c_str(),"w");
  }

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
  fprintf(outFile,"%ld\n",limits.size());
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