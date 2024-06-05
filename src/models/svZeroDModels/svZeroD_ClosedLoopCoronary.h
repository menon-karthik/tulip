#ifndef SVZERODCLOSEDLOOPCORONARY_H
#define SVZERODCLOSEDLOOPCORONARY_H

# include "svZeroDModel.h"

using namespace std;

// DEFINE CONVERSION CONSTANT
const double pConv = 1.334E3;
const double qConv = 1E0;

class svZeroD_ClosedLoopCoronary: public svZeroDModel {
   
  public:
	
    // DEFAULT CONSTRUCTOR
    svZeroD_ClosedLoopCoronary();

    // SET UP MODEL PARAMETERS
    virtual void setupModel(LPNSolverInterface& interface);

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

    // GET THE DEFAULT PARAMETER RANGES FOR THE LPN MODELS (NOT USED CURRENTLY)
    virtual void getDefaultParameterLimits(stdVec& limits);

    virtual void getPriorMapping(int priorModelType,int* prPtr);

    // UPDATE ZEROD MODEL PARAMETERS
    virtual void setModelParams(LPNSolverInterface& interface, stdVec params);

    // POSTPROCESS ZEROD SIMULATION
    virtual void postProcess(LPNSolverInterface& interface, const stdVec& t, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);

    // KEY/NAME FOR EACH TARGET QUANTITY
    virtual void getResultKeys(vector<string>& keys);

    // STANDARD DEVIATION OF EACH TARGET MEASUREMENT
    virtual void getDataStd(stdVec& stdFactors);

    // INVERSE WEIGHT OF EACH TARGET QUANTITY IN LOG LIKELIHOOD
    virtual void getResultWeights(stdVec& weights);
    
    // RETURN PARAMETER SPECIFIED BY STRING SPECIFIER
    virtual void getSpecifiedParameter(string& specifier, double& return_db_param, int& return_int_param, std::vector<double>& return_vector);

    // CUSTOM ERROR EVALUATION FOR SPECIFIC MODELS
    virtual double evalModelError(std::vector<double>& results);

    // CUSTOM DATA OBJECT FOR SPECIFIC MODELS
    virtual daData* createCustomData();

  protected:

    int num_blocks;
    bool use_CCO;
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
    int n_corBC;
    int n_corBC_l;
    int n_corBC_r;
    int n_cor3d_l;
    int n_cor3d_r;
    int n_RCR;
    // Names of coronary BC blocks
    std::vector<std::string> names_corBC_l, names_corBC_r;
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
    bool useRigidSurrogate = false;
    // Target data needed for setting initial conditions
    double targetESV;
    double targetEDV;
    double targetPaoMin;
    double targetPaoMax;
};

#endif // SVZERODCLOSEDLOOPCORONARY_H
