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

bool checkNormalized(const stdVec& pmf){
  double sum = 0.0;
  for(size_t loopA=0;loopA<pmf.size();loopA++){
    sum += pmf[loopA];
  }
  if(fabs(sum - 1.0) > 1.0e-8){
    return false;
  }else{
    return true;
  }
}

uqPDF::uqPDF(int seed){
  // Init random number generator with seed if available
  this->seed = seed;
  if(seed != mt19937_64::default_seed){
    mt.seed(seed);
  }
}

uqPDF::~uqPDF(){
}

stdVec uqPDF::evaluate(double par1, double par2, const stdVec& XVals){
  stdVec res;
  for(size_t loopA=0;loopA<XVals.size();loopA++){
    res.push_back(evaluate(par1,par2,XVals[loopA]));
  }
  return res;
}

stdVec uqPDF::sample(double par1, double par2, int num_samples){
  stdVec res;
  for(int loopA=0;loopA<num_samples;loopA++){
    res.push_back(sample(par1,par2));
  }
  return res;
}

unsigned long long int uqPDF::getSeed(){
  return this->seed;
}

// ============
// GAUSSIAN PDF
// ============

// Constructor
uqGaussianPDF::uqGaussianPDF(int seed):uqPDF(seed){
  dist = new normal_distribution<double>(0.0,1.0);
}

uqGaussianPDF::~uqGaussianPDF(){
}
    
// Eval Gaussian PDF
double uqGaussianPDF::evaluate(double par1, double par2, double XValue){
  return (1.0/(par2*sqrt(2.0*M_PI)))*exp(-(XValue-par1)*(XValue-par1)/(2.0*par2*par2));
}

// Sample from Gaussian PDF
double uqGaussianPDF::sample(double par1, double par2){
  return par1 + dist->operator()(mt)*par2;
}

// ======================
// CONTINUOUS UNIFORM PDF
// ======================

// Constructor
uqUniformPDF::uqUniformPDF(int seed):uqPDF(seed){
  dist = new uniform_real_distribution<double>(0.0,1.0);
}

uqUniformPDF::~uqUniformPDF(){
}
    
double uqUniformPDF::evaluate(double par1, double par2, double XValue){
  // Check Result
  if(XValue < par1){
    return 0.0;
  }else if(XValue > par2){
    return 0.0;
  }else{
    return (1.0/(par2-par1));
  }
}

double uqUniformPDF::sample(double par1, double par2){
  return par1 + (par2-par1)*dist->operator()(mt);
}

// ======================
// TRUNCATED GAUSSIAN PDF
// ======================

// Eval Truncated Gaussian PDF
uqTruncatedGaussianPDF::uqTruncatedGaussianPDF(double trunc, int seed):uqGaussianPDF(seed){
  // Assign Truncation Level
  truncSigma = trunc;
}

uqTruncatedGaussianPDF::~uqTruncatedGaussianPDF(){
}

// Eval Truncated Gaussian PDF
double uqTruncatedGaussianPDF::evaluate(double par1, double par2, double XValue){
  // Check Result
  if(XValue<(par1-truncSigma)){
    return 0.0;
  }else if(XValue>(par1+truncSigma)){
    return 0.0;
  }else{
    return (1.0/(par2*sqrt(2.0*M_PI)))*exp(-(XValue-par1)*(XValue-par1)/(2.0*par2*par2));  
  }
}

double uqTruncatedGaussianPDF::sample(double par1, double par2){
  throw uqException("ERROR: sample function not implemented for uqTruncatedGaussianPDF.");
}

// ===================================
// RESCALED AND TRUNCATED GAUSSIAN PDF
// ===================================

// Rescaled Truncated Gaussian PDF
uqRescaledTruncatedGaussianPDF::uqRescaledTruncatedGaussianPDF(double trunc, int seed):uqTruncatedGaussianPDF(trunc){
  truncFactor = trunc;
}

uqRescaledTruncatedGaussianPDF::~uqRescaledTruncatedGaussianPDF(){
}

// Eval Rescaled Truncated Gaussian PDF
double uqRescaledTruncatedGaussianPDF::evaluate(double par1, double par2, double XValue){
  double newValue = -truncFactor + (2.0*truncFactor)*XValue;
  return (1.0/(par2*sqrt(2.0*M_PI)))*exp(-(newValue-par1)*(newValue-par1)/(2.0*par2*par2));
}

double uqRescaledTruncatedGaussianPDF::sample(double par1, double par2){
  throw uqException("ERROR: sample function not implemented for uqRescaledTruncatedGaussianPDF.");
}

// ===============
// Categorical PMF
// ===============

uqCategoricalPMF::uqCategoricalPMF(int seed){
  uSampler = new uqUniformPDF(seed);
}

uqCategoricalPMF::~uqCategoricalPMF(){
  delete uSampler;
}

// Here xVal is zero-based
double uqCategoricalPMF::evaluate(int xVal, const stdVec& pmf){
  if(!checkNormalized(pmf)){
    throw uqException("ERROR: pmf not normalized.");
  }
  if((xVal >= 0)&&(xVal < pmf.size())){
    return pmf[xVal];
  }else{
    throw uqException("ERROR: invalid x value in uqCategoricalPMF::evaluate.");
  }
}
int uqCategoricalPMF::sample(const stdVec& pmf){
  if(!checkNormalized(pmf)){
    throw uqException("ERROR: pmf not normalized.");
  }    
  // Generate Random uniform number
  double uVal = uSampler->sample(0.0,1.0);
  if(uVal < pmf[0]){
    return 0;
  }
  int i = 1;
  double cumPMF = pmf[0];
  bool found = false;
  while((!found)&&(i < pmf.size())){
    found = (uVal > cumPMF)&&(uVal <= cumPMF + pmf[i]);
    cumPMF += pmf[i];
    if(!found){
      i++;  
    }
  }
  return i;
}

unsigned long long int uqCategoricalPMF::getSeed(){
  return uSampler->getSeed();
}
