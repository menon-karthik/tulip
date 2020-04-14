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
#include "cm_darpaSimple_code1.h"
#include "cm_darpaSimple_code2.h"
#include "cm_darpaSimple_code3.h"
#include "cmDarpaBeamStress.h"
#include "cmOscillator.h"
#include "odeIntegrator.h"
#include "odeIntegratorRK4.h"
#include "odeModel.h"
#include "odeNormalAdultSimplePA.h"
#include "odeCongenital4ChambersPAH.h"
#include "odeRCR.h"
#include "odeRCRCR.h"
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
%include "cm_darpaSimple_code1.h"
%include "cm_darpaSimple_code2.h"
%include "cm_darpaSimple_code3.h"
%include "cmDarpaBeamStress.h"
%include "cmOscillator.h"
%include "odeIntegrator.h"
%include "odeIntegratorRK4.h"
%include "odeModel.h"
%include "odeNormalAdultSimplePA.h"
%include "odeCongenital4ChambersPAH.h"
%include "odeRCR.h"
%include "odeRCRCR.h"
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

