# include <stdio.h>

# include "uqPolyMatrix.h"
# include "uqSamples.h"
# include "uqTypes.h"
# include "uqPolyBasis.h"
# include "uqMultiIndex.h"

// MATRIX CONSTRUCTOR
uqPolyMatrix::uqPolyMatrix(uqSamples xValues, int maxOrder, int polyType, int completeOrderType){

  // Get Dimensions and Number of Samples from xValues
  int totSamples = xValues.getTotSamples();	
  int totDims = xValues.getTotDims();

  // Generate the multi-index
  uqMultiIndex mi(totDims,maxOrder,completeOrderType);
  int num_basis = mi.getTotalBasis();

  // Assign Total Number of Rows and Columns
  rowCount = totSamples;
  colCount = num_basis;

  // Resize Matrix to accommodate entries
  values.resize(rowCount);
  for(int loopA=0;loopA<rowCount;loopA++){
    values[loopA].resize(colCount);
  }

  // Create polynomial interpolator
  uqPolyBasis* myInterp = new uqPolyBasis(polyType);

  // Fill Values Entries
  double xValue = 0.0;
  int currPolyOrder = 0;
  double currBaseValue = 0.0;
  for(int loopA=0;loopA<rowCount;loopA++){
    for(int loopB=0;loopB<colCount;loopB++){
      values[loopA][loopB] = 1.0;
      for(int loopC=0;loopC<totDims;loopC++){
        // Copy X Value
        xValue = xValues(loopA,loopC);        
        // Eval Order from multi-index
        currPolyOrder = mi(loopB,loopC);
        // Eval function Value
        currBaseValue = myInterp->eval(xValue,currPolyOrder);
        // Fill Constraint Mat
        values[loopA][loopB] *= currBaseValue;
      }
    }
  }
}
