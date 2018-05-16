#include "odeRCRCR.h"

using namespace std;

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeRCRCR::getParameterTotal(){
  return 7;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeRCRCR::getStateTotal(){
  return 2; 
}

// ==================
// GET PARAMETER NAME
// ==================
string odeRCRCR::getParamName(int parID){
  string result;
  switch(parID){
    case 0:
    {
      result = "Rtot";
      break;
    }
    case 1:
    {
      result = "Ctot";
      break;
    }
    case 2:
    {      
      result = "C1C2ratio";
      break;
    }
    case 3:
    {      
      result = "R1R2ratio";
      break;
    }
    case 4:
    {      
      result = "R3R2ratio";
      break;
    }
    case 5:
    {      
      result = "Initial-P1";
      break;
    }
    case 6:
    {      
      result = "Initial-P2";
      break;
    }    
  }
  return result;
}

// GET MODEL PARAMETERS
void odeRCRCR::getDefaultParams(stdVec& params){
  params[0] = 3.07;  // Rtot
  params[1] = 0.56;  // Ctot
  params[2] = 0.2;   // C1C2_Ratio
  params[3] = 0.2;   // R1R2_Ratio
  params[4] = 0.2;   // R3R2_Ratio
  params[5] = 5.0;   // Initial P1
  params[6] = 5.0;   // Initial P2
}

// GET PARAMETER RANGES
void odeRCRCR::getParameterLimits(stdVec& limits){
  limits[0]=2.0; limits[1]=4.0;   // Rtot
  limits[2]=0.5; limits[3]=3.0;   // tau
  limits[4]=0.1; limits[5]=1.0;   // C1C2_Ratio
  limits[6]=0.1; limits[7]=1.0;   // R1R2_Ratio
  limits[8]=0.1; limits[9]=1.0;   // R3R2_Ratio
  limits[10]=5.0; limits[11]=5.0; // Initial P1
  limits[12]=5.0; limits[13]=5.0; // Initial P2
}

// GET FOURIER COEFFICIENTS FOR ALL MODELS
void getFourierCoefficients(int patientID,
                            double &ain1,double &ain2,double &ain3,double &ain4,double &ain5,double &ain6,double &ain7,double &ain8,double &ain9,
                            double &bin1,double &bin2,double &bin3,double &bin4,double &bin5,double &bin6,double &bin7,double &bin8,
                            double &aout1,double &aout2,double &aout3,double &aout4,double &aout5,double &aout6,double &aout7,double &aout8,double &aout9,
                            double &bout1,double &bout2,double &bout3,double &bout4,double &bout5,double &bout6,double &bout7,double &bout8){

  ain9 = 25.27; ain1 = 4.215; bin1 = 0.8914; ain2 = -15.74; bin2 = 18.16; ain3 = 0.8737; bin3 = 13.62; ain4 = 1.966;
  bin4 = 1.185; ain5 = -0.9142; bin5 = -0.928; ain6 = -0.8623; bin6 = -0.7006; ain7 = -0.7609; bin7 = -0.7781;
  ain8 = -0.3597; bin8 = -0.2047;

  aout9 = 25.26; aout1 = 2.985; bout1 = 38.75; aout2 = -20.02; bout2 = 5.519; aout3 = -4.378; bout3 = -2.792; aout4 = -3.966;
  bout4 = -2.693; aout5 = 0.3378; bout5 = -1.732; aout6 = -1.418; bout6 = -0.7536; aout7 = 0.4665; bout7 = -1.528; 
  aout8 = 0.1099; bout8 = -0.1978;
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void heartODE(int patientID, double tn, double* Xn, double* params, double* Xn1,double* out){

  // INITIALIZE VARIABLES
  // STORE LOCAL COPIES OF PARAMETERS
  double HR = 117.65;
  double tc = (60.0/(double)HR);

  double Rtot = params[0];
  double tau = params[1];
  double C1C2ratio = params[2];
  double R1R2ratio = params[3];
  double R3R2ratio = params[4];

  // ====================
  // FOURIER COEFFICIENTS
  // ====================
  // IN
  double ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9;
  double bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8;
  // OUT  
  double aout1,aout2,aout3,aout4,aout5,aout6,aout7,aout8,aout9;
  double bout1,bout2,bout3,bout4,bout5,bout6,bout7,bout8;
  // Freq
  double w = 2.0*M_PI/(double)tc;

  // Get The Coefficients for different Models
  getFourierCoefficients(patientID,
                         ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9,
                         bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8,
                         aout1,aout2,aout3,aout4,aout5,aout6,aout7,aout8,aout9,
                         bout1,bout2,bout3,bout4,bout5,bout6,bout7,bout8);
  
  // SHIFT TIME TO ACCOUNT FOR INPUT/OUTPUT CURVE DELAY
  double teff1 = tn;
  double teff2 = tn;

  // SUM UP VENOUS FLUX
  double Qin1 = ain9+ain1*cos(teff1*w)+bin1*sin(teff1*w)+ain2*cos(2*teff1*w);
  double Qin2 = bin2*sin(2*teff1*w)+ain3*cos(3*teff1*w)+bin3*sin(3*teff1*w);
  double Qin3 = ain4*cos(4*teff1*w)+bin4*sin(4*teff1*w)+ain5*cos(5*teff1*w);
  double Qin4 = bin5*sin(5*teff1*w)+ain6*cos(6*teff1*w)+bin6*sin(6*teff1*w);
  double Qin5 = ain7*cos(7*teff1*w)+bin7*sin(7*teff1*w)+ain8*cos(8*teff1*w) ;
  double Qin6 = bin8*sin(8*teff1*w);
  double Qven = Qin1+Qin2+Qin3+Qin4+Qin5+Qin6;

  // EVAL AORTIC FLOW RATE
  double Qout1 = aout9 + aout1*cos(teff2*w) + bout1*sin(teff2*w) + aout2*cos(2*teff2*w);
  double Qout2 = bout2*sin(2*teff2*w)+aout3*cos(3*teff2*w)+bout3*sin(3*teff2*w);
  double Qout3 = aout4*cos(4*teff2*w)+bout4*sin(4*teff2*w)+aout5*cos(5*teff2*w);
  double Qout4 = bout5*sin(5*teff2*w)+aout6*cos(6*teff2*w)+bout6*sin(6*teff2*w);
  double Qout5 = aout7*cos(7*teff2*w)+bout7*sin(7*teff2*w)+aout8*cos(8*teff2*w);
  double Qout6 = bout8*sin(8*teff2*w);
  double QAo = Qout1+Qout2+Qout3+Qout4+Qout5+Qout6;

  // UNPACK STATE VARIABLES
  double P1 = Xn[0]; // Pressure in 1
  double P2 = Xn[1]; // Pressure in 2

  // Resistances
  double R2 = Rtot/(1.0 + R1R2ratio + R3R2ratio);
  double R1 = R1R2ratio*R2;
  double R3 = R3R2ratio*R2;
  // Capacitance
  double Ctot = tau/(double)Rtot;
  double C2 = Ctot/(1.0 + C1C2ratio);
  double C1 = C1C2ratio*C2;
  // Eval other quantities
  double P0 = P1 + QAo*R1;
  double P3 = P2 - Qven*R3;
  // Eval Derivatives
  double Q2 = (P1-P2)/(double)R2;
  double P1p = (QAo-Q2)/(double)C1;
  double P2p = (Q2-Qven)/(double)C2;
  // Other quantities
  double Q1 = QAo;
  double Q3 = Qven;
  
  // STORE TIME DERIVATIVES
  Xn1[0] = P1p;
  Xn1[1] = P2p;

  // STORE SECONDARY OUTPUT
  out[0] = P0;
  out[1] = P3;
  out[2] = Q1;
  out[3] = Q2;
  out[4] = Q3;
  out[5] = tn;
}


// EXTRACT MODEL RESULTS
void postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  double cycleTime = timeStep*totalStepsOnSingleCycle;

  // Copy Auxiliary Variables From result for the last heart cycle
  double t[totalStepsOnSingleCycle];
  double P0[totalStepsOnSingleCycle];
  double P3[totalStepsOnSingleCycle];
  double Q1[totalStepsOnSingleCycle];
  double Q2[totalStepsOnSingleCycle];
  double Q3[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    P0[loopA] = auxOutVals[globalInd][ipP0];
    P3[loopA] = auxOutVals[globalInd][ipP3];
    Q1[loopA] = auxOutVals[globalInd][ipQ1];
    Q2[loopA] = auxOutVals[globalInd][ipQ2];
    Q3[loopA] = auxOutVals[globalInd][ipQ3];
    t[loopA]  = auxOutVals[globalInd][ipT];
    
  }

  // Copy State Variables From result for the last heart cycle
  double P1[totalStepsOnSingleCycle];
  double P2[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    P1[loopA] = outVals[globalInd][ipP1];
    P2[loopA] = outVals[globalInd][ipP2];
  }

  // GET AVERAGE VALUES
  double mP0 = trapz(0,totalStepsOnSingleCycle,t,P0)/(double)cycleTime;
  double mP1 = trapz(0,totalStepsOnSingleCycle,t,P1)/(double)cycleTime;
  double mP2 = trapz(0,totalStepsOnSingleCycle,t,P2)/(double)cycleTime;
  double mP3 = trapz(0,totalStepsOnSingleCycle,t,P3)/(double)cycleTime;
  double mQ1 = trapz(0,totalStepsOnSingleCycle,t,Q1)/(double)cycleTime;
  double mQ2 = trapz(0,totalStepsOnSingleCycle,t,Q2)/(double)cycleTime;
  double mQ3 = trapz(0,totalStepsOnSingleCycle,t,Q3)/(double)cycleTime;

  // GET MAXIMUM AND MINIMUM VALUES
  double minP0 = getMin(0,totalStepsOnSingleCycle,P0);
  double maxP0 = getMax(0,totalStepsOnSingleCycle,P0);
  double minP1 = getMin(0,totalStepsOnSingleCycle,P1);
  double maxP1 = getMax(0,totalStepsOnSingleCycle,P1);
  double minP2 = getMax(0,totalStepsOnSingleCycle,P2);
  double maxP2 = getMax(0,totalStepsOnSingleCycle,P2);
  double minP3 = getMax(0,totalStepsOnSingleCycle,P3);
  double maxP3 = getMax(0,totalStepsOnSingleCycle,P3);
  double minQ1 = getMin(0,totalStepsOnSingleCycle,Q1);
  double maxQ1 = getMax(0,totalStepsOnSingleCycle,Q1);
  double minQ2 = getMax(0,totalStepsOnSingleCycle,Q2);
  double maxQ2 = getMax(0,totalStepsOnSingleCycle,Q2);
  double minQ3 = getMax(0,totalStepsOnSingleCycle,Q3);
  double maxQ3 = getMax(0,totalStepsOnSingleCycle,Q3);

  // COPY FINAL RESULTS
  results[0] = minP0;
  results[1] = mP0;
  results[2] = maxP0;
  results[3] = minP1;
  results[4] = mP1;
  results[5] = maxP1;
  results[6] = minP2;
  results[7] = mP2;
  results[8] = maxP2;
  results[9] = minP3;
  results[10] = mP3;
  results[11] = maxP3;
  results[12] = minQ1;
  results[13] = mQ1;
  results[14] = maxQ1;
  results[15] = minQ2;
  results[16] = mQ2;
  results[17] = maxQ2;
  results[18] = minQ3;
  results[19] = mQ3;
  results[20] = maxQ3;
}
