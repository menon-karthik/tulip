#ifndef ODENORMALADULTSIMPLEPA_H
#define ODENORMALADULTSIMPLEPA_H

# include <stdio.h>
# include <string>
# include "stdlib.h"
# include "math.h"
# include "uqTypes.h"
# include "odeModel.h"
# include "cmModel.h"
# include "cmConstants.h"
# include "cmUtils.h"
# include "cmException.h"

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
const int ip_0001_sRV = 15;
const int ip_0001_dRV = 16;
const int ip_0001_sLV = 17;
const int ip_0001_dLV = 18;

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

// Types of target configuration
const int ipTargetConfig_IncludeAll = 0;
const int ipTargetConfig_ExcludePAP = 1;
const int ipTargetConfig_HRBPCVP    = 2;

// GENERIC CLASS FOR SAMPLES
class odeNormalAdultSimplePA: public odeModel{
  private:
  // Types of result configuration
  int modelVersion;
  // Set Target Configuration Mode
  int targetConfigMode;
  
  public:
  
  // CONSTRUCTOR
  odeNormalAdultSimplePA(int modelVersion,int targetConfigMode);
  
  // DESTRUCTOR
  virtual ~odeNormalAdultSimplePA();
  
  // VIRTUAL FUNCTIONS TO IMPLEMENT
  virtual int    getParameterTotal();
  virtual string getParamName(int parID);
  virtual int    getStateTotal();
  virtual int    getResultTotal();
  virtual int    getAuxStateTotal();
  virtual string getResultName(int resID);
  virtual void   getDefaultParameterLimits(stdVec& limits);
  virtual void   getDefaultParams(stdVec& params);
  virtual void   evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdMat& fn, stdVec& DXk, stdVec& auxOut, stdVec& Ind);
  virtual void   postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdVec& params, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results);
  virtual void   getResultKeys(stdStringVec& keys);
  virtual void   getFinalOutputs(const stdVec& outputs, stdVec& outs);
  virtual void   getDataSTD(stdVec& stds);
  virtual void   getResultWeigths(stdVec& weights);
  virtual int    getHRIndex();
};

#endif // ODENORMALADULTSIMPLEPA_H
