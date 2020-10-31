# include "cmModel.h"

using namespace std;

cmModel::cmModel(){
  data = NULL;
  printLevel = 0;
}

cmModel::~cmModel(){
}

void cmModel::setData(daData* locData){
  data = locData;
}

stdVec cmModel::evalModelError(const stdMat& inputs,stdMat& outputs,stdIntVec &errorCode){
  // NEED TO IMPLENT SOME KIND OF PARALLEL EXECUTION !!!
  stdVec result;
  stdVec simOutputs;
  stdIntVec simErrorCode;
  double ll = 0.0;
  for(int loopA=0;loopA<inputs.size();loopA++){
    //printf("Solving Input Realization n. %d\n",loopA+1);
    //fflush(stdout);
    ll = evalModelError(inputs[loopA],simOutputs,simErrorCode);
    //printf("Output %f\n",simOutputs[0]);
    outputs.push_back(simOutputs);
    errorCode.push_back(simErrorCode[0]);
    result.push_back(ll);
  }
  return result;
}

void cmModel::freezeModelParameters(const stdIntVec& localParamIDX,const stdVec& localParamVal){
  printf("\n");
  printf("FREEZING MODEL PARAMETERS\n");
  int size = localParamIDX.size();
  for(int loopA=0;loopA<size;loopA++){
  	printf("Parameter %d at value %.5f\n",localParamIDX[loopA],localParamVal[loopA]);
  	frozenParamIDX.push_back(localParamIDX[loopA]);
  	frozenParamVAL.push_back(localParamVal[loopA]);
  }
  printf("\n");
}

void cmModel::getParameterLimits(stdVec& limits){
  if(userLimits.size() == 0){
    // Get Default Parameters
    getDefaultParameterLimits(limits);
  }else{
    // Get User-defined limits
    limits = userLimits;
  }

  // Change the limits bases on the Fixed Parameter List
  int currParam = 0;
  for(size_t loopA=0;loopA<frozenParamIDX.size();loopA++){
    currParam = frozenParamIDX[loopA];
    // Assign the new lower and upper bounds to the center
    limits[currParam*2]     = frozenParamVAL[loopA];
    limits[currParam*2 + 1] = frozenParamVAL[loopA];
  }  
}

void cmModel::setParameterLimits(const stdVec& limits){
  // Check Compatibility
  if(limits.size() != getParameterTotal()*2){
    throw cmException("ERROR: Invalid limits in cmModel::setParameterLimits.\n");
  }else{
    userLimits = limits;
  }
}

