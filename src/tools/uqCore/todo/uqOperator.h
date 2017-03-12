#ifndef UQOPERATOR_H
#define UQOPERATOR_H

# include <vector>

# include "uqTypes.h"

using namespace std;

class uqOperator{
  public:
    // DATA MEMBERS
    int rowCount;
    int colCount;
    stdMat values;
    // CONSTRUCTOR AND DISTRUCTOR
    uqOperator();
    // GETTER AND SETTERS
    int getRowCount(){return rowCount;}
    int getColCount(){return colCount;}
    // MEMBER FUNCTIONS
    double* getColumnNorms();
    void convertToUnitaryColums();
    // OPERATOR
    double operator()(const int nRow, const int nCol);
};

#endif // UQOPERATOR_H
