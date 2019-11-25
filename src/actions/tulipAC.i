%module tulipAC
%{
/* Includes the header in the wrapper code */
#include "acAction.h"
#include "acException.h"
#include "acActionGSI.h"
#include "acActionLSI_FD.h"
#include "acAction_OBJProfile.h"
#include "acActionOPT_NM.h"
#include "acActionOPT_RSSD.h"
#include "acActionDREAM.h"
#include "acActionUP.h"
#include "acActionUP_MC.h"
#include "acActionUP_MWBCS.h"
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

%include "acAction.h"
%include "acException.h"
%include "acActionGSI.h"
%include "acActionLSI_FD.h"
%include "acAction_OBJProfile.h"
%include "acActionOPT_NM.h"
%include "acActionOPT_RSSD.h"
%include "acActionDREAM.h"
%include "acActionUP.h"
%include "acActionUP_MC.h"
%include "acActionUP_MWBCS.h"
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