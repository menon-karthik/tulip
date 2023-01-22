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

    // SET UP MODEL PARAMETERS
    virtual void setupModel(LPNSolverInterface& interface) = 0;

    // GET NUMBER OF PARAMETERS
    virtual int getParameterTotal() = 0;

    // GET NUMBER OF PARAMETERS (UKNOWNS)
    virtual int getStateTotal() = 0;
  
    // RETURN THE NUMBER OF EXTRA OUTPUTS
    virtual int  getAuxStateTotal() = 0;

    // GET TOTAL NUMBER OF RESULTS
    virtual int getResultTotal() = 0;

    // GET THE PARAMETER NAMES
    virtual string getParamName(int index) = 0;

    // GET RESULT NAME
    virtual string getResultName(int index) = 0;

    // GET PARAMETER SETS FOR THE LPN MODELS     
    virtual void getDefaultParams(stdVec& zp) = 0;

    // GET THE PARAMETER RANGES FOR THE LPN MODELS
    virtual void getParameterLimits(stdVec& limits) = 0;

    // GET THE DEFAULT PARAMETER RANGES FOR THE LPN MODELS (NOT USED CURRENTLY)
    virtual void getDefaultParameterLimits(stdVec& limits) = 0;

    virtual void getPriorMapping(int priorModelType,int* prPtr) = 0;
        
    // UPDATE ZEROD MODEL PARAMETERS
    virtual void setModelParams(LPNSolverInterface& interface, const stdVec& params) = 0;

    // POSTPROCESS ZEROD SIMULATION
    virtual void postProcess(LPNSolverInterface& interface, const stdVec& t, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;

    // KEY/NAME FOR EACH TARGET QUANTITY
    virtual void getResultKeys(vector<string>& keys) = 0;

    // STANDARD DEVIATION OF EACH TARGET MEASUREMENT
    virtual void getDataStd(stdVec& stdFactors) = 0;

    // INVERSE WEIGHT OF EACH TARGET QUANTITY IN LOG LIKELIHOOD
    virtual void getResultWeights(stdVec& weights) = 0;

};

#endif // SVZERODMODEL_H
