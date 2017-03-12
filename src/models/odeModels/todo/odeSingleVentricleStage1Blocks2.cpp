# include "odeSingleVentricleStage1Blocks2.h"

using namespace std;

// SIMULATION COUNTER
int simulCount = 0;

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleStage1Blocks2::getModelParameterTotal(){
  return 44;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleStage1Blocks2::getModelStateTotal(){
  return 11; 
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int odeSingleVentricleStage1Blocks2::getModelResultTotal(){
  return 26;  
}

// =====================================================
// GET NAME OF RESULT CURVE FOR EVALUATE THE UNCERTAINTY
// =====================================================
void odeSingleVentricleStage1Blocks2::getModelResultCurveFileNames(vector<string> &curve_names){
  curve_names.push_back(string("Stage12BlocksModelData.txt"));
  curve_names.push_back(string("Stage12BlocksModelPVLoops.txt"));
  curve_names.push_back(string("Stage12BlocksModelAV.txt"));
}

// ==================
// GET PARAMETER NAME
// ==================
string odeSingleVentricleStage1Blocks2::getParamName(int index){
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
      return string("Dsh");
      break;
    }
    case 19:           
    {      
      return string("Cp");
      break;
    }
    case 20:           
    {      
      return string("SVR");
      break;
    }
    case 21:           
    {      
      return string("PVR");
      break;
    }
    case 22:           
    {      
      return string("Ctot-SVR");
      break;
    }
    case 23:           
    {      
      return string("C1C2ratio-SVR");
      break;
    }
    case 24:           
    {      
      return string("R1R2ratio-SVR");
      break;
    }
    case 25:           
    {      
      return string("R3R2ratio-SVR");
      break;
    }
    case 26:           
    {      
      return string("L-SVR");
      break;
    }
    case 27:           
    {      
      return string("Ctot-PVR");
      break;
    }
    case 28:           
    {      
      return string("C1C2ratio-PVR");
      break;
    }
    case 29:           
    {      
      return string("R1R2ratio-PVR");
      break;
    }
    case 30:           
    {      
      return string("R3R2ratio-PVR");
      break;
    }
    case 31:           
    {
      return string("L-PVR");
      break;
    }
    case 32:           
    {      
      return string("k-bw");
      break;
    }
    case 33:           
    {      
      return string("Vsv");
      break;
    }
    case 34:           
    {      
      return string("Vsa");
      break;
    }
    case 35:           
    {      
      return string("Pao");
      break;
    }
    case 36:           
    {      
      return string("Qav");
      break;
    }
    case 37:           
    {      
      return string("Pubv");
      break;
    }
    case 38:           
    {      
      return string("Quba");
      break;
    }
    case 39:           
    {      
      return string("Puba");
      break;
    }
    case 40:           
    {      
      return string("Plung");
      break;
    }
    case 41:           
    {      
      return string("Pvp");
      break;
    }
    case 42:           
    {      
      return string("Qapol");
      break;
    }
    case 43:           
    {      
      return string("Ppa");
      break;
    }
  }
}

// ===============
// GET RESULT NAME
// ===============
string odeSingleVentricleStage1Blocks2::getResultName(int index){
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
      return string("SVC-IVC-S");
      break;
    }
    case 10:
    {      
      return string("SVC-IVC-M");
      break;
    }
    case 11:
    {      
      return string("SVC-IVC-D");
      break;
    }
    case 12:
    {      
      return string("SVC-IVC-A");
      break;
    }
    case 13:
    {      
      return string("PVs-S");
      break;
    }
    case 14:
    {      
      return string("PVs-M");
      break;
    }
    case 15:
    {      
      return string("PVs-D");
      break;
    }
    case 16:
    {      
      return string("PVs-A");
      break;
    }
    case 17:
    {      
      return string("Qvc-SD-time");
      break;
    }   
    case 18:
    {      
      return string("Qvc-MA-time");
      break;
    }   
    case 19:
    {      
      return string("Qvp-SD-time");
      break;
    }   
    case 20:
    {      
      return string("Qvp-MA-time");
      break;
    }       
    case 21:
    {      
      return string("minPpa");
      break;
    }    
    case 22:
    {      
      return string("maxPpa");
      break;
    } 
    case 23:
    {      
      return string("mQreg");
      break;
    } 
    case 24:
    {      
      return string("PsvZero");
      break;
    }    
    case 25:
    {      
      return string("maxPsa");
      break;
    }       
  }
}

// ====================
// GET MODEL PARAMETERS
// ====================
void odeSingleVentricleStage1Blocks2::getDefaultModelParams(int modelID, double* zp){
  switch(modelID){
  	case piMUSC7:
      zp[0] = 117.65; zp[1] = 0.76569; zp[2] = 0.01; zp[3] = 0.8; zp[4] = 2.5;
      zp[5] = 0.2186; zp[6] = 0.1499; zp[7] = -0.13626; zp[8] = 13.539; zp[9] = 0.23373;
      zp[10] = 0.065129; zp[11] = 9.7786; zp[12] = 0.3109; zp[13] = 0.10; zp[14] = 24.474;
      zp[15] = 0.00025; zp[16] = 0.20444; zp[17] = 0.95; zp[18] = 3.0451; zp[19] = 0.31362;
      zp[20] = 3.07; zp[21] = 0.75; zp[22] = 0.76713; zp[23] = 2.8799; zp[24] = 0.63254; 
      zp[25] = 0.1; zp[26] = 0.0069709; zp[27] = 0.97019; zp[28] = 10.0; zp[29] = 0.1;
      zp[30] = 1.0; zp[31] = 0.025568; zp[32] = 0.0; zp[33] = 38.869; zp[34] = 2.5537; zp[35] = 28.247;
      zp[36] = -0.1; zp[37] = 24.2; zp[38] = 12.6; zp[39] = 16.5; zp[40] = 18.7; zp[41] = 11.7;
      zp[42] = 13.5; zp[43] = 11.0;
  	  break;
  	case piGOSH22:
      zp[0] = 116.0; zp[1] = 2.7774; zp[2] = 0.24636; zp[3] = 0.19095; zp[4] = 0.52131; zp[5] = 0.23192; zp[6] = 0.022728;
      zp[7] = -0.14473; zp[8] = 14.46; zp[9] = 2.7707; zp[10] = 0.068861; zp[11] = 11.559; zp[12] = 0.3186; zp[13] = 0.22097;
      zp[14] = 8.0573; zp[15] = 0.0015718; zp[16] = 0.25125; zp[17] = 14.601; zp[18] = 3.2431; zp[19] = 0.40103; zp[20] = 3.15;
      zp[21] = 0.301; zp[22] = 0.92242; zp[23] = 0.56175; zp[24] = 0.9639; zp[25] = 0.034549; zp[26] = 0.037443; zp[27] = 1.9167;
      zp[28] = 19.224; zp[29] = 0.12617; zp[30] = 1.0405; zp[31] = 0.017635; zp[32] = 0.0; zp[33] = 36.722; zp[34] = 1.4662;
      zp[35] = 50.567; zp[36] = 1.5078; zp[37] = 18.917; zp[38] = 14.382; zp[39] = 18.328; zp[40] = 9.5184; zp[41] = 15.787;
      zp[42] = 16.15; zp[43] = 10.58; 
      break;
  	case piUM5:
      zp[0] = 90.0; zp[1] = 0.65482; zp[2] = 0.76547; zp[3] = 0.54194; zp[4] = 2.0569; zp[5] = 0.24071; zp[6] = 0.032823;
      zp[7] = -0.15223; zp[8] = 12.384; zp[9] = 0.80805; zp[10] = 0.069646; zp[11] = 9.4388; zp[12] = 0.31955; zp[13] = 0.10937;
      zp[14] = 5.3335; zp[15] = 0.00023445; zp[16] = 0.25036; zp[17] = 0.022262; zp[18] = 3.0456; zp[19] = 0.11745; zp[20] = 3.17;
      zp[21] = 0.663; zp[22] = 0.94567; zp[23] = 0.24461; zp[24] = 0.68369; zp[25] = 0.10636; zp[26] = 0.022719; zp[27] = 2.2831;
      zp[28] = 15.957; zp[29] = 0.30981; zp[30] = 2.4163; zp[31] = 0.013738; zp[32] = 0.0; zp[33] = 32.495; zp[34] = 3.5327;
      zp[35] = 76.334; zp[36] = -0.40646; zp[37] = 26.764; zp[38] = 7.9357; zp[39] = 4.5646; zp[40] = 3.4097; zp[41] = 34.125;
      zp[42] = 7.03; zp[43] = 2.8644;
  	  break;
  	case piUM10:
      zp[0] = 141.0;  zp[1] = 0.13045; zp[2] = 0.034882; zp[3] = 0.60304; zp[4] = 5.5452; zp[5] = 0.2257; zp[6] = -0.0013973;
      zp[7] = -0.020308; zp[8] = 14.297; zp[9] = 0.4043; zp[10] = 0.062005; zp[11] = 7.7254; zp[12] = 0.31886; zp[13] = 0.22595;
      zp[14] = 3.4482; zp[15] = 0.00039138; zp[16] = 0.13265; zp[17] = 9.5459; zp[18] = 3.482; zp[19] = 0.19547; zp[20] = 6.18; zp[21] = 0.5;
      zp[22] = 0.36465; zp[23] = 1.4643; zp[24] = 0.38325; zp[25] = 0.0070007; zp[26] = 0.00062253; zp[27] = 3.3389; zp[28] = 7.4533;
      zp[29] = 0.18924; zp[30] = 1.9972; zp[31] = 0.038933; zp[32] = 0.033135; zp[33] = 16.841; zp[34] = 14.476; zp[35] = 51.572;
      zp[36] = 10.222; zp[37] = 22.117; zp[38] = 13.866; zp[39] = 3.9209; zp[40] = 15.963; zp[41] = 11.414; zp[42] = 4.6516;
      zp[43] = 16.443;
  	  break;
  	case piVIRT:
      zp[0] = 117.65; zp[1] = 0.76569; zp[2] = 0.01; zp[3] = 0.8; zp[4] = 2.5;
      zp[5] = 0.25; zp[6] = 0.1; zp[7] = -0.12; zp[8] = 14; zp[9] = 1;
      zp[10] = 0.065; zp[11] = 9.7786; zp[12] = 0.31; zp[13] = 0.1; zp[14] = 24.474;
      zp[15] = 0.00025; zp[16] = 0.24; zp[17] = 0.95; zp[18] = 3.0451; zp[19] = 0.31362;
      zp[20] = 3.07; zp[21] = 0.75; zp[22] = 0.76713; zp[23] = 5; zp[24] = 0.9; 
      zp[25] = 0.1; zp[26] = 0.0069709; zp[27] = 0.97019; zp[28] = 5; zp[29] = 0.1;
      zp[30] = 1.0; zp[31] = 0.025568; zp[32] = 0.0; zp[33] = 38.869; zp[34] = 2.5537; zp[35] = 38;
      zp[36] = 50.0; zp[37] = 22.0; zp[38] = 14.0; zp[39] = 15.0; zp[40] = 16.0; zp[41] = 13.0;
      zp[42] = 15.0; zp[43] = 10.0;
  	  break;
  }
}

// ====================
// GET PARAMETER RANGES
// ====================
void odeSingleVentricleStage1Blocks2::getModelLimits(int model,int par_num,double* limits){

  // COMMON PARAMETERS
  limits[2]=0.05; limits[3]=2.0; // Csa
  limits[4]=0.01; limits[5]=0.8; // csa
  limits[6]=0.16; limits[7]=0.8; // dsa
  limits[8]=0.1; limits[9]=15.0; // Vsa0
  limits[10]=0.1739; limits[11]=0.2599; // tsas_ratio
  limits[12]=0.01; limits[13]=0.15; // t1_ratio
  limits[14]=-0.20; limits[15]=-0.005; // a
  limits[16]=8.0; limits[17]=15.0; // b
  limits[18]=0.1; limits[19]=5.0; // csv
  limits[20]=0.04; limits[21]=0.08; // dsv
  limits[22]=2.0; limits[23]=15.0; // Vsv0
  limits[24]=0.2478; limits[25]=0.3718; // tsvs_ratio
  limits[26]=0.001; limits[27]=0.1; // Rmyo
  limits[28]=1.0; limits[29]=30.0; // Alpha
  limits[30]=1.0E-04; limits[31]=2.5E-4; // Kao
  limits[32]=0.05; limits[33]=1.0; // CAo
  limits[34]=0.0; limits[35]=1.0; // Beta
  limits[36]=2.5; limits[37]=3.5; // Dsh
  limits[38]=0.1; limits[39]=0.4; // Cp
  limits[46]=0.1; limits[47]=20.0; // C1C2ratio_SVR    
  limits[48]=0.01; limits[49]=3.0; // R1R2ratio_SVR   
  limits[50]=0.01; limits[51]=3.0; // R3R2ratio_SVR    
  limits[52]=0.005; limits[53]=0.05; // L_SVR
  limits[56]=0.1; limits[57]=20.0; // C1C2ratio_PVR   
  limits[58]=0.01; limits[59]=3.0; // R1R2ratio_PVR     
  limits[60]=0.01; limits[61]=3.0;   // R3R2ratio_PVR    
  limits[62]=0.005; limits[63]=0.05; // L_PVR
  limits[66]=5.0; limits[67]=40.0;   //Vsv
  limits[68]=1.0; limits[69]=15.0;   //Vsa
  limits[70]=10.0; limits[71]=80.0;  //Pao
  limits[72]=-0.1; limits[73]=100.0; //Qav
  limits[74]=19.8; limits[75]=24.2;  //Pubv
  limits[76]=12.6; limits[77]=15.4;  //Quba
  limits[78]=13.5; limits[79]=16.5;  //Puba
  limits[80]=15.3; limits[81]=18.7;  //Plung
  limits[82]=11.7; limits[83]=14.3;  //Pvp
  limits[84]=13.5; limits[85]=16.5;  //Qapol
  limits[86]=9.0; limits[87]=11.0;   //Ppa

  switch(model){
    case piMUSC7:
      limits[0]=117.65; limits[1]=117.65; // HR
      limits[40]=3.07; limits[41]=3.07;   // SVR
      limits[42]=0.75; limits[43]=0.75;   // PVR
      limits[44]=0.163; limits[45]=0.977; // Ctot_SVR
      limits[54]=0.667; limits[55]=4.0;  // Ctot_PVR
      limits[64]=0.0; limits[65]=0.0;     // k_bw
      // Unidentifiable Initial Conditions
      limits[72]=50.0; limits[73]=50.0; //Qav
      limits[74]=22.0; limits[75]=22.0;  //Pubv
      limits[76]=14.0; limits[77]=14.0;  //Quba
      limits[78]=15.0; limits[79]=15.0;  //Puba
      limits[80]=16.0; limits[81]=16.0;  //Plung
      limits[82]=13.0; limits[83]=13.0;  //Pvp
      limits[84]=15.0; limits[85]=15.0;  //Qapol
      limits[86]=10.0; limits[87]=10.0;   //Ppa
      break;
    case piGOSH22:
      limits[0]=116.0; limits[1]=116.0;     // HR
      limits[40]=3.15; limits[41]=3.15;     // SVR
      limits[42]=0.301; limits[43]=0.301;   // PVR
      limits[44]=0.1587; limits[45]=0.9524; // Ctot_SVR
      limits[54]=1.6611; limits[55]=9.9668; // Ctot_PVR
      limits[64]=0.0; limits[65]=0.0;       // k_bw
      // Unidentifiable Initial Conditions
      limits[72]=50.0; limits[73]=50.0; //Qav
      limits[74]=22.0; limits[75]=22.0;  //Pubv
      limits[76]=14.0; limits[77]=14.0;  //Quba
      limits[78]=15.0; limits[79]=15.0;  //Puba
      limits[80]=16.0; limits[81]=16.0;  //Plung
      limits[82]=13.0; limits[83]=13.0;  //Pvp
      limits[84]=15.0; limits[85]=15.0;  //Qapol
      limits[86]=10.0; limits[87]=10.0;   //Ppa
      break;
    case piUM5:
      limits[0]=90.0; limits[1]=90.0;       // HR
      limits[40]=3.17; limits[41]=3.17;     // SVR
      limits[42]=0.663; limits[43]=0.663;   // PVR
      limits[44]=0.1577; limits[45]=0.946;  // Ctot_SVR
      limits[54]=0.754; limits[55]=4.525;   // Ctot_PVR
      limits[64]=0.0; limits[65]=0.0;       // k_bw
      // Unidentifiable Initial Conditions
      limits[72]=50.0; limits[73]=50.0; //Qav
      limits[74]=22.0; limits[75]=22.0;  //Pubv
      limits[76]=14.0; limits[77]=14.0;  //Quba
      limits[78]=15.0; limits[79]=15.0;  //Puba
      limits[80]=16.0; limits[81]=16.0;  //Plung
      limits[82]=13.0; limits[83]=13.0;  //Pvp
      limits[84]=15.0; limits[85]=15.0;  //Qapol
      limits[86]=10.0; limits[87]=10.0;   //Ppa
      break;
    case piUM10:
      limits[0]=141.0; limits[1]=141.0;      // HR
      limits[40]=6.18; limits[41]=6.18;      // SVR
      limits[42]=0.5; limits[43]=0.5;        // PVR
      limits[44]=0.081; limits[45]=0.485; // Ctot_SVR
      limits[54]=1.0; limits[55]=6.0;        // Ctot_PVR      
      limits[64]=1.0e-5; limits[65]=0.05;    // k_bw
      // Unidentifiable Initial Conditions
      limits[72]=50.0; limits[73]=50.0; //Qav
      limits[74]=22.0; limits[75]=22.0;  //Pubv
      limits[76]=14.0; limits[77]=14.0;  //Quba
      limits[78]=15.0; limits[79]=15.0;  //Puba
      limits[80]=16.0; limits[81]=16.0;  //Plung
      limits[82]=13.0; limits[83]=13.0;  //Pvp
      limits[84]=15.0; limits[85]=15.0;  //Qapol
      limits[86]=10.0; limits[87]=10.0;   //Ppa
      break;
    case piVIRT:
      limits[0]=117.65; limits[1]=117.65; // HR
      limits[40]=3.07; limits[41]=3.07;   // SVR
      limits[42]=0.75; limits[43]=0.75;   // PVR
      limits[44]=0.163; limits[45]=0.977; // Ctot_SVR
      limits[54]=0.667; limits[55]=4.0;  // Ctot_PVR
      limits[64]=0.0; limits[65]=0.0;     // k_bw
      // Unidentifiable Initial Conditions
      limits[72]=50.0; limits[73]=50.0; //Qav
      limits[74]=22.0; limits[75]=22.0;  //Pubv
      limits[76]=14.0; limits[77]=14.0;  //Quba
      limits[78]=15.0; limits[79]=15.0;  //Puba
      limits[80]=16.0; limits[81]=16.0;  //Plung
      limits[82]=13.0; limits[83]=13.0;  //Pvp
      limits[84]=15.0; limits[85]=15.0;  //Qapol
      limits[86]=10.0; limits[87]=10.0;   //Ppa
      // Fixed Atrial Parameters
      //limits[4]=0.01; limits[5]=0.01; // csa
      //limits[6]=0.8; limits[7]=0.8; // dsa
      //limits[8]=2.5; limits[9]=2.5; // Vsa0
      //limits[10]=0.25; limits[11]=0.25; // tsas_ratio
      break;      
  }

  // COMMON LIMITS
//  limits[2]=0.5; limits[3]=2.0; // Csa
//  limits[4]=0.3; limits[5]=0.8; // csa
//  limits[6]=0.15; limits[7]=0.4; // dsa
//  limits[8]=1.0; limits[9]=5.0; // Vsa0
//  limits[10]=0.15; limits[11]=0.25; // tsas_ratio
//  limits[12]=0.05; limits[13]=0.2; // t1_ratio
//  limits[14]=-0.2; limits[15]=-0.1; // a
//  limits[16]=8.0; limits[17]=15.0; // b
//  limits[18]=1.0; limits[19]=5.0; // csv
//  limits[20]=0.05; limits[21]=0.075; // dsv
//  limits[22]=2.0; limits[23]=10.0; // Vsv0
//  limits[24]=0.3; limits[25]=0.35; // tsvs_ratio
//  limits[26]=0.01; limits[27]=0.1; // Rmyo
//  limits[28]=11.0; limits[29]=33.0; // Alpha
//  limits[30]=1.00E-05; limits[31]=1.00E-04; // Kao
//  limits[32]=0.05; limits[33]=1.0; // CAo
//  limits[34]=0.0; limits[35]=1.0; // Beta
//  limits[36]=2.5; limits[37]=3.5; // Dsh
//  limits[38]=0.1; limits[39]=0.4; // Cp
//  limits[46]=0.1; limits[47]=10.0; // C1C2ratio_SVR    
//  limits[48]=0.1; limits[49]=0.4; // R1R2ratio_SVR   
//  limits[50]=0.01; limits[51]=1.5; // R3R2ratio_SVR    
//  limits[52]=0.005; limits[53]=0.05; // L_SVR
//  limits[56]=0.1; limits[57]=10.0; // C1C2ratio_PVR   
//  limits[58]=0.1; limits[59]=0.4; // R1R2ratio_PVR     
//  limits[60]=0.01; limits[61]=1.5; // R3R2ratio_PVR    
//  limits[62]=0.005; limits[63]=0.05; // L_PVR
//  limits[66]=25.0; limits[67]=33.0; //Vsv
//  limits[68]=2.0; limits[69]=10.0; //Vsa
//  limits[70]=20.0; limits[71]=60.0; //Pao
//  limits[72]=-0.1; limits[73]=1.0; //Qav
//  limits[74]=19.8; limits[75]=24.2; //Pubv
//  limits[76]=12.6; limits[77]=15.4; //Quba
//  limits[78]=13.5; limits[79]=16.5; //Puba
//  limits[80]=15.3; limits[81]=18.7; //Plung
//  limits[82]=11.7; limits[83]=14.3; //Pvp
//  limits[84]=13.5; limits[85]=16.5; //Qapol
//  limits[86]=9.0; limits[87]=11.0; //Ppa

}

// ========================
// GET RESPIRATION PRESSURE
// 30 breaths/minute
// ========================
void getRespirationPressure(double t,double &Pth,double &Pab,double &dPth,double &dPab){
  //  tm=mod(t,2.0);
  //  tsr=0.43*2.0;   % active phase of respiration

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
void stage12BlocksODE(int patientID, double tn, double* Xn, double* params, double* Xn1,double* out){
  // Init Stenosis Variables
  double k_stenosis_UB = 0.0;
  double k_stenosis_LB = 0.0;
  double k_stenosis_Llung = 0.0;
  double k_stenosis_Rlung = 0.0;

  // Heart model parameters
  double HR = params[0]; // Fixed Heart Rate
  double tc = 60.0/(double) HR;
  double Csa = params[1];
  double csa = params[2];
  double dsa = params[3];
  double Vsa0 = params[4];
  double tsas_ratio = params[5];
  double tsas = tsas_ratio*sqrt(tc);
  double t1_ratio = params[6]; 
  double t1=t1_ratio*tc;
  double a = params[7];
  double b = params[8];
  double csv = params[9];
  double dsv = params[10];
  double Vsv0 = params[11];
  double tsvs_ratio = params[12];
  double tsvs = tsvs_ratio*sqrt(tc);
  double Rmyo = params[13];
  double alpha = params[14];
  double Kao = params[15]; // Discharge coefficient Aortic valve
  double Cao = params[16]; // Aortic arch
  double beta = params[17]; // Aortic arch
  double Dsh = params[18]; // Shunt nonlinear resistance model
  double Rsh = (960.0*60.0*0.001)/(double)(Dsh*Dsh*Dsh*Dsh);
  double Ksh = (4500.0*60.0*60.0*1.0E-6)/(double)(Dsh*Dsh*Dsh*Dsh);
  double Cp = params[19]; // Shunt/PA compliance

  // Size of the atrio-ventricular Valve
  double A_fw = 0.0;
  switch(patientID){
  	case piMUSC7:
      A_fw = 2.815; // MUSC7  	
  	  break;
  	case piGOSH22:
      A_fw = 3.263; // GOSH22  	
  	  break;
  	case piUM5:
      A_fw = 4.207; // UM5  	
  	  break;
  	case piUM10:
      A_fw = 3.674; // UM10
  	  break;
  	case piVIRT:
      A_fw = 2.815; // VIRTUAL
  	  break;

  }

  // Systemic and pulmonary circulation parameters
  double SVR = params[20];
  double PVR = params[21];
  // SVR - Parameters
  double Ctot_SVR = params[22]; // Hp stessa Ctot dx e sn
  double C1C2ratio_SVR = params[23];
  double R1R2ratio_SVR = params[24];
  double R3R2ratio_SVR = params[25];
  double R2_SVR = SVR/(1.0 + R1R2ratio_SVR + R3R2ratio_SVR);
  double R1_SVR = R1R2ratio_SVR*R2_SVR;
  double C2_SVR = Ctot_SVR/(double)(1.0 + C1C2ratio_SVR);
  double C1_SVR = C2_SVR*C1C2ratio_SVR;
  double R3_SVR = R3R2ratio_SVR*R2_SVR;
  double L_SVR = params[26];
  // PVR - Parameters
  double Ctot_PVR = params[27]; 
  double C1C2ratio_PVR = params[28];
  double R1R2ratio_PVR = params[29];
  double R3R2ratio_PVR = params[30];
  double R2_PVR = PVR/(double)(1.0+R1R2ratio_PVR+R3R2ratio_PVR);
  double R1_PVR = R1R2ratio_PVR*R2_PVR;
  double C2_PVR = Ctot_PVR/(double)(1.0+C1C2ratio_PVR);
  double C1_PVR = C2_PVR*C1C2ratio_PVR;
  double R3_PVR = R3R2ratio_PVR*R2_PVR;
  double L_PVR = params[31];
  double k_bw = params[32];

  // Unpack state variables
  double Vsv = Xn[0]; // Volume single ventricle
  double Vsa = Xn[1]; // Volume single atrium
  double Pao = Xn[2]; // Pressure aorta
  double Qav = Xn[3]; // Flow atrio-ventricular valve
  double Pubv = Xn[4];  // Pressure SVC
  double Quba = Xn[5];  // Flow upper body arteries
  double Puba = Xn[6]; 
  double Plung = Xn[7];
  double Pvp = Xn[8];
  double Qapol = Xn[9];
  double Ppa = Xn[10];  // Pressure in main pulmonary artery

  // Heart function
  double tmv = fmod(tn,tc); // Ventricle time
  double tma = fmod(tn+tsas-t1,tc); // Atrium time
  
  // Ventricle activation
  double AV = 0.0;
  if(tmv<tsvs){
    AV = (1.0-cos(2.0*pi*tmv/(double)tsvs))/2.0;
  }else{
    AV = 0.0;
  }
  
  // Atrium activation
  double AA = 0.0;
  if(tma<tsas){
    AA = (1.0-cos(2.0*pi*tma/(double)tsas))/2.0; 
  }else{
    AA = 0.0;
  }

  // Atrium Model
  double Psaactive = (Vsa-Vsa0)/(double)Csa;
  double Psapassive = csa*(exp(dsa*(Vsa-Vsa0))-1.0);
  double Psa = AA*Psaactive + Psapassive;

  // Ventricle Model
  double Psvactive = (a*(Vsv-Vsv0) + b)*(Vsv-Vsv0);
  double Psvpassive = csv*(exp(dsv*(Vsv-Vsv0))-1.0);
  double Psv = AV*Psvactive + Psvpassive;

  // Aortic Valve
  double QAo = 0.0;
  if(Psv>Pao){
    QAo = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-Pao)))/(2.0*Kao);
  }else{
    QAo = 0.0;
  }

  // Effective Ventricular Pressure
  double Psveff = Psv-Rmyo*QAo;

  // ATRIVENTRICULAR VALVE
  double Kav = 0.0;
  double Lav = 0.0;
  double A_bw = 0.0;
  double Qavp = 0.0;
  double Kav_fw = 0.0;
  double Lav_fw = 0.0;
  double Kav_bw = 0.0;
  double Lav_bw = 0.0;
  double AV_vel = 0.0;
  double Qreg = 0.0;
  if(k_bw < 1.0e-5){
  	// Assign Valve stiffness and inductance
    Kav = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2;
    Lav = beta*2.0*1.060*sqrt(4.0/(pi*A_fw))/1333.2;
    // Non regurgitant valve
    if((Psa<Psveff)&&(Qav<1.0e-5)){
      Qavp = 0.0;	
    }else{
      Qavp = (Psa-Psv-Kav*Qav*fabs(Qav))/(double)Lav;
    }
    if(Qav<1.0e-5){
      Qav = 0.0;
      Xn[3] = 0.0;    	
    }
  }else{
    // Regurgitant valve
    A_bw = k_bw*A_fw;

    // Forward Parameter
    Kav_fw = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2 ; // factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav_fw = beta*2*1.060*sqrt(4/(pi*A_fw))/1333.2; // factor 1333.2 to have inertance in [mmHg*s^2/ml]
    // Backward Parameters
    Kav_bw = alpha*0.5*1.060*(2.4/(A_bw*A_bw))/1333.2;   
    Lav_bw = beta*2*1.060*sqrt(4/(pi*A_bw))/1333.2; 

    if(Qav<1.0e-5){
      Qavp = (Psa-Qav*fabs(Qav)*Kav_bw-Psv)/(double)Lav_bw; // one- way quadratic valve plus inertance
    }else{
      Qavp = (Psa-Qav*fabs(Qav)*Kav_fw-Psv)/(double)Lav_fw; // one- way quadratic valve plus inertance	
    }

    if(Qav<1.0e-5){
      AV_vel = Qav/(double)A_bw;
    }else{
      AV_vel = Qav/(double)A_fw;
    }

    // Regirgitant Flow Rate
    if(Qav<1.0e-5){
      Qreg = fabs(Qav);
    }
    
  }

  // Cavity respiration pressures
  // [~,~,DPith,DPiab]=Pf(t);
  double Pth,Pab,DPith,dPab; 
  getRespirationPressure(tn,Pth,Pab,DPith,dPab);

  // LUNGS
  double Qlung = (Plung-Pvp)/(double)R2_PVR;
  double Qapolp = (Ppa-Qapol*R1_PVR-Plung-k_stenosis_Rlung*Qapol*Qapol)/(double)L_PVR;
  double Plungp = (Qapol-Qlung)/(double)C1_PVR + DPith;
  // Q PV
  double Qvp = (Pvp-Psa)/(double)R3_PVR;
  double Pvpp = (Qlung-Qvp)/(double)C2_PVR + DPith;

  // SYTEM
  double Qubap = (Pao-Puba-R1_SVR*Quba-k_stenosis_UB*Quba*Quba)/(double)L_SVR;
  double Qub = (Puba-Pubv)/(double)R2_SVR;
  // Q SVC+IVC
  double Qubv = (Pubv-Psa)/(double)R3_SVR;
  double Pubap = (Quba-Qub)/(double)C1_SVR;
  double Pubvp = (Qub-Qubv)/(double)C2_SVR;

  // SHUNT
  double Qsh = (-Rsh+sqrt(Rsh*Rsh+4.0*Ksh*(Pao-Ppa)))/(double)(2*Ksh); // Nonlinear shunt resistance

  // Time-derivatives of state variables
  double Vsvp = Qav-QAo;
  double Vsap = Qubv+Qvp-Qav;
  double Paop = (QAo-Qsh-Quba)/(double)Cao + DPith;
  double Ppap = (Qsh-Qapol)/(double)Cp + DPith;

  // Get Derivatives
  Xn1[0] = Vsvp;
  Xn1[1] = Vsap;
  Xn1[2] = Paop;
  Xn1[3] = Qavp;
  Xn1[4] = Pubvp;
  Xn1[5] = Qubap;
  Xn1[6] = Pubap;
  Xn1[7] = Plungp;
  Xn1[8] = Pvpp;
  Xn1[9] = Qapolp;
  Xn1[10] = Ppap;

  // Get Auxiliary Results
  out[0] = Psveff;
  out[1] = Psa;
  out[2] = QAo;
  out[3] = Qvp;
  out[4] = QAo;
  out[5] = tc;
  out[6] = AA;
  out[7] = AV;
  out[8] = Qubv;
  out[9] = Qvp;
  out[10] = Qsh;
  out[11] = Quba+Qsh;
  out[12] = Psaactive;
  out[13] = Psapassive;
  out[14] = Psvactive;
  out[15] = Psvpassive;
  out[16] = Qreg;
}

// =============================
// EVAL MODEL FOR RK4 ITERATIONS
// =============================
void evalModel(int modelID, double t,int totalStates,double* Xk,double* params,double* DXk,double* auxOut){
  // SOLVE HEART MODEL
  stage12BlocksODE(modelID,t,Xk,params,DXk,auxOut);
}

// ==================
// SOLVE ODE WITH RK4
// ==================
int PerformRK4Steps(int modelID, int totalSteps,double timeStep,int totalStates,int totAuxStates,double* iniVals,int totalParams, double* params,double** outVals,double** auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = 0;
  const bool printIC = false;
  bool isValidDouble = true;

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

    // CHECK IF SOLUTION IS VALID
    if((loopA % 100) == 0){
      for(int loopB=0;loopB<totalStates;loopB++){
        isValidDouble = (Xn[loopB] != Xn[loopB]);
        if(isValidDouble){
          return 1;
        }
      }
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
  // RETURN OK
  return 0;
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
    //if(!result){
    //  printf("PROBLEM %d\n",loopA);
    //}
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
    printf("Csa - ");
    answer = false;
    return answer;
  }

  // csa must be positive 
  if(params[2]<0.0){
    printf("csa - ");
    answer = false;
    return answer;    
  }

  // dsa must be positive and bigger than 0.14
  if(params[3]<0.16){
    printf("dsa - ");
    answer = false;
    return answer;    
  }

  // New Constraints on the times
  // tsas_ratio must be within the range 
  if((params[5]<(0.19521))||(params[5]>(0.23859))){
    printf("tsas-ratio - ");
    answer = false;
    return answer;    
  }

  // tsvs_ratio must be within the range 
  if((params[12]<(0.27882),params[12]>(0.34078))){
    printf("tsvs-ratio - ");
    answer = false;
    return answer;    
  }

  // t1_ratio must be positive and smaller than 0.15
  if((params[6]<0)||(params[6]>0.15)){
    printf("t1-ratio - ");
    answer = false;
    return answer;
  }

  // a must be negative
  if(params[7]>0.0){
    printf("a - ");
    answer = false;
    return answer;    
  }

  // b must be positive
  // b less than 15
  if((params[8]<0.0)||(params[8]>15.0)){
    printf("b - ");
    answer = false;
    return answer;
  }

  // csv must be positive and less then 5.0
  if((params[9]<0.0)||(params[9]>5.0)){
    printf("csv - ");
    answer = false;
    return answer;
  }

  // dsv must be positive 
  // Previous: if(Par(11)<0.05)
  if((params[10]<0.05)||(params[10]>0.08)){
    printf("dsv - ");
    answer = false;
    return answer;
  }

  // Rmyo must be positive 
  if(params[13]<0.075){
    printf("Rmyo - ");
    answer = false;
    return answer;    
  }

  // Alpha must be positive 
  if(params[14]<0.0){
    printf("Alpha - ");
    answer = false;
    return answer;
  }

  // KAo must be positive 
  if(params[15]<0){
    printf("KAo - ");
    answer = false;
    return answer;    
  }
 
  // CAo must be positive 
  if(params[16]<0.0){
    printf("CAo - ");
    answer = false;
    return answer;
  }

  // Beta must be positive and less than one
  if((params[17]<0.0)||(params[17]>1.0)){
    printf("Beta - ");
    answer = false;
    return answer;
  }

  // ADDITIONAL ON PARAMETERS
  // Vsv0 must be greater and equal to Vsa0
  if(params[11]<params[4]){
    printf("Vsv0 - ");
    answer = false;
    return answer;
  }

  // Vsa0 must be greater than Zero
  if(params[4]<0.0){
    printf("Vsa0 - ");
    answer = false;
    return answer;
  }

  // ==================
  // INITIAL CONDITIONS
  // ==================

  // Vsv must be positive 
  if(iniVals[0]<0){
    printf("Vsv - ");
    answer = false;
    return answer;    
  }

  // Vsa must be positive 
  if(iniVals[1]<0){
    printf("Vsa - ");
    answer = false;
    return answer;
  }

  // PAo must be positive 
  if(iniVals[2]<0){
    printf("PAo - ");
    answer = false;
    return answer;    
  }

  // Vsa must me greater than Vsa0
  if(iniVals[1]<params[4]){
    printf("Vsa - ");
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
  fprintf(outFile,"Heart Rate & %f\\\\\nn",params[0]);
  fprintf(outFile,"Heart Cycle Time [s] & %f\\\\\n",cycleTime);
  fprintf(outFile,"Number of Heart Cycles [s] & %d\\\\\n",numCycles);
  fprintf(outFile,"Total Simulation Time [s] & %f\\\\\n",totalTime);
  fprintf(outFile,"Time Step [s] & %f\\\\\n",timeStep);
  fprintf(outFile,"Total Simulation Steps & %d\\\\\n",totalSteps);
  fprintf(outFile,"Steps per Heart Cycle & %d\\\\\n",totalStepsOnSingleCycle);
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Full model (Stage12Blocks) time step settings for patient \\patient}\n");
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
  fprintf(outFile,"Dsh & %f\\\\\n",params[18]);
  fprintf(outFile,"Cp & %f\\\\\n",params[19]);
  fprintf(outFile,"SVR & %f\\\\\n",params[20]);
  fprintf(outFile,"PVR & %f\\\\\n",params[21]);
  fprintf(outFile,"Ctot-SVR & %f\\\\\n",params[22]);
  fprintf(outFile,"C1C2ratio-SVR & %f\\\\\n",params[23]);   
  fprintf(outFile,"R1R2ratio-SVR & %f\\\\\n",params[24]);   
  fprintf(outFile,"R3R2ratio-SVR & %f\\\\\n",params[25]); 
  fprintf(outFile,"L-SVR & %f\\\\\n",params[26]);
  fprintf(outFile,"Ctot-PVR & %f\\\\\n",params[27]);
  fprintf(outFile,"C1C2ratio-PVR & %f\\\\\n",params[28]);
  fprintf(outFile,"R1R2ratio-PVR & %f\\\\\n",params[29]);
  fprintf(outFile,"R3R2ratio-PVR & %f\\\\\n",params[30]);
  fprintf(outFile,"L-PVR & %f\\\\\n",params[31]);
  fprintf(outFile,"k-bw & %f\\\\\n",params[32]);
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Optimal full model (Stage12Blocks) parameters for patient \\patient}\n");
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
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Optimal initial full model (Stage12Blocks) parameters for patient \\patient}\n");
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
  fprintf(outFile,"\\caption{Parameter limits used for full model (Stage12Blocks) identification, patient \\patient}\n");
  fprintf(outFile,"\\end{table}\n");
  // CLOSE THE FILE
  fclose(outFile);
}

// =================
// APPEND TIME CURVE
// =================
void appendCurveToFileTime(string fileName,int start,int stop,double* t){
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e ",t[loopA]-t[start]);
  }
  fprintf(outFile,"\n");
  fclose(outFile);
}

// =====================
// APPEND QUANTITY CURVE
// =====================
void appendCurveToFile(string fileName,int start,int stop,int index, double** data){
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e ",data[loopA][index]);
  }
  fprintf(outFile,"\n");
  fclose(outFile);
}

// =========================
// APPEND CURVE DATA TO FILE
// =========================
void appendCurvesDataToFile(int start, int stop ,double* t,double** outVals,double** auxOutVals){
  // Time 
  appendCurveToFileTime("curve_Time.dat",start,stop,t); // Time
  // Flow Rates
  appendCurveToFile("curve_Qvp.dat",start,stop,ipQvp,auxOutVals); // Qvp
  appendCurveToFile("curve_Qvc.dat",start,stop,ipQvc,auxOutVals); // Qvc
  appendCurveToFile("curve_Qav.dat",start,stop,ipQav,outVals); // Qav
  appendCurveToFile("curve_QAo.dat",start,stop,ipQAo,auxOutVals); // QAo
  // Pressures
  appendCurveToFile("curve_Psa.dat",start,stop,ipPsa,auxOutVals); // Psa
  appendCurveToFile("curve_Psv.dat",start,stop,ipPsv,auxOutVals); // Psv
  appendCurveToFile("curve_PAo.dat",start,stop,ipPAo,outVals); // PAo
  appendCurveToFile("curve_Ppa.dat",start,stop,ipPpa,outVals); // Ppa
  // Volume
  appendCurveToFile("curve_Vsv.dat",start,stop,ipVsv,outVals); // PAo
  appendCurveToFile("curve_Vsa.dat",start,stop,ipVsa,outVals); // Ppa
}

// ========================
// FLUSH MODEL DATA TO FILE
// ========================
void flushHeartModelDataToFile(string outFileName,int start, int stop ,double* t,double** outVals,double** auxOutVals){

  // FLUSH ALL DATA
  FILE* outFile;
  outFile = fopen(outFileName.c_str(),"w");
  fprintf(outFile,"%20s %20s %20s %20s %20s %20s %20s %20s %20s\n","t","Qvp","Qvc","Qav","QAo","Psa","Psv","PAo","Ppa");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e %20e %20e %20e %20e %20e\n",
    	    t[loopA]-t[start],auxOutVals[loopA][ipQvp],auxOutVals[loopA][ipQvc],outVals[loopA][ipQav],auxOutVals[loopA][ipQAo],
          auxOutVals[loopA][ipPsa],auxOutVals[loopA][ipPsv],outVals[loopA][ipPAo],outVals[loopA][ipPpa]);
  }
  fclose(outFile);

  // FLUSH PV-LOOP CURVES
  outFile = fopen("Stage12BlocksModelPVLoops.txt","w");
  fprintf(outFile,"%20s %20s %20s %20s\n","Vsa","Psa","Vsv","Psv");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e\n",outVals[loopA][ipVsa],auxOutVals[loopA][ipPsa],outVals[loopA][ipVsv],auxOutVals[loopA][ipPsv]);
  }
  fclose(outFile);

  // FLUSH AV CURVES
  outFile = fopen("Stage12BlocksModelAV.txt","w");
  fprintf(outFile,"%20s %20s %20s\n","t","Qvp","Qvc");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e\n",t[loopA]-t[start],auxOutVals[loopA][ipQvp],auxOutVals[loopA][ipQvc]);
  }
  fclose(outFile);
}

// ==============================
// CHECK IF THE RESULTS ARE VALID
// ==============================
bool validStage12BlocksLPNResults(double* results){
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
bool areValidStage12BlocksFullParams(double* iniVals,double* params){
  
  // Init Answer
  bool answer = true;

  // ====================================
  // POSITIVITY OF HEART MODEL PARAMETERS
  // ====================================
  if((params[1]<0.0)||
  	 (params[2]<0.0)||
  	 (params[3]<0.0)||
  	 (params[4]<0.0)||
  	 (params[5]<0.0)||
  	 (params[6]<0.0)||
  	 (params[8]<0.0)||
  	 (params[9]<0.0)||
  	 (params[10]<0.0)||
  	 (params[11]<0.0)||
  	 (params[12]<0.0)||
  	 (params[13]<0.0)||

  	 (params[14]<0.0)||
  	 (params[15]<0.0)||
  	 (params[16]<0.0)||
  	 (params[17]<0.0)){

     answer = false;
     printf("NEGATIVE HEART PARAM - ");
     return answer;
  }

  // =====================
  // FULL MODEL PARAMETERS
  // =====================

  // PVR - Tau in [0.5,3.0]
  if((params[27]<0.5/(double)params[21])||(params[27]>3.0/(double)params[21])){
    printf("TAU PVR - ");
    answer = false;
    return answer;
  }

  // SVR - Tau in [0.5,3.0]
  if((params[22]<0.5/(double)params[20])||(params[22]>3.0/(double)params[20])){
    printf("TAU SVR - ");
    answer = false;
    return answer;
  }

  // SET OTHER PARAMS
  // Dsh
  if(params[18]<0.0){
    printf("Dsh - ");
    answer = false;
    return answer;    
  }

  // Cp
  if(params[19]<0.0){
    printf("Cp - ");
    answer = false;
    return answer;
  }
  
  // SVR
  if(params[20]<0.0){
    printf("SVR - ");
    answer = false;
    return answer;
  }

  // PVR
  if(params[21]<0.0){
    printf("PVR - ");
    answer = false;
    return answer;
  }

  // Ctot_SVR
  if(params[22]<0.0){
    printf("Ctot_SVR - ");
    answer = false;
    return answer;
  }
  
  // C1C2ratio_SVR
  if(params[23]<0.0){
    printf("C1C2ratio_SVR - ");
    answer = false;
    return answer;
  }
  
  // R1R2ratio_SVR
  if(params[24]<0.0){
    printf("R1R2ratio_SVR - ");
    answer = false;
    return answer;
  }
  
  // R3R2ratio_SVR
  if(params[25]<0.0){
    printf("R3R2ratio_SVR - ");
    answer = false;
    return answer;
  }
  
  // L_SVR
  if(params[26]<0.0){
    printf("L_SVR - ");
    answer = false;
    return answer;
  }
  
  // Ctot_PVR
  if(params[27]<0.0){
    printf("Ctot_PVR - ");
    answer = false;
    return answer;
  }
  
  // C1C2ratio_PVR
  if(params[28]<0.0){
    printf("C1C2ratio_PVR - ");
    answer = false;
    return answer;
  }
  
  // R1R2ratio_PVR
  if(params[29]<0.0){
    printf("R1R2ratio_PVR - ");
    answer = false;
    return answer;
  }
  
  // R3R2ratio_PVR
  if(params[30]<0.0){
    printf("R3R2ratio_PVR - ");
    answer = false;
    return answer;
  }
  
  // L_PVR
  if(params[31]<0.0){
    printf("L_PVR - ");
    answer = false;
    return answer;
  }
  
  // k_bw
  if(params[32]<0.0){
    printf("k_bw - ");
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
int solveStage12BlocksLPN(int modelID, lpnOptions options, int totalStates, double* iniVals,int totalParams, double* params, double* results){

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
  	// Print Table of Parameters
  	printParamTable(cycleTime,numCycles,totalTime,timeStep,totalSteps,totalStepsOnSingleCycle,params,iniVals);
  	// Print Table of Parameter LImits
    printParamLimitsTable(modelID,totalParams+totalStates);
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
  int totAuxStates = 17;

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
  int error = PerformRK4Steps(modelID,totalSteps,timeStep,totalStates,totAuxStates,iniVals,totalParams,params,outVals,auxOutVals);
  if(error !=0){
    // FREE MEMORY
    printf("ERROR - SOLUTION DIVERGED\n");
    for(int loopA=0;loopA<totalSteps;loopA++){
      delete [] outVals[loopA];
      delete [] auxOutVals[loopA];
    }
    delete [] outVals;
    delete [] auxOutVals;
    // RETURN
    return 1;
  }

  if(options.writeDebugData){
    WriteResultsToFile(std::string("states_output.txt"),totalSteps,totalStates,t,outVals);
    WriteResultsToFile(std::string("auxStates_output.txt"),totalSteps,totAuxStates,t,auxOutVals);
  }

  // Copy Auxiliary Variables From result for the last heart cycle
  double Psv[totalStepsOnSingleCycle];
  double Psa[totalStepsOnSingleCycle];
  double QAo[totalStepsOnSingleCycle];
  double tc2[totalStepsOnSingleCycle];
  double AA[totalStepsOnSingleCycle];
  double AV[totalStepsOnSingleCycle];
  double Qvc[totalStepsOnSingleCycle];
  double Qvp[totalStepsOnSingleCycle];
  double Qsh[totalStepsOnSingleCycle];
  double QaftCao[totalStepsOnSingleCycle];
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
    AA[loopA]         = auxOutVals[globalInd][ipAA];
    AV[loopA]         = auxOutVals[globalInd][ipAV];
    Qvc[loopA]        = auxOutVals[globalInd][ipQvc];
    Qvp[loopA]        = auxOutVals[globalInd][ipQvp];
    Qsh[loopA]        = auxOutVals[globalInd][ipQsh];
    QaftCao[loopA]    = auxOutVals[globalInd][ipQaftCao];
    Psaactive[loopA]  = auxOutVals[globalInd][ipPsaactive];
    Psapassive[loopA] = auxOutVals[globalInd][ipPsapassive];
    Psvactive[loopA]  = auxOutVals[globalInd][ipPsvactive];
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
      printf("ERROR: VIOLATED PSA DIFFERENCE.\n");
      // FREE MEMORY
      for(int loopA=0;loopA<totalSteps;loopA++){
        delete [] outVals[loopA];
        delete [] auxOutVals[loopA];
      }
      delete [] outVals;
      delete [] auxOutVals;
      // RETURN
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
      printf("ERROR: VIOLATED PSV LOCATION.\n");
      // FREE MEMORY
      for(int loopA=0;loopA<totalSteps;loopA++){
        delete [] outVals[loopA];
        delete [] auxOutVals[loopA];
      }
      delete [] outVals;
      delete [] auxOutVals;      
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
  double mQsh = trapz(0,totalStepsOnSingleCycle,t,Qsh)/(double)cycleTime;
  double mQreg = trapz(0,totalStepsOnSingleCycle,t,Qreg)/(double)cycleTime;

  // Extreme Values
  double maxPAo = getMax(0,totalStepsOnSingleCycle,PAo);
  double minPAo = getMin(0,totalStepsOnSingleCycle,PAo);
  double maxPsv = getMax(0,totalStepsOnSingleCycle,Psv);
  double maxVsv = getMax(0,totalStepsOnSingleCycle,Vsv);
  double minVsv = getMin(0,totalStepsOnSingleCycle,Vsv);
  double maxQAo = getMax(0,totalStepsOnSingleCycle,QAo);
  double maxPsa = getMax(0,totalStepsOnSingleCycle,Psa);

  double minQvc = getMin(0,totalStepsOnSingleCycle,Qvc);
  double maxQvc = getMax(0,totalStepsOnSingleCycle,Qvc);
  double minQvp = getMin(0,totalStepsOnSingleCycle,Qvp);
  double maxQvp = getMax(0,totalStepsOnSingleCycle,Qvp);

  double minPpa = getMin(0,totalStepsOnSingleCycle,Ppa);
  double maxPpa = getMax(0,totalStepsOnSingleCycle,Ppa);


  // ==============
  // PEAK DETECTION
  // ====================================
  // APPLY CIRCULAR SHIFT TO MODEL CURVES
  // ====================================
  double Qvc_S,Qvc_M,Qvc_D,Qvc_A;
  double tc_S,tc_M,tc_D,tc_A;
  double Qvp_S,Qvp_M,Qvp_D,Qvp_A;    
  double tp_S,tp_M,tp_D,tp_A;

  // Iterate on shifted Qvc Until Peaks are Detected
  int shiftValue = 0;
  if(model == piUM5){
    shiftValue = 200;
  }
  int pfCount = 0;
  bool isValidQvc = false;
  while((!isValidQvc)&&(pfCount<10)){
    applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvc); 
    isValidQvc = extractSMDAPeaks(totalStepsOnSingleCycle,t,Qvc,Qvc_S,tc_S,Qvc_M,tc_M,Qvc_D,tc_D,Qvc_A,tc_A);
    if(!isValidQvc){
      shiftValue += floor(totalStepsOnSingleCycle/10);
      applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvc);	
    }
    pfCount++;
  }
 
  // Iterate on shifted Qvp Until Peaks are Detected
  shiftValue = 0;
  if(model == piUM5){
    shiftValue = 0;
  }
  pfCount = 0;
  bool isValidQvp = false;
  while((!isValidQvp)&&(pfCount<10)){
    applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvp); 
    isValidQvp = extractSMDAPeaks(totalStepsOnSingleCycle,t,Qvp,Qvp_S,tp_S,Qvp_M,tp_M,Qvp_D,tp_D,Qvp_A,tp_A);
    if(!isValidQvp){
      shiftValue += floor(totalStepsOnSingleCycle/10);
      applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvp);	
    }
    pfCount++;
  }

  // IF INVALID SHAPE EXIT
  if(options.checkCurves){
    if(((!isValidQvc)||(!isValidQvp))){
      // PRINT CURVES THAT FAIL PEAK DETECTION
      /*char simulString[15];
      sprintf(simulString, "%d", simulCount);
      string fileNameQvc("vcCurve_"+string(simulString)+".txt");
      string fileNameQvp("vpCurve_"+string(simulString)+".txt");
      printCurvesAndPeaks(fileNameQvc,totalStepsOnSingleCycle,t,Qvc,Qvc_S,tc_S,Qvc_M,tc_M,Qvc_D,tc_D,Qvc_A,tc_A);
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
  }else{
    Qvc_S = 0.0; Qvc_M = 0.0; Qvc_D = 0.0; Qvc_A = 0.0;
    tc_S = 0.0; tc_M = 0.0; tc_D = 0.0; tc_A = 0.0;
    Qvp_S = 0.0; Qvp_M = 0.0; Qvp_D = 0.0; Qvp_A = 0.0;    
    tp_S = 0.0; tp_M = 0.0; tp_D = 0.0; tp_A = 0.0;
  }

  // PRINT
  //printCurvesAndPeaks(fileNameQvc,totalStepsOnSingleCycle,t,Qvc,Qvc_S,tc_S,Qvc_M,tc_M,Qvc_D,tc_D,Qvc_A,tc_A);
  //printCurvesAndPeaks(fileNameQvp,totalStepsOnSingleCycle,t,Qvp,Qvp_S,tp_S,Qvp_M,tp_M,Qvp_D,tp_D,Qvp_A,tp_A);

  // Save Time delays
  double vc_SD_time = fabs(tc_S-tc_D);
  double vc_MA_time = fabs(tc_M-tc_A);
  double vp_SD_time = fabs(tp_S-tp_D); 
  double vp_MA_time = fabs(tp_M-tp_A);

  // ================================
  // WRITE CURVES AND PEAKS FOR DEBUG
  // ================================
  if(options.writeDebugData){
    char simulString[15];
    sprintf(simulString, "%d", simulCount);
    string fileNameQvc("vcCurve_"+string(simulString)+".txt");
    string fileNameQvp("vpCurve_"+string(simulString)+".txt");
    printCurvesAndPeaks(fileNameQvc,totalStepsOnSingleCycle,t,Qvc,Qvc_S,tc_S,Qvc_M,tc_M,Qvc_D,tc_D,Qvc_A,tc_A);
    printCurvesAndPeaks(fileNameQvp,totalStepsOnSingleCycle,t,Qvp,Qvp_S,tp_S,Qvp_M,tp_M,Qvp_D,tp_D,Qvp_A,tp_A);
  }

  // =====================
  // PLOT DATA IF REQUIRED
  // =====================
  if(options.flushData){
    string outFileName("Stage12BlocksModelData.txt");
    flushHeartModelDataToFile(outFileName,totalSteps-3*totalStepsOnSingleCycle,totalSteps,t,outVals,auxOutVals);
  }

  // =============
  // APPEND CURVES
  // =============
  if(options.appendOutputCurves){
    appendCurvesDataToFile(totalSteps-3*totalStepsOnSingleCycle,totalSteps,t,outVals,auxOutVals);
  }

  // Copy Final Results
  results[0] = mPascAo;
  results[1] = minPAo;
  results[2] = maxPAo;
  results[3] = mPsa;
  results[4] = maxPsv;
  results[5] = maxVsv;
  results[6] = (mQsh/(CO-mQsh));
  results[7] = mPpa;
  results[8] = CO;

  if(modelID == piUM10){
    results[9] = maxQvc;
    results[10] = 0.0;
    results[11] = 0.0;
    results[12] = minQvc;
    results[13] = maxQvp;
    results[14] = 0.0;
    results[15] = 0.0;
    results[16] = minQvp;
    // Curve time Delays
    results[17] = 0.0;
    results[18] = 0.0;
    results[19] = 0.0;
    results[20] = 0.0;
  }else{
    // All points are available
    results[9] = Qvc_S;
    results[10] = Qvc_M;
    results[11] = Qvc_D;
    results[12] = Qvc_A;
    results[13] = Qvp_S;
    results[14] = Qvp_M;
    results[15] = Qvp_D;
    results[16] = Qvp_A;
    // Curve time Delays
    results[17] = vc_SD_time; 
    results[18] = vc_MA_time; 
    results[19] = vp_SD_time; 
    results[20] = vp_MA_time; 
  }
  results[21] = minPpa; // minPpa
  results[22] = maxPpa; // maxPpa
  results[23] = mQreg; // Qreg
  results[24] = Psv[0]; // PsvZero
  results[25] = maxPsa; // maxPsa
  
  // printf("%f %f %f %f %f\n",mPascAo,maxPAo,mPsa,maxPsv,maxVsv);
  if(!validStage12BlocksLPNResults(results)){
  	// Return invalid
    printf("ERROR: INVALID LPN RESULTS.\n");
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
    case piMUSC7:
      // Average
      meanData[0] = 53.0;   // mPascAo
      meanData[1] = 30.0;   // minPAo
      meanData[2] = 80.0;   // maxPAo
      meanData[3] = 7.0;    // mPsa
      meanData[4] = 82.0;   // maxPsv
      meanData[5] = 29.0;   // maxVsv
      meanData[6] = 0.8;    // QpQs
      meanData[7] = 16.0;   // mPpa
      meanData[8] = 27.0;   // CO
      meanData[9] = 43.0;   // SVC+IVC_S
      meanData[10] = -1.5;  // SVC+IVC_M
      meanData[11] = 36.0;  // SVC+IVC_D
      meanData[12] = -16.5; // SVC+IVC_A
      meanData[13] = 13.0;  // PVs_S
      meanData[14] = 4.0;   // PVs_M
      meanData[15] = 23.0;  // PVs_D
      meanData[16] = 7.5;   // PVs_A
      // Time Delays
      meanData[17] = 0.33;  //Qvc_SD_time
      meanData[18] = 0.19;  //Qvc_MA_time
      meanData[19] = 0.33;  //Qvp_SD_time
      meanData[20] = 0.32;  //Qvp_MA_time         
      // Min e Max Ppa
      meanData[21] = 10.8;  // minPpa
      meanData[22] = 24.2;  // maxPpa
      // Qreg
      meanData[23] = 0.0;  // mQreg
      // PsvZero and MaxPsa
      meanData[24] = 9.88;  // PsvZero
      meanData[25] = 11.437;  // maxPsa
      
      // STD DEV
      stdData[24] = 0.83;
      stdData[25] = 0.559;
      break;
    case piGOSH22:
      // Average
      meanData[0] = 51.0;  // mPascAo
      meanData[1] = 31.0;  // minPAo
      meanData[2] = 78.0;  // maxPAo
      meanData[3] = 6.0;   // mPsa
      meanData[4] = 91.0;  // maxPsv
      meanData[5] = 31.0;  // maxVsv
      meanData[6] = 1.16;  // QpQs
      meanData[7] = 11.0;  // mPpa 
      meanData[8] = 30.9;  // CO 
      meanData[9] = 42.0;  // SVC+IVC_S 
      meanData[10] = 1.3;  // SVC+IVC_M 
      meanData[11] = 23.0; // SVC+IVC_D 
      meanData[12] = -5.2; // SVC+IVC_A 
      meanData[13] = 28.0; // PVs_S 
      meanData[14] = 10.5; // PVs_M 
      meanData[15] = 30.0; // PVs_D 
      meanData[16] = 6.7;  // PVs_A 
      // Time Delays
      meanData[17] = 0.36;  //Qvc_SD_time
      meanData[18] = 0.21;  //Qvc_MA_time
      meanData[19] = 0.36;  //Qvp_SD_time
      meanData[20] = 0.21;  //Qvp_MA_time
      // Min e Max Ppa
      meanData[21] = 0.0;  // minPpa
      meanData[22] = 0.0;  // maxPpa
      // Qreg
      meanData[23] = 0.0;  // mQreg
      // PsvZero and maxPsa
      meanData[24] = 10.0; // PsvZero
      meanData[25] = 11.0; // maxPsa
      
      // STD DEV
      stdData[24] = 0.5;
      stdData[25] = 0.55;
      break;
    case piUM5:
      // Average
      meanData[0] = 53.0;   // mPascAo
      meanData[1] = 30.0;   // minPAo
      meanData[2] = 87.0;   // maxPAo
      meanData[3] = 5.0;    // mPsa
      meanData[4] = 90.0;   // maxPsv
      meanData[5] = 35.5;   // maxVsv
      meanData[6] = 0.8;    // QpQs
      meanData[7] = 13.35;  // mPpa 
      meanData[8] = 27.0;   // CO 
      meanData[9] = 24.5;   // SVC+IVC_S 
      meanData[10] = -1.9;  // SVC+IVC_M 
      meanData[11] = 42.8;  // SVC+IVC_D 
      meanData[12] = 14.9;  // SVC+IVC_A 
      meanData[13] = 13.9;  // PVs_S 
      meanData[14] = 7.7;   // PVs_M 
      meanData[15] = 22.6;  // PVs_D 
      meanData[16] = 11.8;  // PVs_A 
      // Time Delays
      meanData[17] = 0.47;  //Qvc_SD_time
      meanData[18] = 0.24;  //Qvc_MA_time
      meanData[19] = 0.47;  //Qvp_SD_time
      meanData[20] = 0.46;  //Qvp_MA_time      
      // Min e Max Ppa
      meanData[21] = 12.4;  // minPpa
      meanData[22] = 18.3;  // maxPpa
      // Qreg
      meanData[23] = 0.0;  // mQreg
      // PsvZero e Max Psa
      meanData[24] = 6.30;  // PsvZero
      meanData[25] = 6.993;  // maxPsa
      
      // STD DEV
      stdData[24] = 1.56;
      stdData[25] = 0.212;
      break;
    case piUM10:
      // Average
      meanData[0] = 72.0;   // mPascAo
      meanData[1] = 40.0;   // minPAo
      meanData[2] = 113.0;  // maxPAo
      meanData[3] = 4.0;    // mPsa
      meanData[4] = 114.0;  // maxPsv
      meanData[5] = 30.0;   // maxVsv
      meanData[6] = 1.82;   // QpQs
      meanData[7] = 14.0;   // mPpa 
      meanData[8] = 31.0;   // CO 
      meanData[9] = 67.0;   // SVC+IVC S 
      meanData[10] = 0.0;   // SVC+IVC M - NOT AVAIL
      meanData[11] = 0.0;   // SVC+IVC D - NOT AVAIL
      meanData[12] = -83.0; // SVC+IVC A 
      meanData[13] = 47.3;  // PVs_S 
      meanData[14] = 0.0;   // PVs_M - NOT AVAIL
      meanData[15] = 0.0;   // PVs_D - NOT AVAIL
      meanData[16] = -29.0; // PVs_A
      // Time Delays
      meanData[17] = 0.0;  //Qvc_SD_time
      meanData[18] = 0.0;  //Qvc_MA_time
      meanData[19] = 0.0;  //Qvp_SD_time
      meanData[20] = 0.0;  //Qvp_MA_time            
      // Min e Max Ppa
      meanData[21] = 9.2;  // minPpa
      meanData[22] = 18.8;  // maxPpa
      // Qreg
      meanData[23] = 10.5;  // mQreg
      // PsvZero and maxPsa
      meanData[24] = 6.63; // PsvZero
      meanData[25] = 8.807; // maxPsa
      
      // STD DEV
      stdData[24] = 1.70;
      stdData[25] = 0.639;
      break;
    case piVIRT:
      // Average
      meanData[0] = 48.855719;   // mPascAo
      meanData[1] = 31.449007;   // minPAo
      meanData[2] = 73.625456;   // maxPAo
      meanData[3] = 6.503433;    // mPsa
      meanData[4] = 74.933028;   // maxPsv
      meanData[5] = 28.230853;   // maxVsv
      meanData[6] = 0.831239;    // QpQs
      meanData[7] = 15.126642;   // mPpa
      meanData[8] = 25.337582;   // CO
      meanData[9] = 24.683764;   // SVC+IVC_S
      meanData[10] = 4.853743;  // SVC+IVC_M
      meanData[11] = 24.514120;  // SVC+IVC_D
      meanData[12] = 5.437089; // SVC+IVC_A
      meanData[13] = 19.633752;  // PVs_S
      meanData[14] = 0.240727;   // PVs_M
      meanData[15] = 16.094992;  // PVs_D
      meanData[16] = 7.113991;   // PVs_A
      // Time Delays
      meanData[17] = 0.216000;  //Qvc_SD_time
      meanData[18] = 0.284000;  //Qvc_MA_time
      meanData[19] = 0.321600;  //Qvp_SD_time
      meanData[20] = 0.254400;  //Qvp_MA_time         
      // Min e Max Ppa
      meanData[21] = 13.405366;  // minPpa
      meanData[22] = 17.239139;  // maxPpa
      // Qreg
      meanData[23] = 0.0;  // mQreg
      // PsvZero and MaxPsa
      meanData[24] = 2.251292;  // PsvZero
      meanData[25] = 12.228537;  // maxPsa
      
      // STD DEV
      stdData[24] = 0.83;
      stdData[25] = 0.559;
      break;      
  }

  // ====================
  // STD - 5% UNCERTAINTY
  // ====================
  //
  // PUT ZERO STD ON RESULTS YOU WANT TO EXCLUDE !!!
  //
  for(int loopA=0;loopA<(res_num-2);loopA++){
    stdData[loopA] = fabs(DATA_UNCERTAINTY*meanData[loopA]);
  }

  // IF PEAK TIMES NEED NOT BE USED, SET ZERO STD
  if(!USE_PEAKTIME_ASTARGETS){
  	// Time Delays
    stdData[17] = 0.0;
    stdData[18] = 0.0;
    stdData[19] = 0.0;
    stdData[20] = 0.0;
  }

  if(EXCLUDE_INVASIVE_TARG){
  	// Average
    stdData[3] = 0.0;    // mPsa
    stdData[4] = 0.0;   // maxPsv
    stdData[7] = 0.0;   // mPpa
    // Min e Max Ppa
    stdData[21] = 0.0;  // minPpa
    stdData[22] = 0.0;  // maxPpa
    // PsvZero and MaxPsa
    stdData[24] = 0.0;  // PsvZero
    stdData[25] = 0.0;  // maxPsa
  }

  // ==============
  // ASSIGN WEIGHTS
  // ==============
  weights[0] = 0.5;   // mPascAo - Gianca
  weights[1] = 1.0;   // minPAo
  weights[2] = 1.0;   // maxPAo
  weights[3] = 1.0;   // mPsa
  weights[4] = 1.0;   // maxPsv
  weights[5] = 1.0;   // maxVsv
  weights[6] = 1.0;   // QpQs
  weights[7] = 1.0;   // mPpa 
  weights[8] = 0.5;   // CO - Gianca
  weights[9] = PRIMARY_CURVEPEAK_WEIGHT;  // SVC+IVC S 
  weights[10] = PRIMARY_CURVEPEAK_WEIGHT; // SVC+IVC M - NOT AVAIL
  weights[11] = PRIMARY_CURVEPEAK_WEIGHT; // SVC+IVC D - NOT AVAIL
  weights[12] = PRIMARY_CURVEPEAK_WEIGHT; // SVC+IVC A 
  weights[13] = SECONDARY_CURVEPEAK_WEIGHT; // PVs_S 
  weights[14] = SECONDARY_CURVEPEAK_WEIGHT; // PVs_M - NOT AVAIL
  weights[15] = SECONDARY_CURVEPEAK_WEIGHT; // PVs_D - NOT AVAIL
  weights[16] = SECONDARY_CURVEPEAK_WEIGHT; // PVs_A
  // Curve time Delays
  weights[17] = PRIMARY_CURVEPEAK_WEIGHT; //Qvc_SD_time
  weights[18] = PRIMARY_CURVEPEAK_WEIGHT; //Qvc_MA_time
  weights[19] = PRIMARY_CURVEPEAK_WEIGHT; //Qvp_SD_time
  weights[20] = PRIMARY_CURVEPEAK_WEIGHT; //Qvp_MA_time
  // Other Targets
  weights[21] = 1.0; // minPpa
  weights[22] = 1.0; // maxPpa
  weights[23] = 1.0; // mQreg
  weights[24] = 1.0; // PsvZero
  weights[25] = 1.0; // maxPsa

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
    fprintf(outFile,"\\caption{Full model (Stage12Blocks) results for patient \\patient}\n");
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
  options.checkCurves = true;
  options.useFullConstraints = false;
  options.appendOutputCurves = false;

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
    error = solveStage12BlocksLPN(model,options,totalStates,iniVals,totalParams,paramsVals,results);
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