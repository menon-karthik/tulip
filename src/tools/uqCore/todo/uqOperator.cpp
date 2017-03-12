#include "uqOperator.h"

// =============================
// PUT NORM OF COLUMNS IN VECTOR
// =============================
uqOperator::uqOperator(int rows, int cols, uqInterpolator interp){
}

// =============================
// PUT NORM OF COLUMNS IN VECTOR
// =============================
double* uqOperator::getColumnNorms(){
  double* result = new double(rowCount);
  for(int loopA=0;loopA<colCount;loopA++){
    result[loopA] = 0.0;
    for(int loopB=0;loopB<rowCount;loopB++){
      result[loopA] += values[loopB][loopA]*values[loopB][loopA];
    }
    // Normalize
    result[loopA] = sqrt(result[loopA]);
  }
  return result;
}

// ================================
// NORMALIZE TO UNIT 2-NORM COLUMNS
// ================================
void uqOperator::convertToUnitaryColums(){

}

// =============================
// OVERLOAD PARENTHESIS OPERATOR
// =============================
double uqOperator::operator()(const int nRow, const int nCol){
  return values[nRow][nCol];
}

