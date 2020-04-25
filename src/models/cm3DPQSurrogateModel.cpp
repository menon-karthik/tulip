
# include "cm3DPQSurrogateModel.h"

// CONSTRUCTOR
cm3DPQSurrogateModel::cm3DPQSurrogateModel(int polyType, int polyOrder, bool normalizeData,
                                           stdMat limits, stdMat locExpCoeffs){
  // COPY LOCAL VARIABLES
  this->polyType = polyType;
  this->polyOrder = polyOrder;
  this->normalizeData = normalizeData;
  totDims = locExpCoeffs[0].size();

  // COPY LIMITS
  this->limits.clear();
  stdVec temp;
  for(int loopA=0;loopA<totDims;loopA++){
    temp.clear();    
    temp.push_back(limits[loopA][0]);
    temp.push_back(limits[loopA][1]);    
    this->limits.push_back(temp);
  }

  // COPY EXPANSION COEFFICIENTS
  int totOutlets = totDims;
  int totBasis = locExpCoeffs.size();
  expCoeffs.clear();
  for(int loopA=0;loopA<totBasis;loopA++){
  	temp.clear();
  	for(int loopB=0;loopB<totOutlets;loopB++){
      temp.push_back(locExpCoeffs[loopA][loopB]);
  	}
  	expCoeffs.push_back(temp);
  }
}

// FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
int cm3DPQSurrogateModel::getParameterTotal(){
  return totDims;
}

string cm3DPQSurrogateModel::getParamName(int parID){
  if(parID<totDims){
    return string("Outlet_" + to_string(parID));
  }else{
  	throw cmException("ERROR: Invalid Parameter ID in getParamName.\n");
  }
}

int cm3DPQSurrogateModel::getStateTotal(){
  return 0;
}

int cm3DPQSurrogateModel::getResultTotal(){
  return totDims;
}

void cm3DPQSurrogateModel::getDefaultParameterLimits(stdVec& limits){
  // Resize Limit Array
  limits.resize(2*totDims);
  // Fill Limits
  for(int loopA=0;loopA<totDims;loopA++){
    limits[loopA*2 + 0] = 0.0;
    limits[loopA*2 + 1] = 1.0;
  }
}

void cm3DPQSurrogateModel::getDefaultParams(stdVec& params){
  params.resize(totDims);
  for(int loopA=0;loopA<totDims;loopA++){
    params[loopA] = 0.0;
  }
}

void cm3DPQSurrogateModel::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping not implemented.\n");	
}

string cm3DPQSurrogateModel::getResultName(int resID){
 throw cmException("ERROR: getResultName not implemented.\n");   
}

// EVAL MODEL 
double cm3DPQSurrogateModel::evalModelError(const stdVec& inputs,stdVec& outputs, stdIntVec& errorCodes){

  // Normalize to [-1,1]
  stdVec normInputs;
  for(int loopA=0;loopA<totDims;loopA++){
    if(normalizeData){
      normInputs.push_back(2.0*((inputs[loopA]-limits[loopA][0])/(limits[loopA][1]-limits[loopA][0]))-1.0);
    }else{
      normInputs.push_back(inputs[loopA]);        
    } 
  }

  // Create One Sample with input Data
  uqSamples* sample = new uqSamples(normInputs);
  
  // Build a Polynomial Matrix 
  uqPolyMatrix uqPolyMat(sample,polyOrder,polyType,kMIPartialOrder);
  int num_basis = uqPolyMat.getColCount();
    
  // Apply the Polynomial Matrix
  outputs.clear();
  stdVec temp;
  stdVec res;
  for(int loopA=0;loopA<totDims;loopA++){
    // Push Current Coefficient
    temp.clear();
    for(int loopB=0;loopB<expCoeffs.size();loopB++){
      temp.push_back(expCoeffs[loopB][loopA]);      
    }
    res = uqPolyMat.applyTo(temp);
    outputs.push_back(res[0]);
  }

  // free
  delete sample;

  // Return
  errorCodes.push_back(0);
  return 0.0;
}
