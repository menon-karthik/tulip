#ifndef ODENORMALADULTSIMPLEPD_H
#define ODENORMALADULTSIMPLEPD_H

# include <string>
# include <stdio.h>
# include <string>
# include "math.h"
# include "uqTypes.h"
# include "odeModel.h"
# include "cmUtils.h"
# include "cmConstants.h"
# include "cmException.h"

// Constants for Variable and Auxiliary outputs
const int ipAuxRAPressure = 5;
const int ipAuxRVPressure = 7;
const int ipAuxLVPressure = 8;
const int ipPAAPressure = 5;
const int ipPAVPressure = 6;
const int ipAOPressure = 9;
const int ipRVVolume = 2;
const int ipLVVolume = 3;
const int ipAOFlow = 10;

// Constants for Result Quantities
// Results v-0001
const int ip_0001_HR   = 0;
const int ip_0001_SBP  = 1;
const int ip_0001_DBP  = 2;
const int ip_0001_mBP  = 3;
const int ip_0001_RAP  = 4;
const int ip_0001_sPAP = 5;
const int ip_0001_dPAP = 6;
const int ip_0001_mPAP = 7;
const int ip_0001_PWP  = 8;
const int ip_0001_CO   = 9;
const int ip_0001_PVR  = 10;
const int ip_0001_SVR  = 11;
const int ip_0001_CVP  = 12;
const int ip_0001_LVEF = 13;
const int ip_0001_RVEF = 14;

// Results v-0002
const int ip_0002_heart_rate2                = 0;
const int ip_0002_systolic_bp_2              = 1;
const int ip_0002_diastolic_bp_2             = 2;
const int ip_0002_cardiac_output             = 3;
const int ip_0002_systemic_vascular_resistan = 4;
const int ip_0002_pulmonary_vascular_resista = 5;
const int ip_0002_cvp                        = 6;
const int ip_0002_right_ventricle_diastole   = 7;
const int ip_0002_right_ventricle_systole    = 8;
const int ip_0002_rvedp                      = 9;
const int ip_0002_aov_mean_pg                = 10;
const int ip_0002_aov_peak_pg                = 11;
const int ip_0002_mv_decel_time              = 12;
const int ip_0002_mv_e_a_ratio               = 13;
const int ip_0002_pv_at                      = 14;
const int ip_0002_pv_max_pg                  = 15;
const int ip_0002_ra_pressure                = 16;
const int ip_0002_ra_vol_a4c                 = 17;
const int ip_0002_la_vol_a4c                 = 18;
const int ip_0002_lv_esv                     = 19;
const int ip_0002_lv_vol                     = 20;
const int ip_0002_lvef                       = 21;
const int ip_0002_lvot_max_flow              = 22;
const int ip_0002_pap_diastolic              = 23;
const int ip_0002_pap_systolic               = 24;
const int ip_0002_wedge_pressure             = 25;

class odeNormalAdultSimplePD: public odeModel{
  private:
    // Types of result configuration
    int modelVersion;

  public:

    //! Default constructor
    odeNormalAdultSimplePD(int modelVersion);
    //! Virtual destructor
    virtual ~odeNormalAdultSimplePD();

    // Virtual functions to be implemented by child classes
    virtual int    getParameterTotal() = 0;
    virtual int    getStateTotal() = 0;
    virtual int    getAuxStateTotal() = 0;
    virtual void   getDefaultParams(stdVec& params) = 0;
    virtual void   getDefaultParameterLimits(stdVec& limits) = 0;
    virtual string getParamName(int parID) = 0;
    virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind) = 0;
    virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results) = 0;
    virtual void   getResultKeys(stdStringVec& keys) = 0;
    virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs) = 0;
    virtual void   getDataSTD(stdVec& stds) = 0;
    virtual void   getResultWeigths(stdVec& weights) = 0;    
    virtual int    getHRIndex() = 0;
    
};

#endif // ODENORMALADULTSIMPLEPD_H
