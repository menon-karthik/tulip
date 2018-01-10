#ifndef CMPDF_KNOWNPDF_H
#define CMPDF_KNOWNPDF_H

# include <string>
# include <stdio.h>
# include "stdlib.h"
# include "math.h"

# include "uqTypes.h"
# include "cmModel.h"
# include "cmException.h"
# include "cmUtils.h"

// Enum with PDF Types
enum PDFTypes{
  stdGaussianPDF, bimodalPDF, uniformPDF, multimodalPDF
};

// GENERIC CLASS FOR SAMPLES
class cmPDFKnownPDF: public cmModel{
  protected:
    //! Type of PDF
    int PDFType;

  public:

  	/*! 
    \verbatim embed:rst
    **Purpose**
    Constructor
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] PDFType type of analytical PDF
    */
  	cmPDFKnownPDF(int PDFType);

    // Functions that need implementation
    virtual int    getParameterTotal();
    virtual int    getResultTotal();
    virtual void   getDefaultParams(stdVec& params);
    virtual void   getParameterLimits(stdVec& limits);    
    virtual string getParamName(int parID){return string("");}
    virtual string getResultName(int resID){return string("");}
    virtual int    getStateTotal(){return 0;}
    virtual int    getAuxStateTotal(){return 0;}
    virtual void   getPriorMapping(int priorModelType,int* prPtr){}
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
    
};

#endif // CMPDF_KNOWNPDF_H
