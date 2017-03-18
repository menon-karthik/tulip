%module tulipDA
%{
/* Includes the header in the wrapper code */
#include "daData.h"
#include "daException.h"
#include "daData_Scalar_MultiplePatients.h"
#include "daData_Variable_SinglePatient.h"
%}

/* Parse the header file to generate wrappers */

%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

%include "daData.h"
%include "daException.h"
%include "daData_Scalar_MultiplePatients.h"
%include "daData_Variable_SinglePatient.h"

namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
} 
