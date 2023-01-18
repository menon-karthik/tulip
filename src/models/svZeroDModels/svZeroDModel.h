#ifndef SVZERODMODEL_H
#define SVZERODMODEL_H

# include <string>
# include <vector>
# include <iostream>
# include <time.h>
# include <stdlib.h>
# include <stdio.h>
# include <fstream>
# include <math.h>
# include <limits>
# include <ctime>

# include "uqTypes.h"
# include "cmModel.h"
# include "cmUtils.h"
# include "cmException.h"
# include "uqConstants.h"
# include "LPNSolverInterface.h"

using namespace std;

class svZeroDModel {
  public:
	// CONSTRUCTOR
	svZeroDModel();
	
    // DESTRUCTOR
	virtual ~svZeroDModel();

	// GET NUMBER OF PARAMETERS
	virtual int getParameterTotal() = 0;

	// GET NUMBER OF PARAMETERS (UKNOWNS)
	virtual int getStateTotal() = 0;

	// GET TOTAL NUMBER OF RESULTS
	virtual int getResultTotal() = 0;

	// GET THE PARAMETER NAMES
	virtual string getParamName(int index) = 0;

	// GET PARAMETER SETS FOR THE LPN MODELS     
	virtual void getDefaultParams(stdVec& zp) = 0;

	// GET THE PARAMETER RANGES FOR THE LPN MODELS
	virtual void getParameterLimits(stdVec& limits) = 0;

    virtual void getDefaultParameterLimits(stdVec& limits);

	// SOLVE MODEL AND GET LOG LIKELIHOOD TO INTERFACE WITH DREAM
	virtual double evalModelError(const stdVec& inputs, stdVec& outputs, stdIntVec& errorCode) = 0;

	// GET RESULT NAME
	virtual string getResultName(int index) = 0;

    // READ CORONARY PARAMETERS FROM FILE
    virtual void readParamsFromFile(stdVec& inputs, std::string param_path) = 0;

    // PRINT OUT RESULTS
    virtual void printResults(int totalResults, double *Xn) = 0;
    
    // Read target data from file
    virtual void readTargetsFromFile(string targetFileName) = 0;

    virtual void getPriorMapping(int priorModelType,int* prPtr) = 0;
        
    virtual void setModelParams(LPNSolverInterface& interface) = 0;

    virtual void setupModel(LPNSolverInterface& interface) = 0;

    virtual void postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;

};

#endif // SVZERODMODEL_H
