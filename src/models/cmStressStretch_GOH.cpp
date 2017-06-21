# include "cmStressStretch_GOH.h"

using namespace std;

cmStressStretch_GOH::cmStressStretch_GOH(const stdVec& lambda){
  lambdaZ = lambda;
}
cmStressStretch_GOH::~cmStressStretch_GOH(){
  lambdaZ.clear();
}
int cmStressStretch_GOH::getParameterTotal(){
  return 5;
}
int cmStressStretch_GOH::getStateTotal(){
  return 0;
}
int cmStressStretch_GOH::getResultTotal(){
  return lambdaZ.size();
}
void cmStressStretch_GOH::getParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 0.0; limits[1] = 1.0e8;
  limits[2] = 0.0; limits[3] = 1.0e8;
  limits[4] = 0.0; limits[5] = 1.0e3;
  limits[6] = 0.0; limits[7] = 1.0/3.0;
  limits[8] = 0.0; limits[9] = M_PI/2.0;
}
void cmStressStretch_GOH::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 0.50;
  params[1] = 1.0;
  params[2] = 350.0;
  params[3] = 0.15;
  params[4] = 0.2;
}
void cmStressStretch_GOH::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented in cmStressStretch_GOH.\n");
}
string cmStressStretch_GOH::getParamName(int parID){
  if(parID == 0){
    return string("ce");
  }else if(parID == 1){
    return string("c1");
  }else if(parID == 2){
    return string("c2");
  }else if(parID == 3){
    return string("K");
  }else if(parID == 4){
    return string("alpha");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}
string cmStressStretch_GOH::getResultName(int resID){
  string res(string("Stress") + to_string(resID+1));
  return res;
}

void cmStressStretch_GOH::setModelResults(const stdVec& outputs,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths){

  // KEYS
  keys.clear();
  keys.push_back(string("str01"));
  keys.push_back(string("str02"));
  keys.push_back(string("str03"));
  keys.push_back(string("str04"));
  keys.push_back(string("str05"));
  keys.push_back(string("str06"));
  keys.push_back(string("str07"));
  keys.push_back(string("str08"));
  keys.push_back(string("str09"));
  keys.push_back(string("str10"));
  keys.push_back(string("str11"));

  // COMPUTED VALUES
  computedValues.clear();
  for(int loopA=0;loopA<lambdaZ.size();loopA++){
    computedValues.push_back(outputs[loopA]);
  }
  
  // STANDARD DEVIATIONS
  stdFactors.clear();
  //stdFactors.push_back(5.981552e+03); // str01
  //stdFactors.push_back(6.796677e+03); // str02
  //stdFactors.push_back(1.183938e+04); // str03
  //stdFactors.push_back(3.592660e+04); // str04
  //stdFactors.push_back(9.279702e+04); // str05
  //stdFactors.push_back(2.004188e+05); // str06
  //stdFactors.push_back(3.637891e+05); // str07
  //stdFactors.push_back(5.768587e+05); // str08
  //stdFactors.push_back(8.251717e+05); // str09
  //stdFactors.push_back(1.088304e+06); // str10
  //stdFactors.push_back(1.348686e+06); // str11
  stdFactors.push_back(1.0); // str01
  stdFactors.push_back(1.0); // str02
  stdFactors.push_back(1.0); // str03
  stdFactors.push_back(1.0); // str04
  stdFactors.push_back(1.0); // str05
  stdFactors.push_back(1.0); // str06
  stdFactors.push_back(1.0); // str07
  stdFactors.push_back(1.0); // str08
  stdFactors.push_back(1.0); // str09
  stdFactors.push_back(1.0); // str10
  stdFactors.push_back(1.0); // str11

  // WEIGHTS
  weigths.clear();
  for(int loopA=0;loopA<lambdaZ.size();loopA++){
    weigths.push_back(1.0);
  }
}

double cmStressStretch_GOH::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
// ===========================================================================
//  NOTE: the parameters are collected in x_GOH vector in the following order
//    x_GOH(1) = ce       Neo-Hookean (Ground Substance) stress-like parameter
//    x_GOH(2) = c1       Stress-like collagen parameter
//    x_GOH(3) = c2       Non-dimensional exponential collagen parameter
//    x_GOH(4) = K        Collagen fiber dispersion parameter
//    x_GOH(5) = alpha    Fiber family deviation angle from tendon long axis 
// ===========================================================================

  // Local copy of the parameters
  double ce    = inputs[0];
  double c1    = inputs[1];
  double c2    = inputs[2];
  double K     = inputs[3];
  double alpha = inputs[4];

  // Init quantities derived from lambdaZ
  vector<double> Frr(lambdaZ);
  vector<double> Fqq(lambdaZ);
  vector<double> Fzz(lambdaZ);
  vector<double> Crr(lambdaZ);
  vector<double> Cqq(lambdaZ);
  vector<double> Czz(lambdaZ);

  // Set the quantities derived from lambdaZ
  for(int loopA=0;loopA<lambdaZ.size();loopA++){
    Frr[loopA] = 1.0/(sqrt(lambdaZ[loopA]));
    Fqq[loopA] = 1.0/(sqrt(lambdaZ[loopA]));
    Fzz[loopA] = lambdaZ[loopA];
    Crr[loopA] = Frr[loopA]*Frr[loopA];
    Cqq[loopA] = Fqq[loopA]*Fqq[loopA];
    Czz[loopA] = Fzz[loopA]*Fzz[loopA];
  }  

  // Init Vectors
  vector<double> I1(lambdaZ.size(),0.0);
  vector<double> I4(lambdaZ.size(),0.0);
  vector<double> v(lambdaZ.size(),0.0);
  vector<double> w(lambdaZ.size(),0.0);
  vector<double> chi(lambdaZ.size(),0.0);

  // Eval Stresses
  outputs.clear();
  double currStress = 0.0;
  for(int loopA=0;loopA<lambdaZ.size();loopA++){
    I1[loopA]  = Crr[loopA] + Cqq[loopA] + Czz[loopA];
    I4[loopA]  = Cqq[loopA]*sin(alpha)*sin(alpha) + Czz[loopA]*cos(alpha)*cos(alpha);
    v[loopA]   = K*I1[loopA] + (1.0-3.0*K)*I4[loopA] - 1.0;
    w[loopA]   = c2*v[loopA]*v[loopA];
    chi[loopA] = 4.0*c1*v[loopA]*exp(w[loopA]);
    currStress = (ce + chi[loopA]*K)*(Czz[loopA]-Cqq[loopA]) + chi[loopA]*(1.0-3.0*K)*(Czz[loopA]*cos(alpha)*cos(alpha) - 0.5*Cqq[loopA]*sin(alpha)*sin(alpha));
    outputs.push_back(currStress);
  }

  // SET RESULTS FOR VARIOUS VERSIONS
  stdStringVec keys;
  stdVec computedValues;
  stdVec stdFactors;
  stdVec weigths;
  setModelResults(outputs,keys,computedValues,stdFactors,weigths);

  // Print and compare
  double result = 0.0;
  if(data != NULL){

    // Print Info
    //if(options.flushData){
    data->printAndCompare(datasetColumn,keys,computedValues,weigths);
    //}
  
    // Evaluate Objective Function
    result = data->evalLikelihood(datasetColumn,keys,computedValues,stdFactors,weigths);
    //result = data->evalOBJ(datasetColumn,keys,computedValues,weigths);
    
  }

  // Set Error Codes
  errorCode.clear();
  errorCode.push_back(0);

  // Return 
  return result;
}
