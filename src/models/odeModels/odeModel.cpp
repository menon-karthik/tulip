# include "odeModel.h"

using namespace std;

odeModel::odeModel(){
}

odeModel::~odeModel(){
}

void odeModel::getParameterLimits(stdVec& limits){
  // Get Default Parameters
  getDefaultParameterLimits(limits);
  // Change the limits bases on the Fixed Parameter List
  int currParam = 0;
  for(size_t loopA=0;loopA<frozenParamIDX.size();loopA++){
    currParam = frozenParamIDX[loopA];
    // Assign the new lower and upper bounds to the center
    limits[currParam*2]     = frozenParamVAL[loopA];
    limits[currParam*2 + 1] = frozenParamVAL[loopA];
  }	
}
