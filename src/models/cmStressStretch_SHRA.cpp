# include "cmStressStretch_SHRA.h"

using namespace std;

cmStressStretch_SHRA::cmStressStretch_SHRA(const stdVec& lambda){
  lambdaZ = lambda;
}

cmStressStretch_SHRA::~cmStressStretch_SHRA(){
  lambdaZ.clear();
}

int cmStressStretch_SHRA::getParameterTotal(){
  return 4;
}
int cmStressStretch_SHRA::getStateTotal(){
  return 0;
}
int cmStressStretch_SHRA::getResultTotal(){
  return lambdaZ.size();
}
void cmStressStretch_SHRA::getParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 0.0; limits[1] = 1.0e8;
  limits[2] = 0.0; limits[3] = 1.0e8;
  limits[4] = 0.0; limits[5] = M_PI/2.0;
  limits[6] = 0.0; limits[7] = M_PI/2.0;
}
void cmStressStretch_SHRA::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  params[0] = 1.0;
  params[1] = 10.0;
  params[2] = M_PI/2.0;
  params[3] = M_PI/16.0;
}
void cmStressStretch_SHRA::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping Not implemented in cmStressStretch_SHRA.\n");
}
string cmStressStretch_SHRA::getParamName(int parID){
  if(parID == 0){
    return string("mu");
  }else if(parID == 1){
    return string("E");
  }else if(parID == 2){
    return string("theta0");
  }else if(parID == 3){
    return string("psi");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}
string cmStressStretch_SHRA::getResultName(int resID){
  string res(string("Stress") + to_string(resID+1));
  return res;
}

void cmStressStretch_SHRA::setModelResults(stdVec outputs,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths){

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

  // stdFactors.push_back(5.981552e+03); // str01
  // stdFactors.push_back(6.796677e+03); // str02
  // stdFactors.push_back(1.183938e+04); // str03
  // stdFactors.push_back(3.592660e+04); // str04
  // stdFactors.push_back(9.279702e+04); // str05
  // stdFactors.push_back(2.004188e+05); // str06
  // stdFactors.push_back(3.637891e+05); // str07
  // stdFactors.push_back(5.768587e+05); // str08
  // stdFactors.push_back(8.251717e+05); // str09
  // stdFactors.push_back(1.088304e+06); // str10
  // stdFactors.push_back(1.348686e+06); // str11

  stdFactors.push_back(5.0e5); // str01
  stdFactors.push_back(5.0e5); // str02
  stdFactors.push_back(5.0e5); // str03
  stdFactors.push_back(5.0e5); // str04
  stdFactors.push_back(5.0e5); // str05
  stdFactors.push_back(5.0e5); // str06
  stdFactors.push_back(5.0e5); // str07
  stdFactors.push_back(5.0e5); // str08
  stdFactors.push_back(5.0e5); // str09
  stdFactors.push_back(5.0e5); // str10
  stdFactors.push_back(5.0e5); // str11


  // WEIGHTS
  weigths.clear();
  for(int loopA=0;loopA<lambdaZ.size();loopA++){
    weigths.push_back(1.0);
  }
}

double cmStressStretch_SHRA::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
// =====================================================================================
// NOTE: the parameters are collected in x_SHR_a in the following order for SHR_a SEF
//   inputs[0] = (1-phi)*mu         Ground state shear modulus of ligament/tendon matrix
//   inputs[1] = phi*E              Fibril Young's modulus
//   inputs[2] = theta0             Crimp angle of outermost fibrils
//   inputs[3] = psi                Fascicle helix angle
// =====================================================================================                                                    

  // Write local copy of the parameters
  double mu     = inputs[0];
  double E      = inputs[1];
  double theta0 = inputs[2];
  double psi    = inputs[3];
  // Fibril helix angle
  double alpha  = 0.0;

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

  // Pre-allocation
  vector<double> beta_a(lambdaZ.size(),0.0);
  vector<double> ki_a(lambdaZ.size(),0.0);
  vector<double> I4_a(lambdaZ.size(),0.0);
  vector<double> y_a(lambdaZ.size(),0.0);

  // Right Cauchy-Green Strain Tensor 4th Invariant
  for(int i=0;i<lambdaZ.size();i++){
    I4_a[i] = Cqq[i]*(sin(psi)*sin(psi)) + Czz[i]*(cos(psi)*cos(psi));
    y_a[i]  = sin(alpha)*sin(alpha) + I4_a[i]*cos(alpha)*cos(alpha);
  }

  // (lambda*)^2, Transition stretch^2
  double l2_a = 1.0/(cos(theta0)*cos(theta0));

  // Compute Stress Values
  outputs.clear();
  double currStress = 0.0;
  for(int i=0;i<lambdaZ.size();i++){

    if(I4_a[i] < 1.0){
      // Compression
      ki_a[i] = 0.0;                     
    }else if((I4_a[i] >= 1.0)&&(I4_a[i] <= l2_a)){
      // Toe Region
      ki_a[i] = (E*cos(alpha))/(6.0*sqrt(I4_a[i])*(sin(theta0)*sin(theta0)))*(2.0-(3.0/sqrt(y_a[i]))+pow(y_a[i],-1.5));
    }else if(I4_a[i] > l2_a){
      // Linear Region
      beta_a[i] = 2.0*(1.0-cos(theta0)*cos(theta0)*cos(theta0))/(3.0*sin(theta0)*sin(theta0));
      ki_a[i]   = ((E*cos(alpha))/(2.0*sqrt(I4_a[i])))*(beta_a[i]-(1.0/sqrt(y_a[i])));
    }
    currStress = mu*(Czz[i]-Cqq[i]) + 2.0*ki_a[i]*(Czz[i]*(cos(psi)*cos(psi)) - 0.5*Cqq[i]*sin(psi)*sin(psi));
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
    data->printAndCompare(keys,computedValues,weigths);
    result = data->evalLogLikelihood(keys,computedValues,stdFactors,weigths);
    //result = data->evalOBJ(datasetColumn,keys,computedValues,weigths);
  }

  // Set Error Codes
  errorCode.clear();
  errorCode.push_back(0);

  // Return 
  return result;


}



