%module tulipDA

%{
/* Includes the header in the wrapper code */
#include "daData.h"
#include "daException.h"
#include "daData_single_GPatient.h"
#include "daData_multiple_Table.h"
#include "uqTypes.h"
%}

/* Parse the header file to generate wrappers */

%include <typemaps.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

%apply const int & { int & }; 
%apply const double & { double & }; 

%include "daData.h"
%include "daException.h"
%include "daData_single_GPatient.h"
%include "daData_multiple_Table.h"
%include "uqTypes.h"

namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
  %template(stdBoolVec) vector<bool>;
  %template(stdBoolMat) vector<vector<bool> >;
} 

%apply const stdVec & { stdVec & }; 
%apply const stdMat & { stdMat & }; 
