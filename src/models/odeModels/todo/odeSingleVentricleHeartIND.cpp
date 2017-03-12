# include "odeSingleVentricleHeartIND.hpp"

using namespace std;

// SIMULATION COUNTER
int simulCount = 0;

// Numerical Constants
const double pi = 3.1415926535897;

// Output State Constants
const int ipVsv = 0;
const int ipVsa = 1;
const int ipQav = 2;
const int ipPAA = 3;

// Auxiliary Output Constants
const int ipPsvEff     = 0;
const int ipPsa        = 1;
const int ipQven       = 2;
const int ipLVOF       = 3;
const int iptc         = 4;
const int ipAA         = 5;
const int ipAV         = 6;
const int ipPsaActive  = 7;
const int ipPsaPassive = 8;
const int ipPsvActive  = 9;
const int ipPsvPassive = 10;
const int ipPAo        = 11;
const int ipQreg       = 12;
const int ipQAA	       = 13;

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleHeartIND::getModelParameterTotal(){
  return 27;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleHeartIND::getModelStateTotal(){
  return 4; 
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int odeSingleVentricleHeartIND::getModelResultTotal(){
  return 9;  
}

// ===========================
// GET NAME OF RESULT QUANTITY
// ===========================
string odeSingleVentricleHeartIND::getResultName(int index){
  switch(index){
    case 0:
    {      
      return string("mPascAo");
      break;
    }
    case 1:
    {      
      return string("maxPAo");
      break;
    }
    case 2:
    {      
      return string("mPsa");
      break;
    }
    case 3:
    {      
      return string("maxPsv");
      break;
    }
    case 4:
    {      
      return string("maxVsv");
      break;
    }
    case 5:
    {      
      return string("maxQAA");
      break;
    }
    case 6:
    {      
      return string("PsvZero");
      break;
    }
    case 7:
    {      
      return string("maxPsa");
      break;
    }
    case 8:
    {      
      return string("mQreg");
      break;
    }    
  }
}

// ==================
// GET PARAMETER NAME
// ==================
string odeSingleVentricleHeartIND::getParamName(int index){
  switch(index){
    case 0:
    {
      return string("Heart Rate");
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
      return string("CAo");
      break;
    }
    case 17:
    {      
      return string("tdelay1");
      break;
    }
    case 18:
    {      
      return string("tdelay2");
      break;
    }
    case 19:
    {      
      return string("beta");
      break;
    }
    case 20:
    {      
      return string("k-bw");
      break;
    }
    case 21:
    {      
      return string("RAA");
      break;
    }
    case 22:
    {      
      return string("CAA");
      break;
    }    
    case 23:
    {      
      return string("Vsv");
      break;
    }
    case 24:
    {      
      return string("Vsa");
      break;
    }
    case 25:
    {      
      return string("Qav");
      break;
    }
    case 26:
    {      
      return string("PAA");
      break;
    }
    case 27:
    {      
      return string("QAA");
      break;
    }    
  }
}

// ====================
// GET MODEL PARAMETERS
// ====================
void odeSingleVentricleHeartIND::getDefaultModelParams(int modelID, double* zp){
  switch(modelID){
// [0]HR [1]Csa [2]csa [3]dsa [4]Vsa0 [5]tsas [6]t1_ratio [7]a [8]b [9]csv [10]dsv 
// [11]Vsv0 [12]tsvs [13]Rmyo [14]Kav [15]Kao [16]CAo [17]tdelay1 [18]tdelay2 [19]Lav [20]k_bw
// [21]RAA [22]CAA  
// [23]Vsv [24]Vsa [25]Qav [26]PAA [27]QAA  
  	case piMUSC7:
      zp[0] = 117.65; zp[1] = 1; zp[2] = 0.78541; zp[3] = 0.20142;
      zp[4] = 0.96742; zp[5] = 0.20149; zp[6] = 0.11024; zp[7] = -0.10783;
      zp[8] = 14.106; zp[9] = 0.97372; zp[10] = 0.065337; zp[11] = 9.5631;
      zp[12] = 0.31523; zp[13] = 0.091782; zp[14] = 24.203; zp[15] = 0.0002;
      zp[16] = 0.15; zp[17] = 0; zp[18] = 0; zp[19] = 0.72;
      zp[20] = 0.0; zp[21] = 0.1;zp[22] = 0.1; zp[23] = 32.937; zp[24] = 1.632; zp[25] = 0;	
      zp[26] =35;//zp[27] = 0;    //fare lo stesso per gli altri pazienti
  //      zp[0] = 117.65; zp[1]=	0.76569; zp[2]=	0.01; zp[3]=	0.8; zp[4]=	2.5;
  //	zp[5]=	0.25; zp[6]=	0.1; zp[7]=	-0.12; zp[8]=	14; zp[9]=	1; zp[10]=	0.065;
  //	zp[11]=	9.7786; zp[12]=	0.31; zp[13]=	0.1; zp[14]=	24.474;
  //	zp[15]=	0.00025; zp[16]=	0.24; zp[17]=	0; zp[18]=	0; zp[19]=	0.95; zp[20]=	0;
  //	zp[21] = 0.000356;zp[22] = 0.188; zp[23] = 32.937; zp[24] = 1.632; zp[25] = -1.3068;	
  //     zp[26] =35;//zp[27] = 0;  

  	  break;
  	case piGOSH22:
      zp[0] = 116.0; zp[1] = 2.4765; zp[2] = 0.1728; zp[3] = 0.17056;
      zp[4] = 0.079096; zp[5] = 0.23413; zp[6] = 0.0055602; zp[7] = -0.085834;
      zp[8] = 13.533; zp[9] = 2.8973;  zp[10] = 0.065016; zp[11] = 11.498;
      zp[12] = 0.31666; zp[13] = 0.20595; zp[14] = 9.9642; zp[15] = 0.0011649;
      zp[16] = 0.25607; zp[17] = -0.21709; zp[18] = 0.13899; zp[19] = 12.835;
      zp[20] = 0.0; zp[21] = 35.348; zp[22] = 2.2357; zp[23] = 56.622; zp[24] = 0.56854;
  	  break;
  	case piUM5:
      zp[0] = 90.0; zp[1] = 0.18912; zp[2] = 0.30883; zp[3] = 0.27342;
      zp[4] = 0.82973; zp[5] = 0.20019; zp[6] = 0.079075; zp[7] = -0.1482;
      zp[8] = 12.1; zp[9] = 1.3514; zp[10] = 0.076565; zp[11] = 10.135; zp[12] = 0.3192;
      zp[13] = 0.11537; zp[14] = 11.615; zp[15] = 0.00022209; zp[16] = 0.12032; zp[17] = 0.75256;
      zp[18] = 0.2024; zp[19] = 1.7829; zp[20] = 0.0; zp[21] = 32.549;
      zp[22] = 3.1777; zp[23] = 61.526; zp[24] = 1.3197;
  	  break;
  	case piUM10:
      zp[0] = 141.0; zp[1] = 0.29453; zp[2] = 0.024235; zp[3] = 0.65256;
      zp[4] = 6.0299; zp[5] = 0.23062; zp[6] = 0.024539; zp[7] = -0.010698;
      zp[8] = 14.575; zp[9] = 0.14423; zp[10] = 0.060348; zp[11] = 7.7896;
      zp[12] = 0.28067; zp[13] = 0.156; zp[14] = 8.2046; zp[15] = 0.00045862;
      zp[16] = 0.17557; zp[17] = -1.3849; zp[18] = 0.012945; zp[19] = 10.07;
      zp[20] = 0.036166; zp[21] = 11.208; zp[22] = 13.566; zp[23] = 50.049; zp[24] = 12.469;
  	  break;
  }
}

// ====================
// GET PARAMETER RANGES
// ====================
void odeSingleVentricleHeartIND::getModelLimits(int model,int par_num,double* limits){
  switch(model){
    case piMUSC7:
      limits[0]=117.65; limits[1]=117.65; // HR
      limits[40]=0.000; limits[41]=0.000; // k_bw
      break;
    case piGOSH22:
      limits[0]=116.00; limits[1]=116.00; // HR
      limits[40]=0.000; limits[41]=0.000; // k_bw
      break;
    case piUM5:
      limits[0]=90.000; limits[1]=90.000; // HR
      limits[40]=0.000; limits[41]=0.000; // k_bw
      break;
    case piUM10:
      limits[0]=141.00; limits[1]=141.00; // HR
      limits[40]=0.015; limits[41]=0.022; // k_bw
      break;
  }

// COMMON PARAMETER RANGES
limits[2]=0.05;      limits[3]=2.0;         // Csa
limits[4]=0.01;      limits[5]=0.8;         // csa
limits[6]=0.16;     limits[7]=0.8;          // dsa
limits[8]=0.1;      limits[9]=15.0;         // Vsa0
limits[10]=0.1739;     limits[11]=0.2599; // tsas
limits[12]=0.01;     limits[13]=0.15;       // t1_ratio
limits[14]=-0.20;     limits[15]=-0.005;    // a
limits[16]=8.0;      limits[17]=15.0;       // b
limits[18]=0.1;      limits[19]=5.0;        // csv
limits[20]=0.04;     limits[21]=0.08;       // dsv
limits[22]=2.0;      limits[23]=15.0;       // Vsv0
limits[24]=0.2478;  limits[25]=0.3718;    // tsvs
limits[26]=0.001;     limits[27]=0.1;       // Rmyo
limits[28]=1.0;     limits[29]=30.0;       // alpha
limits[30]=1.00E-04; limits[31]=2.50E-04;   // Kao
limits[32]=0.05;     limits[33]=1.0;        // CAo
limits[34]=-0.2;     limits[35]=0.2;        // tdelay1
limits[36]=-0.2;     limits[37]=0.2;        // tdelay2
limits[38]=0.0;     limits[39]=1.00;        // beta
limits[42]=0.1;   limits[43]=0.7;           //RAA
limits[44]=0.05;    limits[45]=1.0;         //CAA
limits[46]=5.0;     limits[47]=40.0;        // Vsv
limits[48]=1.0;      limits[49]=15.0;       // Vsa
limits[50]=-0.1;     limits[51]=100.0;      // Qav
// CHECK RANGES ??
limits[52]=10.0;    limits[53]=80.0; //PAA
//limits[54]=-10.0;   limits[55]=100.0; //QAA

}

// =======================================
// GET FOURIER COEFFICIENTS FOR ALL MODELS
// =======================================
void getFourierCoefficients(int patientID,
                            double &ain1,double &ain2,double &ain3,double &ain4,double &ain5,double &ain6,double &ain7,double &ain8,double &ain9,
                            double &bin1,double &bin2,double &bin3,double &bin4,double &bin5,double &bin6,double &bin7,double &bin8,
                            double &aout1,double &aout2,double &aout3,double &aout4,double &aout5,double &aout6,double &aout7,double &aout8,double &aout9,
                            double &bout1,double &bout2,double &bout3,double &bout4,double &bout5,double &bout6,double &bout7,double &bout8){

  switch(patientID){
    case piMUSC7:
      // IN
      ain9  = 25.27; ain1  = 4.215; bin1  = 0.8914; ain2  = -15.74;
      bin2  = 18.16; ain3  = 0.8737; bin3  = 13.62; ain4  = 1.966;
      bin4  = 1.185; ain5  = -0.9142; bin5  = -0.928; ain6  = -0.8623;
      bin6  = -0.7006; ain7  = -0.7609; bin7  = -0.7781; ain8  = -0.3597;
      bin8  = -0.2047;
      // OUT
 //     aout9 = 25.27; aout1 = 2.985; bout1 = 38.75; aout2 = -20.02;
 //     bout2 = 5.519; aout3 = -4.378; bout3 = -2.792; aout4 = -3.966;
 //     bout4 = -2.693; aout5 = 0.3378; bout5 = -1.732; aout6 = -1.418;
 //     bout6 = -0.7536; aout7 = 0.4665; bout7 = -1.528; aout8 = 0.1099;
 //     bout8 = -0.1978;

       aout9 = 52.41; aout1 = -10.71; bout1 = 16.74; aout2 = -8.7; bout2 = 0.7979;
       aout3 = -3.689; bout3 = -3.076; aout4 = -0.3673; bout4 = -2.749; aout5 = 0.8319;       
       bout5 = -1.328; aout6 = 0.7661; bout6 = -0.3316; aout7 = 0.4237;
       bout7 = 0.006969; aout8 = 0.2383; bout8 = 0.04639;

      break;
    case piGOSH22:
      // IN
      ain9 = 30.9000; ain1 = 12.68; bin1 = 8.084; ain2 = -5.854;
      bin2 = 12.94; ain3 = 2.268; bin3 = 14.5; ain4 = 5.428;
      bin4 = 5.509; ain5 = 1.127; bin5 = -0.02417; ain6 = -0.9529;
      bin6 = 1.288; ain7 = 0.02309; bin7 = 2.042; ain8 = 1.701;
      bin8 = 1.185; 

      // OUT
      aout9 = 30.9; aout1 = -6.947; bout1 = 44.99; aout2 = -19.54;
      bout2 = -5.84; aout3 = -4.985; bout3 = -1.725; aout4 = -0.6823;
      bout4 = -7.198; aout5 = 0.9297; bout5 = 0.1342; aout6 = 2.621;
      bout6 = -2.509; aout7 = -0.3764; bout7 = 0.2258; aout8 = 3.036;
      bout8 = 0.981;
      break;    
    case piUM5:
      // IN
      ain9  = 27.0; ain1  = 7.542; bin1  = -13.82; ain2  = -6.66;
      bin2  = 4.644; ain3  = 9.326; bin3  = 5.277; ain4  = 2.712;
      bin4  = -4.035; ain5  = -2.085; bin5  = 0.6408; ain6  = 0.6892;
      bin6  = 0.9538; ain7  = 0.1489; bin7  = -0.4545; ain8  = -0.146;
      bin8  = 0.1484;

      // UM5
      aout9 = 27.0; aout1 = -3.363; bout1 = 45.75; aout2 = -27.08;
      bout2 = -6.219; aout3 = 2.727; bout3 = -5.342; aout4 = -4.479;
      bout4 = -4.271; aout5 = 6.097; bout5 = -3.46; aout6 = 0.006244;
      bout6 = 1.076; aout7 = 3.617; bout7 = -0.4042; aout8 = -0.8963;
      bout8 = 2.68;
      break;    
    case piUM10:
      // IN
      ain9  = 31.00; ain1  = 52.14; bin1  = 48.68; ain2  = 41.51;
      bin2  = -16.3; ain3  = -10.94; bin3  = -15.69; ain4  = -2.48;
      bin4  = 8.581; ain5  = 5.62; bin5  = -2.684; ain6  = -3.14;
      bin6  = -1.915; ain7  = -0.7532; bin7  = 2.063; ain8  = 0.6098;
      bin8  = -0.06847;
      // OUT
      aout9 = 31.0; aout1 = -21.39; bout1 = 38.55; aout2 = -6.096;
      bout2 = -10.44; aout3 = -5.572; bout3 = -0.8667; aout4 = 3.533;
      bout4 = -1.758; aout5 = -0.03423; bout5 = -1.978; aout6 = 2.069;
      bout6 = 0.1064; aout7 = 0.3559; bout7 = -0.8598; aout8 = -0.3213;
      bout8 = 1.997;
      break;
  }
}

// =====================
// SOLVE SINGLE ODE STEP
// =====================
void heartODE(int patientID, double tn, double* Xn, double* params, double* Xn1,double* out){

  // INITIALIZE VARIABLES
  // STORE LOCAL COPIES OF PARAMETERS
  double HR = params[0]; // FIXED HEART RATE
  double tc = (60.0/(double)HR);
  double Csa = params[1];
  double csa = params[2];
  double dsa = params[3];
  double Vsa0 = params[4];
  // 21JAN2014 - Modified DES
  // ========================
  double tsas_ratio = params[5];
  double tsas = tsas_ratio * sqrt(tc);
  // ========================
  double tsvs_ratio = params[12];  // Apply tsvs=0.16+0.3*tc
  double tsvs = tsvs_ratio * sqrt(tc);
  // ========================
  double t1_ratio = params[6];
  double t1 = t1_ratio*tc;
  double a = params[7];
  double b = params[8];
  double csv = params[9];
  double dsv = params[10];
  double Vsv0 = params[11];
  double Rmyo = params[13];
  double alpha = params[14];
  double Kao = params[15]; // Discharge coefficient Aortic valve
  double CAo = params[16];
  double tdelay1 = params[17];
  double tdelay2 = tdelay1;
  double beta = params[19];
  double k_bw = params[20];
  double RAA = params[21];
  double CAA = params[22];

  // ======================
  // ATRIO-VENTRICULAR AREA
  // ======================
  double A_fw = 0.0;
  switch(patientID){
    case piMUSC7:
      A_fw = 2.815;
      break;
    case piGOSH22:
      A_fw = 3.263;
      break;
    case piUM5:
      A_fw = 4.207;
      break;
    case piUM10:
      A_fw = 3.674;
      break;
  }  

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
  double w = 2.0*pi/(double)tc;

  // Get The Coefficients for different Models
  getFourierCoefficients(patientID,
                         ain1,ain2,ain3,ain4,ain5,ain6,ain7,ain8,ain9,
                         bin1,bin2,bin3,bin4,bin5,bin6,bin7,bin8,
                         aout1,aout2,aout3,aout4,aout5,aout6,aout7,aout8,aout9,
                         bout1,bout2,bout3,bout4,bout5,bout6,bout7,bout8);
  
  // SHIFT TIME TO ACCOUNT FOR INPUT/OUTPUT CURVE DELAY
  double teff1 = tn + tdelay1;
  double teff2 = tn + tdelay2;

  // SUM UP VENOUS FLUX
  double Qin1 = ain9+ain1*cos(teff1*w)+bin1*sin(teff1*w)+ain2*cos(2*teff1*w);
  double Qin2 = bin2*sin(2*teff1*w)+ain3*cos(3*teff1*w)+bin3*sin(3*teff1*w);
  double Qin3 = ain4*cos(4*teff1*w)+bin4*sin(4*teff1*w)+ain5*cos(5*teff1*w);
  double Qin4 = bin5*sin(5*teff1*w)+ain6*cos(6*teff1*w)+bin6*sin(6*teff1*w);
  double Qin5 = ain7*cos(7*teff1*w)+bin7*sin(7*teff1*w)+ain8*cos(8*teff1*w) ;
  double Qin6 = bin8*sin(8*teff1*w);
  double Qven = Qin1+Qin2+Qin3+Qin4+Qin5+Qin6;

  // EVAL AORTIC FLOW RATE
  //double Qout1 = aout9 + aout1*cos(teff2*w) + bout1*sin(teff2*w) + aout2*cos(2*teff2*w);
  //double Qout2 = bout2*sin(2*teff2*w)+aout3*cos(3*teff2*w)+bout3*sin(3*teff2*w);
  //double Qout3 = aout4*cos(4*teff2*w)+bout4*sin(4*teff2*w)+aout5*cos(5*teff2*w);
  //double Qout4 = bout5*sin(5*teff2*w)+aout6*cos(6*teff2*w)+bout6*sin(6*teff2*w);
  //double Qout5 = aout7*cos(7*teff2*w)+bout7*sin(7*teff2*w)+aout8*cos(8*teff2*w);
  //double Qout6 = bout8*sin(8*teff2*w);
  //double QAo = Qout1+Qout2+Qout3+Qout4+Qout5+Qout6;
  
  // EVAL AORTIC PRESSURE
  double Pout1 = aout9 + aout1*cos(teff2*w) + bout1*sin(teff2*w) + aout2*cos(2*teff2*w);
  double Pout2 = bout2*sin(2*teff2*w)+aout3*cos(3*teff2*w)+bout3*sin(3*teff2*w);
  double Pout3 = aout4*cos(4*teff2*w)+bout4*sin(4*teff2*w)+aout5*cos(5*teff2*w);
  double Pout4 = bout5*sin(5*teff2*w)+aout6*cos(6*teff2*w)+bout6*sin(6*teff2*w);
  double Pout5 = aout7*cos(7*teff2*w)+bout7*sin(7*teff2*w)+aout8*cos(8*teff2*w);
  double Pout6 = bout8*sin(8*teff2*w);
  double PAo = Pout1+Pout2+Pout3+Pout4+Pout5+Pout6;


  // UNPACK STATE VARIABLES
  double Vsv = Xn[0]; // Volume single ventricle
  double Vsa = Xn[1]; // Volume single atrium
  double Qav = Xn[2]; // Flow Atrial-Ventricular valve
  double PAA = Xn[3]; // Pressure aorta
  //double QAA = Xn[4]; // Flow Atrial-Ventricular valve
  // ===================
  // EVAL HEART FUNCTION
  // ===================
  double tmv = fmod(tn,tc);           // Ventricle time
  double tma = fmod((tn+tsas-t1),tc); // Atrium time

  // VENTRICULAR ACTIVATION TIME
  double AV = 0.0;
  if(tmv<tsvs){
    AV = (1-cos(2*pi*tmv/(double)tsvs))/2.0;
  }else{
    AV = 0.0;
  }

  // ATRIAL ACTIVATION TIME
  double AA = 0.0;
  if(tma<tsas){
    AA = (1-cos(2*pi*tma/(double)tsas))/2.0;
  }else{
    AA = 0.0;
  }

  // ATRIUM MODEL
  double Psaactive = (Vsa-Vsa0)/(double)Csa; 
  double Psapassive = csa*(exp(dsa*(Vsa-Vsa0))-1.0);
  double Psa = AA*Psaactive + Psapassive;

  // VENTRICLE MODEL
  double Psvactive = (a*(Vsv-Vsv0) + b)*(Vsv-Vsv0);
  double Psvpassive = csv*(exp(dsv*(Vsv-Vsv0))-1.0);
  double Psv = AV*Psvactive + Psvpassive;

  // AORTIC VALVE
  double LVOF = 0.0;
  if(Psv>PAA){
    //LVOF = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAo)))/(2.0*Kao);
    LVOF = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAA)))/(2.0*Kao);
  }else{
    LVOF = 0.0;  
  }
  
  // EFFECTIVE VENTRICULAR PRESSURE
  double Psveff = Psv-Rmyo*LVOF;

  // =================
  // REGURGITANT VALVE
  // =================
  double Kav = 0.0;
  double Lav = 0.0;
  double Kav_fw = 0.0;
  double Lav_fw = 0.0;
  double Kav_bw = 0.0;
  double Lav_bw = 0.0;
  double A_bw = 0.0;
  double Qavp = 0.0;
  double AV_vel = 0.0;
  double Qreg = 0.0;

  if(fabs(k_bw)<1.0e-5){

    // Non regurgitant valve

    Kav = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2; // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav = beta*2*1.060*sqrt(4.0/(pi*A_fw))/1333.2;

    if((Psa<Psveff)&&(Qav<1.0e-5)){
      Qavp = 0.0;
    }else{
      Qavp = (Psa-Psv-Kav*Qav*fabs(Qav))/(double)Lav;
    }

    if(Qav<1.0e-5){
      Qav = 0.0;
      Xn[2] = 0.0;
    }
    
  }else{

    // Regurgitant valve     

    A_bw = k_bw*A_fw;

    Kav_fw = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2 ;  // Factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav_fw = beta*2*1.060*sqrt(4/(pi*A_fw))/1333.2;  // Factor 1333.2 to have inertance in [mmHg*s^2/ml]

    Kav_bw = alpha*0.5*1.060*(2.4/(A_bw*A_bw))/1333.2;   
    Lav_bw = beta*2*1.060*sqrt(4/(pi*A_bw))/1333.2; 

    if (Qav<0.0){
      Qavp=(Psa-Qav*fabs(Qav)*Kav_bw-Psv)/(double)Lav_bw; // One- way quadratic valve plus inertance
    }else {
      Qavp=(Psa-Qav*fabs(Qav)*Kav_fw-Psv)/(double)Lav_fw; // One- way quadratic valve plus inertance
    }

    if(Qav<0.0){
      AV_vel = Qav/(double)A_bw;
    }else{
      AV_vel = Qav/(double)A_fw;
    }
    
    if(Qav<0.0){
      Qreg = fabs(Qav);
    }else{
      Qreg = 0.0;
    }    
  }

  // COMPUTE THE FINAL DERIVATIVES
  double Vsvp = Qav-LVOF;
  double Vsap = Qven-Qav;
  double QAA = (PAA-PAo)/(double)RAA;
  double PAAp = (LVOF-QAA)/(double)CAA;
  //double QAAp = (PAA-PAo)/(double)LAA;



  // STORE TIME DERIVATIVES
  Xn1[0] = Vsvp;
  Xn1[1] = Vsap;
  Xn1[2] = Qavp;
  Xn1[3] = PAAp;
  //Xn1[4] = 0;

  // STORE SECONDARY OUTPUT
  out[0] = Psveff;
  out[1] = Psa;
  out[2] = Qven;
  out[3] = LVOF;
  out[4] = tc;
  out[5] = AA;
  out[6] = AV;
  out[7] = Psaactive;
  out[8] = Psapassive;
  out[9] = Psvactive;
  out[10] = Psvpassive;
  out[11] = PAo;
  out[12] = Qreg;
  out[13] = QAA;
}

// =============================
// EVAL MODEL FOR RK4 ITERATIONS
// =============================
void evalModel(int modelID, double t,int totalStates,double* Xk,double* params,double* DXk,double* auxOut){
  // SOLVE HEART MODEL
  heartODE(modelID,t,Xk,params,DXk,auxOut);
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

// ================================
// WRITE TARGET QUANTITTIES TO FILE
// ================================
void WriteTargetsToFile(std::string outputFileName,int totTargets,double* targetVals){
  // WRITE TO FILE
  FILE* outFile;
  outFile = fopen(outputFileName.c_str(),"w");
  for(int loopA=0;loopA<totTargets;loopA++){
    fprintf(outFile,"%f\n",targetVals[loopA]);
  }
  // CLOSE THE FILE
  fclose(outFile);
}

// =================================
// EVAL SIMULATION TARGET QUANTITIES
// =================================
void EvalSimulationTargets(int totalSteps,double timeStep,double singleCycleTime,double** outVals,
                           double& minPAo, double& maxPAo, double& minPpa, double& maxPpa, double& strokeVolLV){
  double minVol = std::numeric_limits<double>::max();
  double maxVol = -std::numeric_limits<double>::max();
  minPAo = std::numeric_limits<double>::max();
  maxPAo = -std::numeric_limits<double>::max();
  minPpa = std::numeric_limits<double>::max();
  maxPpa = -std::numeric_limits<double>::max();
  strokeVolLV = 0.0;
  double currPAo = 0.0;
  double currPpa = 0.0;
  double currVol = 0.0;
  // WRITE OUTPUT QUANTITIES
  // TARGET - LAST Cycle
  // Max Aortic Pressure: 120 mmHg - max X[9]
  // Min Aortic Pressure: 80 mmHg - min X[9]
  // Min Pulmonary Pressure: 15 mmHg - min X[4]
  // Max Pulmonary Pressure: 35 mmHg - max X[4]
  // Stroke Volume: 80 LV PV loop - fabs(max X[7] - min X[7])
  int startStep = totalSteps - floor(singleCycleTime/(double)timeStep) + 1;
  for(int loopA=startStep;loopA<totalSteps;loopA++){
    currPAo = outVals[loopA][9];
    currPpa = outVals[loopA][4];
    currVol = outVals[loopA][7];
    // PAo
    if(currPAo<minPAo){
      minPAo = currPAo;
    }
    if(currPAo>maxPAo){
      maxPAo = currPAo;
    }
    // Ppa
    if(currPpa<minPpa){
      minPpa = currPpa;
    }
    if(currPpa>maxPpa){
      maxPpa = currPpa;
    }
    // Volume
    if(currVol<minVol){
      minVol = currVol;
    }
    if(currVol>maxVol){
      maxVol = currVol;
    }
  }
  strokeVolLV = maxVol - minVol;
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
  fprintf(outFile,"\\midrule\n");
  fprintf(outFile,"Heart Rate & %f\\\\\n",params[0]);
  fprintf(outFile,"$C_{sa}$ & %f\\\\\n",params[1]);
  fprintf(outFile,"$c_{sa}$ & %f\\\\\n",params[2]);
  fprintf(outFile,"$dsa$ & %f\\\\\n",params[3]);
  fprintf(outFile,"$V_{sa,0}$ & %f\\\\\n",params[4]);
  fprintf(outFile,"$t_{sas,r}$ & %f\\\\\n",params[5]);    
  fprintf(outFile,"$t_{1,r}$ & %f\\\\\n",params[6]);
  fprintf(outFile,"$a$ & %f\\\\\n",params[7]);
  fprintf(outFile,"$b$ & %f\\\\\n",params[8]);
  fprintf(outFile,"$c_{sv}$ & %f\\\\\n",params[9]);
  fprintf(outFile,"$d_{sv}$ & %f\\\\\n",params[10]);
  fprintf(outFile,"$V_{sv,0}$ & %f\\\\\n",params[11]);
  fprintf(outFile,"$t_{svs,r}$ & %f\\\\\n",params[12]);
  fprintf(outFile,"$R_{myo}$ & %f\\\\\n",params[13]);
  fprintf(outFile,"$\\alpha$ & %f\\\\\n",params[14]);
  fprintf(outFile,"$K_{ao}$ & %f\\\\\n",params[15]);
  fprintf(outFile,"$C_{Ao}$ & %f\\\\\n",params[16]);
  fprintf(outFile,"$t_{d,1}$ & %f\\\\\n",params[17]);
  fprintf(outFile,"$t_{d,2}$ & %f\\\\\n",params[17]);
  fprintf(outFile,"$\\beta$ & %f\\\\\n",params[19]);
  fprintf(outFile,"$k_{bw}$ & %f\\\\\n",params[20]);
  fprintf(outFile,"$L_{AA}$ & %f\\\\\n",params[21]);
  fprintf(outFile,"$Q_{AA}$ & %f\\\\\n",params[22]);
  fprintf(outFile,"\\midrule\n");
  fprintf(outFile,"$V_{sv}$ & %f\\\\\n",iniVals[0]);
  fprintf(outFile,"$V_{sa}$ & %f\\\\\n",iniVals[1]);
  fprintf(outFile,"$Q_{av}$ & %f\\\\\n",iniVals[2]);
  fprintf(outFile,"$P_{AA}$ & %f\\\\\n",iniVals[3]);
  //fprintf(outFile,"$Q_{AA}$ & %f\\\\\n",iniVals[4]);
  fprintf(outFile,"\\bottomrule\n");
  fprintf(outFile,"\\end{tabular}\n");
  fprintf(outFile,"\\caption{Optimal Heart Model (with Inductance) Parameters for patient \\patient}\n");
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
  fprintf(outFile,"\\caption{Parameter limits used for heart model (with Inductance) identification, patient \\patient}\n");
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
  fprintf(outFile,"%20s %20s %20s %20s %20s %20s %20s %20s %20s %20s\n","t","Vsv","Vsa","Qven","Qav","QAA","Psv","Psa","PAo","PAA");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e %20e %20e %20e %20e %20e %20e\n",
    	    t[loopA],outVals[loopA][ipVsv],outVals[loopA][ipVsa],auxOutVals[loopA][ipQven],outVals[loopA][ipQav],auxOutVals[loopA][ipQAA],
            auxOutVals[loopA][ipPsvEff],auxOutVals[loopA][ipPsa],auxOutVals[loopA][ipPAo],outVals[loopA][ipPAA]);
  }
  fclose(outFile);
}

// ==============================
// CHECK IF THE RESULTS ARE VALID
// ==============================
bool validHeartLPNResults(double* results){
  bool answer = true;

  // mPascAo;
  if((results[0]<0.0)||(results[0]>300.0)){
  	answer = false;
  	return answer;
  }

  // maxPAo;
  if((results[1]<0.0)||(results[1]>300.0)){
  	answer = false;
  	return answer;
  }

  // mPsa;
  if((results[2]<0.0)||(results[2]>300.0)){
  	answer = false;
  	return answer;
  }

  // maxPsv;
  if((results[3]<0.0)||(results[3]>300.0)){
  	answer = false;
  	return answer;
  }

  // maxVsv;
  if((results[4]<0.0)||(results[4]>300.0)){
  	answer = false;
  	return answer;
  }

  // RETURN
  return answer;

}

// ================================
// MAIN FUNCTION FOR HEART SUBMODEL
// ================================
int solveLPN(int modelID, lpnOptions options, int totalStates, double* iniVals,int totalParams, double* params, double* results){

  // SET PARAMETERS
  // FIXED HR
  double heartRate = params[0]; 
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
    // Print Parameter Limits
    printParamLimitsTable(modelID,totalParams+totalStates);
  }

  // CHECK PARAMS ARE VALID
  if(options.applyConstraints){

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

  // Get thoracic cavity pressure
  // Pth=Pf(t);

  // Copy Auxiliary Variables From result for the last heart cycle
  double Psv[totalStepsOnSingleCycle];
  double Psa[totalStepsOnSingleCycle];
  double Qven[totalStepsOnSingleCycle];
  double LVOF[totalStepsOnSingleCycle];
  double tc2[totalStepsOnSingleCycle];
  double AA[totalStepsOnSingleCycle];
  double AV[totalStepsOnSingleCycle];
  double PsaActive[totalStepsOnSingleCycle];
  double PsaPassive[totalStepsOnSingleCycle];
  double PsvActive[totalStepsOnSingleCycle];
  double PsvPassive[totalStepsOnSingleCycle];
  double PAo[totalStepsOnSingleCycle];
  double Qreg[totalStepsOnSingleCycle];
  double QAA[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Psv[loopA] = auxOutVals[globalInd][ipPsvEff];
    Psa[loopA] = auxOutVals[globalInd][ipPsa];
    Qven[loopA] = auxOutVals[globalInd][ipQven];
    LVOF[loopA] = auxOutVals[globalInd][ipLVOF];
    AA[loopA] = auxOutVals[globalInd][ipAA];
    AV[loopA] = auxOutVals[globalInd][ipAV];
    PsaActive[loopA] = auxOutVals[globalInd][ipPsaActive];
    PsaPassive[loopA] = auxOutVals[globalInd][ipPsaPassive];
    PsvActive[loopA] = auxOutVals[globalInd][ipPsvActive];
    PsvPassive[loopA] = auxOutVals[globalInd][ipPsvPassive];
    PAo[loopA] = auxOutVals[globalInd][ipPAo];
    Qreg[loopA] = auxOutVals[globalInd][ipQreg];
    QAA[loopA] = auxOutVals[globalInd][ipQAA];
  }

  // Copy State Variables From result for the last heart cycle
  double Vsv[totalStepsOnSingleCycle];
  double Vsa[totalStepsOnSingleCycle];
  double Qav[totalStepsOnSingleCycle];
  double PAA[totalStepsOnSingleCycle];
  //double QAA[totalStepsOnSingleCycle];
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Vsv[loopA] = outVals[globalInd][ipVsv];
    Vsa[loopA] = outVals[globalInd][ipVsa];
    Qav[loopA] = outVals[globalInd][ipQav];
    PAA[loopA] = outVals[globalInd][ipPAA];
   // QAA[loopA] = outVals[globalInd][ipQAA];  
 }

  // =========================
  // CHECK FURTHER CONSTRAINTS
  // =========================
  if(options.applyConstraints){
    double maxDiff = 0.0;
    double currDiff = 0.0;
    for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
      currDiff = fabs(PsaActive[loopA]-PsaPassive[loopA]);  
      if(currDiff>maxDiff){
        maxDiff = currDiff;
      }
    }
    if(maxDiff<3.0){
      printf("ERROR: ACTIVE PASSIVE ATRIAL DIFFERENCE.\n");
      return 1;
    }    
  }

  // ===============================
  // CHECK POSITION OF MAX PSVACTIVE
  // ===============================
  int maxInd = 0;
  if(options.applyConstraints){
    maxInd = getMaxLoc(totalSteps,totalSteps-totalStepsOnSingleCycle+1,PsvActive);
    double maxVsv = getMax(totalSteps,totalSteps-totalStepsOnSingleCycle+1,Psv);
    if((Vsv[maxInd]-maxVsv)<-1.0e-3){
      printf("ERROR: PSV LOCATION.\n");
      return 1;
    }
  }

  // WHY 0.5?????
  // GET AVERAGE VALUES
  double CO = trapz(0,totalStepsOnSingleCycle,t,QAA)/(double)cycleTime;
  double mPsa = trapz(0,totalStepsOnSingleCycle,t,Psa)/(double)cycleTime;
  double mPascAo = trapz(0,totalStepsOnSingleCycle,t,PAo)/(double)cycleTime;
  double mQAA = trapz(0,totalStepsOnSingleCycle,t,QAA)/(double)cycleTime;
  double mQven = trapz(0,totalStepsOnSingleCycle,t,Qven)/(double)cycleTime;
  double mQreg = trapz(0,totalStepsOnSingleCycle,t,Qreg)/(double)cycleTime;

  // GET MAXIMUM AND MINIMUM VALUES
  double maxPsv = getMax(0,totalStepsOnSingleCycle,Psv);
  double maxPsa = getMax(0,totalStepsOnSingleCycle,Psa);
  double maxVsv = getMax(0,totalStepsOnSingleCycle,Vsv);
  double minVsv = getMin(0,totalStepsOnSingleCycle,Vsv);
  double maxPAo = getMax(0,totalStepsOnSingleCycle,PAo);
  double minPAo = getMin(0,totalStepsOnSingleCycle,PAo);
  double maxQAA = getMax(0,totalStepsOnSingleCycle,QAA);


  // =====================
  // PLOT DATA IF REQUIRED
  // =====================
  if(options.flushData){
    string outFileName("heartModelData.txt");
    flushHeartModelDataToFile(outFileName,totalSteps-3*totalStepsOnSingleCycle,totalSteps,t,outVals,auxOutVals);
  }

  //% =============
  //% STORE PV-LOOP
  //% =============
  //if(storePVLoop)
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Vsa_UM10.txt'),Vsa_temp','-append');
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Psa_UM10.txt'),Psa_temp','-append');
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Vsv_UM10.txt'),Vsv_temp','-append');
  //  dlmwrite(strcat('../04_results/09_paramstorage/pvLoops_Psv_UM10.txt'),Psv_temp','-append');
  //end

  // Copy Final Results
  results[0] = mPascAo;
  results[1] = maxPAo;
  results[2] = mPsa;
  results[3] = maxPsv;
  results[4] = maxVsv;
  results[5] = maxQAA;
  results[6] = Psv[0];
  results[7] = maxPsa;
  results[8] = mQreg;

  // printf("%f %f %f %f %f\n",mPascAo,maxPAo,mPsa,maxPsv,maxVsv);

  if(!validHeartLPNResults(results)){
  	// Return invalid
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
int getAVSD(int model,int res_num,double* meanData,double* stdData){
  double sdDefault = DATA_UNCERTAINTY;
  switch(model){
    case piMUSC7:
      // Average
      meanData[0] = 53.0;   // mPascAo
      meanData[1] = 80.0;   // maxPAo
      meanData[2] = 7.0;    // mPsa
      meanData[3] = 82.0;   // maxPsv
      meanData[4] = 29.0;   // maxVsv
      meanData[5] = 95.2;   // maxQAA
      meanData[6] = 9.88;   // PsvZero
      meanData[7] = 11.437; // maxPsa
      meanData[8] = 0.0;    // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0];
      stdData[1] = sdDefault*meanData[1];
      stdData[2] = sdDefault*meanData[2]; 
      stdData[3] = sdDefault*meanData[3];
      stdData[4] = sdDefault*meanData[4];
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = 0.83;
      stdData[7] = 0.559;
      stdData[8] = sdDefault*meanData[8];
      break;
    case piGOSH22:
      // Average
      meanData[0] = 51.0; // mPascAo
      meanData[1] = 78.0; // maxPAo
      meanData[2] = 6.0;  // mPsa
      meanData[3] = 91.0; // maxPsv
      meanData[4] = 31.0; // maxVsv
      meanData[5] = 97.0; // maxQAA
      meanData[6] = 10.0; // PsvZero
      meanData[7] = 11.0; // maxPsa
      meanData[8] = 0.0;  // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0];
      stdData[1] = sdDefault*meanData[1];
      stdData[2] = sdDefault*meanData[2];
      stdData[3] = sdDefault*meanData[3];
      stdData[4] = sdDefault*meanData[4];
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = sdDefault*meanData[6];
      stdData[7] = sdDefault*meanData[7];
      stdData[8] = sdDefault*meanData[8];
      break;
    case piUM5:
      // Average
      meanData[0] = 53.0; // mPascAo
      meanData[1] = 87.0; // maxPAo
      meanData[2] = 5.0;  // mPsa
      meanData[3] = 90.0; // maxPsv
      meanData[4] = 35.5; // maxVsv
      meanData[5] = 97.4; // maxQAA
      meanData[6] = 6.50; // PsvZero
      meanData[7] = 7.00; // maxPsa
      meanData[8] = 0.0;  // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0]; 
      stdData[1] = sdDefault*meanData[1]; 
      stdData[2] = sdDefault*meanData[2]; 
      stdData[3] = sdDefault*meanData[3]; 
      stdData[4] = sdDefault*meanData[4]; 
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = 1.56;
      stdData[7] = 0.212;
      stdData[8] = sdDefault*meanData[8];
      break;
    case piUM10:
      // Average
      meanData[0] = 72.0;  // mPascAo
      meanData[1] = 113.0; // maxPAo
      meanData[2] = 4.0;   // mPsa
      meanData[3] = 114.0; // maxPsv
      meanData[4] = 30.0;  // maxVsv
      meanData[5] = 81.3;  // maxQAA
      meanData[6] = 6.63;  // PsvZero
      meanData[7] = 8.807; // maxPsa
      meanData[8] = 10.5;  // mQreg
      // STD
      stdData[0] = sdDefault*meanData[0];  
      stdData[1] = sdDefault*meanData[1]; 
      stdData[2] = sdDefault*meanData[2];    
      stdData[3] = sdDefault*meanData[3];  
      stdData[4] = sdDefault*meanData[4];   
      stdData[5] = sdDefault*meanData[5];
      stdData[6] = 1.70;      
      stdData[7] = 0.639;  
      stdData[8] = sdDefault*meanData[8];    
      break;
  }

  // EXCLUDE INVASIVE TARGETS
  if(EXCLUDE_INVASIVE_TARG){
    meanData[2] = 0.0; // mPsa
    meanData[3] = 0.0; // maxPsv
    meanData[6] = 0.0; // PsvZero
    meanData[7] = 0.0; // maxPsa

    stdData[2] = 0.0; // mPsa
    stdData[3] = 0.0; // maxPsv
    stdData[6] = 0.0; // PsvZero
    stdData[7] = 0.0; // maxPsa
  }

  return 0;
}

// ============================
// EVAL LOG-LIKELIHOOD FUNCTION
// ============================
double evalHeartModelLogLikelihood(int model, lpnOptions options, int res_num, double* results){

  // Get Average Values and Standard Deviation for Measurements
  double meanData[res_num];
  double stdData[res_num];
  int error = getAVSD(model,res_num,meanData,stdData);
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
      fprintf(outFile,"%20s & %20f &  %20f & %20f & %20f\\\\\n",resName.c_str(),results[loopA],meanData[loopA],stdData[loopA],1.0);
    }
    fprintf(outFile,"\\bottomrule\n");
    fprintf(outFile,"\\end{tabular}\n");
    fprintf(outFile,"\\caption{Heart model (with Inductance) results for patient \\patient}\n");
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
      logLH += (meanDiff*meanDiff)/(stdData[loopA]*stdData[loopA]);   
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
  options.applyConstraints = false;
  options.flushData = true;
  options.writeDebugData = false;

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
    error = solveLPN(model,options,totalStates,iniVals,totalParams,paramsVals,results);
  }catch(...){
    error = 1;
  }

  // COMPUTE THE POSTERIOR DISTRIBUTION
  double logLIKE = 0.0;
  if(error == 0){
    logLIKE = evalHeartModelLogLikelihood(model,options,res_num,results);    
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
