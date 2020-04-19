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
    unsigned long long int seed;

    // Constructor and distructor
    uqPDF(int seed = mt19937_64::default_seed);
    virtual ~uqPDF();
    
    // Multiple realizations
    stdVec evaluate(double par1, double par2, const stdVec& XVals);
    stdVec sample(double par1, double par2, int num_samples);
    unsigned long long int getSeed();
    
    // Virtual Member Functions
    virtual double evaluate(double par1, double par2, double XValue) = 0;
    virtual double sample(double par1, double par2) = 0;
};

// Gaussian PDF
class uqGaussianPDF: public uqPDF{
  public:
    // Distribution object
    normal_distribution<double>* dist;

    // Constructor and Destructor
    uqGaussianPDF(int seed=mt19937_64::default_seed);
    virtual ~uqGaussianPDF();

    // Member Functions
    virtual double evaluate(double par1, double par2, double XValue);
    virtual double sample(double par1, double par2);
};

// Uniform PDF
class uqUniformPDF: public uqPDF{
  public:
    // Distribution object
    uniform_real_distribution<double>* dist;

    // Constructor and Destructor
    uqUniformPDF(int seed=mt19937_64::default_seed);
    virtual ~uqUniformPDF();

    // Member Functions
    virtual double evaluate(double par1, double par2, double XValue);
    virtual double sample(double par1, double par2);
};

// Categorical PMF
class uqCategoricalPMF{
  public:
    // Distribution object
    uqUniformPDF* uSampler;

    // Constructor and Destructor
    uqCategoricalPMF(int seed=mt19937_64::default_seed);
    virtual ~uqCategoricalPMF();

    // Member Functions
    double evaluate(int xVal, const stdVec& pmf);
    int sample(const stdVec& pmf);
    unsigned long long int getSeed();
};

// Truncated Gaussian PDF
class uqTruncatedGaussianPDF: public uqGaussianPDF{
  public:
    // Parameters: TEMP
    double truncSigma;

    // Constructor and Destructor
    uqTruncatedGaussianPDF(double trunc, int seed=mt19937_64::default_seed);
    virtual ~uqTruncatedGaussianPDF();

    // Member Functions
    virtual double evaluate(double par1, double par2, double XValue);
    virtual double sample(double par1, double par2);
};

// Rescaled Truncated Gaussian PDF
class uqRescaledTruncatedGaussianPDF: public uqTruncatedGaussianPDF{
public:
    // Data Members: TEMP
    double truncFactor;

    // Constructor and Destructor
    uqRescaledTruncatedGaussianPDF(double truncFactor, int seed=mt19937_64::default_seed);
    virtual ~uqRescaledTruncatedGaussianPDF();

    // Member Functions
    virtual double evaluate(double par1, double par2, double XValue);
    virtual double sample(double par1, double par2);
};

#endif // UQPDF_H
