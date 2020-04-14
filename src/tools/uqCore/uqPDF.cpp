# include "uqPDF.h"

// TO DO: Available distributions
// uniform_int_distribution
// uniform_real_distribution
// bernoulli_distribution
// binomial_distribution
// negative_binomial_distribution
// geometric_distribution
// poisson_distribution
// exponential_distribution
// gamma_distribution
// weibull_distribution
// extreme_value_distribution
// normal_distribution
// lognormal_distribution
// chi_squared_distribution
// cauchy_distribution
// fisher_f_distribution
// student_t_distribution
// discrete_distribution
// piecewise_constant_distribution  
// piecewise_linear_distribution

using namespace std;

uqPDF::uqPDF(int seed){
  // Init random number generator with seed if available
  if(seed != 0){
    mt.seed(seed);
  }
}

uqPDF::~uqPDF(){
}

stdVec uqPDF::evaluate(const stdVec& XVals){
  stdVec res;
  for(size_t loopA=0;loopA<XVals.size();loopA++){
    res.push_back(evaluate(XVals[loopA]));
  }
  return res;
}

stdVec uqPDF::sample(int num_samples){
  stdVec res;
  for(int loopA=0;loopA<num_samples;loopA++){
    res.push_back(sample());
  }
  return res;
}

// GAUSSIAN PDF

// Constructor
uqGaussianPDF::uqGaussianPDF(double meanValue, double stdevValue, int seed):uqPDF(seed){
  mean = meanValue;
  stdev = stdevValue;
  dist = new normal_distribution<double>(meanValue,stdevValue);
}

uqGaussianPDF::~uqGaussianPDF(){
}
    
// Eval Gaussian PDF
double uqGaussianPDF::evaluate(double XValue){
  return (1.0/(stdev*sqrt(2.0*M_PI)))*exp(-(XValue-mean)*(XValue-mean)/(2.0*stdev*stdev));
}

// Sample from Gaussian PDF
double uqGaussianPDF::sample(){
  return dist->operator()(mt);
}

// CONTINUOUS UNIFORM PDF

// Constructor
uqUniformPDF::uqUniformPDF(double a, double b, int seed):uqPDF(seed){
  this->a = a;
  this->b = b;
  dist = new uniform_real_distribution<double>(a,b);
}

uqUniformPDF::~uqUniformPDF(){
}
    
double uqUniformPDF::evaluate(double XValue){
  // Check Result
  if(XValue<a){
    return 0.0;
  }else if(XValue>b){
    return 0.0;
  }else{
    return (1.0/(b-a));
  }
}

double uqUniformPDF::sample(){
  return dist->operator()(mt);
}

// TRUNCATED GAUSSIAN PDF

// Eval Truncated Gaussian PDF
uqTruncatedGaussianPDF::uqTruncatedGaussianPDF(double mean, double stdev, double trunc):uqGaussianPDF(mean,stdev){
  // Assign Truncation Level
  truncSigma = trunc;
}

uqTruncatedGaussianPDF::~uqTruncatedGaussianPDF(){
}

// Eval Truncated Gaussian PDF
double uqTruncatedGaussianPDF::evaluate(double XValue){
  // Check Result
  if(XValue<(mean-truncSigma)){
    return 0.0;
  }else if(XValue>(mean+truncSigma)){
    return 0.0;
  }else{
    return (1.0/(stdev*sqrt(2.0*M_PI)))*exp(-(XValue-mean)*(XValue-mean)/(2.0*stdev*stdev));  
  }
}

double uqTruncatedGaussianPDF::sample(){
  throw uqException("ERROR: sample function not implemented for uqTruncatedGaussianPDF.");
}

// RESCALED AND TRUNCATED GAUSSIAN PDF

// Rescaled Truncated Gaussian PDF
uqRescaledTruncatedGaussianPDF::uqRescaledTruncatedGaussianPDF(double trunc):uqTruncatedGaussianPDF(0.0,1.0,trunc){
  truncFactor = trunc;
}

uqRescaledTruncatedGaussianPDF::~uqRescaledTruncatedGaussianPDF(){
}

// Eval Rescaled Truncated Gaussian PDF
double uqRescaledTruncatedGaussianPDF::evaluate(double XValue){
  double newValue = -truncFactor + (2.0*truncFactor)*XValue;
  return (1.0/(stdev*sqrt(2.0*M_PI)))*exp(-(newValue-mean)*(newValue-mean)/(2.0*stdev*stdev));
}

double uqRescaledTruncatedGaussianPDF::sample(){
  throw uqException("ERROR: sample function not implemented for uqRescaledTruncatedGaussianPDF.");
}
