# include <string>
# include "lpnUtils.hpp"

using namespace std;

// DEFINE MODEL CONSTANTS
const int piMUSC9  = 0;
const int piGOSH30 = 1;

#ifdef MUSC9
  const int model = piMUSC9;
#endif
#ifdef GOSH30
  const int model = piGOSH30;
#endif

// Numerical Constants
const double pi = 3.1415926535897;

// STATE VARIABLES
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
const int ipQsvc       = 4;
const int ipQcoll      = 5;
const int ipQivc       = 6;
const int ipQubv       = 7;
const int ipPsaactive  = 9;
const int ipPsapassive = 10;
const int ipPsvactive  = 11;
const int ipPsvpassive = 12;
const int ipQreg       = 13;

// MODEL CONSTANTS
const double PRIMARY_CURVEPEAK_WEIGHT = 10.0;
const double SECONDARY_CURVEPEAK_WEIGHT = 15.0;
const double DATA_UNCERTAINTY = 0.05;
const bool   USE_PEAKTIME_ASTARGETS = false;
const bool   EXCLUDE_INVASIVE_TARG = false;

// DEFINE STRUCTS
struct lpnOptions{
  bool writeDebugData;
  bool flushData;
  bool useHeartConstraints;
  bool useFullConstraints;
};

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