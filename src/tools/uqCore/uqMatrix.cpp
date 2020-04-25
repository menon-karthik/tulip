#include <stdio.h>
#include "uqMatrix.h"

// CONSTRUCTOR
uqMatrix::uqMatrix(){
}

uqMatrix::~uqMatrix(){
  
}

// ACCESSS TO VALUES
double uqMatrix::operator()(const int nRow, const int nCol){
  return values[nRow][nCol];
}

// PRINT MATRIX TO FILE
void uqMatrix::printToFile(string outMatFile,bool printTitle){
  FILE *f;
  f = fopen(outMatFile.c_str(),"w");    
  // Print Basis Matrix
  if(printTitle){
    fprintf(f,"Matrix, Rows: %d;Columns: %d\n",rowCount,colCount);	
  }
  for(int loopA=0;loopA<rowCount;loopA++){
    for(int loopB=0;loopB<colCount;loopB++){
      fprintf(f,"%15.6e ",values[loopA][loopB]);  
    }
    fprintf(f,"\n");
  }
  fclose(f);
}

// ===========================
// APPLY TO COEFFICIENT VECTOR
// ===========================
stdVec uqMatrix::applyTo(const stdVec& coeff){
  stdVec result;
  double currVal = 0.0;
  for(int loopA=0;loopA<rowCount;loopA++){
    currVal = 0.0;
    for(int loopB=0;loopB<colCount;loopB++){
      currVal += values[loopA][loopB]*coeff[loopB]; 
    }
    result.push_back(currVal);
  }
  // Return Values
  return result;
}

// ========================
// SET FIRST COLUMN TO ZERO
// ========================
void uqMatrix::setColumnToZero(int colId){
  for(int loopA=0;loopA<rowCount;loopA++){
    values[loopA][colId] = 0.0;
  }
}

// =============
// REMOVE COLUMN
// =============
void uqMatrix::removeColumn(int colId){
  // Remove entry from all rows
  for(int loopA=0;loopA<rowCount;loopA++){
    values[loopA].erase(values[loopA].begin() + colId);
  }
  // Decrease Counter
  colCount--;
}

// ================================
// GET MATRIX AT A CERTAIN LOCATION
// ================================
double uqMatrix::getMatrixAt(int row, int col){
  return values[row][col];
}
