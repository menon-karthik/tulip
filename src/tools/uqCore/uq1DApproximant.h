#ifndef UQ1DAPPROXIMANT_H
#define UQ1DAPPROXIMANT_H

# include "uqBasis.h"
# include "uqTypes.h"

using namespace std;

// General class for one-dimensional approximants
class uq1DApproximant{
  public:
    
    uq1DApproximant();
    virtual ~uq1DApproximant();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Pointwise evaluation of the 1D approximant
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] XValue the X value where to evaluate the approximant
    \return The value of the approximant at the XValue location
    */
    virtual double evaluate(double XValue) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Export approximant to text file
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] fileName the name of the file storing the approximant
    \param[in] append append approximant to file
    */
    virtual void exportToTextFile(string fileName, bool append = false) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Import approximant to file
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] fileName the name of the file where to read the approximant
    \param[in] startFromTop
    \param[in] startLine
    \return an integer error code, if 0 the approximant is correctly imported
    */
    virtual int  importFromTextFile(string fileName, bool startFromTop = true,int startLine = 0) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Normalize the approximant by dividing its coefficients by a real number
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] normValue a real number used as a normalization constant
    */
    virtual void normalizeByConstant(double normValue) = 0;
    
    /*! 
    \verbatim embed:rst
    **Purpose**
    Normalize the approximant by dividing its coefficients by a real number
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[out] result min and max real values defining the support of 
                a local approximant
    */
    virtual void getLimits(stdVec& limits) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Evaluate the approximant multiple times
    
    **Discussion**
    None
    
    **Modified**
    2017 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] XValue a std vector containing the x locations 
    \param[out] results the approximant evaluated at the XValue locations
    */    
    void evaluate(const stdVec& XValue,stdVec& results);
};

#endif // UQ1DAPPROXIMANT_H
