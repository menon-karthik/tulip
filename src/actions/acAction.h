#ifndef ACACTION_H
#define ACACTION_H

# include "cmModel.h"

/*! 
Abstract parent class to perform actions (operations on models).
An action can be, for example, optimization, estimation, uncertainty propagation, etc.
*/
class acAction{
  protected:
    
    //! Pointer to the computational model 
    cmModel* model;

  public:

  	//! Default Constructor
  	acAction();
    //! Virtual Destructor
    ~acAction();

  	/*! 
    \verbatim embed:rst
    **Purpose**
    Set the computational model that acAction should process.
    
    **Discussion**
    Generic actions can call the cmModel::evalModelError function to interrogate the model.
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] currModel pointer to a computational model derived from cmModel.
    */
    void setModel(cmModel* currModel){model = currModel;};

  	/*! 
    \verbatim embed:rst
    **Purpose**
    Perform action
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \return error errorcode from execution.
    */
  	virtual int go() = 0;
};

#endif //ACACTION_H
