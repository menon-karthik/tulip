#ifndef DADATA_MULTIPLE_TABLE_H
#define DADATA_MULTIPLE_TABLE_H

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
Child class to read patient information for multiple patients in the same CSV file.
The first column contains the key of the clinical measurement.
The second column contains a description of the clinical quantity identifying the row.
The other column contain the patient data (one patient for each column).
Missing data are identified using the "none" string and discarded.
*/

class daData_multiple_Table: public daData{
  public:
  	//! Default constructor
  	daData_multiple_Table();
  	virtual ~daData_multiple_Table();
  	
    virtual void readFromFile(string fileName);
    virtual double evalOBJ(int dataIndex,stdStringVec keys,stdVec values,stdVec weights);
    virtual double evalLogLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights);
    virtual double evalLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights);
    virtual void printAndCompare(int dataIndex,stdStringVec keys,stdVec values,stdVec weigths);
    virtual int getPatientValue(int patientID,string key,double &result);
    
};

#endif // DADATA_MULTIPLE_TABLE_H