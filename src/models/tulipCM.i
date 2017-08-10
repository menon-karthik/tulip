%module tulipCM

%{
/* Includes the header in the wrapper code */
#include "cmModel.h"
#include "cmConstants.h"
#include "cmException.h"
#include "cmUtils.h"
#include "cmTutorial.h"
#include "cmSobolFunction.h"
#include "cmAnalyticalExpressionModel.h"
#include "cmLPNModel.h"
#include "cmTableInterpolator.h"
#include "cmSimpleNonLin.h"
#include "cmResistanceModel.h"
#include "cmPDF_KnownPDF.h"
#include "cmKraichnanOrszagModel.h"
#include "cmArmVenousOcclusion.h"
#include "cm3DPQSurrogateModel.h"
#include "cmStressStretch_SHRA.h"
#include "cmStressStretch_GOH.h"
#include "cmStressStretch_FR.h"
#include "odeIntegrator.h"
#include "odeIntegratorRK4.h"
#include "odeModel.h"
#include "odeNormalAdultSimplePA.h"
#include "uqTypes.h"
%}

/* Parse the header file to generate wrappers */

%include <typemaps.i>
%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

%apply const int & { int & }; 
%apply const double & { double & }; 

%include "cmModel.h"
%include "cmConstants.h"
%include "cmException.h"
%include "cmUtils.h"
%include "cmTutorial.h"
%include "cmSobolFunction.h"
%include "cmAnalyticalExpressionModel.h"
%include "cmLPNModel.h"
%include "cmTableInterpolator.h"
%include "cmSimpleNonLin.h"
%include "cmResistanceModel.h"
%include "cmPDF_KnownPDF.h"
%include "cmKraichnanOrszagModel.h"
%include "cmArmVenousOcclusion.h"
%include "cm3DPQSurrogateModel.h"
%include "cmStressStretch_SHRA.h"
%include "cmStressStretch_GOH.h"
%include "cmStressStretch_FR.h"
%include "odeIntegrator.h"
%include "odeIntegratorRK4.h"
%include "odeModel.h"
%include "odeNormalAdultSimplePA.h"
%include "uqTypes.h"

namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
} 

%apply const stdVec & { stdVec & }; 
%apply const stdMat & { stdMat & }; 

