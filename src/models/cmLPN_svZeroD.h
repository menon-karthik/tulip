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

// DEFINE CONVERSION CONSTANT
const double pConv = 1.334E3;
const double qConv = 1E0;

class cmLPN_svZeroD: public cmModel {

   
  public:

    // svZeroD model object
    svZeroDModel* model;

	//DEFAULT CONSTRUCTOR
	cmLPN_svZeroD(std::string model_path);

	// GET NUMBER OF PARAMETERS
	virtual int getParameterTotal();

	// GET NUMBER OF PARAMETERS (UKNOWNS)
	virtual int getStateTotal();

	// GET TOTAL NUMBER OF RESULTS
	virtual int getResultTotal();
  
    // RETURN THE NUMBER OF EXTRA OUTPUTS
    virtual int  getAuxStateTotal();

	// GET THE PARAMETER NAMES
	virtual string getParamName(int index);

	// GET PARAMETER SETS FOR THE LPN MODELS     
	virtual void getDefaultParams(stdVec& zp);

	// GET THE PARAMETER RANGES FOR THE LPN MODELS
	virtual void getParameterLimits(stdVec& limits);

    virtual void getDefaultParameterLimits(stdVec& limits);

	// SOLVE MODEL AND GET LOG LIKELIHOOD TO INTERFACE WITH DREAM
	virtual double evalModelError(const stdVec& inputs, stdVec& outputs, stdIntVec& errorCode);

	// GET RESULT NAME
	string getResultName(int index);

    // READ CORONARY PARAMETERS FROM FILE
    virtual void readParamsFromFile(stdVec& inputs, std::string param_path);

    // PRINT OUT RESULTS
    virtual void printResults(int totalResults, double *Xn);
    
    // Read target data from file
    virtual void readTargetsFromFile(string targetFileName);

    virtual void getPriorMapping(int priorModelType,int* prPtr);

  protected:

        LPNSolverInterface interface;
        int num_blocks;
        std::vector<double> init_state_y, init_state_ydot;
        std::vector<double> coronary_params;
        std::vector<double> rcr_params;
        std::vector<double> heart_params;
        std::vector<double> vessel_params;
        std::vector<int> Q_rcr_ids;
        std::vector<int> Q_lca_ids;
        std::vector<int> Q_rca_ids;
        int Q_aorta_id = -1, P_aorta_id = -1, P_pul_id = -1, Q_LV_id = -1, Q_LA_id = -1, Q_RV_id = -1, V_LV_id = -1, P_RA_id = -1, P_RV_id = -1;

  	int numCycles;
  	int total3DSteps;
  	int total0DSteps;
        // Number of outlets and unknowns
        int nUnknowns;
        int nFaces;
        int nCOR;
        int nCOR_l;
        int nCOR_r;
        int nRCR;
        // Base vectors to store raw output from reading mesh data
        std::vector<double> Ra_l_base;
        std::vector<double> Ra_r_base;
        std::vector<double> Ram_l_base;
        std::vector<double> Ram_r_base;
        std::vector<double> Rv_l_base;
        std::vector<double> Rv_r_base;
        std::vector<double> Cim_l_base;
        std::vector<double> Ca_l_base;
        std::vector<double> Cim_r_base;
        std::vector<double> Ca_r_base;
        std::vector<double> Rp_rcr_base;
        std::vector<double> Rd_rcr_base;
        std::vector<double> C_rcr_base;
        double Rrv_base;
        double Rlv_base;
        double Rpd_base;
        // Flag which determines which surrogate model to run
        bool useRigidSurrogate;
        
        // Target data needed for setting initial conditions
        double targetESV;
        double targetEDV;
        double targetPaoMin;
        double targetPaoMax;

        // Assign parameter values from input
        //void assignParameters(double* params);
        int  solveCoronaryLPN(double* params, double* results);
};

#endif // CMLPNSVZEROD_H
