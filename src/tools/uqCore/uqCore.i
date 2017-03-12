%module uqcore
 
 %{
 /* Includes the header in the wrapper code */
 #include "uq1DQuadratureRule.h"
 #include "uqAlgorithm.h"
 #include "uqAlgorithmOMP.h"
 #include "uqClenshawCurtis1DQuadratureRule.h"
 #include "uqConstants.h"
 #include "uqException.h"
 #include "uqInterpolator.h"
 #include "uqMatrix.h"
 #include "uqMultiIndex.h"
 #include "uqMWInterpolator.h"
 #include "uqMWTesting.h"
 #include "uqOperator.h"
 #include "uqOptions.h"
 #include "uqOrthoPolyInterpolator.h"
 #include "uqPolyMatrix.h"
 #include "uqPDF.h"
 #include "uqPolyInterpolator.h"
 #include "uqTypes.h"
 #include "uqUtils.h"
 #include "uqVector.h"
 %}
 
 /* Parse the header file to generate wrappers */
 %include "uq1DQuadratureRule.h"
 %include "uqAlgorithm.h"
 %include "uqAlgorithmOMP.h"
 %include "uqClenshawCurtis1DQuadratureRule.h"
 %include "uqConstants.h"
 %include "uqException.h"
 %include "uqInterpolator.h"
 %include "uqMatrix.h"
 %include "uqMultiIndex.h"
 %include "uqMWInterpolator.h"
 %include "uqMWTesting.h"
 %include "uqOperator.h"
 %include "uqOptions.h"
 %include "uqOrthoPolyInterpolator.h"
 %include "uqPolyMatrix.h"
 %include "uqPDF.h"
 %include "uqPolyInterpolator.h"
 %include "uqTypes.h"
 %include "uqUtils.h"
 %include "uqVector.h"
 
 