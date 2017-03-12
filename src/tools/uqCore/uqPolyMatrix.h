#ifndef UQPOLYMATRIX_H
#define UQPOLYMATRIX_H

# include "uqMatrix.h"
# include "uqSamples.h"
# include "uqTypes.h"
# include "uqPolyBasis.h"

class uqPolyMatrix: public uqMatrix{
  public:
    // CONSTUCTOR
    uqPolyMatrix(uqSamples xValues, int maxOrder, int polyType, int completeOrderType);
};

#endif // UQPOLYMATRIX_H
