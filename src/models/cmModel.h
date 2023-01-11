#ifndef CMMODEL_H
#define CMMODEL_H

# include <string>
# include <iostream>
# include <stdio.h>

# include "cmException.h"
# include "daData.h"
# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmModel{
  public:
    //! Data Object containing patient data
    daData* data;
    //! List of frozen parameters IDs
    stdIntVec frozenParamIDX;
    //! List of frozen parameters values
    stdVec    frozenParamVAL;
    //! Level of printing for model debug information
    int printLevel;
    //! Limits set by users
    stdVec userLimits;

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
    Gather the default parameter limits
    
    **Discussion**
    None
    
    **Modified**
    2015 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[out] params std vector of size 2*getParameterTotal() containing the default parameter limits
    */
    virtual void getDefaultParameterLimits(stdVec& limits) = 0;

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
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode) = 0;

    // SET DATA OBJECT
    void setData(daData* locData);

    // EVAL MULTIPLE MODELS
    stdVec evalModelError(const stdMat& inputs,stdMat& outputs, stdIntVec &errorCode);

    // FREEZE MODEL PARAMETERS
    void freezeModelParameters(const stdIntVec& localParamIDX,const stdVec& localParamVal);

    // GET PARAMETER LIMITS
    virtual void getParameterLimits(stdVec& limits);

    // GET PARAMETER LIMITS
    void setParameterLimits(const stdVec& limits);
};

#endif //CMMODEL_H
