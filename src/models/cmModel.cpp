# include "cmModel.h"

using namespace std;

cmModel::cmModel(){
}

cmModel::~cmModel(){
}

void cmModel::setData(daData* locData,int setIDX){
  data = locData;
  datasetColumn = setIDX;
}

stdVec cmModel::evalModelError(stdMat inputs,stdMat& outputs, stdIntVec &errorCode){
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

void cmModel::freezeModelParameters(stdIntVec localParamIDX,stdVec localParamVal){
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



