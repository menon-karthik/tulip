%module tulipNT
%{
/* Includes the header in the wrapper code */
#include "ntNetIO.h"
#include "ntNet.h"
#include "ntNode.h"
#include "ntEdge.h"
#include "ntFactor.h"
#include "ntMessage.h"
#include "ntUtils.h"
#include "ntConstants.h"
#include "ntException.h"
#include "uqTypes.h"
%}

%exception{
    try {
        $action
    }
    catch (const std::exception & e)
    {
        SWIG_exception(SWIG_RuntimeError, (std::string("C++ std::exception: ") + e.what()).c_str());
    }
    catch (...)
    {
        SWIG_exception(SWIG_UnknownError, "C++ anonymous exception");
    }
}

/* Parse the header file to generate wrappers */

%include <typemaps.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

%apply const int & { int & }; 
%apply const double & { double & }; 

%include "ntNetIO.h"
%include "ntNet.h"
%include "ntNode.h"
%include "ntEdge.h"
%include "ntFactor.h"
%include "ntMessage.h"
%include "ntUtils.h"
%include "ntConstants.h"
%include "ntException.h"
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
%apply const stdIntVec & { stdIntVec & };
%apply const stdMat & { stdMat & }; 