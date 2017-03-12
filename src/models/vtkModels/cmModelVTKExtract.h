#ifndef CMMODELVTKEXTRACT_H
#define CMMODELVTKEXTRACT_H

# include <string>
# include <vtkSmartPointer.h>
# include <vtkPoints.h>
# include <vtkPolyData.h>
# include <vtkDataArray.h>
# include <vtkDoubleArray.h>
# include <vtkPointData.h>
# include <vtkXMLPolyDataReader.h>
# include <vtkXMLPolyDataWriter.h>
# include <vtkVectorNorm.h>

# include <iostream>
# include <fstream>

# include "cmModel.h"
# include "cmException.h"
# include "lpnUtils.h"
# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR SAMPLES
class cmModelVTKExtract: public cmModel{
  protected:
    string vtkFileName;
    int fileIDStart;
    int fileIDEnd;
    int fileResultType;
    // Store All Values from VTK Files
    stdMat nodeValsTable;
  public:
  	// CONSTRUCTOR
  	cmModelVTKExtract(string fileName, int fileStart, int fileEnd, int fileResultType);

    // Get Total Nodes
    int getTotalVTKNodes(){return (int)nodeValsTable[0].size();};
    void applyNodeStatistics(stdMat& nodeStats);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int getParameterTotal();
    virtual string getParamName(int parID);
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual string getResultName(int resID);
  	virtual void getParameterLimits(stdVec& limits);
  	virtual void getDefaultParams(stdVec& params);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);

    // EVAL MODEL 
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode);
};

#endif //CMMODELVTKEXTRACT_H
