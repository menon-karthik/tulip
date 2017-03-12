%module LPN_NormalAdult
 %{
 /* Includes the header in the wrapper code */
 #include "uqTypes.h"
 #include "cmModel.h"
 #include "cmLPN_NormalAdult.h"
 using namespace std;
 %}
 
 /* Parse the header file to generate wrappers */

 %include "uqTypes.h"
 %include "cmModel.h"
 %include "cmLPN_NormalAdult.h"
 %include "std_string.i"
 %include "std_vector.i"

 namespace std {
  %template(stdVec)  vector < double >;
  %template(stdIntVec)  vector < int >;
  %template(stdMat) vector < vector < double > >;
 }  

