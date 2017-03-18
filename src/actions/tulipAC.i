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
%}

/* Parse the header file to generate wrappers */

%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

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

namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
} 
