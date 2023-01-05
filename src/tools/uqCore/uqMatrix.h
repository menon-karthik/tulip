#ifndef UQMATRIX_H
#define UQMATRIX_H

# include <vector>
# include <string>
# include "uqTypes.h"

using namespace std;

class uqMatrix{
  protected:
    int rowCount;
    int colCount;
    stdMat values;
  public:
    
    uqMatrix();
    virtual ~uqMatrix();
    
    // GET
    int getRowCount(){return rowCount;}
    int getColCount(){return colCount;}
    stdMat getMatrix(){return values;}
    double getMatrixAt(int row, int col);
    
    // SET
    void setRowCount(int rc){rowCount = rc;}
    void setColCount(int cc){colCount = cc;}
        
    // EASY ACCESS TO VALUES
    double operator()(const int nRow, const int nCol);
    
    // PERFORM MATRIX VECTOR PRODUCT
    stdVec applyTo(const stdVec& coeff);
    
    // SET FIRST COLUMN TO ZERO
    void setColumnToZero(int colId);
    
    // REMOVE COLUMN
    void removeColumn(int colId);
    
    // Print to File
    void printToFile(string outMatFile,bool printTitle);

};

#endif // UQMATRIX_H
