# include "odeSingleVentricleStage2.h"

using namespace std;

// ==================
// SIMULATION COUNTER
// ==================
int simulCount = 0;

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int getModelParameterTotal(){
  return 53;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int getModelStateTotal(){
  return 14; 
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int getModelResultTotal(){
  return 29;  
}

// ==================
// GET PARAMETER NAME
// ==================
string getParamName(int index){
  switch(index){
    case 0: 
    {          
      return string("HR");
      break;
    }
    case 1:           
    {      
      return string("Csa");
      break;
    }
    case 2:           
    {      
      return string("csa");
      break;
    }
    case 3:           
    {      
      return string("dsa");
      break;
    }

    case 4:           
    {      
      return string("Vsa0");
      break;
    }
    case 5:           
    {      
      return string("tsas-ratio");
      break;
    }
    case 6:           
    {      
      return string("t1-ratio");
      break;
    }
    case 7:           
    {      
      return string("a");
      break;
    }
    case 8:           
    {      
      return string("b");
      break;
    }
    case 9:           
    {     
      return string("csv");
      break;
    }
    case 10:           
    {      
      return string("dsv");
      break;
    }
    case 11:           
    {      
      return string("Vsv0");
      break;
    }
    case 12:                 
    {      
      return string("tsvs-ratio");
      break;
    }
    case 13:           
    {      
      return string("Rmyo");
      break;
    }
    case 14:           
    {      
      return string("alpha");
      break;
    }
    case 15:           
    {      
      return string("Kao");
      break;
    }
    case 16:           
    {      
      return string("Cao");
      break;
    }
    case 17:           
    {      
      return string("beta");
      break;
    }
    case 18:           
    {      
      return string("Rcoll");
      break;
    }
    case 19:           
    {      
      return string("Cp");
      break;
    }
    case 20:           
    {      
      return string("UBSVR");
      break;
    }
    case 21:           
    {      
      return string("LBSVR");
      break;
    }
    case 22:           
    {      
      return string("PVR");
      break;
    }
    case 23:           
    {      
      return string("Ctot-UBSVR");
      break;
    }
    case 24:           
    {      
      return string("C1C2ratio-UBSVR");
      break;
    }
    case 25:           
    {      
      return string("R1R2ratio-UBSVR");
      break;
    }
    case 26:           
    {      
      return string("R3R2ratio-UBSVR");
      break;
    }
    case 27:           
    {      
      return string("L-UBSVR");
      break;
    }
    case 28:           
    {      
      return string("Ctot-LBSVR");
      break;
    }
    case 29:           
    {      
      return string("C1C2ratio-LBSVR");
      break;
    }
    case 30:           
    {      
      return string("R1R2ratio-LBSVR");
      break;
    }
    case 31:           
    {      
      return string("R3R2ratio-LBSVR");
      break;
    }
    case 32:           
    {
      return string("L-LBSVR");
      break;
    }
    case 33:           
    {      
      return string("Ctot-PVR");
      break;
    }
    case 34:           
    {      
      return string("C1C2ratio-PVR");
      break;
    }
    case 35:           
    {      
      return string("R1R2ratio-PVR");
      break;
    }
    case 36:           
    {      
      return string("R3R2ratio-PVR");
      break;
    }
    case 37:           
    {
      return string("L-PVR");
      break;
    }
    case 38:           
    {      
      return string("k-bw");
      break;
    }
    case 39:           
    {      
      return string("Vsv");
      break;
    }
    case 40:           
    {      
      return string("Vsa");
      break;
    }
    case 41:           
    {      
      return string("Pao");
      break;
    }
    case 42:           
    {      
      return string("Qav");
      break;
    }
    case 43:           
    {      
      return string("Pubv");
      break;
    }
    case 44:           
    {      
      return string("Quba");
      break;
    }
    case 45:           
    {      
      return string("Puba");
      break;
    }
    case 46:           
    {      
      return string("Plung");
      break;
    }
    case 47:           
    {      
      return string("Pvp");
      break;
    }
    case 48:           
    {      
      return string("Qapol");
      break;
    }
    case 49:           
    {      
      return string("Ppa");
      break;
    }
    case 50:           
    {      
      return string("Qlba");
      break;
    }
    case 51:           
    {      
      return string("Plba");
      break;
    }
    case 52:           
    {      
      return string("Plbv");
      break;
    }
  }
}

// ===============
// GET RESULT NAME
// ===============
string getResultName(int index){
  switch(index){
    case 0:
    {      
      return string("mPascAo");
      break;
    }
    case 1:
    {      
      return string("minPAo");
      break;
    }
    case 2:
    {      
      return string("maxPAo");
      break;
    }
    case 3:
    {      
      return string("mPsa");
      break;
    }
    case 4:
    {      
      return string("maxPsv");
      break;
    }
    case 5:
    {      
      return string("maxVsv");
      break;
    }
    case 6:
    {      
      return string("QpQs");
      break;
    }
    case 7:
    {      
      return string("mPpa");
      break;
    }
    case 8:
    {      
      return string("CO");
      break;
    }
    case 9:
    {      
      return string("Qcoll");
      break;
    }    
    case 10:
    {      
      return string("IVC-S");
      break;
    }
    case 11:
    {      
      return string("IVC-M");
      break;
    }
    case 12:
    {      
      return string("IVC-D");
      break;
    }
    case 13:
    {      
      return string("IVC-A");
      break;
    }
    case 14:
    {      
      return string("SVC-MIN");
      break;
    }
    case 15:
    {      
      return string("SVC-MAX");
      break;
    }    
    case 16:
    {      
      return string("PVs-S");
      break;
    }
    case 17:
    {      
      return string("PVs-M");
      break;
    }
    case 18:
    {      
      return string("PVs-D");
      break;
    }
    case 19:
    {      
      return string("PVs-A");
      break;
    }

    case 20:
    {      
      return string("ivc-SD-time");
      break;
    }    
    case 21:
    {      
      return string("ivc-MA-time");
      break;
    }    
    case 22:
    {      
      return string("vp-SD-time");
      break;
    }    
    case 23:
    {      
      return string("vp-MA-time");
      break;
    }    
    case 24:
    {      
      return string("minPpa");
      break;
    }    
    case 25:
    {      
      return string("maxPpa");
      break;
    }    
    case 26:
    {      
      return string("mQreg");
      break;
    }    
    case 27:
    {      
      return string("PsvZero");
      break;
    }    
    case 28:
    {      
      return string("maxPsa");
      break;
    }    
  }
}

// ====================
// GET MODEL PARAMETERS
// ====================
void getDefaultModelParams(int modelID, double* zp){
  switch(modelID){
  	case piMUSC9:
      /*zp[0] = 110.0; zp[1] = 1.7568; zp[2] = 0.37132; zp[3] = 0.14891; zp[4] = 6.9949; zp[5] = 0.2188;
      zp[6] = 0.063184; zp[7] = -0.032165; zp[8] = 11.585; zp[9] = 2.5336; zp[10] = 0.068586; zp[11] = 6.3478;
      zp[12] = 0.31448; zp[13] = -0.0039507; zp[14] = 5.0; zp[15] = 4.2362e-06; zp[16] = 0.45959; zp[17] = 5.0;
      zp[18] = 49.573; zp[19] = 0.056609; zp[20] = 3.8811; zp[21] = 3.7706; zp[22] = 0.12389; zp[23] = 19.305; 
      zp[24] = 0.011989; zp[25] = 0.28576; zp[26] = 0.32621; zp[27] = 0.015807; zp[28] = 12.587; zp[29] = 12.428;
      zp[30] = 0.15504; zp[31] = 0.02791; zp[32] = 0.035894; zp[33] = 19.085; zp[34] = 2.5304; zp[35] = 0.20704;
      zp[36] = 0.85377; zp[37] = 0.014225; zp[38] = 0.0; zp[39] = 46.311; zp[40] = 11.928; zp[41] = 46.101;
      zp[42] = -4.0418; zp[43] = 21.485; zp[44] = 12.757; zp[45] = 13.298; zp[46] = 14.359; zp[47] = 12.919;
      zp[48] = 15.676; zp[49] = 12.712; zp[50] = 22.269; zp[51] = 22.197; zp[52] = 12.661;*/

      zp[0] = 110.0000; zp[1] = 1.7568; zp[2] = 0.3713; zp[3] = 0.2; zp[4] = 6.9949;zp[5] = 0.2188;zp[6] = 0.0632;zp[7] = -0.1000;
      zp[8] = 11.5850; zp[9] = 2.5336; zp[10] = 0.0686; zp[11] = 6.3478; zp[12] = 0.3145; zp[13] = 0.1000; zp[14] = 25.0000; zp[15] = 0.0001;
      zp[16] = 0.4596; zp[17] = 0.9500;  zp[18] = 10.0000; zp[19] = 0.3000; zp[20] = 3.89;  zp[21] = 3.77;  zp[22] = 0.119;  zp[23] = 0.5000;
      zp[24] = 5.0000; zp[25] = 0.2858; zp[26] = 0.3262; zp[27] = 0.0158; zp[28] = 0.5000; zp[29] = 5.0000; zp[30] = 0.1550; zp[31] = 0.0279;
      zp[32] = 0.0359; zp[33] = 19.0850; zp[34] = 2.5304; zp[35] = 0.2070; zp[36] = 0.8538; zp[37] = 0.0142; zp[38] = 0.0200; zp[39] = 20.0000;
      zp[40] = 11.9280; zp[41] = 46.1010; zp[42] = 10.0000;zp[43] = 21.4850; zp[44] = 12.7570; zp[45] = 15.0000; zp[46] = 16.0000; zp[47] = 12.9190;
      zp[48] = 15.6760; zp[49] = 10.0000; zp[50] = 20.0000; zp[51] = 15.0000; zp[52] = 12.6610;
  	  break;
  	case piGOSH30:
      zp[0] = 105.0; zp[1] = 1.562; zp[2] = 0.31295; zp[3] = 0.24666; zp[4] = 5.9632; zp[5] = 0.21941; zp[6] = 0.09039;
      zp[7] = -0.061349; zp[8] = 4.3333; zp[9] = 0.5087; zp[10] = 0.074604; zp[11] = 10.166; zp[12] = 0.33135; zp[13] = 0.0048125;
      zp[14] = 5.0; zp[15] = 1.476e-05; zp[16] = 0.76725;  zp[17] = 5.0; zp[18] = 67.347; zp[19] = 0.42445; zp[20] = 1.96;
      zp[21] = 3.64; zp[22] = 0.152; zp[23] = 11.495; zp[24] = 22.59; zp[25] = 0.95075; zp[26] = 0.025995; zp[27] = 0.090911;
      zp[28] = 23.461; zp[29] = 39.344; zp[30] = 1.996; zp[31] = 0.99488; zp[32] = 0.096913; zp[33] = 68.778; zp[34] = 6.9713;
      zp[35] = 0.76053; zp[36] = 1.7976; zp[37] = 0.15648; zp[38] = 0.0; zp[39] = 48.383; zp[40] = 6.819; zp[41] = 46.181; zp[42] = 0.0019525;
      zp[43] = 44.393; zp[44] = 3.7897; zp[45] = 49.294; zp[46] = 8.5342; zp[47] = 5.1519; zp[48] = 15.116; zp[49] = 13.095; zp[50] = 13.125;
      zp[51] = 20.528; zp[52] = 3.3192;
      break;
  }
}

// ====================
// GET PARAMETER RANGES
// ====================
void getModelLimits(int model,int par_num,double* limits){
  switch(model){
    case piMUSC9:
      limits[0]=110.0; limits[1]=110.0; //HR
      limits[40]=3.89; limits[41]=3.89; //UBSVR
      limits[42]=3.77; limits[43]=3.77; //LBSVR
      limits[44]=0.119; limits[45]=0.119; //PVR   
      limits[46]=0.128; limits[47]=0.771; //Cot_UBSVR
      limits[56]=0.133; limits[57]=0.796; //Ctot_LBSVR
      limits[66]=4.202; limits[67]=25.210; //Ctot_PVR
      limits[76]=1.0e-5; limits[77]=0.05; //k_bw
      break;
    case piGOSH30:
      limits[0]=105.0; limits[1]=105.0; //HR
      limits[40]=1.96; limits[41]=1.96; //UBSVR
      limits[42]=3.64; limits[43]=3.64; //LBSVR
      limits[44]=0.152; limits[45]=0.152; //PVR   
      limits[46]=0.255; limits[47]=1.531; //Cot_UBSVR
      limits[56]=0.137; limits[57]=0.824; //Ctot_LBSVR
      limits[66]=3.289; limits[67]=19.737; //Ctot_PVR
      limits[76]=0.0; limits[77]=0.0; //k_bw    
      break;
  }

  // COMMON LIMITS
  limits[2]=0.05; limits[3]=2.0; //Csa
  limits[4]=0.01; limits[5]=0.8; //csa
  limits[6]=0.16; limits[7]=0.8; //dsa
  limits[8]=0.1; limits[9]=15.0; //Vsa0
  limits[10]=0.1739; limits[11]=0.2599; //tsas_ratio
  limits[12]=0.01; limits[13]=0.15; //t1_ratio
  limits[14]=-0.2; limits[15]=-0.05; //a
  limits[16]=8.0; limits[17]=15.0; //b
  limits[18]=0.1; limits[19]=5.0; //csv
  limits[20]=0.04; limits[21]=0.08; //dsv
  limits[22]=2.0; limits[23]=15.0; //Vsv0
  limits[24]=0.2478; limits[25]=0.3718; //tsvs_ratio
  limits[26]=0.001; limits[27]=0.1; //Rmyo
  limits[28]=1.0; limits[29]=30.0; //Alpha
  limits[30]=1.0E-4; limits[31]=2.5E-4; //Kao
  limits[32]=0.05; limits[33]=1.0; //Cao
  limits[34]=0.0; limits[35]=1.0; // Beta
  limits[36]=2.0; limits[37]=15.0; //Rcoll
  limits[38]=0.1; limits[39]=0.4; //Cp
  limits[48]=0.1; limits[49]=20.0; //C1C2ratio_UBSVR
  limits[50]=0.01; limits[51]=3.0; //R1R2ratio_UBSVR
  limits[52]=0.01; limits[53]=3.0; //R3R2ratio_UBSVR
  limits[54]=0.005; limits[55]=0.05; //Luba
  limits[58]=0.1; limits[59]=20.0; //C1C2ratio_LBSVR
  limits[60]=0.01; limits[61]=3.0; //R1R2ratio_LBSVR
  limits[62]=0.01; limits[63]=3.0; //R3R2ratio_LBSVR
  limits[64]=0.005; limits[65]=0.05; //Llb
  limits[68]=0.1; limits[69]=20.0; //C1C2ratio_PVR
  limits[70]=0.01; limits[71]=3.0; //R1R2ratio_PVR
  limits[72]=0.01; limits[73]=3.0; //R3R2ratio_PVR
  limits[74]=0.005; limits[75]=0.05; //Lapol
  limits[78]=5.0; limits[79]=40.0; //Vsv
  limits[80]=1.0; limits[81]=15.0; //Vsa
  limits[82]=10.0; limits[83]=80.0; //Pao
  limits[84]=-0.1; limits[85]=100.0; //Qav
  // CHECK RANGES ??
  limits[86]=19.8; limits[87]=24.2; //Pubv
  limits[88]=12.6; limits[89]=15.4; //Quba
  limits[90]=13.5; limits[91]=16.5; //Puba
  limits[92]=15.3; limits[93]=18.7; //Plung
  limits[94]=11.7; limits[95]=14.3; //Pvp
  limits[96]=13.5; limits[97]=16.5; //Qapol
  limits[98]=9.0; limits[99]=11.0; //Ppa
  limits[100]=18.0; limits[101]=22.0; //Qlbap 
  limits[102]=13.5; limits[103]=16.5; //Plbap 
  limits[104]=10.8; limits[105]=13.2; //Plbvp
}

// ========================
// GET RESPIRATION PRESSURE
// 30 breaths/minute
// ========================
void getRespirationPressure(double t,double &Pth,double &Pab,double &dPth,double &dPab){
  //  tm=mod(t,2.0);
  //  tsr=0.43*2.0;   // active phase of respiration

  //  Respiration pressures
  //  sinr=sin(2*pi*tm/tsr);
  //  cosr=cos(2*pi*tm/tsr);
  //  form=(1-cosr).*(tm<tsr)/2;
  //  Pth=-4*form-2;
  //  Pab= 2*form+1;

  //  Rate of change of thoracic pressures
  //  form=pi/tsr*sinr.*(tm<tsr);
  //  dPth=-4*form;
  //  dPab= 2*form;

  // NO RESPIRATION
  Pth=0.0;
  Pab=0.0;
  dPth=0.0;
  dPab=0.0; 
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void stage2ODE(int patientID, double tn, double* Xn, double* params, double* Xn1,double* out){

  // Init Stenosis
  double k_stenosis_UB = 0.0;
  double k_stenosis_LB = 0.0;
  double k_stenosis_Llung = 0.0;
  double k_stenosis_Rlung = 0.0;

  // Set Heart Model Parameters
  double HR = params[0]; // Fixed Heart Rate
  double tc = 60.0/(double)HR;
  double Csa = params[1];
  double csa = params[2];
  double dsa = params[3];
  double Vsa0 = params[4];
  double tsas_ratio = params[5];
  double tsas = tsas_ratio*sqrt(tc);
  double t1_ratio = params[6];
  double t1 = t1_ratio*tc;
  double a = params[7];
  double b = params[8];
  double csv = params[9];
  double dsv = params[10];
  double Vsv0 = params[11];
  double tsvs_ratio = params[12]; // Apply tsvs=0.16+0.3*tc
  double tsvs = tsvs_ratio*sqrt(tc);
  double Rmyo = params[13];
  double alpha = params[14]; //  AV valve
  double Kao = params[15]; // Discharge coefficient Aortic valve
  double Cao = params[16]; // Aortic arch
  double beta = params[17]; // Aortic arch
  double Rcoll = params[18]; // Collateral
  double Cp = params[19]; // Shunt/PA compliance

  // Size of the atrio-ventricular Valve
  double A_fw = 0.0;
  switch(patientID){
  	case piMUSC9:
      A_fw = 6.76; // MUSC9
  	  break;
  	case piGOSH30:
      A_fw = 7.0; // GOSH30
  	  break;
  }
  
  // SYSTEMIC AND PULMONARY CIRCULATION PARAMETERS
  double UBSVR = params[20];
  double LBSVR = params[21];
  double PVR = params[22];
  // UBSVR
  double Ctot_UBSVR = params[23]; // HP stessa Ctot dx e sn
  double C1C2ratio_UBSVR = params[24];
  double R1R2ratio_UBSVR = params[25];
  double R3R2ratio_UBSVR = params[26];
  double R2_UBSVR = UBSVR/(double)(1.0+R1R2ratio_UBSVR+R3R2ratio_UBSVR);
  double R1_UBSVR = R1R2ratio_UBSVR*R2_UBSVR;
  double C2_UBSVR = Ctot_UBSVR/(double)(1.0+C1C2ratio_UBSVR);
  double C1_UBSVR = C2_UBSVR*C1C2ratio_UBSVR;
  double R3_UBSVR = R3R2ratio_UBSVR*R2_UBSVR;
  double L_UBSVR = params[27];
  // LBSVR
  double Ctot_LBSVR = params[28]; // HP stessa Ctot dx e sn
  double C1C2ratio_LBSVR = params[29];
  double R1R2ratio_LBSVR = params[30];
  double R3R2ratio_LBSVR = params[31];
  double R2_LBSVR = LBSVR/(double)(1.0+R1R2ratio_LBSVR+R3R2ratio_LBSVR);
  double R1_LBSVR = R1R2ratio_LBSVR*R2_LBSVR;
  double C2_LBSVR = Ctot_LBSVR/(double)(1.0+C1C2ratio_LBSVR);
  double C1_LBSVR = C2_LBSVR*C1C2ratio_LBSVR;
  double R3_LBSVR = R3R2ratio_LBSVR*R2_LBSVR;
  double L_LBSVR = params[32];

  // PVR
  double Ctot_PVR = params[33]; 
  double C1C2ratio_PVR = params[34];
  double R1R2ratio_PVR = params[35];
  double R3R2ratio_PVR = params[36];
  double R2_PVR = PVR/(double)(1.0+R1R2ratio_PVR+R3R2ratio_PVR);
  double R1_PVR = R1R2ratio_PVR*R2_PVR;
  double C2_PVR = Ctot_PVR/(double)(1.0+C1C2ratio_PVR);
  double C1_PVR = C2_PVR*C1C2ratio_PVR;
  double R3_PVR = R3R2ratio_PVR*R2_PVR;
  double L_PVR = params[37];
  double k_bw = params[38];

  // Unpack state variables
  double Vsv   = Xn[0]; // volume single ventricle
  double Vsa   = Xn[1]; // volume single atrium
  double PAo   = Xn[2]; // pressure aorta
  double Qav   = Xn[3]; // pressure upper body
  double Pubv  = Xn[4]; // pressure SVC
  double Quba  = Xn[5]; // flow upper body arteries
  double Puba  = Xn[6]; // flow Atrial-Ventricular valve
  double Plung = Xn[7];
  double Pvp   = Xn[8];
  double Qapol = Xn[9];
  double Ppa   = Xn[10]; // pressure in main pulmonary artery
  double Qlba  = Xn[11];
  double Plba  = Xn[12];
  double Plbv  = Xn[13];

  // HEART FUNCTION
  double tmv = fmod(tn,tc); // Ventricle time
  double tma = fmod(tn+tsas-t1,tc); // Atrium time

  // Ventricle activation
  double AV = 0.0;
  if(tmv<tsvs){
    AV = (1.0-cos(2.0*pi*tmv/tsvs))/2.0;
  }else{
    AV = 0.0;
  }

  // Atrium activation
  double AA = 0.0;
  if(tma<tsas){
    AA = (1.0-cos(2.0*pi*tma/tsas))/2.0;
  }else{
    AA = 0.0;
  }

  // Atrium model
  double Psaactive = (Vsa-Vsa0)/(double)Csa;
  double Psapassive = csa*(exp(dsa*(Vsa-Vsa0))-1.0);
  double Psa = AA*Psaactive + Psapassive;

  // Ventricle model
  double Psvactive = (a*(Vsv-Vsv0) + b)*(Vsv-Vsv0);    
  double Psvpassive = csv*(exp(dsv*(Vsv-Vsv0))-1.0);
  double Psv = AV*Psvactive + Psvpassive;

  double QAo = 0.0;
  if(Psv>PAo){
    QAo = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAo)))/(2.0*Kao);
  }else{
    QAo = 0.0;
  }

  double Psveff=Psv-Rmyo*QAo;

  // =====================
  // ATRIVENTRICULAR VALVE
  // =====================
  double Kav = 0.0;
  double Lav = 0.0;
  double Qavp = 0.0;
  double A_bw = 0.0;
  double Kav_fw = 0.0;
  double Lav_fw = 0.0;
  double Kav_bw = 0.0;
  double Lav_bw = 0.0;
  double AV_vel = 0.0;
  double Qreg = 0.0;
  if(k_bw < 1.0E-5){
  	// NON REGIRGITANT VALVE
  	// Atrio-ventricular resistance and inductance
    Kav = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2; // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav = beta*2*1.060*sqrt(4/(pi*A_fw))/1333.2;
   
    if((Psa<Psveff)&&(Qav<1.0E-5)){
      Qavp = 0.0;
    }else{
      Qavp = (Psa-Psv-Kav*Qav*fabs(Qav))/(double)Lav;
    }
      
    if(Qav<1.0E-5){
      Qav = 0.0;
      Xn[3] = 0.0;    	
    }

  }else{
    // REGURGITANT VALVE
    A_bw = k_bw*A_fw;

    Kav_fw = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2; // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav_fw = beta*2.0*1.060*sqrt(4.0/(pi*A_fw))/1333.2; // Factor 1333.2 to have inertance in [mmHg*s^2/ml]

    Kav_bw = alpha*0.5*1.060*(2.4/(A_bw*A_bw))/1333.2;     
    Lav_bw = beta*2*1.060*sqrt(4.0/(pi*A_bw))/1333.2; 

    if(Qav<1.0E-5){
      Qavp = (Psa-Qav*fabs(Qav)*Kav_bw-Psv)/(double)Lav_bw; // One- way quadratic valve plus inertance
    }else{
      Qavp = (Psa-Qav*fabs(Qav)*Kav_fw-Psv)/(double)Lav_fw; // one- way quadratic valve plus inertance	
    }

    if(Qav<1.0E-5){
      AV_vel = Qav/(double)A_bw;
    }else{
      AV_vel = Qav/(double)A_fw;
    }

    if(Qav<1.0E-5){
      Qreg = fabs(Qav);
    }

  }

  // Cavity respiration pressures
  double Pth,Pab,DPith,dPab; 
  getRespirationPressure(tn,Pth,Pab,DPith,dPab);

  // COLLATERALS
  double Qcoll = (PAo-Plung)/(double)Rcoll; // Nonlinear COLLATERAL resistance

  // LUNGS
  double Qlung = (Plung-Pvp)/(double)R2_PVR;
  double Qapolp = (Ppa-Qapol*R1_PVR-Plung-k_stenosis_Rlung*Qapol*Qapol)/(double)L_PVR; // If stenosis is present k!=0
  double Plungp = (Qapol-Qlung+Qcoll)/(double)C1_PVR + DPith;
  // Q PVs TARGET !!!
  double Qvp = (Pvp-Psa)/(double)R3_PVR;
  double Pvpp = (Qlung-Qvp)/(double)C2_PVR + DPith;

  // SYTEM UB
  double Qubap = (PAo-Puba-R1_UBSVR*Quba-k_stenosis_UB*Quba*Quba)/(double)L_UBSVR; // If stenosis is present k!=0
  double Qub = (Puba-Pubv)/(double)R2_UBSVR;
  // Q - SVC -TARGET PICCHI !!!!
  double Qubv = (Pubv-Ppa)/(double)R3_UBSVR;
  double Pubap = (Quba-Qub)/(double)C1_UBSVR;
  double Pubvp = (Qub-Qubv)/(double)C2_UBSVR;

  // SYTEM LB
  double Qlbap=(PAo-Plba-R1_LBSVR*Qlba-k_stenosis_LB*Qlba*Qlba)/(double)L_LBSVR; // If stenosis is present k!=0
  double Qlb = (Plba-Plbv)/(double)R2_LBSVR;
  // Q -IVC -TARGET PICCHI !!!!
  double Qlbv = (Plbv-Psa)/(double)R3_LBSVR;
  double Plbap = (Qlba-Qlb)/(double)C1_LBSVR;
  double Plbvp = (Qlb-Qlbv)/(double)C2_LBSVR;

  // TIME DERIVATIVES OF STATE VARIABLES
  double Vsvp = (Qav-QAo);
  double Vsap = (Qlbv+Qvp-Qav);
  double PAop = (QAo-Qcoll-Quba-Qlba)/(double)Cao + DPith;

  double Ppap=(Qubv-Qapol)/(double)Cp + DPith;

  // STATE VARIABLES
  Xn1[0] = Vsvp;
  Xn1[1] = Vsap; 
  Xn1[2] = PAop; 
  Xn1[3] = Qavp; 
  Xn1[4] = Pubvp; 
  Xn1[5] = Qubap; 
  Xn1[6] = Pubap; 
  Xn1[7] = Plungp; 
  Xn1[8] = Pvpp; 
  Xn1[9] = Qapolp; 
  Xn1[10] = Ppap; 
  Xn1[11] = Qlbap; 
  Xn1[12] = Plbap; 
  Xn1[13] = Plbvp;

  // AUX VARIABLES
  out[0] = Psveff;
  out[1] = Psa;
  out[2] = QAo;
  out[3] = Qvp;
  out[4] = Qubv;
  out[5] = Qcoll;
  out[6] = Qlbv;
  out[7] = Qubv;
  out[8] = Qvp;
  out[9] = Psaactive;
  out[10] = Psapassive;
  out[11] = Psvactive;
  out[12] = Psvpassive;
  out[13] = Qreg;

}

// =============================
// EVAL MODEL FOR RK4 ITERATIONS
// =============================
void evalModel(int modelID, double t,int totalStates,double* Xk,double* params,double* DXk,double* auxOut){
  // SOLVE HEART MODEL
  stage2ODE(modelID,t,Xk,params,DXk,auxOut);
}

// ==================
// SOLVE ODE WITH RK4
// ==================
void PerformRK4Steps(int modelID, int totalSteps,double timeStep,int totalStates,int totAuxStates,double* iniVals,int totalParams, double* params,double** outVals,double** auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = 0;
  const bool printIC = false;

  // Allocate Temporary Variables
  double k1[totalStates];
  double k2[totalStates];
  double k3[totalStates];
  double k4[totalStates];
  double k1AuxOut[totAuxStates];
  double k2AuxOut[totAuxStates];
  double k3AuxOut[totAuxStates];
  double k4AuxOut[totAuxStates];
  double Xk2[totalStates];
  double Xk3[totalStates];
  double Xk4[totalStates];

  // Initialize State Vectors and Copy Initial Conditions
  double Xn[totalStates];
  double Xn1[totalStates];
  for(int loopA=0;loopA<totalStates;loopA++){
    Xn[loopA] = iniVals[loopA];
  }

  // TIME LOOP
  for(int loopA=0;loopA<totalSteps;loopA++){
    // Increment Time Step
    stepId++;

    // Update Current Time
    if(loopA>0){
      currTime += timeStep;
    }
    // Eval K1
    evalModel(modelID,currTime,totalStates,Xn,params,k1,k1AuxOut);
    // Eval K2
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk2[loopB] = Xn[loopB] + (0.5*timeStep) * k1[loopB];
    }
    evalModel(modelID,currTime + 0.5 * timeStep,totalStates,Xk2,params,k2,k2AuxOut);
    // Eval K3
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk3[loopB] = Xn[loopB] + (0.5*timeStep) * k2[loopB];
    }
    evalModel(modelID,currTime + 0.5 * timeStep,totalStates,Xk3,params,k3,k3AuxOut);
    // Eval K4
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk4[loopB] = Xn[loopB] + timeStep * k3[loopB];
    }
    evalModel(modelID,currTime + timeStep,totalStates,Xk4,params,k4,k4AuxOut);

    // Eval Xn1
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn1[loopB] = Xn[loopB] + (1.0/6.0)*timeStep*(k1[loopB] + 2.0 * k2[loopB] + 2.0 * k3[loopB] + k4[loopB]);
    }

    // Update Xn
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn[loopB] = Xn1[loopB];
    }

    // Copy back k1AuxOut for each time step
    for(int loopB=0;loopB<totAuxStates;loopB++){
      auxOutVals[loopA][loopB] = k1AuxOut[loopB];
    }
    // Copy The solution back for each time step
    for(int loopB=0;loopB<totalStates;loopB++){
      outVals[loopA][loopB] = Xn1[loopB];
    }
  }
}

// ========================
// PARAMETERS INSIDE LIMITS
// ========================
bool areParamsInsideLimits(int model,int totalStates,double* iniVals,int totalParams,double* params){
  // GET LIMITS
  int par_num = (totalParams+totalStates);
  double currParam = 0.0;
  double limits[2*par_num];
  getModelLimits(model,par_num,limits);
  
  bool result = true;
  for(int loopA=0;loopA<par_num;loopA++){
    if(loopA>(totalParams-1)){
      currParam = iniVals[loopA-totalParams];
    }else{
      currParam = params[loopA];
    }
    result = result && ((currParam>=limits[loopA*2 + 0])&&(currParam<=limits[loopA*2 + 1]));
    if(!result){
      printf("INDEX: %d\n",loopA);
    }
  }

  // Return
  return result;
}

// ====================================================
// CHECK IF PARAMETER IS POSITIVE - BASED ON THE RANGES
// ====================================================
bool isParamPositive(int curr_par, double* limits){
  double limitMin = limits[curr_par*2 + 0];
  double limitMax = limits[curr_par*2 + 1];
  if((limitMin>=0.0)&&(limitMax>=0.0)){
    return true;
  }else{
    return false;
  }
}

// ====================================================
// CHECK IF PARAMETER IS POSITIVE - BASED ON THE RANGES
// ====================================================
bool isParamNegative(int curr_par, double* limits){
  double limitMin = limits[curr_par*2 + 0];
  double limitMax = limits[curr_par*2 + 1];
  if((limitMin<0.0)&&(limitMax<0.0)){
    return true;
  }else{
    return false;
  }
}

// ====================
// CHECK PARAMETER SIGN
// ====================
bool isParamsSignCorrect(int model,int totalStates,double* iniVals,int totalParams,double* params){
  // GET LIMITS
  int par_num = (totalParams+totalStates);
  double currParam = 0.0;
  double limits[2*par_num];
  getModelLimits(model,par_num,limits);

  bool result = true;
  for(int loopA=0;loopA<par_num;loopA++){
    if(loopA>(totalParams-1)){
      currParam = iniVals[loopA-totalParams];
    }else{
      currParam = params[loopA];
    }
    if(isParamPositive(loopA,limits)||isParamNegative(loopA,limits)){
      result = result && (isParamPositive(loopA,limits) == (currParam>=0.0)) && (isParamNegative(loopA,limits) == (currParam<0.0));  
    }    
  }
  // RETURN
  return result;
}

// ================================
// CHECK THE VALIDITY OF PARAMETERS
// ================================
bool validHeartParams(double* iniVals,double* params){
  
  // Init Results
  bool answer = true;

  // Csa must be positive
  if(params[1]<0.0){
    answer = false;
    return answer;
  }

  // csa must be positive 
  if(params[2]<0.0){
    answer = false;
    return answer;    
  }

  // dsa must be positive and bigger than 0.14
  if(params[3]<0.16){
    answer = false;
    return answer;    
  }

  // New Constraints on the times
  // tsas_ratio must be within the range 
  if((params[5]<(0.2169-0.02169))||(params[5]>(0.2169+0.02169))){
    answer = false;
    return answer;    
  }

  // tsvs_ratio must be within the range 
  if((params[12]<(0.3098-0.03098),params[12]>(0.3098+0.03098))){
    answer = false;
    return answer;    
  }

  // t1_ratio must be positive and smaller than 0.15
  if((params[6]<0)||(params[6]>0.15)){
    answer = false;
    return answer;
  }

  // a must be negative
  if(params[7]>0.0){
    answer = false;
    return answer;    
  }

  // b must be positive
  // b less than 15
  if((params[8]<0.0)||(params[8]>15.0)){
    answer = false;
    return answer;
  }

  // csv must be positive and less then 5.0
  if((params[9]<0.0)||(params[9]>5.0)){
    answer = false;
    return answer;
  }

  // dsv must be positive 
  // Previous: if(Par(11)<0.05)
  if((params[10]<0.05)||(params[10]>0.08)){
    answer = false;
    return answer;
  }

  // tsvs must be positive 
  if(params[12]<0.0){
    answer = false;
    return answer;    
  }
  
  // Rmyo must be positive 
  if(params[13]<0.075){
    answer = false;
    return answer;    
  }

  // Dav must be positive 
  if(params[14]<0.0){
    answer = false;
    return answer;
  }

  // KAo must be positive 
  if(params[15]<0){
    answer = false;
    return answer;    
  }
 
  // CAo must be positive 
  if(params[16]<0.0){
    answer = false;
    return answer;
  }

  // ADDITIONAL ON PARAMETERS
  // Vsv0 must be greater and equal to Vsa0
  if(params[11]<params[4]){
    answer = false;
    return answer;
  }

  // Vsa0 must be greater than Zero
  if(params[4]<0.0){
    answer = false;
    return answer;
  }

  // ==================
  // INITIAL CONDITIONS
  // ==================

  // Vsv must be positive 
  if(iniVals[0]<0){
    answer = false;
    return answer;    
  }

  // Vsa must be positive 
  if(iniVals[1]<0){
    answer = false;
    return answer;
  }

  // PAo must be positive 
  if(iniVals[2]<0){
    answer = false;
    return answer;    
  }

  // Vsa must me greater than Vsa0
  if(iniVals[1]<params[4]){
    answer = false;
    return answer;    
  }

  // Return True 
  return answer;
}

// =====================
// PRINT PARAMETER TABLE
// =====================
void printParamTable(double cycleTime,int numCycles,double totalTime,double timeStep,
                   int totalSteps,int totalStepsOnSingleCycle,double* params,double* iniVals){
  FILE* outFile;
  outFile = fopen("paramTable.txt","w");
  // Print Header
  fprintf(outFile,"\\begin{table}[h!]\n");
  fprintf(outFile,"\\centering\n");
  fprintf(outFile,"\\begin{tabular}{l c}\n");
  fprintf(outFile,"\\toprule\n");
  fprintf(outFile,"Parameter & Value\\\\\n");
  fprintf(outFile,"\\midrule\n");
  fprintf(outFile,"Heart Rate & %f\\\\\n",params[0]);
  fprintf(outFile,"Heart Cycle Time [s] & %f\\\\\n",cycleTime);
  fprintf(outFile,"Number of Heart Cycles [s] & %d\\\\\n",numCycles);
  fprintf(outFile,"Total Simulation Time [s] & %f\\\\\n",totalTime);
  fprintf(outFile,"Time Step [s] & %f\\\\\n",timeStep);
  fprintf(outFile,"Total Simulation Steps & %d\\\\\n",totalSteps);
  fprintf(outFile,"Steps per Heart Cycle & %d\\\\\n",totalStepsOnSingleCycle);
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Full model (Stage2) time step settings for patient \\patient}\n");
  fprintf(outFile,"\\end{table}\n");

  fprintf(outFile,"\\begin{table}[h!]\n");
  fprintf(outFile,"\\centering\n");
  fprintf(outFile,"\\begin{tabular}{l c}\n");
  fprintf(outFile,"\\toprule\n");
  fprintf(outFile,"Parameter & Value\\\\\n");
  fprintf(outFile,"\\midrule\n");
  fprintf(outFile,"Heart Rate & %f\\\\\n",params[0]);
  fprintf(outFile,"Csa & %f\\\\\n",params[1]);
  fprintf(outFile,"csa & %f\\\\\n",params[2]);
  fprintf(outFile,"dsa & %f\\\\\n",params[3]);
  fprintf(outFile,"Vsa0 & %f\\\\\nn",params[4]);
  fprintf(outFile,"tsas-ratio & %f\\\\\n",params[5]);
  fprintf(outFile,"t1-ratio & %f\\\\\n",params[6]);
  fprintf(outFile,"a & %f\\\\\n",params[7]);
  fprintf(outFile,"b & %f\\\\\n",params[8]);
  fprintf(outFile,"csv & %f\\\\\n",params[9]);
  fprintf(outFile,"dsv & %f\\\\\n",params[10]);
  fprintf(outFile,"Vsv0 & %f\\\\\n",params[11]);
  fprintf(outFile,"tsvs-ratio & %f\\\\\n",params[12]);
  fprintf(outFile,"Rmyo & %f\\\\\n",params[13]);
  fprintf(outFile,"Alpha & %f\\\\\n",params[14]);
  fprintf(outFile,"Kao & %f\\\\\n",params[15]);
  fprintf(outFile,"CAo & %f\\\\\n",params[16]);
  fprintf(outFile,"Beta & %f\\\\\n",params[17]);
  fprintf(outFile,"Rcoll & %f\\\\\n",params[18]);
  fprintf(outFile,"Cp & %f\\\\\n",params[19]);
  fprintf(outFile,"UBSVR & %f\\\\\n",params[20]);
  fprintf(outFile,"LBSVR & %f\\\\\n",params[21]);
  fprintf(outFile,"PVR & %f\\\\\n",params[22]);
  fprintf(outFile,"Ctot-UBSVR & %f\\\\\n",params[23]);
  fprintf(outFile,"C1C2ratio-UBSVR & %f\\\\\n",params[24]);   
  fprintf(outFile,"R1R2ratio-UBSVR & %f\\\\\n",params[25]);   
  fprintf(outFile,"R3R2ratio-UBSVR & %f\\\\\n",params[26]); 
  fprintf(outFile,"L-UBSVR & %f\\\\\n",params[27]);
  fprintf(outFile,"Ctot-LBSVR & %f\\\\\n",params[28]);
  fprintf(outFile,"C1C2ratio-LBSVR & %f\\\\\n",params[29]);   
  fprintf(outFile,"R1R2ratio-LBSVR & %f\\\\\n",params[30]);   
  fprintf(outFile,"R3R2ratio-LBSVR & %f\\\\\n",params[31]); 
  fprintf(outFile,"L-LBSVR & %f\\\\\n",params[32]);
  fprintf(outFile,"Ctot-PVR & %f\\\\\n",params[33]);
  fprintf(outFile,"C1C2ratio-PVR & %f\\\\\n",params[34]);
  fprintf(outFile,"R1R2ratio-PVR & %f\\\\\n",params[35]);
  fprintf(outFile,"R3R2ratio-PVR & %f\\\\\n",params[36]);
  fprintf(outFile,"L-PVR & %f\\\\\n",params[37]);
  fprintf(outFile,"k-bw & %f\\\\\n",params[38]);
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Optimal Full Model (Stage2) parameters for patient \\patient}\n");
  fprintf(outFile,"\\end{table}\n");

  fprintf(outFile,"\\begin{table}[h!]\n");
  fprintf(outFile,"\\centering\n");
  fprintf(outFile,"\\begin{tabular}{l c}\n");
  fprintf(outFile,"\\toprule\n");
  fprintf(outFile,"Parameter & Value\\\\\n");
  fprintf(outFile,"\\midrule\n");
  fprintf(outFile,"Vsv & %f\\\\\n",iniVals[0]);
  fprintf(outFile,"Vsa & %f\\\\\n",iniVals[1]);
  fprintf(outFile,"Pao & %f\\\\\n",iniVals[2]);
  fprintf(outFile,"Qav & %f\\\\\n",iniVals[3]);
  fprintf(outFile,"Pubv & %f\\\\\n",iniVals[4]);
  fprintf(outFile,"Quba & %f\\\\\n",iniVals[5]);
  fprintf(outFile,"Puba & %f\\\\\n",iniVals[6]);
  fprintf(outFile,"Plung & %f\\\\\n",iniVals[7]);
  fprintf(outFile,"Pvp & %f\\\\\n",iniVals[8]);
  fprintf(outFile,"Qapol & %f\\\\\n",iniVals[9]);
  fprintf(outFile,"Ppa & %f\\\\\n",iniVals[10]);
  fprintf(outFile,"Qlba & %f\\\\\n",iniVals[11]);
  fprintf(outFile,"Plba & %f\\\\\n",iniVals[12]);
  fprintf(outFile,"Plbv & %f\\\\\n",iniVals[13]);
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Optimal initial Full Model (Stage2) parameters for patient \\patient}\n");
  fprintf(outFile,"\\end{table}\n");
  // CLOSE THE FILE
  fclose(outFile);
}

// =====================
// PRINT PARAMETER TABLE
// =====================
void printParamLimitsTable(int model, int par_num){
  string currParamName;
  // Get Model Limits
  double limits[2*par_num];
  getModelLimits(model,par_num,limits);
  // Print Table
  FILE* outFile;
  outFile = fopen("paramLimitsTable.txt","w");
  // Print Header
  fprintf(outFile,"\\begin{table}[h!]\n");
  fprintf(outFile,"\\centering\n");
  fprintf(outFile,"\\begin{tabular}{l c c}\n");
  fprintf(outFile,"\\toprule\n");
  fprintf(outFile,"Parameter & Min & Max\\\\\n");
  fprintf(outFile,"\\midrule\n");
  for(int loopA=0;loopA<par_num;loopA++){
    currParamName = getParamName(loopA);
    fprintf(outFile,"%s & %f & %f\\\\\n",currParamName.c_str(),limits[loopA*2 + 0],limits[loopA*2 + 1]);
  }
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Parameter limits used for full model (Stage2) identification, patient \\patient}\n");
  fprintf(outFile,"\\end{table}\n");
  // CLOSE THE FILE
  fclose(outFile);
}

// ========================
// FLUSH MODEL DATA TO FILE
// ========================
void flushHeartModelDataToFile(string outFileName,int start, int stop ,double* t,double** outVals,double** auxOutVals){

  FILE* outFile;
  outFile = fopen(outFileName.c_str(),"w");
  fprintf(outFile,"%20s %20s %20s %20s %20s %20s %20s %20s %20s %20s\n","t","Qvp","Qsvc","Qivc","Qav","QAo","Psa","Psv","PAo","Ppa");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e %20e %20e %20e %20e %20e %20e\n",
    	    t[loopA],auxOutVals[loopA][ipQvp],auxOutVals[loopA][ipQsvc],auxOutVals[loopA][ipQivc],outVals[loopA][ipQav],auxOutVals[loopA][ipQAo],
          auxOutVals[loopA][ipPsa],auxOutVals[loopA][ipPsv],outVals[loopA][ipPAo],outVals[loopA][ipPpa]);
  }
  fclose(outFile);
  // FLUSH PV-LOOP CURVES
  outFile = fopen("Stage2ModelPVLoops.txt","w");
  fprintf(outFile,"%20s %20s %20s %20s\n","Vsa","Psa","Vsv","Psv");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e\n",outVals[loopA][ipVsa],auxOutVals[loopA][ipPsa],outVals[loopA][ipVsv],auxOutVals[loopA][ipPsv]);
  }
  fclose(outFile);

  // FLUSH AV CURVES
  outFile = NULL;
  outFile = fopen("Stage2ModelAV.txt","w");
  fprintf(outFile,"%20s %20s %20s %20s\n","t","Qvp","Qivc","Qsvc");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e\n",t[loopA]-t[start],auxOutVals[loopA][ipQvp],auxOutVals[loopA][ipQivc],auxOutVals[loopA][ipQsvc]);
  }
  fclose(outFile);
}

// ==============================
// CHECK IF THE RESULTS ARE VALID
// ==============================
bool validStage2LPNResults(double* results){
  bool answer = true;

  // mPascAo;
  if((results[0]<0.0)||(results[0]>300.0)){
    printf("mPascAo - ");
  	answer = false;
  	return answer;
  }

  // minPAo;
  if((results[1]<0.0)||(results[1]>300.0)){
    printf("minPAo - ");
  	answer = false;
  	return answer;
  }

  // maxPAo;
  if((results[2]<0.0)||(results[2]>300.0)){
    printf("maxPAo - ");
    answer = false;
    return answer;
  }

  // mPsa;
  if((results[3]<0.0)||(results[3]>300.0)){
    printf("mPsa - ");
  	answer = false;
  	return answer;
  }

  // maxPsv;
  if((results[4]<0.0)||(results[4]>300.0)){
    printf("maxPsv - ");
  	answer = false;
  	return answer;
  }

  // maxVsv;
  if((results[5]<0.0)||(results[5]>300.0)){
    printf("maxVsv - ");
  	answer = false;
  	return answer;
  }

  // QpQs;
  if((results[6]<0.0)||(results[6]>300.0)){
    printf("QpQs - ");
    answer = false;
    return answer;
  }

  // mPpa;
  if((results[7]<0.0)||(results[7]>300.0)){
    printf("mPpa - ");
    answer = false;
    return answer;
  }

  // CO;
  if((results[8]<0.0)||(results[8]>300.0)){
    printf("CO - ");
    answer = false;
    return answer;
  }

  // RETURN
  return answer;

}

// ===============================================
// CHECK VALIDITY OF PARAMS FOR LPN Stage1-2Blocks
// ===============================================
bool areValidStage2FullParams(double* iniVals,double* params){
  
  // Init Answer
  bool answer = true;

  // UBSVR - Tau in [0.5,3.0]
  if((params[23]<0.5/params[20])||(params[23]>3.0/params[20])){
    printf("UBSVR - ");
    answer = false;
    return answer;
  }

  // LBSVR - Tau in [0.5,3.0]
  if((params[28]<0.5/params[21])||(params[28]>3.0/params[21])){
    printf("LBSVR - ");
    answer = false;
    return answer;
  }  

  // PVR - Tau in [0.5,3.0]
  if((params[33]<0.5/params[22])||(params[33]>3.0/params[22])){
    printf("PVR - ");
    answer = false;
    return answer;
  }

  // SET OTHER PARAMS

  // Rcoll
  if(params[18]<0.0){
    printf("Rcoll - ");
    answer = false;
    return answer;    
  }

  // Cp
  if(params[19]<0.0){
    printf("Cp - ");
    answer = false;
    return answer;
  }

  // UBSVR
  if(params[20]<0.0){
    printf("UBSVR - ");
    answer = false;
    return answer;
  }

  // LBSVR
  if(params[21]<0.0){
    printf("LBSVR - ");
    answer = false;
    return answer;
  }

  // PVR
  if(params[22]<0.0){
    printf("PVR - ");
    answer = false;
    return answer;
  }

  // Ctot_UBSVR
  if(params[23]<0.0){
    printf("Ctot-UBSVR - ");
    answer = false;
    return answer;
  }
  
  // C1C2ratio_UBSVR
  if(params[24]<0.0){
    printf("C1C2ratio-UBSVR - ");
    answer = false;
    return answer;
  }
  
  // R1R2ratio_UBSVR
  if(params[25]<0.0){
    printf("R1R2ratio-UBSVR - ");
    answer = false;
    return answer;
  }
  
  // R3R2ratio_UBSVR
  if(params[26]<0.0){
    printf("R3R2ratio-UBSVR - ");
    answer = false;
    return answer;
  }
  
  // L_UBSVR
  if(params[27]<0.0){
    printf("L-UBSVR - ");
    answer = false;
    return answer;
  }

  // Ctot_LBSVR
  if(params[28]<0.0){
    printf("Ctot-LBSVR - ");
    answer = false;
    return answer;
  }
  
  // C1C2ratio_LBSVR
  if(params[29]<0.0){
    printf("C1C2ratio-LBSVR - ");
    answer = false;
    return answer;
  }
  
  // R1R2ratio_LBSVR
  if(params[30]<0.0){
    printf("R1R2ratio-LBSVR - ");
    answer = false;
    return answer;
  }
  
  // R3R2ratio_LBSVR
  if(params[31]<0.0){
    printf("R3R2ratio-LBSVR - ");
    answer = false;
    return answer;
  }
  
  // L_LBSVR
  if(params[32]<0.0){
    printf("L_LBSVR - ");
    answer = false;
    return answer;
  }  
  
  // Ctot_PVR
  if(params[33]<0.0){
    printf("Ctot-PVR - ");
    answer = false;
    return answer;
  }
  
  // C1C2ratio_PVR
  if(params[34]<0.0){
    printf("C1C2ratio-PVR - ");
    answer = false;
    return answer;
  }
  
  // R1R2ratio_PVR
  if(params[35]<0.0){
    printf("R1R2ratio-PVR - ");
    answer = false;
    return answer;
  }
  
  // R3R2ratio_PVR
  if(params[36]<0.0){
    printf("R3R2ratio-PVR - ");
    answer = false;
    return answer;
  }
  
  // L_PVR
  if(params[37]<0.0){
    printf("L-PVR - ");
    answer = false;
    return answer;
  }
  
  // k_bw
  if(params[38]<0.0){
    printf("k-bw - ");
    answer = false;
    return answer;
  }

  // Return the answer
  return answer;
}

// ==============
// EXCTRACT PEAKS
// ==============
bool extractSMDAPeaks(int size, double* t, double* Qvc,
                      double &Qvc_S, double &t_S,
                      double &Qvc_M, double &t_M,
                      double &Qvc_D, double &t_D,
                      double &Qvc_A, double &t_A){

  // Detect Peaks
  int error = 0;
  try{
    error = detect_peak(size,t,Qvc,t_S,Qvc_S,t_M,Qvc_M,t_D,Qvc_D,t_A,Qvc_A);  
  }catch(...){
    error = 1;
  }
  
  // Return
  if(error == 0){
    return true;
  }else{
    return false;
  }  
}

// ==========================================
// MAIN FUNCTION FOR STAGE1-2BLOCKS LPN MODEL
// ==========================================
int solveStage2LPN(int modelID, lpnOptions options, int totalStates, double* iniVals,int totalParams, double* params, double* results){


  // SET PARAMETERS
  // FIXED HR
  double heartRate = params[0]; // FIXED HEART RATE
  double cycleTime = 60.0/(double)heartRate;

  // TIME RESULTS
  int numCycles = 8;
  double totalTime = numCycles*cycleTime;
  double timeStep = 0.0008;
  int totalSteps = floor(totalTime/(double)timeStep);
  int totalStepsOnSingleCycle = floor(cycleTime/(double)timeStep);

  // Write Debug Info
  if(options.flushData){
    // Print Parameter Table
    printParamTable(cycleTime,numCycles,totalTime,timeStep,totalSteps,totalStepsOnSingleCycle,params,iniVals);
    // Print Parameter Limits Table
    printParamLimitsTable(modelID,totalStates+totalParams);
  }

  // CHECK PARAMS INSIDE LIMITS
  if(options.useFullConstraints){
    if(!areParamsInsideLimits(modelID,totalStates,iniVals,totalParams,params)){
      printf("ERROR: VIOLATED PARAMETER LIMITS.\n");
      return 1;
    }
  }else{
    // CHECK PARAMS INSIDE LIMITS
    if(!isParamsSignCorrect(modelID,totalStates,iniVals,totalParams,params)){
      printf("ERROR: VIOLATED PARAMETER SIGN.\n");
      return 1;
    }
  }

  // TIME STEP ARRAY
  double t[totalSteps];
  t[0] = 0.0;
  for(int loopA=1;loopA<totalSteps;loopA++){
    t[loopA] = t[loopA-1] + timeStep;
  }

  // NUMBER OF AUXILIARY VARIABLES
  int totAuxStates = 14;

  // NUMBER OF RESULTS
  int totalResults = getModelResultTotal();

  // State Variables
  double** outVals = NULL;
  outVals = new double*[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    outVals[loopA] = new double[totalStates];
  }

  // Auxiliary Outputs
  double** auxOutVals;
  auxOutVals = new double*[totalSteps];
  for(int loopA=0;loopA<totalSteps;loopA++){
    auxOutVals[loopA] = new double[totAuxStates];
  }

  // Solve All Cycles
  PerformRK4Steps(modelID,totalSteps,timeStep,totalStates,totAuxStates,iniVals,totalParams,params,outVals,auxOutVals);

  if(options.writeDebugData){
    WriteResultsToFile(std::string("states_output.txt"),totalSteps,totalStates,t,outVals);
    WriteResultsToFile(std::string("auxStates_output.txt"),totalSteps,totAuxStates,t,auxOutVals);
  }

  // Copy Auxiliary Variables From result for the last heart cycle
  double Psv[totalStepsOnSingleCycle];
  double Psa[totalStepsOnSingleCycle];
  double QAo[totalStepsOnSingleCycle];
  double Qvp[totalStepsOnSingleCycle];
  double Qsvc[totalStepsOnSingleCycle];
  double Qcoll[totalStepsOnSingleCycle];
  double Qivc[totalStepsOnSingleCycle];
  double Psaactive[totalStepsOnSingleCycle];
  double Psapassive[totalStepsOnSingleCycle];
  double Psvactive[totalStepsOnSingleCycle];
  double Psvpassive[totalStepsOnSingleCycle];
  double Qreg[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Psv[loopA]        = auxOutVals[globalInd][ipPsv];
    Psa[loopA]        = auxOutVals[globalInd][ipPsa];
    QAo[loopA]        = auxOutVals[globalInd][ipQAo];
    Qvp[loopA]        = auxOutVals[globalInd][ipQvp];
    Qsvc[loopA]       = auxOutVals[globalInd][ipQsvc];
    Qcoll[loopA]      = auxOutVals[globalInd][ipQcoll];
    Qivc[loopA]       = auxOutVals[globalInd][ipQivc];
    Psaactive[loopA]  = auxOutVals[globalInd][ipPsaactive];
    Psapassive[loopA] = auxOutVals[globalInd][ipPsapassive];
    Psvactive[loopA]  = auxOutVals[globalInd][ipPsvactive];
    Psvpassive[loopA] = auxOutVals[globalInd][ipPsvpassive];
    Qreg[loopA]       = auxOutVals[globalInd][ipQreg];
  }

  // Copy State Variables From result for the last heart cycle
  double Vsv[totalStepsOnSingleCycle];
  double Vsa[totalStepsOnSingleCycle];
  double PAo[totalStepsOnSingleCycle]; 
  double Qav[totalStepsOnSingleCycle];
  double Pubv[totalStepsOnSingleCycle];
  double Quba[totalStepsOnSingleCycle];
  double Puba[totalStepsOnSingleCycle];
  double Plung[totalStepsOnSingleCycle];
  double Pvp[totalStepsOnSingleCycle];
  double Qapol[totalStepsOnSingleCycle];
  double Ppa[totalStepsOnSingleCycle];
  double Qlba[totalStepsOnSingleCycle];
  double Plba[totalStepsOnSingleCycle];
  double Plbv[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Vsv[loopA]   = outVals[globalInd][ipVsv];
    Vsa[loopA]   = outVals[globalInd][ipVsa];
    PAo[loopA]   = outVals[globalInd][ipPAo];
    Qav[loopA]   = outVals[globalInd][ipQav];
    Pubv[loopA]  = outVals[globalInd][ipPubv];
    Quba[loopA]  = outVals[globalInd][ipQuba];
    Puba[loopA]  = outVals[globalInd][ipPuba];
    Plung[loopA] = outVals[globalInd][ipPlung];
    Pvp[loopA]   = outVals[globalInd][ipPvp];
    Qapol[loopA] = outVals[globalInd][ipQapol];
    Ppa[loopA]   = outVals[globalInd][ipPpa];
    Qlba[loopA]  = outVals[globalInd][ipQlba];
    Plba[loopA]  = outVals[globalInd][ipPlba];
    Plbv[loopA]  = outVals[globalInd][ipPlbv];
  }

  // =========================
  // CHECK FURTHER CONSTRAINTS
  // =========================
  if(options.useHeartConstraints){
    double maxDiff = 0.0;
    double currDiff = 0.0;
    for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
      currDiff = fabs(Psaactive[loopA]-Psapassive[loopA]);  
      if(currDiff>maxDiff){
        maxDiff = currDiff;
      }
    }
    if(maxDiff<3.0){
      return 1;
    }    
  }

  // ===============================
  // CHECK POSITION OF MAX PSVACTIVE
  // ===============================
  int maxInd = 0;
  if(options.useHeartConstraints){
    maxInd = getMaxLoc(totalSteps,totalSteps-totalStepsOnSingleCycle+1,Psvactive);
    double maxVsv = getMax(totalSteps,totalSteps-totalStepsOnSingleCycle+1,Psv);
    if((Vsv[maxInd]-maxVsv)<-1.0e-3){
      return 1;
    }
  }

  // EVALUATE AVERAGE AND EXTREME VALUES
  // Integrals
  double CO = trapz(0,totalStepsOnSingleCycle,t,QAo)/(double)cycleTime; // Cardiac Output
  double mPsa = trapz(0,totalStepsOnSingleCycle,t,Psa)/(double)cycleTime;    // Single Atrium
  double mPascAo = trapz(0,totalStepsOnSingleCycle,t,PAo)/(double)cycleTime; // Aorta
  double mPpa = trapz(0,totalStepsOnSingleCycle,t,Ppa)/(double)cycleTime;    // Pulmonary Arteries
  double mQapol = trapz(0,totalStepsOnSingleCycle,t,Qapol)/(double)cycleTime;
  double mQvp = trapz(0,totalStepsOnSingleCycle,t,Qvp)/(double)cycleTime;
  double mQcoll = trapz(0,totalStepsOnSingleCycle,t,Qcoll)/(double)cycleTime;
  double mQreg = trapz(0,totalStepsOnSingleCycle,t,Qreg)/(double)cycleTime;
  // Extreme Values
  double maxPAo = getMax(0,totalStepsOnSingleCycle,PAo);
  double minPAo = getMin(0,totalStepsOnSingleCycle,PAo);
  double maxPsv = getMax(0,totalStepsOnSingleCycle,Psv);
  double maxPsa = getMax(0,totalStepsOnSingleCycle,Psa);

  double maxVsv = getMax(0,totalStepsOnSingleCycle,Vsv);
  double minVsv = getMin(0,totalStepsOnSingleCycle,Vsv);
  double maxQAo = getMax(0,totalStepsOnSingleCycle,QAo);
  double minQsvc = getMin(0,totalStepsOnSingleCycle,Qsvc);
  double maxQsvc = getMax(0,totalStepsOnSingleCycle,Qsvc);

  double minPpa = getMin(0,totalStepsOnSingleCycle,Ppa);
  double maxPpa = getMax(0,totalStepsOnSingleCycle,Ppa);

  // ==============
  // PEAK DETECTION
  // ====================================
  // APPLY CIRCULAR SHIFT TO MODEL CURVES
  // ====================================
  double Qivc_S,Qivc_M,Qivc_D,Qivc_A;
  double tic_S,tic_M,tic_D,tic_A;
  double Qvp_S,Qvp_M,Qvp_D,Qvp_A;    
  double tp_S,tp_M,tp_D,tp_A;

  // Iterate on shifted Qvc Until Peaks are Detected
  int shiftValue = 0;
  if(modelID == piMUSC9){
    shiftValue = 30;
  }
  int pfCount = 0;
  bool isValidQivc = false;
  while((!isValidQivc)&&(pfCount<10)){
    isValidQivc = extractSMDAPeaks(totalStepsOnSingleCycle,t,Qivc,Qivc_S,tic_S,Qivc_M,tic_M,Qivc_D,tic_D,Qivc_A,tic_A);
    if(!isValidQivc){
      shiftValue += floor(totalStepsOnSingleCycle/10);
      applyCircShift(shiftValue,totalStepsOnSingleCycle,Qivc);  
    }
    pfCount++;
  }

  // Iterate on shifted Qvp Until Peaks are Detected
  shiftValue = 0;
  if(modelID == piMUSC9){
    shiftValue = 30;
  }
  pfCount = 0;
  bool isValidQvp = false;
  while((!isValidQvp)&&(pfCount<10)){
    isValidQvp = extractSMDAPeaks(totalStepsOnSingleCycle,t,Qvp,Qvp_S,tp_S,Qvp_M,tp_M,Qvp_D,tp_D,Qvp_A,tp_A);
    if(!isValidQvp){
      shiftValue += floor(totalStepsOnSingleCycle/10);
      applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvp); 
    }
    pfCount++;
  }

  // IF INVALID SHAPE EXIT
  if((!isValidQivc)||(!isValidQvp)){    
    // PRINT CURVES THAT FAIL PEAK DETECTION
    /*char simulString[15];
    sprintf(simulString, "%d", simulCount);
    string fileNameQivc("ivcCurve_"+string(simulString)+".txt");
    string fileNameQsvc("svpCurve_"+string(simulString)+".txt");
    string fileNameQvp("vpCurve_"+string(simulString)+".txt");
    printCurvesAndPeaks(fileNameQivc,totalStepsOnSingleCycle,t,Qivc,Qivc_S,tic_S,Qivc_M,tic_M,Qivc_D,tic_D,Qivc_A,tic_A);
    printCurvesAndPeaks(fileNameQvp,totalStepsOnSingleCycle,t,Qvp,Qvp_S,tp_S,Qvp_M,tp_M,Qvp_D,tp_D,Qvp_A,tp_A);*/
    printf("ERROR: INVALID PEAKS SMDA.\n");
    // FREE MEMORY
    for(int loopA=0;loopA<totalSteps;loopA++){
      delete [] outVals[loopA];
      delete [] auxOutVals[loopA];
    }
    delete [] outVals;
    delete [] auxOutVals;
    // Return
    return 1;
  }

  // TIME DELAYS
  double ivc_SD_time = fabs(tic_S-tic_D);
  double ivc_MA_time = fabs(tic_M-tic_A);
  double vp_SD_time = fabs(tp_S-tp_D);
  double vp_MA_time = fabs(tp_M-tp_A);

  // ================================
  // WRITE CURVES AND PEAKS FOR DEBUG
  // ================================
  if(options.writeDebugData){
    char simulString[15];
    sprintf(simulString, "%d", simulCount);
    string fileNameQivc("ivcCurve_"+string(simulString)+".txt");
    string fileNameQsvc("svpCurve_"+string(simulString)+".txt");
    string fileNameQvp("vpCurve_"+string(simulString)+".txt");
    printCurvesAndPeaks(fileNameQivc,totalStepsOnSingleCycle,t,Qivc,Qivc_S,tic_S,Qivc_M,tic_M,Qivc_D,tic_D,Qivc_A,tic_A);
    printCurvesAndPeaks(fileNameQvp,totalStepsOnSingleCycle,t,Qvp,Qvp_S,tp_S,Qvp_M,tp_M,Qvp_D,tp_D,Qvp_A,tp_A);
  }

  // =====================
  // PLOT DATA IF REQUIRED
  // =====================
  if(options.flushData){
    string outFileName("Stage2ModelData.txt");
    flushHeartModelDataToFile(outFileName,totalSteps-3*totalStepsOnSingleCycle,totalSteps,t,outVals,auxOutVals);
  }    

  // Copy Final Results
  results[0] = mPascAo;
  results[1] = minPAo;
  results[2] = maxPAo;
  results[3] = mPsa;
  results[4] = maxPsv;
  results[5] = maxVsv;
  results[6] = mQvp/(CO-mQcoll);
  results[7] = mPpa;
  results[8] = CO;
  results[9] = mQcoll;
  results[10] = Qivc_S;
  results[11] = Qivc_M;
  results[12] = Qivc_D;
  results[13] = Qivc_A;
  results[14] = minQsvc;
  results[15] = maxQsvc;
  results[16] = Qvp_S;
  results[17] = Qvp_M;
  results[18] = Qvp_D;
  results[19] = Qvp_A;
  // Time Delays
  results[20] = ivc_SD_time;
  results[21] = ivc_MA_time;
  results[22] = vp_SD_time;
  results[23] = vp_MA_time;
  // Aux Results
  results[24] = minPpa;
  results[25] = maxPpa;
  results[26] = mQreg;
  results[27] = Psv[0];
  results[28] = maxPsa;  

  // FREE MEMORY
  for(int loopA=0;loopA<totalSteps;loopA++){
    delete [] outVals[loopA];
    delete [] auxOutVals[loopA];
  }
  delete [] outVals;
  delete [] auxOutVals;

  // Solution Successful
  return 0;
}

// ==============================
// GET TARGETS FOR VARIOUS MODELS
// ==============================
int getAVSD(int model,int res_num,double* meanData,double* stdData,double* weights){
  switch(model){
    case piMUSC9:
      // Average
      meanData[0] = 56.0;   // mPascAo
      meanData[1] = 45.0;   // minPAo
      meanData[2] = 70.0;   // maxPAo
      meanData[3] = 7.0;    // mPsa
      meanData[4] = 68.0;   // maxPsv
      meanData[5] = 42.0;   // maxVsv
      meanData[6] = 0.98;   // QpQs
      meanData[7] = 10.0;   // mPpa
      meanData[8] = 38.2;   // CO
      meanData[9] = 12.6;   // Qcoll
      meanData[10] = 27.4;  // IVC_S
      meanData[11] = 0.64;  // IVC_M
      meanData[12] = 12.3;  // IVC_D
      meanData[13] = 8.2;   // IVC_A
      meanData[14] = 8.0;   // SVC_minQ
      meanData[15] = 19.5;  // SVC_maxQ
      meanData[16] = 37.0;  // PVs_S
      meanData[17] = 13.6;  // PVs_M
      meanData[18] = 35.6;  // PVs_D
      meanData[19] = 25.9;  // PVs_A
      // Time Delays
      meanData[20] = 0.35; // ivc_SD_time;
      meanData[21] = 0.13; // ivc_MA_time;
      meanData[22] = 0.43; // vp_SD_time;
      meanData[23] = 0.22; // vp_MA_time;
      // Min e Max Ppa
      meanData[24] = 4.1;  // minPpa
      meanData[25] = 11.7;  // maxPpa
      // Qreg
      meanData[26] = 10.5;  // mQreg
      // PsvZero and maxPsa
      meanData[27] = 8.9; // PsvZero
      meanData[28] = 8.3; // maxPsa
      
      // STD DEV
      stdData[27] = 1.6;
      stdData[28] = 1.2;

      break;
    case piGOSH30:
      // Average
      meanData[0] = 57.0;  // mPascAo
      meanData[1] = 30.0;  // minPAo
      meanData[2] = 75.0;  // maxPAo
      meanData[3] = 6.0;   // mPsa
      meanData[4] = 60.0;  // maxPsv
      meanData[5] = 50.0;  // maxVsv
      meanData[6] = 0.83;  // QpQs
      meanData[7] = 11.0;  // mPpa
      meanData[8] = 47.0;  // CO
      meanData[9] = 7.0;   // Qcoll
      meanData[10] = 44.5;  // IVC_S
      meanData[11] = -7.0;  // IVC_M
      meanData[12] = 37.2;  // IVC_D
      meanData[13] = -19.0; // IVC_A
      meanData[14] = 19.5;  // SVC_minQ
      meanData[15] = 34.5;  // SVC_maxQ
      meanData[16] = 54.4;  // PVs_S
      meanData[17] = 20.4;  // PVs_M
      meanData[18] = 60.5;  // PVs_D
      meanData[19] = 7.2;   // PVs_A
      // Time Delays
      meanData[20] = 0.43; // ivc_SD_time;
      meanData[21] = 0.26; // ivc_MA_time;
      meanData[22] = 0.43; // vp_SD_time;
      meanData[23] = 0.31; // vp_MA_time;
      // Min e Max Ppa
      meanData[24] = 0.0;  // minPpa
      meanData[25] = 0.0;  // maxPpa
      // Qreg
      meanData[26] = 3.4;  // mQreg
      // PsvZero and maxPsa
      meanData[27] = 0.0; // PsvZero
      meanData[28] = 0.0; // maxPsa
      
      // STD DEV
      stdData[27] = 0.0;
      stdData[28] = 0.0;
      break;
  }

  // ====================
  // STD - 5// UNCERTAINTY
  // ====================
  for(int loopA=0;loopA<(res_num-2);loopA++){
    stdData[loopA] = fabs(DATA_UNCERTAINTY*meanData[loopA]);
  }

  // IF PEAK TIMES NEED NOT BE USED, SET ZERO STD
  if(!USE_PEAKTIME_ASTARGETS){
    // Time Delays
    stdData[20] = 0.0; // ivc_SD_time;
    stdData[21] = 0.0; // ivc_MA_time;
    stdData[22] = 0.0; // vp_SD_time;
    stdData[23] = 0.0; // vp_MA_time;
  }

  // EXCLUDE INVASIVE TARGETS
  if(EXCLUDE_INVASIVE_TARG){
    stdData[3] = 0.0;   // mPsa
    stdData[4] = 0.0;  // maxPsv
    stdData[7] = 0.0;  // mPpa
    // Min e Max Ppa
    stdData[24] = 0.0;  // minPpa
    stdData[25] = 0.0;  // maxPpa
    // PsvZero and maxPsa
    stdData[27] = 0.0; // PsvZero
    stdData[28] = 0.0; // maxPsa
  }

  // ==============
  // ASSIGN WEIGHTS
  // ==============
  weights[0] = 0.5;
  weights[1] = 1.0;
  weights[2] = 1.0;
  weights[3] = 1.0;
  weights[4] = 1.0;
  weights[5] = 1.0;
  weights[6] = 1.0;
  weights[7] = 1.0;
  weights[8] = 0.5;
  weights[9] = 1.0;
  weights[10] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[11] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[12] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[13] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[14] = SECONDARY_CURVEPEAK_WEIGHT;
  weights[15] = SECONDARY_CURVEPEAK_WEIGHT;
  weights[16] = SECONDARY_CURVEPEAK_WEIGHT;
  weights[17] = SECONDARY_CURVEPEAK_WEIGHT;
  weights[18] = SECONDARY_CURVEPEAK_WEIGHT;
  weights[19] = SECONDARY_CURVEPEAK_WEIGHT;
  // Time Delays
  weights[20] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[21] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[22] = PRIMARY_CURVEPEAK_WEIGHT;
  weights[23] = PRIMARY_CURVEPEAK_WEIGHT;
  // Extra targets
  weights[24] = 1.0;
  weights[25] = 1.0;
  weights[26] = 1.0;
  weights[27] = 1.0;
  weights[28] = 1.0;

  // RETURN
  return 0;
}

// ============================
// EVAL LOG-LIKELIHOOD FUNCTION
// ============================
double evalModelLogLikelihood(int model, lpnOptions options, int res_num, double* results){

  // Get Average Values and Standard Deviation for Measurements
  double meanData[res_num];
  double stdData[res_num];
  double weights[res_num];
  int error = getAVSD(model,res_num,meanData,stdData,weights);
  if(error != 0){
    printf("Invalid Result Data. Terminating.\n");
    exit(1);
  }

  // PRINT TABLE WITH TARGETS
  if(options.flushData){
    FILE* outFile;
    string resName;
    outFile = fopen("targetTable.txt","w");
    // Print Header
    fprintf(outFile,"\\begin{table}[h!]\n");
    fprintf(outFile,"\\centering\n");
    fprintf(outFile,"\\begin{tabular}{l c c c c}\n");
    fprintf(outFile,"\\toprule\n");
    fprintf(outFile,"Target Quantity & Model Results & Target AV & Target SD & Weight\\\\\n");
    fprintf(outFile,"\\midrule\n");
    for(int loopA=0;loopA<res_num;loopA++){
      resName = getResultName(loopA);
      fprintf(outFile,"%20s & %20f &  %20f & %20f & %20f\\\\\n",resName.c_str(),results[loopA],meanData[loopA],stdData[loopA],weights[loopA]);
    }
    fprintf(outFile,"\\bottomrule\n");
    fprintf(outFile,"\\end{tabular}\n");
    fprintf(outFile,"\\caption{Full model (Stage2) results for patient \\patient}\n");
    fprintf(outFile,"\\end{table}\n");
    // CLOSE THE FILE
    fclose(outFile);
  }

  // Compute the Log-likelihood
  double logLH = 0.0;
  double meanDiff = 0.0;
  for(int loopA=0;loopA<res_num;loopA++){
    meanDiff = results[loopA]-meanData[loopA];
    if(stdData[loopA]>1.0e-8){
      logLH += (meanDiff*meanDiff)/(weights[loopA]*stdData[loopA]*stdData[loopA]);  
    }
  }

  // Return Value
  return logLH;
  
}

// =====================================================
// MAIN FUNCTION: SOLVE THE MODEL AND GET THE LIKELIHOOD
// =====================================================
double eval_LPN_loglikelihood(int model, int state_num, int par_num, int res_num, double* params, int &error){
  
  // OPTIONS  
  lpnOptions options;
  options.writeDebugData = false;
  options.flushData = true;
  options.useHeartConstraints = false;
  options.useFullConstraints = true;

  // Partition Parameters and Initial Conditions Based on state_num and par_num
  int totalStates = state_num;
  int totalParams = par_num - state_num;
  // Initial Values
  double iniVals[totalStates];
  for(int loopA=0;loopA<totalStates;loopA++){
    iniVals[loopA] = params[totalParams+loopA];
  }
  // Parameters
  double paramsVals[totalParams];
  for(int loopA=0;loopA<totalParams;loopA++){
    paramsVals[loopA] = params[loopA];
  }
  // Results
  double results[res_num];
  for(int loopA=0;loopA<res_num;loopA++){
    results[loopA] = 0.0;
  }

  // SOLVE LPN MODEL   
  error = 0;
  string outCome;
  try{
    error = solveStage2LPN(model,options,totalStates,iniVals,totalParams,paramsVals,results);
  }catch(...){
    error = 1;
  }

  // COMPUTE THE POSTERIOR DISTRIBUTION
  double logLIKE = 0.0;
  if(error == 0){
    logLIKE = evalModelLogLikelihood(model,options,res_num,results);    
  }else{
    logLIKE = 10000.0;
  }

  // PRINT MESSAGE
  if(error == 0){
    printf("Model Results: %d %s, LL: %f\n",simulCount,"OK",logLIKE);	
  }else{
  	printf("Model Results: %d %s, LL: %f\n",simulCount,"ERROR",logLIKE);	
  }
  simulCount++;
  
  // RETURN POSTERIOR VALUE
  return logLIKE;
}