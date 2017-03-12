# ifndef UQMWMATRIX_H
# define UQMWMATRIX_H

# include <stdio.h>
# include <stdlib.h> 
# include <math.h>

# include "uqMatrix.h"
# include "uqTypes.h"
# include "uqSamples.h"
# include "uqPolyBasis.h"
# include "uqMWBasis.h"
# include "uqMultiIndex.h"
# include "uqException.h"
# include "uqConstants.h"

// MATRIX TYPE CONSTANTS
const int kMWFixedMaxDetailLevel = 0;
const int kMWFixedColumns = 1;

// MW MATRIX CLASS
class uqMWMatrix: public uqMatrix{
  public:
    // Arrays with Basis Characteristics
    stdIntVec  scaleIdx;
    stdIntVec  shiftIdx;
    stdIntVec  multiIdx;
    stdIntVec  orderIdx;
    stdBoolVec isMW;
    stdIntMat  multiIndex;
    int        num_basis;
    
    // CONSTUCTOR
    uqMWMatrix(int maxOrder,uqSamples* XValues,
               bool addLegendrePoly, bool addMW, bool useBinaryPartitions,
               int mwMatType, bool mwMatIncludeNullColumns,
               bool useExactMW, int mwQuadOrder, stdMat measure,
               int &maxColumns, int &maxDetailLevel);
    
    // DESTRUCTOR
    ~uqMWMatrix();

};

# endif // UQMWMATRIX_H