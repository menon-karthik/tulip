# include "uqUtils.h"

using namespace std;
using namespace arma;

namespace uqUtils{

void convertTo01Array(int intValue, int numberOfDimension, int* resultVector){
  for (int loopA=0;loopA<numberOfDimension;loopA++){
    resultVector[numberOfDimension-loopA+1] = (intValue % 2);
    intValue = (intValue/2);
  }
}

// F[] should satisfy symmetry property F[k] = conj(F[N-k]), so just  one
// half of frequencies array is needed - elements from 0 to floor(N/2).  F[0]
// is ALWAYS real. If N is even F[floor(N/2)] is real too. If N is odd,  then
// F[floor(N/2)] has no special properties.
void fft1DInv(int size, int gSize, double* GX, double* GY, double* fftWeights){
  cx_vec cx_in(size);
  cx_vec cx_out(size);
  for(int loopA=0;loopA<size;loopA++){
    cx_in[loopA].real(0.0);
    cx_in[loopA].imag(0.0);
    cx_out[loopA].real(0.0);
    cx_out[loopA].imag(0.0);
  }

  // Put Samples in complex data Structure
  for(int loopA=0;loopA<gSize;loopA++){
    cx_in[loopA].real(GX[loopA]);
    cx_in[loopA].imag(GY[loopA]);
  }
  for(int loopA=0;loopA<gSize-1;loopA++){
    cx_in[size-loopA-1].real(GX[loopA+1]);
    cx_in[size-loopA-1].imag(GY[loopA+1]);
  }  

  // Inverse Transform
  cx_out = ifft(cx_in);

  // Transform Back Values
  for(int loopA=0;loopA<size;loopA++){
    fftWeights[loopA] = cx_out[loopA].real();
  }
}

// Solve Dense Linear System Of Equations
void solveDenseLinearSystem(int totRows, int totCols, stdMat coeffMat, const stdVec& currentRHS, stdVec& sol){  
  mat A(totRows,totCols);
  vec b(totRows);
  vec xVec(totCols);

  // Copy quantities
  for(int loopA=0;loopA<totRows;loopA++){
    b(loopA) = currentRHS[loopA];
    for(int loopB=0;loopB<totCols;loopB++){
      A(loopA,loopB) = coeffMat[loopA][loopB];
    }
  }

  // Solve System
  bool solvedOK = solve(xVec,A,b);
  if(!solvedOK){
    throw uqException("ERROR: Armadillo solve...\n");
  }

  // Copy Result Back
  for(int loopA=0;loopA<totCols;loopA++){
    sol[loopA] = xVec[loopA];
  }
}

// Apply Fourier Smoothing
void applyFourierSmoothing(int numFreq,stdVec& margY){

  int N = margY.size();
  
  // Declaration
  vec orig(margY);

  // Make the Fourier Transform
  cx_vec mergYFreq = fft(orig);

  printf("PRIMA DEL FILTRO\n");
  for(int loopA=0;loopA<mergYFreq.size();loopA++){
    printf("Freq: %d, Ampl: %f\n",loopA,norm(mergYFreq[loopA]));
  }
  printf("Press a key to continue...\n");
  getchar();

  // Filter in the Frequency Domain 
  // Y(1) is the constant offset
  // Y(2:N/2 + 1) is the set of positive frequencies
  // Y(N/2 + 2:end)
  // Filter the Positive Coefficients
  for(int loopA=numFreq+1;loopA<N/2+2;loopA++){
    mergYFreq[loopA].real(0.0);
    mergYFreq[loopA].imag(0.0);
  }
  for(int loopA=N-numFreq-1;loopA>N/2;loopA--){
    mergYFreq[loopA].real(0.0);
    mergYFreq[loopA].imag(0.0);
  }

  // Transform back
  cx_vec mergYFilt = ifft(mergYFreq);

  // Transform Back Values
  margY.clear();
  for(int loopA=0;loopA<mergYFilt.size();loopA++){
    margY.push_back(mergYFilt[loopA].real());
  }
}

// Evaluate the One-dimensional Gaussian
double evalDiscreteGaussian(double x,double stdVal){
  return (1.0/(stdVal * sqrt(2.0 * M_PI)))* exp(-0.5*(x/stdVal)*(x/stdVal));
}

// Perform Convolution with Gaussian
void convolutionWithGaussian(double stdVal, stdVec& margY){

  // Get Signal Size
  int N = margY.size();

  // Create a Discrete Gaussian
  vec gaussSignal(N);
  double currX = 0.0;
  for(int loopA=0;loopA<N;loopA++){
    currX = loopA - N/2;
    gaussSignal[loopA] = evalDiscreteGaussian(currX,stdVal);
  }

  // Declaration
  vec orig(margY);

  // Perform Convolution
  vec mergYFilt = conv(orig,gaussSignal,"same");

  // Transform Back Values
  margY.clear();
  for(int loopA=0;loopA<mergYFilt.size();loopA++){
    margY.push_back(mergYFilt[loopA]);
  }
}

// Count the number of lines in a file
int countLinesInFile(string fileName){
  std::ifstream inFile(fileName); 
  return std::count(std::istreambuf_iterator<char>(inFile), 
         std::istreambuf_iterator<char>(), '\n');
}


double sumvec(int num,double vec[]){
  double res = 0.0;
  for(int loopA=0;loopA<num;loopA++){
    res = res + vec[loopA];
  }
  return res;
}

void printTimestamp(){
  static char time_buffer[40];
  const struct tm *tm;
  time_t now;
  now = time(NULL);
  tm = localtime(&now);
  strftime ( time_buffer, 40, "%d %B %Y %I:%M:%S %p", tm );
  cout << time_buffer << "\n";
}

}