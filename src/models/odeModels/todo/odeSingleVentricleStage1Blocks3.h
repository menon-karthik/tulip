#ifndef ODESINGLEVENTRICLESTAGE1BLOCKS3_H
#define ODESINGLEVENTRICLESTAGE1BLOCKS3_H

# include "odeModel.h"

using namespace std;

// Numerical Constants
const double pi = 3.1415926535897;

// Output State Constants
const int ipVsv   = 0;
const int ipVsa   = 1;
const int ipPAo   = 2;
const int ipQav   = 3;
const int ipPubv  = 4;
const int ipQuba  = 5;
const int ipPuba  = 6;
const int ipPlung = 7;
const int ipPvp   = 8;
const int ipQapol = 9;
const int ipPpa   = 10;
const int ipQlba  = 11;
const int ipPlba  = 12;
const int ipPlbv  = 13;

// Auxiliary Output Constants
const int ipPsv        = 0;
const int ipPsa        = 1;
const int ipQAo        = 2;
const int ipQvp        = 3;
const int iptc         = 5;
const int ipAA         = 6;
const int ipAV         = 7;
const int ipQsvc       = 8;
const int ipQsh        = 10;
const int ipQaftCAo    = 11;
const int ipQivc       = 12;
const int ipPsaactive  = 13;
const int ipPsapassive = 14;
const int ipPsvactive  = 15;
const int ipPsvpassive = 16;
const int ipQreg       = 17;

// MODEL CONSTANTS
const double PRIMARY_CURVEPEAK_WEIGHT = 12.0;
const double SECONDARY_CURVEPEAK_WEIGHT = 18.0;
const double DATA_UNCERTAINTY = 0.05;
const bool   USE_PEAKTIME_ASTARGETS = false;
const bool   EXCLUDE_INVASIVE_TARG = false;

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleStage1Blocks3: public odeModel{
  protected:
    // Data Members
    double proxResistance;
    double capacitance;
    double distResistance;
    double iniPressure;
    // Distal Pressure time history
    stdMat distPressure;  
  public:
  	// CONSTRUCTOR
  	odeSingleVentricleStage1Blocks3();

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual void   eval(double t,const stdVec& Xk,const stdVec& params,stdVec& DXk,stdVec& auxOut);
};

/*
// GET NUMBER OF PARAMETERS
int getModelParameterTotal();

// GET NUMBER OF PARAMETERS
int getModelStateTotal();

// GET TOTAL NUMBER OF RESULTS
int getModelResultTotal();

// GET THE PARAMETER NAMES
string getParamName(int index);

// GET RESULT NAME
string getResultName(int index);

// GET THE PARAMETER RANGES FOR THE LPN MODELS
void getModelLimits(int model,int par_num,double* limits);

// GET PARAMETER SETS FOR THE LPN MODELS     
void getDefaultModelParams(int modelID, double* zp);

// EVAL LOG LIKELIHOOD OF THE HEART LPN MODELS
double eval_LPN_loglikelihood(int model, int state_num, int par_num, int res_num, double* params, int &error);
*/

#endif // ODESINGLEVENTRICLESTAGE1BLOCKS3_H