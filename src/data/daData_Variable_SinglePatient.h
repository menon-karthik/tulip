#ifndef DADATA_VARIABLE_SINGLEPATIENT_H
#define DADATA_VARIABLE_SINGLEPATIENT_H

# include <string>
# include <map>
# include <stdio.h>
# include <stdlib.h>
# include "math.h"

# include "uqTypes.h"
# include "daData.h"
# include "daException.h"
# include "cmUtils.h"

using namespace std;

/*! 
Child class to read patient information through a single CSV file.
Only the information from REDCap source is retained. 
*/
class daData_Variable_SinglePatient: public daData{
  protected:    
    // Column in file storing the key
    int keyColumn;
    // Time stamp
    int timeStampColumn;

  public:
    /*! 
    \verbatim embed:rst
    **Purpose**
    Constructor
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] keyColumn the column in the patient CSV file containing the keys identifying the clinical quantity.
    \param[in] timeStampColumn column in the patient CSV file with the measurement timestamp
    */
  	daData_Variable_SinglePatient(int keyColumn,int timeStampColumn);
    virtual void   readFromFile(string filaName);
    virtual double evalOBJ(int dataIndex,stdStringVec keys,stdVec values,stdVec weights);
    virtual double evalLogLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights);
    virtual void   printAndCompare(int dataIndex,stdStringVec keys,stdVec values,stdVec weigths);
    virtual int    getPatientValue(int patientID,string key,double &result);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Return a std vector with the list of keys.
    
    **Discussion**
    None
    
    **Modified**
    Feb 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[out] foundKeys the list of keys available for this patient
    */
    void getAvailableKeys(stdStringVec& foundKeys);

};

#endif // DADATA_VARIABLE_SINGLEPATIENT_H
