#ifndef ODECORONARY_H
#define ODECORONARY_H

# include "uqTypes.h"
# include "uqConstants.h"
# include "odeModel.h"
# include "cmException.h"
# include "cmUtils.h"

using namespace std;

// DEFINE MODEL CONSTANTS
const int piPATIENT1  = 0;
const int nUnknowns   = 48;
const int nFaces      = 23;
  
// RESULT START POINTER
const int ipRCRStart     = 10;
const int ipCorLeftStart = 20;
const int ipMainFlow     = 30;

// DEFINE NUMBER OF VESSELS
const int nCOR   = 16;
const int nCOR_l = 11;
const int nCOR_r = 5;       
const int nRCR   = 6;

// DEFINE CONVERSION CONSTANT
const double pConv = 1.334E3;

#ifdef PATIENT1
    const int model = piPATIENT1;
#endif

const int numElasModes = 25;
const double Ft_elastance[][2] = {{0.283748803,0.000000000},
                                  {0.031830626,-0.374299825},
                                  {-0.209472400,-0.018127770},
                                  {0.020520047,0.073971113},
                                  {0.008316883,-0.047249597},
                                  {-0.041677660,0.003212163}, 
                                  {0.000867323,0.019441411},
                                  {-0.001675379,-0.005565534},
                                  {-0.011252277,0.003401432}, 
                                  {-0.000414677,0.008376795},
                                  {0.000253749,-0.000071880},
                                  {-0.002584966,0.001566861}, 
                                  {0.000584752,0.003143555},
                                  {0.000028502,-0.000024787},
                                  {0.000022961,-0.000007476},
                                  {0.000018735,-0.000001281},
                                  {0.000015573,0.000001781},
                                  {0.000013133,0.000003494},
                                  {0.000011199,0.000004507},
                                  {0.000009634,0.000005117},
                                  {0.000008343,0.000005481},
                                  {0.000007265,0.000005687},
                                  {0.000006354,0.000005789},
                                  {0.000005575,0.000005821},
                                  {0.000004903,0.000005805}};

const double Ft_elasprime[][2] = {{0.000160588,0.000000000},
                                  {0.679406633,0.059306043},
                                  {0.069559757,-0.759915413},
                                  {-0.403119137,0.108979197},
                                  {0.342718135,0.063494904},
                                  {-0.024532652,-0.378425035},
                                  {-0.211456487,0.006588383},
                                  {0.071342502,-0.020124010},
                                  {-0.046083942,-0.164174687},
                                  {-0.136311395,-0.009522642},
                                  {0.001519248,0.004677455},
                                  {-0.029656507,-0.052308083},
                                  {-0.068446545,0.010917996},
                                  {0.000883363,0.000747380},
                                  {0.000489711,0.000651536},
                                  {0.000335507,0.000577723},
                                  {0.000249511,0.000521240},
                                  {0.000193846,0.000476325},
                                  {0.000154681,0.000439495},
                                  {0.000125614,0.000408582},
                                  {0.000103215,0.000382144},
                                  {0.000085449,0.000359209},
                                  {0.000071059,0.000339082},
                                  {0.000059194,0.000321230},
                                  {0.000049261,0.000305268}};

const double model3dRes[] = {245.548339843750,
                             1780.34997558594,
                             9029.58398437500,     
                             2158.10839843750,
                             1415.05004882812,
                             2412.14990234375,     
                             111130.500000000,
                             152204.000000000,
                             108127.500000000,     
                             179523.500000000,
                             200037.500000000,
                             179289.500000000,     
                             337415.000000000,
                             337415.000000000,
                             177697.000000000,     
                             311512.500000000,
                             172126.500000000,
                             209118.000000000,     
                             115401.000000000,
                             213479.484375000,
                             208370.500000000,
                             290465.500000000};

/***************************************FIX THESE INDICES***************************************/

//const int faceToStateMapping[] = {11,12,13,14,15,16,17,19,
//                                  21,23,25,27,29,31,33,
//                                  35,37,39,41,43,45,47,49,10};

const int faceToStateMapping[] = {10,11,12,13,14,15,16,
                                18,20,22,24,26,28,30,
                                32,34,36,38,40,42,44,46,9};

/***********************************************************************************************/

class odeCoronary: public odeModel{ 
  public:
    // Constructor
    odeCoronary();
    // Destructor
    virtual ~odeCoronary();

    virtual int    getParameterTotal();
    virtual int    getStateTotal();
    virtual int    getResultTotal();
    virtual int    getAuxStateTotal();
    virtual void   getParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual void   eval(double t,const stdVec& Xk,const stdVec& params,const stdVec& fn, stdVec& DXk, stdVec& auxOut);
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
};

#endif // ODECORONARY_H
