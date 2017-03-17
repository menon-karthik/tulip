%module tulipUQ
 
%{
/* Includes the header in the wrapper code */
#include "uq1DApproximant.h"
#include "uq1DApproximant_SE.h"
#include "uq1DApproximant_ME.h"
#include "uq1DQuadratureRule.h"
#include "uq1DQuadratureRule_Regular.h"
#include "uq1DQuadratureRule_CC.h"
#include "uqAlgorithm.h"
#include "uqAlgorithmBCS.h"
#include "uqBasis.h"
#include "uqPolyBasis.h"
#include "uqOrthoPolyBasis.h"
#include "uqMWBasis.h"
#include "uqMultiIndex.h"
#include "uqMatrix.h"
#include "uqPolyMatrix.h"
#include "uqMWMatrix.h"
#include "uqPartition.h"
#include "uqPartitionBinaryTree.h"
#include "uqPDF.h"
#include "uqSamples.h"
#include "uqException.h"
#include "uqConstants.h"
#include "uqTypes.h"
#include "uqUtils.h"
%}
 
/* Parse the header file to generate wrappers */

%include <std_string.i>
%include <std_vector.i>
%include <std_map.i>

%include "uq1DApproximant.h"
%include "uq1DApproximant_SE.h"
%include "uq1DApproximant_ME.h"
%include "uq1DQuadratureRule.h"
%include "uq1DQuadratureRule_Regular.h"
%include "uq1DQuadratureRule_CC.h"
%include "uqAlgorithm.h"
%include "uqAlgorithmBCS.h"
%include "uqBasis.h"
%include "uqPolyBasis.h"
%include "uqOrthoPolyBasis.h"
%include "uqMWBasis.h"
%include "uqMultiIndex.h"
%include "uqMatrix.h"
%include "uqPolyMatrix.h"
%include "uqMWMatrix.h"
%include "uqPartition.h"
%include "uqPartitionBinaryTree.h"
%include "uqPDF.h"
%include "uqSamples.h"
%include "uqException.h"
%include "uqConstants.h"
%include "uqTypes.h"
%include "uqUtils.h"
 
namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
}  

 