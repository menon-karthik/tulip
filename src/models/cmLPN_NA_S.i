%module LPN_NASimple
 %{
 /* Includes the header in the wrapper code */
 #include "uqTypes.h"
 #include "daData.h"
 #include "cmModel.h"
 #include "cmLPN_NA_S_PA.h"
 #include "cmLPN_NA_S_PD.h"
 #include "lpnUtils.hpp"
 using namespace std;
 %}
 
 /* Parse the header file to generate wrappers */

 %include "std_string.i"
 %include "std_vector.i"
 %include "std_map.i"
 %include "uqTypes.h"
 %include "daData.h"
 %include "cmModel.h"
 %include "cmLPN_NA_S_PA.h"
 %include "cmLPN_NA_S_PD.h"
 %include "lpnUtils.hpp"
 
 namespace std {
  typedef std::string String;
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
  %template(stdStringVec) vector<string>;
  %template(stdStringMat) vector<vector<string> >;
 }  

