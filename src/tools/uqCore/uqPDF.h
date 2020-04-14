#ifndef UQPDF_H
#define UQPDF_H

# include <random>
# include "uqException.h"
# include "uqTypes.h"

using namespace std;

// Generic PDF Function
class uqPDF{
  public:
    // Random number generator
    mt19937_64 mt;

    uqPDF(int seed = 0);
    virtual ~uqPDF();
    // Multiple realizations
    stdVec evaluate(const stdVec& XVals);
    stdVec sample(int num_samples);
    // Virtual Member Functions
    virtual double evaluate(double XValue) = 0;
    virtual double sample() = 0;
};

// Gaussian PDF
class uqGaussianPDF: public uqPDF{
  public:
    // Distribution object
    normal_distribution<double>* dist;
    // Parameters: TEMP
    double mean;
    double stdev; 
    // Constructor and Destructor
    uqGaussianPDF(double mean, double stdev, int seed=0);
    virtual ~uqGaussianPDF();
    // Member Functions
    virtual double evaluate(double XValue);
    virtual double sample();
};

// Uniform PDF
class uqUniformPDF: public uqPDF{
  public:
    // Distribution object
    uniform_real_distribution<double>* dist;
    // Parameters: TEMP
    double a;
    double b; 
    // Constructor and Destructor
    uqUniformPDF(double a, double b, int seed=0);
    virtual ~uqUniformPDF();
    // Member Functions
    virtual double evaluate(double XValue);
    virtual double sample();
};


// Truncated Gaussian PDF
class uqTruncatedGaussianPDF: public uqGaussianPDF{
  public:
    // Parameters: TEMP
    double truncSigma;
    // Constructor and Destructor
    uqTruncatedGaussianPDF(double mean, double stdev, double trunc);
    virtual ~uqTruncatedGaussianPDF();
    // Member Functions
    virtual double evaluate(double XValue);
    virtual double sample();
};

// Rescaled Truncated Gaussian PDF
class uqRescaledTruncatedGaussianPDF: public uqTruncatedGaussianPDF{
public:
    // Data Members: TEMP
    double truncFactor;
    // Constructor and Destructor
    uqRescaledTruncatedGaussianPDF(double truncFactor);
    virtual ~uqRescaledTruncatedGaussianPDF();
    // Member Functions
    virtual double evaluate(double XValue);
    virtual double sample();
};

#endif // UQPDF_H
