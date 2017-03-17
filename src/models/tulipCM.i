%module LPN_Coronary
%{
/* Includes the header in the wrapper code */
#include "cmModel.h"
#include "cmConstants.h"
#include "cmException.h"
#include "cmUtils.h"
#include "cmTutorial.h"
#include "cmAnalyticalExpressionModel.h"
#include "cmLPNModel.h"
#include "cmTableInterpolator.h"
#include "cmSimpleNonLin.h"
#include "cmResistanceModel.h"
#include "cmPDF_KnownPDF.h"
#include "cmKraichnanOrszagModel.h"
#include "cmArmVenousOcclusion.h"
#include "cm3DPQSurrogateModel.h"
%}

/* Parse the header file to generate wrappers */

%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

%include "cmModel.h"
%include "cmConstants.h"
%include "cmException.h"
%include "cmUtils.h"
%include "cmTutorial.h"
%include "cmAnalyticalExpressionModel.h"
%include "cmLPNModel.h"
%include "cmTableInterpolator.h"
%include "cmSimpleNonLin.h"
%include "cmResistanceModel.h"
%include "cmPDF_KnownPDF.h"
%include "cmKraichnanOrszagModel.h"
%include "cmArmVenousOcclusion.h"
%include "cm3DPQSurrogateModel.h"

namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
} 


