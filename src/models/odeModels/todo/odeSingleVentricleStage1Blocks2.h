#ifndef ODESINGLEVENTRICLESTAGE1BLOCKS2_H
#define ODESINGLEVENTRICLESTAGE1BLOCKS2_H

# include <string>
# include <vector>

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

// Auxiliary Output Constants
const int ipPsv        = 0;
const int ipPsa        = 1;
const int ipQAo        = 2;
const int iptc         = 5;
const int ipAA         = 6;
const int ipAV         = 7;
const int ipQvc        = 8;
const int ipQvp        = 9;
const int ipQsh        = 10;
const int ipQaftCao    = 11;
const int ipPsaactive  = 12;
const int ipPsapassive = 13;
const int ipPsvactive  = 14;
const int ipPsvpassive = 15;
const int ipQreg       = 16;

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleStage1Blocks2: public odeModel{
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
    odeRCR(double r1, double c, double r2, stdMat p);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual string getResultName(int resID);
    virtual void   getParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual void   getPriorMapping(int priorModelType,int* prPtr);

    // EVAL MODEL 
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCodes);
    virtual stdVec evalModelError(stdMat inputs,stdMat& outputs, stdIntVec& errorCodes);

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

// GET RESULT CURVE NAMES
void getModelResultCurveFileNames(vector<string> &curve_names);

// GET THE PARAMETER RANGES FOR THE LPN MODELS
void getModelLimits(int model,int par_num,double* limits);

// GET PARAMETER SETS FOR THE LPN MODELS     
void getDefaultModelParams(int modelID, double* zp);

// EVAL LOG LIKELIHOOD OF THE HEART LPN MODELS
double eval_LPN_loglikelihood(int model, int state_num, int par_num, int res_num, double* params, int &error);

// SOLVE MODEL
int solveStage12BlocksLPN(int modelID, lpnOptions options, int totalStates, double* iniVals,int totalParams, double* params, double* results);
*/

#endif // ODESINGLEVENTRICLESTAGE1BLOCKS2_H