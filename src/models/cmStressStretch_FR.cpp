# include "cmStressStretch_FR.h"

using namespace std;

cmStressStretch_FR::cmStressStretch_FR(const stdVec& lambdaZ){
  this->lambdaZ = lambdaZ;
}
cmStressStretch_FR::~cmStressStretch_FR(){
  lambdaZ.clear();
}
int cmStressStretch_FR::getParameterTotal(){
  return 3;
}
int cmStressStretch_FR::getStateTotal(){
  return 0;
}
int cmStressStretch_FR::getResultTotal(){
  return lambdaZ.size();
}
void cmStressStretch_FR::getParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 0.0;  limits[1] = 1.4e+10;
  limits[2] = 0.0;  limits[3] = 1.4e+10;
  limits[4] = 10.0; limits[5] = 1000.0;
}
void cmStressStretch_FR::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 1.0;
  params[1] = 1.0;
  params[2] = 100.0;
}
void cmStressStretch_FR::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented in cmStressStretch_FR.\n");
}
string cmStressStretch_FR::getParamName(int parID){
  if(parID == 0){
    return string("Ec");
  }else if(parID == 1){
    return string("Ee");
  }else if(parID == 2){
    return string("b");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}

string cmStressStretch_FR::getResultName(int resID){
  string res(string("Stretch") + to_string(resID+1));
  return res;
}

double cmStressStretch_FR::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
// ==========================================================================================
//  NOTE: the parameters are collected in x_GOH vector in the following order
//    x_FR(1) = Ec    (>Ee) is the elastic modulus of the linear region
//    x_FR(2) = Ee    (> 0) is the elastic modulus of the toe region
//    x_FR(3) = b     (> 1) is the true strain at which point, collagen fascicles 
//                            have stretch straight (i.e. transition strain/critical strain)
// ==========================================================================================

  // Set Input Quantities
  double Ec = inputs[0];
  double Ee = inputs[1];
  double b  = inputs[2];

  // Get Stresses from Data Object
  stdMat stressVals;
  stdMat strainVals;
  if(data != NULL){
    data->getTable(stressVals);
  }else{
    throw cmException("ERROR: Data object needs to be defined for cmStressStretch_FR.\n"); 
  }

  /*
  printf("Stress Values\n");
  for(int loopA=0;loopA<stressVals.size();loopA++){    
    for(int loopB=0;loopB<stressVals[loopA].size();loopB++){
      printf("%5.3f ",stressVals[loopA][loopB]);
    }
    printf("\n");
  }
  */


  // Compute the strains for all stress values
  stdVec tmp;
  double currStretch = 0.0;
  for(int loopA=0;loopA<stressVals.size();loopA++){    
    tmp.clear();
    for(int loopB=0;loopB<stressVals[loopA].size();loopB++){
      currStretch = exp(stressVals[loopA][loopB]/Ec + (1.0/b)*(1.0 - pow(1.0 + (b - 1.0)*(stressVals[loopA][loopB]/Ee),-b/(b-1.0))));
      tmp.push_back(currStretch);
    }
    strainVals.push_back(tmp);
  }  

  /*
  printf("Stretch Values\n");
  for(int loopA=0;loopA<strainVals.size();loopA++){    
    for(int loopB=0;loopB<strainVals[loopA].size();loopB++){
      printf("%5.3f ",strainVals[loopA][loopB]);
    }
    printf("\n");
  }
  */
  
  // KEYS
  stdStringVec keys;
  keys.clear();
  keys.push_back(string("stretch01"));
  keys.push_back(string("stretch02"));
  keys.push_back(string("stretch03"));
  keys.push_back(string("stretch04"));
  keys.push_back(string("stretch05"));
  keys.push_back(string("stretch06"));
  keys.push_back(string("stretch07"));
  keys.push_back(string("stretch08"));
  keys.push_back(string("stretch09"));
  keys.push_back(string("stretch10"));
  keys.push_back(string("stretch11"));
  
  // Set standard deviations
  stdVec stdFactors;
  stdFactors.clear();
  stdFactors.push_back(0.01); // str01
  stdFactors.push_back(0.01); // str02
  stdFactors.push_back(0.01); // str03
  stdFactors.push_back(0.01); // str04
  stdFactors.push_back(0.01); // str05
  stdFactors.push_back(0.01); // str06
  stdFactors.push_back(0.01); // str07
  stdFactors.push_back(0.01); // str08
  stdFactors.push_back(0.01); // str09
  stdFactors.push_back(0.01); // str10
  stdFactors.push_back(0.01); // str11

  // Weights
  stdVec weigths;
  for(int loopA=0;loopA<lambdaZ.size();loopA++){
    weigths.push_back(1.0);
  }

  // Compare columns of strains with experimental values
  FILE* fp;
  fp = fopen("outputTargets.out","w");
  double computed = 0.0;
  string measuredString;
  double measured = 0.0;
  double weight = 0.0;
  int dataIndex = 0;
  for(int loopA=0;loopA<strainVals[0].size();loopA++){
    fprintf(fp,"%15s %15s %15s\n", "Key", "Measured", "Computed");
    for(int loopB=0;loopB<strainVals.size();loopB++){
      fprintf(fp,"%15s %15.5f %15.5f\n",keys[loopB].c_str(), lambdaZ[loopB], strainVals[loopB][loopA]);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);

  // Eval negative log likelihood
  double result = 0.0;
  double perc = 0.0;
  for(int loopA=0;loopA<strainVals.size();loopA++){
    for(int loopB=0;loopB<strainVals[loopA].size();loopB++){
      perc = 0.5*(strainVals[loopA][loopB] - lambdaZ[loopA])*(strainVals[loopA][loopB] - lambdaZ[loopA])/((stdFactors[loopA] * stdFactors[loopA]) * weigths[loopA]);
      result += perc;
    }
  }

  // Add the Stretch outputs
  outputs.clear();
  for(int loopA=0;loopA<strainVals.size();loopA++){
    outputs.push_back(strainVals[loopA][0]);
  }
  // Add the stress references
  for(int loopA=0;loopA<stressVals.size();loopA++){
    outputs.push_back(stressVals[loopA][0]);
  }

  // Set error codes
  errorCode.clear();
  errorCode.push_back(0);

  // Return 
  return result;  
}





