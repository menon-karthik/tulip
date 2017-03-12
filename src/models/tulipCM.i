%module LPN_Coronary
 %{
 /* Includes the header in the wrapper code */
 #include "uqTypes.h"
 #include "daData.h"
 #include "daData_Scalar_MultiplePatients.h"
 #include "cmModel.h"
 #include "cmLPN_Coronary.h"
 #include "lpnUtils.hpp"
 using namespace std;
 %}

 /* Parse the header file to generate wrappers */

 %include "std_string.i"
 %include "std_vector.i"
 %include "std_map.i"
 %include "uqTypes.h"
 %include "daData.h"
 %include "daData_Scalar_MultiplePatients.h"
 %include "cmModel.h"
 %include "cmLPN_Coronary.h"
 %include "lpnUtils.hpp"
 
 namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
 }  

