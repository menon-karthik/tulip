#ifndef UQMULTIINDEX_H
#define UQMULTIINDEX_H

# include <string>

# include "uqTypes.h"
# include "uqConstants.h"
# include "polynomial.h"

using namespace std;

class uqMultiIndex{
  protected:
    int basis_num;
    stdIntMat multiIndex;
    // MEMBER FUNCTION
    void generateMultiIndex_Partial(int dim, int order);
    void generateMultiIndex_Full(int dim, int order);
  public:
    
    // CONSTRUCTOR
    uqMultiIndex(int dim, int order, int miType);

    // GET NUMBER OF BASIS
    int getTotalBasis();

    // ACCESS TO MEMBERS
    double operator()(const int nRow, const int nCol);
    double getMultiIndexAt(const int nRow, const int nCol);
    void   getMultiIndex(stdIntMat& res);

    // CHECK IF A CERTAIN COMPONENT OF MULTI-INDEX CONTAINS A VARIABLE
    bool contains(int variable,int component);
    bool containsOnly(int variable, int component);

    // PRINT TO FILE
    void printToFile(string outMatFile,bool printTitle);
};

#endif // UQMULTIINDEX_H
