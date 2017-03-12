#ifndef ODESINGLEVENTRICLEHEARTIND_H
#define ODESINGLEVENTRICLEHEARTIND_H

# include "odeModel.h"

using namespace std;

// GENERIC CLASS FOR SAMPLES
class odeSingleVentricleHeartIND: public odeModel{
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
  	odeSingleVentricleHeartIND();

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

    // AUX FUNCTIONS
    int solveLPN(double iniTime, double timeStep, int totalSteps, double iniFlowRate, stdMat distPressure, int totalParams, double* params, double* results);

    // Plot Element to STD OUT
    void show();
};

/*
// DEFINE STRUCTS
struct lpnOptions {
  bool applyConstraints;
  bool flushData;
  bool writeDebugData;
};

// GET TOTAL NUMBER OF PARAMETERS
int getModelParameterTotal();

// GET TOTAL NUMBER OF STATES
int getModelStateTotal();

// GET TOTAL NUMBER OF STATES
int getModelResultTotal();

// GET NAME OF RESULT QUANTITY
string getResultName(int index);

// GET PARAMETER NAME
string getParamName(int index);

// GET THE PARAMETER RANGES FOR THE LPN MODELS
void getModelLimits(int model,int par_num,double* limits);

// GET PARAMETER SETS FOR THE LPN MODELS
void getDefaultModelParams(int modelID, double* zp);

// SOLVE LPN MODEL OF THE HEART
int solveLPN(int modelID, lpnOptions options, int totalStates, double* iniVals,int totalParams, double* params, double* results);

// EVAL LOG LIKELIHOOD OF THE HEART LPN MODELS
double eval_LPN_loglikelihood(int model, int state_num, int par_num, int res_num, double* params, int &error);
*/

#endif // ODESINGLEVENTRICLEHEARTIND_H