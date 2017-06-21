#ifndef DADATA_H
#define DADATA_H

# include <string>
# include <map>
# include <sstream>
# include "math.h"

# include "uqTypes.h"
# include "daException.h"

using namespace std;

/*! 
Abstract parent class to read data from various sources 
and evaluate objective function/likelihood
*/
class daData{
  protected:
    /*! Contains the DATA DICTIONARY.
        Data is stored as a two key dictionary 
        with a pair = (key, array of strings).
    */
    dataMap dict;

  public:
  	//! Empty data constructor
  	daData();

    //! virtual distructor
    virtual ~daData();

    /*! 
    \verbatim embed:rst
    **Purpose**
    Pure virtual function to read data from file
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] fileName a string with the name of the file to read
    */
    virtual void readFromFile(string fileName) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Pure virtual function to evaluated the Objective Function
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES

    **Reference**
    No Reference    
    \endverbatim
    \param dataIndex inputs the column identifying the patient in a multi-patient dataset, not used with single patient datasets
    \param keys inputs a list of quantities evaluated from the model
    \param values inputs values of the model results associated with the vector keys
    \param weights inputs weights that multiply the measurement variance for each quantity
    */
    virtual double evalOBJ(int dataIndex,stdStringVec keys,stdVec values,stdVec weights) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Pure virtual function to evaluated the log-likelihood
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference    
    \endverbatim
    \param dataIndex inputs the column identifying the patient in a multi-patient dataset, not used with single patient datasets
    \param keys inputs a list of quantities evaluated from the model
    \param avValues inputs values of the model results associated with the vector keys
    \param sdValues inputs ratio between standard deviation and average value for all keys
    \param weights inputs weights that multiply the measurement variance for each quantity
    \return value of the log-likelihood at the current parameter realization
    */
    virtual double evalLogLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights) = 0;

    /*! 
    \verbatim embed:rst
    **Purpose**
    Pure virtual function to evaluated a Gaussian likelihood
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference    
    \endverbatim
    \param dataIndex inputs the column identifying the patient in a multi-patient dataset, not used with single patient datasets
    \param keys inputs a list of quantities evaluated from the model
    \param avValues inputs values of the model results associated with the vector keys
    \param sdValues inputs ratio between standard deviation and average value for all keys
    \param weights inputs weights that multiply the measurement variance for each quantity
    \return value of the Gaussian likelihood at the current parameter realization
    */
    virtual double evalLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights) = 0;    

    /*!
    \verbatim embed:rst 
    **Purpose**
    Print and compare the clinical data and model results for the current patient
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference    
    \endverbatim
    \param dataIndex inputs the column identifying the patient in a multi-patient dataset, not used with single patient datasets
    \param keys inputs a list of quantities evaluated from the model
    \param values inputs values of the model results associated with the vector keys
    \param weights inputs weights that multiply the measurement variance for each quantity
    \return value of the log-likelihood at the current parameter realization
    */
    virtual void printAndCompare(int dataIndex,stdStringVec keys,stdVec values,stdVec weigths) = 0;

    /*! 
    \verbatim embed:rst 
    **Purpose**
    Get patient related quantity for a selected key
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param patientID inputs the column identifying the patient in a multi-patient dataset, not used with single patient datasets
    \param key inputs a list of quantities evaluated from the model
    \param result outputs values of the model results associated with the vector keys
    \return error outputs error code 0-OK, 1-Error
    */
    virtual int getPatientValue(int patientID,string key,double &result) = 0;

    // ===========================
    // PRINTING DICTIONARY CONTENT
    // ===========================

    /*! 
    \verbatim embed:rst 
    **Purpose**
    Print data to stdout
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference    
    \endverbatim
    \param[in] patientIndex the index of the patient column for files supporting multiple patients
    */
    void printToScreen(int patientIndex);


    // ========================
    // DIRECT DICTIONARY ACCESS
    // ========================

    /*! 
    \verbatim embed:rst 
    **Purpose**
    Clear data dictionary
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    */
    void clear();

    /*! 
    \verbatim embed:rst 
    **Purpose**
    Extract keys according to an externally provided list. 
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] refKeys Provided list of keys. Only the keys both in the dictionary and in this list should be extracted.
    \param[in] unUsedKeys Specifies which of the refKeys should be considered. 
    \param[out] outKeys List of extracted keys.
    \param[out] outValues List of extracted values corresponding to keys.
    */    
    void extractKeyValues(const stdStringVec& refKeys,const stdBoolVec& unUsedKeys,stdStringVec& outKeys,stdVec& outValues);

    /*! 
    \verbatim embed:rst 
    **Purpose**
    Add (key,value) pair to data dictionary.
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] key String key to add.
    \param[in] value Double value to add.
    */    
    void addKeyValue(string key,double value);

    /*!
    \verbatim embed:rst  
    **Purpose**
    Remove (key,value) pair from data dictionary.
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim
    \param[in] key String key to be removed.
    */    
    void removeKeyValue(string key);

};

#endif //DADATA
