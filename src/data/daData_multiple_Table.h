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
The other columns contain the patient data (one patient for each column).
Missing data are identified using the "none" string and discarded.
*/

class daData_multiple_Table: public daData{
  public:
    
    //! Data Members
    bool useSingleColumn;
    int  columnID;

  	//! Default constructor
  	daData_multiple_Table(bool useSingleColumn,int columnID);
  	virtual ~daData_multiple_Table();
  	
    virtual void   readFromFile(string fileName);
    virtual double evalOBJ(stdStringVec keys,stdVec values,stdVec weights);
    virtual double evalLogLikelihood(stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights);
    virtual void   printAndCompare(stdStringVec keys,stdVec values,stdVec weigths);
    virtual int    getPatientValue(string key,double &result);
    virtual void   printToScreen();
    virtual void   getTable(stdMat& table);

    void getIndexSet(stdIntVec& indexSet);
    
};

#endif // DADATA_MULTIPLE_TABLE_H
