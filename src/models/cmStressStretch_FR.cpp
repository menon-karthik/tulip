# include "cmStressStretch_FR.h"

using namespace std;

cmStressStretch_FR::cmStressStretch_FR(const stdVec& sigma){
  sigmaZ = sigma;
}
cmStressStretch_FR::~cmStressStretch_FR(){
  sigmaZ.clear();
}
int cmStressStretch_FR::getParameterTotal(){
  return 3;
}
int cmStressStretch_FR::getStateTotal(){
  return 0;
}
int cmStressStretch_FR::getResultTotal(){
  return sigmaZ.size();
}
void cmStressStretch_FR::getParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 0.0; limits[1] = 1.4e+10;
  limits[2] = 0.0; limits[3] = 1.4e+10;
  limits[4] = 10.0; limits[5] = 1000.0;
}
void cmStressStretch_FR::getDefaultParams(stdVec& params){

}
void cmStressStretch_FR::getPriorMapping(int priorModelType,int* prPtr){

}
string cmStressStretch_FR::getParamName(int parID){

}
string cmStressStretch_FR::getResultName(int resID){

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
  double b = inputs[2];

  // Compute the Associated Strains
  outputs.clear();
  double currStrain = 0.0;
  for(int loopA=0;loopA<sigmaZ.size();loopA++){
    currStrain = sigmaZ[loopA]/Ec + (1.0/b)*(1.0 - pow(1.0 + (b - 1.0)*(sigmaZ[loopA]/Ee),-b/(b-1.0)));
    outputs.push_back(currStrain);
  }
  
}





