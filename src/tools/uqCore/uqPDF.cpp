#include <math.h>

#include "uqPDF.h"

uqPDF::uqPDF()
{
}

void printToFile(std::string fileName);

// Constructor for Gaussian PDF
uqGaussianPDF::uqGaussianPDF(double meanValue, double stdevValue){
  mean = meanValue;
  stdev = stdevValue;
}
    
// Eval Gaussian PDF
double uqGaussianPDF::eval(double XValue){
  return (1.0/(stdev*sqrt(2.0*kPI)))*exp(-(XValue-mean)*(XValue-mean)/(2.0*stdev*stdev));
}

// Eval Truncated Gaussian PDF
uqTruncatedGaussianPDF::uqTruncatedGaussianPDF(double mean, double stdev, double trunc):uqGaussianPDF(mean,stdev){
  // Assign Truncation Level
  truncSigma = trunc;
}

// Eval Gaussian PDF
double uqTruncatedGaussianPDF::eval(double XValue){
  // Check Result
  if(XValue<(mean-truncSigma)){
    return 0.0;
  }else if(XValue>(mean+truncSigma)){
    return 0.0;
  }else{
    return (1.0/(stdev*sqrt(2.0*kPI)))*exp(-(XValue-mean)*(XValue-mean)/(2.0*stdev*stdev));  
  }
}

// Rescaled Truncated Gaussian PDF
uqRescaledTruncatedGaussianPDF::uqRescaledTruncatedGaussianPDF(double trunc):uqTruncatedGaussianPDF(0.0,1.0,trunc){
  truncFactor = trunc;
}

// Eval Rescaled Truncated Gaussian PDF
double uqRescaledTruncatedGaussianPDF::eval(double XValue){
  double newValue = -truncFactor + (2.0*truncFactor)*XValue;
  return (1.0/(stdev*sqrt(2.0*kPI)))*exp(-(newValue-mean)*(newValue-mean)/(2.0*stdev*stdev));
}






