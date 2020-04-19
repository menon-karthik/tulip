#ifndef CMUTILS_H
#define CMUTILS_H

# include <string.h>
# include <vector>
# include <boost/algorithm/string.hpp>
# include <cstdlib>
# include <stdio.h>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>
# include <algorithm>
# include <vector>
# include <limits>

# include "uqTypes.h"
# include "uqConstants.h"
# include "cmException.h"

using namespace std;

namespace cmUtils{

//! Descending sorting options
const int ipSortDescending = 0;
//! Ascending sorting options
const int ipSortAscending  = 1;

/*! 
Record with x,y data for 1D curve storage
*/
struct data{ 
  //! Storage of x coords, usually time.
  double xVal;
  //! Curva value
  double yVal;
};

/*!
Sorting operator
*/
struct by_number { 
  bool operator()(data const &left, data const &right){ 
    return left.xVal < right.xVal;
  }
};

/*! 
Options for LPN Models
*/
struct lpnOptions{
  //! Write debug information  
  bool writeDebugData;
  //! Write results traces to file. If false the allData file is not saved.
  bool flushData;
  //! Activate/deactivate parameter limit checking.
  bool checkLimits;
  //! Activate/deactivate parameter sign checks.
  bool checkSign;
  //! Constrains the model parameters (Deprecated)
  bool useFullConstraints;
};

/*!
Record for 3DModel Surrogate.
*/
struct model3DRecord{
  //! Type of polynomial expansion used for RVM Surrogate.
  int polyType;
  //! Order of the polynomial expansion.
  int polyOrder;
  //! Total number of dimensions, i.e., model interfaces. 
  int totDims;
  //! The surrogate was built after data normalization.
  bool normalizeData;
  //! Limits are needed to de-normalize the surrogate if normalization was used.
  stdMat limits;
  //! Matrix of expansion coefficients for the current RVM Surrogate.
  stdMat expCoeffs;
};

/*! 
\verbatim embed:rst
**Purpose**
Get maximum value in array between two
prescribed lower and upped index bounds
[start, stop]
    
**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index
\param[in] stop Index to the last array component to consider
\param[in] vector The vector whose maximum value needs to be found
return Maximum value in the array in the index interval [start,stop]
*/
double getMax(int start, int stop, const double* vector);
double getMax(int start, int stop, const stdVec& vector); // added so that it can take vector in standard vector form

/*! 
\verbatim embed:rst
**Purpose**
Get minimum value in array between two
prescribed lower and upped index bounds
[start, stop]

**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index
\param[in] stop Index to the last array component to consider
\param[in] vector The vector whose minimum value needs to be found
return Minimum value in the array in the index interval [start,stop]
*/

double getMin(int start, int stop, const double* vector);
double getMin(int start, int stop, const stdVec& vector); // added so that it can take vector in standard vector form


/*! 
\verbatim embed:rst
**Purpose**
Get average value in array between two
prescribed lower and upped index bounds
[start, stop]

**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index
\param[in] stop Index to the last array component to consider
\param[in] vector The vector whose average value needs to be found
return Average value in the array in the index interval [start,stop]
*/
double getMean(int start, int stop, const double* vector);
double getMean(int start, int stop, const stdVec& vector);

/*! 
\verbatim embed:rst
**Purpose**
Get summation of elements in array between two
prescribed lower and upped index bounds
[start, stop]

**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index
\param[in] stop Index to the last array component to consider
\param[in] vector The vector whose summation needs to be found
return Sum of array components in the index interval [start,stop]
*/
double getSum(int start, int stop, const double* vector);
double getSum(int start, int stop, const stdVec& vector);

/*! 
\verbatim embed:rst
**Purpose**
Get location of maximum value in array
between two prescribed lower and upped index 
bounds [start, stop]

**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index
\param[in] stop Index to the last array component to consider
\param[in] vector The vector whose summation needs to be found
return The index of the maximum component in the array
*/
int getMaxLoc(int start, int stop, const double* vector);

/*! 
\verbatim embed:rst
**Purpose**
Performs numerical quadrature with trapeziodal rule
in array within the array index [start,stop]

**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index
\param[in] stop Index to the last array component to consider
\param[in] xVals Abscissa
\param[in] yVals Ordinate
return An approximation of the integral of the 1D function discretized through xVals,yVals.
*/
double trapz(int start, int stop, double* xVals, double* yVals);
double trapz(int start, int stop, const stdVec& xVals, const stdVec& yVals);

/*! 
\verbatim embed:rst
**Purpose**
Split space-separated string to vector

**Discussion**
None
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] sentence The space-separated string to split
\param[out] tokens the resultant string vector
*/
void schSplit(string sentence,stdStringVec& tokens,const char* separator = " ");

/*! 
\verbatim embed:rst
**Purpose**
Applies a circular shift to an array by specifying the 
number of components

**Discussion**
The function assumes Qvp to be a periodic curve and that one
period is available.
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] shift Number of components for circular shift.
\param[in] size Number of components in array
\param[in,out] Qvp Array to be shifted 
*/
void applyCircShift(int shift,int size,double* Qvp);

/*! 
\verbatim embed:rst
**Purpose**
Read array from file

**Discussion**
Each component of the array should be 
locate in a separate row. Empty rows or
rows beginning with "/" are ignored.
Note that the array "params" should be allocated 
before executing ReadParamsFromFile.
    
**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] inputFileName A string containing the name of the file to parse
\param[out] params the resultant parameter vector
return Error code (if =0 execution OK)
*/
int readParamsFromFile(string inputFileName,double* params);

/*! 
\verbatim embed:rst
**Purpose**
Read two arrays from file

**Discussion**
Each row should contain two space-separated
array components.Empty rows or
rows beginning with "/" are ignored.

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] inputFileName The name of the input file
\param[out] prior_num the cardinality of the two arrays
\param[out] prAv The first array in the file (first column)
\param[out] prSd The second array in the file (second column)
return Error code (if =0 execution OK)
*/
int readPriorFromFile(string inputFileName,int &prior_num,stdVec& prAv,stdVec& prSd);

/*! 
\verbatim embed:rst
**Purpose**
Read space-separated double table from file and store
the content in std matrix

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to read
\param[in] samples std Matrix where to store the table
return Error code
*/
int readTableFromFile(string fileName,stdMat& samples);

/*! 
\verbatim embed:rst
**Purpose**
Read space-separated integer table from file and store
the content in std matrix

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to read
\param[in] samples std Matrix where to store the table
return Error code
*/
int readIntTableFromCSVFile(string fileName,stdIntMat& samples);

/*! 
\verbatim embed:rst
**Purpose**
Read a continuous integer vector from file

**Discussion**
No comments or empty lines are tolerated in the text file containing the index array

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName The name of the text file to read
\param[out] vec The vector where the indexes read from a "fileName" will be stored
return Error code
*/
int readIntVectorFromFile(string fileName,stdIntVec& vec);

/*! 
\verbatim embed:rst
**Purpose**
Write space-separated table to file

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to be written
\param[in] table std Matrix to print
*/
void writeTableToFile(string fileName,const stdMat& table);

/*! 
\verbatim embed:rst
**Purpose**
Write vector to file one component per row

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to be written
\param[in] vec vector to print
*/
void writeVectorToFile(string fileName,const stdVec& vec);

/*! 
\verbatim embed:rst
**Purpose**
Read a file with a single vector. 
Read a full matrix and stores the specified column in the
vector provided.

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to be written
\param[in] vec vector to print
*/
int readVectorFromFile(string fileName,int column,stdVec& vec);

/*! 
\verbatim embed:rst
**Purpose**
Write LPN model result to file.

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] debugFileName File where to write the results.
\param[in] totalSteps Total number of saved time steps (rows in the matrix "outVals")
\param[in] totalStates Total number of result quantities (columns in the matrix "outVals")
\param[in] time Array with simulation time for all time steps. 
\param[in] outVals Matrix containing the LPN model results.
*/
void writeResultsToFile(string debugFileName, int totalSteps, int totalStates, double* time, double** outVals);

/*! 
\verbatim embed:rst
**Purpose**
Function to print flow rate curves and associated peaks to file
for printing/visualization purposes.
The flow rate curve is assumed biphasic, i.e., characterized by two
maxima and two minima (SMDA).

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the output file.
\param[in] size Number of components in time and flow rate arrays.
\param[in] t Array containing the simulation times.
\param[in] Q Array containing flow rates for each time instant.
\param[in] Q_S Flow rate at the first maximum
\param[in] t_S Simulation time for the first maximum value
\param[in] Q_M Flow rate at the first minimum
\param[in] t_M Simulation time for the first minimum value
\param[in] Q_D Flow rate at the second maximum
\param[in] t_D Simulation time for the second maximum value
\param[in] Q_A Flow rate at the second minimum
\param[in] t_A Simulation time for the second minimum value
*/
void printCurvesAndPeaks(string fileName,int size,double* t,double* Q, 
	                       double Q_S,double t_S,
	                       double Q_M,double t_M,
	                       double Q_D,double t_D,
	                       double Q_A,double t_A);
/*! 
\verbatim embed:rst
**Purpose**
SMDA Peak detection function

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] data_count Size of the time (t) and data arrays
\param[in] t Array with simulation times
\param[in] data Array with curve for peak detection
\param[out] S_time Location in time of the S peak
\param[out] S_peak Value of the S peak
\param[out] S_found True is the peak has been found
\param[out] M_time Location in time of the M peak
\param[out] M_peak Value of the M peak
\param[out] M_found True is the peak has been found
\param[out] D_time Location in time of the D peak
\param[out] D_peak Value of the D peak
\param[out] D_found True is the peak has been found
\param[out] A_time Location in time of the A peak
\param[out] A_peak Value of the A peak
\param[out] A_found True is the peak has been found
\param[out] min_Found_Peaks Number of Found Minima (1 or 2)
\param[out] max_Found_Peaks Number of Found Maxima (1 or 2)
\return Error Code
*/
int detect_peak(int data_count, double* t, double* data,
                double &S_time, double &S_peak, bool &S_found,
                double &M_time, double &M_peak, bool &M_found,
                double &D_time, double &D_peak, bool &D_found,
                double &A_time, double &A_peak, bool &A_found,
                int& min_Found_Peaks, int& max_Found_Peaks);

/*! 
\verbatim embed:rst
**Purpose**
Linear interpolation of data from matrix

**Discussion**
If currX is outside the ranges, extrapolation is performed 
using the slope for the first or last two data points

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] table The data matrix
\param[in] xColumn Column index with x values
\param[in] yColumn Column index with y values
\param[in] currX The abscissa where the linear interpolant is needed
result The value of the linear interpolant at currX
*/
double linInterp(const stdMat& table,int xColumn,int yColumn,double currX);

/*! 
\verbatim embed:rst
**Purpose**
Read 3D Surrogate model from file

**Discussion**
A 3D surrogate model is a multivariate interpolator defined 
using the following quantities:
polyType - Formulation of polynomial approximant, either kPolyLegendre or kPolyHermite
polyOrder - Order of the polynomial approximant
totDims - Dimensionality of the polynomial approximant
normalizeData - Use normalized data (i.e., for Legendre interpolation where data is scale to [-1,1]^d)
limits - dx2 Matrix storing the limits of each dimension.
expCoeffs - Matrix with polynomial expansion coefficients.

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the 3D Surrogate Model file
\param[out] model Model data that need to be filled with the information contained in the 3D Surrogate Model file
\param[in] printReport Print a brief report with the summary of the acquired model
*/
void read3DModelSurrogateFromFile(string fileName,model3DRecord& model,bool printReport);

/*! 
\verbatim embed:rst
**Purpose**
Get String from integer storing the type of the polynomials approximant

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] polyType Integer type of the polynomial approximant either kPolyLegendre or kPolyHermite
return Either "LEGENDRE" or "HERMITE"
*/
string getPolyTypeString(int polyType);

/*! 
\verbatim embed:rst
**Purpose**
Finds whether or not a given parameter is expected
to be positive. 

**Discussion**
Check whether the admissible parameter ranges are 
both positive and returns true, otherwise false.

**Modified**
24 April 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] curr_par the index of the parameter whose sign is to be determined.
\param[in] limits parameter limit array.
result true if the parameters are both positive, false otherwise.

*/
bool isParamPositive(int curr_par, const stdVec& limits);

/*! 
\verbatim embed:rst
**Purpose**
Same as above but for negative ranges

**Discussion**
None

**Modified**
24 April 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] curr_par the index of the parameter whose sign is to be determined.
\param[in] limits parameter limit array 
result true if the parameters are inside the admissible ranges, false otherwise.
*/
bool isParamNegative(int curr_par, const stdVec& limits);

/*! 
\verbatim embed:rst
**Purpose**
Read comma-separated string table from file and store
the content in string matrix

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to read.
\param[in] table String Matrix where to store the values.
return Error code.
*/
int readCSStringTableFromFile(string fileName,stdStringMat& table);

/*! 
\verbatim embed:rst
**Purpose**
Read file of comma-separated index and values
of frozen model parameters

**Discussion**
None

**Modified**
2 May 2015 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] fileName Name of the file to read.
\param[out] frozParIDX std array with index set of frozen parameters.
\param[out] frozParVAL std array with frozen parameter values.
return Error code.
*/
int getFrozenParametersFromCSVFile(string fileName,stdIntVec& frozParIDX,stdVec& frozParVAL);

/*! 
\verbatim embed:rst
**Purpose**
Get acceleration time between two prescribed lower and upped index bounds [start, stop].
This is implemented using peak-finding. The acceleration time is the time need to reach the 
maximum of the curve.

**Discussion**
None

**Modified**
14 Jan 2016 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index.
\param[in] stop Index to the last array component to consider.
\param[in] timeStep The underlying time step, assuming that vector has beeen sampled uniformly in time.
\param[in] vector The vector with the time signal.
\param[out] at The acceleration time computed by the function.
return True if a maximum has been found otherwise false.
*/
bool getAccelerationTime(int start, int stop, double timeStep, double* vector, double& at);

/*! 
\verbatim embed:rst
**Purpose**
Get deceleration time between two
prescribed lower and upped index bounds
[start, stop]

**Discussion**
None

**Modified**
14 Jan 2016 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index.
\param[in] stop Index to the last array component to consider.
\param[in] timeStep The underlying time step, assuming that vector has beeen sampled uniformly in time.
\param[in] vector The vector containing the time signal.
\param[out] dt The deceleration time computed by the function.
return true if a maximum followed by a minimum has been found otherwise false.
*/
bool getDecelerationTime(int start, int stop, double timeStep, double* vector, double& dt);

/*! 
\verbatim embed:rst
**Purpose**
Get E/A ratio between two
prescribed lower and upped index bounds
[start, stop]

**Discussion**
None

**Modified**
14 Jan 2016 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index.
\param[in] stop Index to the last array component to consider.
\param[in] vector The vector containing the time signal to be analyzed.
\param[out] EARatio The ratio between two successive peaks in the time signal curve.
return true if both E and A peaks have been detected, false otherwise.
*/
bool getEARatio(int start, int stop, double* vector,double& EARatio);

/*! 
\verbatim embed:rst
**Purpose**
Circular shift of a curve to get zero at the valve opening time

**Discussion**
None

**Modified**
14 Jan 2016 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] start Starting array index.
\param[in] stop Index to the last array component to consider.
\param[in] input curve to process.
\param[in] array with valve status (0 - closed, 1 - open).
*/
bool zeroAtValveOpening(int start, int stop, double* curve, double* valveIsOpen);

/*! 
\verbatim embed:rst
**Purpose**
Form a table from a random subsample of realization in the MCMC traces "currTraceFile"
and also gets back the indexes

**Discussion**
None

**Modified**
Jan 2016 - DES
    
**Reference**
No Reference
\endverbatim

\param[in] currTraceFile File with parameter traces.
\param[in] startColumn starting column.
\param[in] endColumn end of columns in file.
\param[out] subSampleTable Table with parameters sub-samples.
\param[out] sampleIndexes Vector with the indexes of the sub-samples.
*/
void subSampleTableData(string currTraceFile,int& totSubSamples,int startColumn,int endColumn,stdMat& subSampleTable,stdIntVec& sampleIndexes);

/*! 
\verbatim embed:rst
**Purpose**
Sample an existing Table with provided indexes.

**Discussion**
None

**Modified**
Jan 2016 - DES
    
**Reference**
No Reference
\endverbatim
\param[in] currTraceFile File with parameter traces.
\param[in] sampleIndexes Indexes where to sample.
\param[in] startColumn Starting column of output quantities in file.
\param[in] endColumn Last column of output quantities in file.
\param[out] subSampleTable Final sample table
*/
void subSampleTableDataWithIndex(string currTraceFile,stdIntVec sampleIndexes,int startColumn,int endColumn,stdMat& subSampleTable);

/*! 
\verbatim embed:rst
**Purpose**
Generate normalized (PDF) or unnormalized frequency plot
from a sequence of doubles

**Discussion**
None

**Modified**
Aug 2016 - DES
    
**Reference**
No Reference
\endverbatim
\param[in] minVal start of the binning interval
\param[in] maxVal end of the binning interval
\param[in] values sequence of values whose frequency plot is needed
\param[in] totBins total number of bins
\param[in] normalize normalize data to construct an approximant of the marginal PDF
\param[out] resX abscissae of resulting frequency plot
\param[out] resY ordinate values of resulting frequency plot
*/
void generateFrequencyPlot(double minVal,double maxVal,stdVec values,int totBins,bool normalize,stdVec& resX,stdVec& resY);

/*! 
\verbatim embed:rst
**Purpose**
get air properties given altitude for International atmospheric model

**Discussion**
None

**Modified**
Nov 2019 - DES
    
**Reference**
No Reference
\endverbatim
\param[in] alt altitude in meters
\param[out] airTemperature in C
\param[out] airPressure in Pa
\param[out] airDensity in kg/m3
*/
void getAirProps(double alt,double& airTemperature,double& airPressure,double& airDensity);

}
#endif // CMUTILS_H