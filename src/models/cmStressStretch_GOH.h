#ifndef CMMODEL_H
#define CMMODEL_H

# include <string>

# include "uqTypes.h"
# include "daData.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmModel{
  public:
    //! Data Object containing patient data
    daData* data;
    //! dataset column identifying the specific patient for multiple patients datasets
    int datasetColumn;
    //! List of frozen parameters IDs
    stdIntVec frozenParamIDX;
    //! List of frozen parameters values
    stdVec    frozenParamVAL;

  	//! Default constructor
  	cmModel();
    //! Virtual destructor
    virtual ~cmModel();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Get the total number of parameters in model
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    virtual int getParameterTotal() = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Get the total number of state variables in the model
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    virtual int getStateTotal() = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Get the total number of results computed by the instantiated model
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    virtual int getResultTotal() = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Returns an array with the parameter upper and lower bounds
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[out] limits std vector of size 2*getParameterTotal() with the upper and lower bounds.
    The parameter with ID i has lower bound equal to limits[2*i] and upper bound equal to limits[2*i+1]
    */
  	virtual void getParameterLimits(stdVec& limits) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Gather the default model parameters
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[out] params std vector of size getParameterTotal() containing the default model parameters
    */
  	virtual void getDefaultParams(stdVec& params) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    This function is used when prior marginal probabilities are red from file. 
    The marginal moments (typically average and standard deviation for Gaussian marginals) are
    written to a single line of a text file. 
    This function provides the mapping between the lines of the text file and the model parameter 
    these are referred to. 
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] priorModelType the type of prior mapping to be used.
    \param[out] prPtr an integer pointer where prPtr[i] contains the line in the prior text file 
    that stores the marginal moments of the parameter with ID i.
    */
  	virtual void getPriorMapping(int priorModelType,int* prPtr) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Return the name of the parameter from its ID. This function is useful to print tables with parameter values.
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] parID the ID of the parameter of interest.
    */
    virtual string getParamName(int parID) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Return the name of the result with value in outputs[resID].
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] resID the ID of the result of interest.
    */
    virtual string getResultName(int resID) = 0;


    /*! 
    \verbatim embed:rst
    **Purpose**
    Solve the model.
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] inputs the ID of the result of interest.
    \param[out] outputs the ID of the result of interest.
    \param[out] errorCode the ID of the result of interest.
    \return 
    */
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode) = 0;

    // SET DATA OBJECT
    void setData(daData* locData,int setIDX);

    // EVAL MULTIPLE MODELS
    stdVec evalModelError(stdMat inputs,stdMat& outputs, stdIntVec &errorCode);

    // FREEZE MODEL PARAMETERS
    void freezeModelParameters(stdIntVec localParamIDX,stdVec localParamVal);
};

#endif //CMMODEL_H
