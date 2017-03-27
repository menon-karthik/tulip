#ifndef UQUTILS_H
#define UQUTILS_H

# define ARMA_DONT_USE_CXX11
# include <armadillo>

# include "uqSamples.h"
# include "uqMWMatrix.h"
# include "uqConstants.h"
# include "uqException.h"
# include "uqTypes.h"

namespace uqUtils{

  /*! 
  \verbatim embed:rst
  **Purpose**
  Apply Fourier low-pass filering to signals

  **Discussion**
  None

  **Modified**
  Aug 2016 - DES
    
  **Reference**
  No Reference
  \endverbatim
  \param[in] numFreq Number of frequencies to be retained.
  \param[in/out] margY signal to be filtered.
  */
  void convertTo01Array(int intValue, int numberOfDimension, int* resultVector);

  // Perform Inverse FFT - ARMADILLO LIBRARY
  void fft1DInv(int size, int gSize, double* GX, double* GY, double* fftWeights);

  // Solve Dense Linear System Of Equations
  void solveDenseLinearSystem(int totRows, int totCols, stdMat coeffMat, stdVec currentRHS, stdVec& sol);

  /*! 
  \verbatim embed:rst
  **Purpose**
  Apply Fourier low-pass filering to signals

  **Discussion**
  None

  **Modified**
  Aug 2016 - DES
    
  **Reference**
  No Reference
  \endverbatim
  \param[in] numFreq Number of frequencies to be retained.
  \param[in/out] margY signal to be filtered.
  */
  void applyFourierSmoothing(int numFreq,stdVec& margY);

  /*! 
  \verbatim embed:rst
  **Purpose**
  Convolve the function with a zero mean Gaussian
  of specified standard deviation

  **Discussion**
  This is used for example for kernel density estimation
  as it produces a dacaying tail of the resulting 
  distribution that frequency based filters (e.g. low-pass) do not guarantee 

  **Modified**
  Aug 2016 - DES
    
  **Reference**
  No Reference
  \endverbatim
  \param[in] Value of standard deviation in length units of margY
  \param[in/out] margY signal whose convolution is sought
  */
  void convolutionWithGaussian(double stdVal, stdVec& margY);

  /*! 
  \verbatim embed:rst
  **Purpose**
  Count the number of lines in a file

  **Discussion**
  None
  
  **Modified**
  Aug 2016 - DES
    
  **Reference**
  No Reference
  \endverbatim
  \param[in] fileName the name of the file whose line count is sought
  \return number of lines in file
  */
  int countLinesInFile(string fileName);

}

#endif // UQUTILS_H
