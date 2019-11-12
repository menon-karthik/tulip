#ifndef ODECONGENITAL4CHAMBERSPAH_H
#define ODECONGENITAL4CHAMBERSPAH_H

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

// GENERIC CLASS FOR SAMPLES
class odeCongenital4ChambersPAH: public odeModel{
  private:

  // Constants for Result Quantities
  // Results v-0001
  static const int ip_0001_HR   = 0;
  static const int ip_0001_SBP  = 1;
  static const int ip_0001_DBP  = 2;
  static const int ip_0001_mBP  = 3;
  static const int ip_0001_RAP  = 4;
  static const int ip_0001_sPAP = 5;
  static const int ip_0001_dPAP = 6;
  static const int ip_0001_mPAP = 7;
  static const int ip_0001_PWP  = 8;
  static const int ip_0001_CO   = 9;
  static const int ip_0001_PVR  = 10;
  static const int ip_0001_SVR  = 11;
  static const int ip_0001_CVP  = 12;
  static const int ip_0001_LVEF = 13;
  static const int ip_0001_RVEF = 14;
  static const int ip_0001_sRV = 15;
  static const int ip_0001_dRV = 16;
  static const int ip_0001_sLV = 17;
  static const int ip_0001_dLV = 18;

  // Results v-0002
  static const int ip_0002_heart_rate2                = 0;
  static const int ip_0002_systolic_bp_2              = 1;
  static const int ip_0002_diastolic_bp_2             = 2;
  static const int ip_0002_cardiac_output             = 3;
  static const int ip_0002_systemic_vascular_resistan = 4;
  static const int ip_0002_pulmonary_vascular_resista = 5;
  static const int ip_0002_cvp                        = 6;
  static const int ip_0002_right_ventricle_diastole   = 7;
  static const int ip_0002_right_ventricle_systole    = 8;
  static const int ip_0002_rvedp                      = 9;
  static const int ip_0002_aov_mean_pg                = 10;
  static const int ip_0002_aov_peak_pg                = 11;
  static const int ip_0002_mv_decel_time              = 12;
  static const int ip_0002_mv_e_a_ratio               = 13;
  static const int ip_0002_pv_at                      = 14;
  static const int ip_0002_pv_max_pg                  = 15;
  static const int ip_0002_ra_pressure                = 16;
  static const int ip_0002_ra_vol_a4c                 = 17;
  static const int ip_0002_la_vol_a4c                 = 18;
  static const int ip_0002_lv_esv                     = 19;
  static const int ip_0002_lv_vol                     = 20;
  static const int ip_0002_lvef                       = 21;
  static const int ip_0002_lvot_max_flow              = 22;
  static const int ip_0002_pap_diastolic              = 23;
  static const int ip_0002_pap_systolic               = 24;
  static const int ip_0002_wedge_pressure             = 25;
  // New Values Introduced by Karlyn
  static const int ip_0002_RVEDV_mri                  = 26;
  static const int ip_0002_RVESV_mri                  = 27;
  static const int ip_0002_QPA_mri                    = 28;
  static const int ip_0002_LVEDV_mri                  = 29;
  static const int ip_0002_LVESV_mri                  = 30;
  static const int ip_0002_QAO_mri                    = 31;

  // Types of target configuration
  static const int ipTargetConfig_IncludeAll = 0;
  static const int ipTargetConfig_ExcludePAP = 1;
  static const int ipTargetConfig_HRBPCVP    = 2;

  // Types of result configuration
  int modelVersion;
  // Set Target Configuration Mode
  int targetConfigMode;
  
  public:
  
  // CONSTRUCTOR
  odeCongenital4ChambersPAH(int modelVersion,int targetConfigMode);
  
  // DESTRUCTOR
  virtual ~odeCongenital4ChambersPAH();
  
  // VIRTUAL FUNCTIONS TO IMPLEMENT
  virtual int    getParameterTotal();
  virtual string getParamName(int parID);
  virtual int    getStateTotal();
  virtual int    getAuxStateTotal();
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

#endif // ODECONGENITAL4CHAMBERSPAH_H
