#ifndef UQPDF_H
#define UQPDF_H

# include <string>
# include "uqConstants.h"

using namespace std;

// Generic PDF Function
class uqPDF{
  public:
    uqPDF();
    virtual ~uqPDF(){}
    // Member Functions
    virtual double eval(double XValue){return 0.0;}
};

// Gaussian PDF
class uqGaussianPDF: public uqPDF 
{
  public:
    // Parameters: TEMP
    double mean;
    double stdev; 
    // Constructor and Destructor
    uqGaussianPDF(double mean, double stdev);
    virtual ~uqGaussianPDF(){}
    // Member Functions
    virtual double eval(double XValue);
    void printToFile(string fileName);
};

// Truncated Gaussian PDF
class uqTruncatedGaussianPDF: public uqGaussianPDF
{
  public:
    // Parameters: TEMP
    double truncSigma;
    // Constructor and Destructor
    uqTruncatedGaussianPDF(double mean, double stdev, double trunc);
    virtual ~uqTruncatedGaussianPDF(){}
    // Member Functions
    virtual double eval(double XValue);
};

// Rescaled Truncated Gaussian PDF
class uqRescaledTruncatedGaussianPDF: public uqTruncatedGaussianPDF
{
public:
    // Data Members: TEMP
    double truncFactor;
    // Constructor and Destructor
    uqRescaledTruncatedGaussianPDF(double truncFactor);
    virtual ~uqRescaledTruncatedGaussianPDF(){}
    // Member Functions
    virtual double eval(double XValue);
};

#endif // UQPDF_H
