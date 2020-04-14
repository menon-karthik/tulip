# include "cmOscillator.h"

using namespace std;

cmOscillator::cmOscillator(){
  // k    = K;
  // w    = sqrt(k / M);
  // bbeta = Wb / w;
  // wd   = w * sqrt(1 - Xi * Xi);
  // inter = P0 / k / (1 - bbeta * bbeta + (2 * Xi * bbeta) * (2 * Xi * bbeta));
  // A = 2 * Xi * bbeta * inter;
  // B = (2 * Xi * Xi * bbeta * w - (1 - bbeta) * (1 - bbeta) * Wb) * inter / wd;
}

cmOscillator::~cmOscillator(){

}

int cmOscillator::getParameterTotal(){
  return 1;
}
int cmOscillator::getStateTotal(){
  return 0;
}
int cmOscillator::getResultTotal(){
  return 1;
}

void cmOscillator::getDefaultParams(stdVec& params){
  params.resize(1);
  params[0] = 3.0*(220000-140000);
}

void cmOscillator::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = 140000.0;limits[1] = 220000.0;
}

void cmOscillator::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: Not Implemented!.\n");
}

string cmOscillator::getParamName(int parID){
  if(parID == 0){
    return string("K");
  }else{
    throw cmException("ERROR: invalid parameter ID.\n");
  }
}

string cmOscillator::getResultName(int resID){
  if(resID == 0){
    return string("Ec");
  }else{
    throw cmException("ERROR: invalid result ID.\n");
  }
}

// Theoretical function of the oscilation f(t)
double cmOscillator::f(double t, double  w, double  bbeta, double  wd, double  inter, double  A, double  B){ 
  double Xi = 0.05;
  double Wb = 20;
  return (A*cos(wd*t) + B*sin(wd*t))*exp(0-Xi*w*t)+inter*((1-bbeta)*(1-bbeta)*sin(Wb*t)-2*Xi*bbeta*cos(Wb*t));
};

// neg_f = -f
double cmOscillator::neg_f(double t, double  w, double  bbeta, double  wd, double  inter, double  A, double  B){
  return 0.0 - f(t,w,bbeta,wd,inter,A,B);
};

int Stopping_Rule(double x0, double x1, double tolerance){
  double xm = 0.5 * fabs( x1 + x0 );
  if ( xm <= 1.0 ){
    return ( fabs( x1 - x0 ) < tolerance ) ? 1 : 0;
  } 
  return ( fabs( x1 - x0 ) < tolerance * xm ) ? 1 : 0;
};

void cmOscillator::Min_Search_Golden_Section(double k, bool isPos, double& a, double& fa, double& b, double& fb, double tolerance){

  double M = 45;       // kg
  //static constexpr double K           = 181875;   // N/m
  double Xi          = 0.05;     // ratio
  double P0          = 2100;     // N
  double Wb          = 20;       // rad/s
  //static constexpr double W           = sqrt(K / M);
  //static constexpr double Wd          = W * sqrt(1 - Xi * Xi);
  //static constexpr double kmax        = 220000;
  //static constexpr double kmin        = 140000;
  double DBL_EPSILON = 2.2204460492503131e-16;


  double lambda = 0.5 * (sqrt(5.0) - 1.0);
  double mu = 0.5 * (3.0 - sqrt(5.0));
  double w    = sqrt(k / M);
  double bbeta = Wb / w;
  double wd   = w * sqrt(1 - Xi * Xi);
  double inter = P0 / k / (1 - bbeta * bbeta + (2 * Xi * bbeta) * (2 * Xi * bbeta));
  double A = 2 * Xi * bbeta * inter;
  double B = (2 * Xi * Xi * bbeta * w - (1 - bbeta) * (1 - bbeta) * Wb) * inter / wd;

  double x1;
  double x2;
  double fx1;
  double fx2;

  x1 = b - lambda * (b - a);
  x2 = a + lambda * (b - a);
  if(isPos){
    fx1 = f(x1,w,bbeta,wd,inter,A,B);
    fx2 = f(x2,w,bbeta,wd,inter,A,B);
  }else{
    fx1 = neg_f(x1,w,bbeta,wd,inter,A,B);
    fx2 = neg_f(x2,w,bbeta,wd,inter,A,B);
  }
  if (tolerance <= 0.0){
    tolerance = sqrt(DBL_EPSILON) * (b - a);
  }
  while ( ! Stopping_Rule(a,b,tolerance) ) {
    if (fx1 > fx2) {
      a = x1;
      fa = fx1;
      if ( Stopping_Rule(a,b,tolerance) ) break;
      x1 = x2;
      fx1 = fx2;
      x2 = b - mu * (b - a);
      if(isPos){
        fx2 = f(x2,w,bbeta,wd,inter,A,B);
      }else{
        fx2 = neg_f(x2,w,bbeta,wd,inter,A,B);
      }
    } else {
      b = x2;
      fb = fx2;
      if ( Stopping_Rule(a,b,tolerance) ) break;
      x2 = x1;
      fx2 = fx1;
      x1 = a + mu * (b - a);
      if(isPos){
        fx1 = f(x1,w,bbeta,wd,inter,A,B);
      }else{
        fx1 = neg_f(x1,w,bbeta,wd,inter,A,B);
      }
    }
  }
  return;
};

double cmOscillator::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){
  double k = inputs[0];
  double M = 45;       // kg
  double Xi = 0.05;     // ratio
  double P0 = 2100;     // N
  double Wb = 20;       // rad/s
  double w    = sqrt(k / M);
  double bbeta = Wb / w;
  double wd   = w * sqrt(1 - Xi * Xi);
  double inter = P0 / k / (1 - bbeta * bbeta + (2 * Xi * bbeta) * (2 * Xi * bbeta));
  double A = 2 * Xi * bbeta * inter;
  double B = (2 * Xi * Xi * bbeta * w - (1 - bbeta) * (1 - bbeta) * Wb) * inter / wd;
  double a = 0.0;
  double b = 2 * M_PI * max(1 / wd, 1 / Wb);
  double fa = f(a,w,bbeta,wd,inter,A,B);
  double fb1 = f(b,w,bbeta,wd,inter,A,B);
  double tolerance   = 1.0e-7;
  Min_Search_Golden_Section(k, true, a, fa, b, fb1, tolerance);
  a = 0.0;
  b = 2 * M_PI * max(1 / wd, 1 / Wb);
  fa = f(a,w,bbeta,wd,inter,A,B);
  double fb2 = f(b,w,bbeta,wd,inter,A,B);
  Min_Search_Golden_Section(k, false, a, fa, b, fb2, tolerance);
  // Add the results to the output vector
  outputs.clear();
  outputs.push_back(max(0.0 - fb1, 0.0 - fb2));

  // Define keys and weights
  stdStringVec keys;
  keys.push_back(string("dmax"));
  // Set standard deviations
  stdVec stds;
  stds.push_back(0.1*0.007504);
  // Weights
  stdVec weights;
  weights.push_back(1.0);
  
  // Need to evaluate log-likelihood/posterior and return
  double result = 0.0;
  if(data != NULL){
    if(printLevel > 0){
      // Print Info
      data->printAndCompare(keys,outputs,weights); 
    }
    // Evaluate Objective Function
    result = data->evalLogLikelihood(keys,outputs,stds,weights);
  }
  // Return 
  errorCode.clear();
  errorCode.push_back(0);
  return result;
}





