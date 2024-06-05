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
    virtual double evalOBJ(const stdStringVec& keys,const stdVec& values,const stdVec& weights);
    virtual double evalLogLikelihood(const stdStringVec& keys,const stdVec& avValues,const stdVec& stdFactors,const stdVec& weights);
    virtual void   printAndCompare(const stdStringVec& keys,const stdVec& values,const stdVec& weights);
    virtual int    getPatientValue(string key,double& result);
    virtual void   printToScreen();
    virtual void   getTable(stdMat& table);

    void getIndexSet(stdIntVec& indexSet);

    void assignFromLabelsAndMat(const stdStringVec& labels,const stdMat& values, bool useMean);
    void overwriteStandardDeviations(const stdStringVec& labels,const stdVec& stds);
    virtual double evalLogLikelihood_cov(const stdVec& computed);
    
};

#endif // DADATA_MULTIPLE_TABLE_H
