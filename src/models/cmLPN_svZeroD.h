#ifndef CMLPNSVZEROD_H
#define CMLPNSVZEROD_H

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
# include "svZeroDModel.h"

using namespace std;

class cmLPN_svZeroD: public cmModel {

   
  public:

    // DEFAULT CONSTRUCTOR
    cmLPN_svZeroD(std::string model_path, svZeroDModel* model, std::string interface_lib, bool custom_error_eval = false);
    
    // DESTRUCTOR
    virtual ~cmLPN_svZeroD();

    // GET NUMBER OF PARAMETERS
    virtual int getParameterTotal();

    // GET NUMBER OF PARAMETERS (UKNOWNS)
    virtual int getStateTotal();
  
    // RETURN THE NUMBER OF EXTRA OUTPUTS
    virtual int  getAuxStateTotal();

    // GET TOTAL NUMBER OF RESULTS
    virtual int getResultTotal();

    // GET THE PARAMETER NAMES
    virtual string getParamName(int index);
	
    // GET RESULT NAME
    virtual string getResultName(int index);

    // GET PARAMETER SETS FOR THE LPN MODELS     
    virtual void getDefaultParams(stdVec& zp);

    // GET THE PARAMETER RANGES FOR THE LPN MODELS
    virtual void getParameterLimits(stdVec& limits);

    // GET THE DEAFULT PARAMETER RANGES FOR THE LPN MODELS
    virtual void getDefaultParameterLimits(stdVec& limits);

    // PRINT OUT RESULTS
    virtual void printResults(const int totalResults, const stdVec results);

    // SETUP, RUN AND POSTPROCESS THE ZEROD MODEL
    int  solveLPN(const stdVec& params, stdVec& results);

    // SOLVE MODEL AND GET LOG LIKELIHOOD TO INTERFACE WITH DREAM
    virtual double evalModelError(const stdVec& inputs, stdVec& outputs, stdIntVec& errorCode);
    
    virtual void getPriorMapping(int priorModelType,int* prPtr);

  protected:

    LPNSolverInterface interface;
    svZeroDModel* zeroDmodel;
    std::vector<double> init_state_y, init_state_ydot;
    bool custom_error_eval= false;
};

#endif // CMLPNSVZEROD_H
