# include "odeSingleVentricleStage1BlocksIND2.h"

using namespace std;

// SIMULATION COUNTER
int simulCount = 0;
// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleStage1BlocksIND2::getParameterTotal(){
  return 47;
}

// ========================
// GET NUMBER OF PARAMETERS
// ========================
int odeSingleVentricleStage1BlocksIND2::getStateTotal(){
  return 12; 
}

// ===========================
// GET TOTAL NUMBER OF RESULTS
// ===========================
int odeSingleVentricleStage1BlocksIND2::getResultTotal(){
  return 27;  
}

// ==================
// GET PARAMETER NAME
// ==================
string odeSingleVentricleStage1BlocksIND2::getParamName(int parID){
  switch(parID){
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
      return string("RAA");
      break;
    }     
    case 34:           
    {      
      return string("CAA");
      break;
    }       
    case 35:           
    {      
      return string("Vsv");
      break;
    }
    case 36:           
    {      
      return string("Vsa");
      break;
    }
    case 37:           
    {      
      return string("Pao");
      break;
    }
    case 38:           
    {      
      return string("Qav");
      break;
    }
    case 39:           
    {      
      return string("Pubv");
      break;
    }
    case 40:           
    {      
      return string("Quba");
      break;
    }
    case 41:           
    {      
      return string("Puba");
      break;
    }
    case 42:           
    {      
      return string("Plung");
      break;
    }
    case 43:           
    {      
      return string("Pvp");
      break;
    }
    case 44:           
    {      
      return string("Qapol");
      break;
    }
    case 45:           
    {      
      return string("Ppa");
      break;
    }
    case 46:           
    {      
      return string("PAA");
      break;
    }
  }
}

// ===============
// GET RESULT NAME
// ===============
string odeSingleVentricleStage1BlocksIND2::getResultName(int index){
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
      return string("vc-SD-time");
      break;
    }
    case 18:
    {      
      return string("vc-MA-time");
      break;
    }
    case 19:
    {      
      return string("vp-SD-time");
      break;
    }
    case 20:
    {      
      return string("vp-MA-time");
      break;
    }
    case 21:
    {      
      return string("maxQAA");
      break;
    }  
    case 22:
    {      
      return string("minPpa");
      break;
    }  
    case 23:
    {      
      return string("maxPpa");
      break;
    }  
    case 24:
    {      
      return string("mQreg");
      break;
    }  
    case 25:
    {      
      return string("PsvZero");
      break;
    }  
    case 26:
    {      
      return string("maxPsa");
      break;
    }  
  }
}

// ====================
// GET MODEL PARAMETERS
// ====================
void odeSingleVentricleStage1BlocksIND2::getDefaultParams(stdVec& params){
  params[0]  = 117.65;    // [0]HR
  params[1]  = 1.9608;    // [1]Csa
  params[2]  = 0.743745;  // [2]csa
  params[3]  = 0.234977;  // [3]dsa
  params[4]  = 12.2251;   // [4]Vsa0
  params[5]  = 0.227418;  // [5]tsas
  params[6]  = 0.149638;  // [6]t1_ratio
  params[7]  = -0.094775; // [7]a
  params[8]  = 14.8258;   // [8]b
  params[9]  = 1.847;     // [9]csv 
  params[10] = 0.079666;  // [10]dsv
  params[11] = 8.00639;   // [11]Vsv0
  params[12] = 0.358697;  // [12]tsvs
  params[13] = 0.028477;  // [13]Rmyo
  params[14] = 10.0582;   // [14]alpha
  params[15] = 0.000105;  // [15]Kao
  params[16] = 0.187339;  // [16]CAo
  params[17] = 0.672354;  // [17]beta
  params[18] = 3.01702;   // [18]Dsh
  params[19] = 0.154845;  // [19]Cp
  params[20] = 3.07;      // [20]SVR 
  params[21] = 0.75;      // [21]PVR
  params[22] = 0.879791;  // [22]Ctot_SVR
  params[23] = 1.1494;    // [23]C1C2ratio_SVR
  params[24] = 1.6777;    // [24]R1R2ratio_SVR
  params[25] = 0.10532;   // [25]R3R2ratio_SVR
  params[26] = 0.006058;  // [26]L_SVR 
  params[27] = 2.72694;   // [27]Ctot_PVR
  params[28] = 9.09504;   // [28]C1C2ratio_PVR
  params[29] = 0.219625;  // [29]R1R2ratio_PVR
  params[30] = 0.597933;  // [30]R3R2ratio_PVR
  params[31] = 0.044443;  // [31]L_PVR
  params[32] = 0.0;       // [32]k_bw 
  params[33] = 0.1;       // [33]RAA
  params[34] = 0.1;       // [34]CAA 
  params[35] = 33.809;    // [35]Vsv
  params[36] = 5.4992;    // [36]Vsa
  params[37] = 25.043;    // [37]Pao
  params[38] = -0.57539;  // [38]Qav
  params[39] = 17.501;    // [39]Pubv
  params[40] = 9.4518;    // [40]Quba
  params[41] = 7.2226;    // [41]Puba
  params[42] = 19.104;    // [42]Plung
  params[43] = 13.815;    // [43]Pvp
  params[44] = 31.28;     // [44]Qapol
  params[45] = 17.145;    // [45]Ppa 
  params[46] = 30.0;      // [46]PAA 
  // params[47] = 1.0;    // [47]QAA
}

// ====================
// GET PARAMETER RANGES
// ====================
void odeSingleVentricleStage1BlocksIND2::getParameterLimits(stdVec& limits){
  // Get parameter Limits
  limits[0]=117.65; limits[1]=117.65; // HR
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
  limits[30]=1.00E-04; limits[31]=2.50E-04; // Kao
  limits[32]=0.05; limits[33]=1.0; // CAo
  limits[34]=0.00; limits[35]=1.0; // Beta
  limits[36]=2.5; limits[37]=3.5; // Dsh
  limits[38]=0.1; limits[39]=0.4; // Cp
  limits[40]=3.07; limits[41]=3.07;   // SVR
  limits[42]=0.75; limits[43]=0.75;   // PVR
  limits[44]=0.163; limits[45]=0.977; // Ctot_SVR
  limits[46]=0.1; limits[47]=20.0; // C1C2ratio_SVR    
  limits[48]=0.01; limits[49]=3.0; // R1R2ratio_SVR   
  limits[50]=0.01; limits[51]=3.0; // R3R2ratio_SVR    
  limits[52]=0.005; limits[53]=0.05; // L_SVR
  limits[54]=0.667; limits[55]=4.0;   // Ctot_PVR
  limits[56]=0.1; limits[57]=20.0; // C1C2ratio_PVR   
  limits[58]=0.01; limits[59]=3.0; // R1R2ratio_PVR     
  limits[60]=0.01; limits[61]=3.0; // R3R2ratio_PVR    
  limits[62]=0.005; limits[63]=0.05; // L_PVR
  limits[64]=0.0; limits[65]=0.0;     // k_bw
  limits[66]=0.1; limits[67]=0.8; //RAA
  limits[68]=0.01; limits[69]=1.0; //CAA
  limits[70]=5.0; limits[71]=40.0; //Vsv
  limits[72]=1.0; limits[73]=15.0; //Vsa
  limits[74]=10.0; limits[75]=80.0; //Pao
  limits[76]=-0.1; limits[77]=100.0; //Qav
  limits[78]=19.8; limits[79]=24.2; //Pubv
  limits[80]=12.6; limits[81]=15.4; //Quba
  limits[82]=13.5; limits[83]=16.5; //Puba
  limits[84]=15.3; limits[85]=18.7; //Plung
  limits[86]=11.7; limits[87]=14.3; //Pvp
  limits[88]=13.5; limits[89]=16.5; //Qapol
  limits[90]=9.0; limits[91]=11.0; //Ppa
  limits[92]=10.0; limits[93]=80.0; //PAA
  //limits[94]=-10.0; limits[95]=100.0; //QAA
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

// ==============
// EVAL ODE MODEL
// ==============
void eval(double t,const stdVec& Xk,const stdVec& params,stdVec& DXk,stdVec& auxOut){

  // Init Stenosis Variables
  double k_stenosis_UB = 0.0;
  double k_stenosis_LB = 0.0;
  double k_stenosis_Llung = 0.0;
  double k_stenosis_Rlung = 0.0;

  // Heart model parameters
  double HR = params[0]; // Fixed Heart Rate
  double tc = 60.0/(double)HR;
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
  double RAA = params[33];
  double CAA = params[34];
  // Size of the atrio-ventricular Valve
  double A_fw = params[35];
  
  // Unpack state variables
  double Vsv   = Xk[0]; // Volume single ventricle
  double Vsa   = Xk[1]; // Volume single atrium
  double Pao   = Xk[2]; // Pressure aorta
  double Qav   = Xk[3]; // Flow atrio-ventricular valve
  double Pubv  = Xk[4]; // Pressure SVC
  double Quba  = Xk[5]; // Flow upper body arteries
  double Puba  = Xk[6]; 
  double Plung = Xk[7];
  double Pvp   = Xk[8];
  double Qapol = Xk[9];
  double Ppa   = Xk[10]; // Pressure in main pulmonary artery
  double PAA   = Xk[11];

  // Heart function
  double tmv = fmod(t,tc); // Ventricle time
  double tma = fmod(t+tsas-t1,tc); // Atrium time
  
  // Ventricle activation
  double AV = 0.0;
  if(tmv<tsvs){
    AV = (1.0-cos(2.0*M_PI*tmv/(double)tsvs))/2.0;
  }else{
    AV = 0.0;
  }
  
  // Atrium activation
  double AA = 0.0;
  if(tma<tsas){
    AA = (1.0-cos(2.0*M_PI*tma/(double)tsas))/2.0; 
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
  double LVOF = 0.0;
  if(Psv>PAA){
  // QAo = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-Pao)))/(2.0*Kao);
    LVOF = (-Rmyo+sqrt(Rmyo*Rmyo+4.0*Kao*(Psv-PAA)))/(2.0*Kao);
  }
  else{
  //  QAo = 0.0;
    LVOF = 0.0;
  }
 
  // Effective Ventricular Pressure
  //double Psveff = Psv-Rmyo*QAo;
  double Psveff = Psv-Rmyo*LVOF;

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
    Lav = beta*2.0*1.060*sqrt(4.0/(M_PI*A_fw))/1333.2;
    // Non regurgitant valve
    if((Psa<Psveff)&&(Qav<1.0e-5)){
      Qavp = 0.0;  
    }else{
      Qavp = (Psa-Psv-Kav*Qav*fabs(Qav))/(double)Lav;
    }
    if(Qav<1.0e-5){
      Qav = 0.0;
    }
  }else{
    // Regurgitant valve
    A_bw = k_bw*A_fw;

    // Forward Parameter
    Kav_fw = alpha*0.5*1.060*(2.4/(A_fw*A_fw))/1333.2 ; // factor 1333.2 to have resistance in [mmHg*s/ml]
    Lav_fw = beta*2*1.060*sqrt(4/(M_PI*A_fw))/1333.2; // factor 1333.2 to have inertance in [mmHg*s^2/ml]
    // Backward Parameters
    Kav_bw = alpha*0.5*1.060*(2.4/(A_bw*A_bw))/1333.2;   
    Lav_bw = beta*2*1.060*sqrt(4/(M_PI*A_bw))/1333.2; 

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
  getRespirationPressure(t,Pth,Pab,DPith,dPab);

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
  //  double Vsvp = Qav-QAo;
  double Vsvp = Qav-LVOF;
  double Vsap = Qubv+Qvp-Qav;
  // double Paop = (QAo-Qsh-Quba)/(double)Cao + DPith;
  double QAA = (PAA-Pao)/(double)RAA;
  double Paop = (QAA-Qsh-Quba)/(double)Cao + DPith;
  double Ppap = (Qsh-Qapol)/(double)Cp + DPith;
  double PAAp = (LVOF-QAA)/(double)CAA + DPith;

  // Get Derivatives
  DXk[0] = Vsvp;
  DXk[1] = Vsap;
  DXk[2] = Paop;
  DXk[3] = Qavp;
  DXk[4] = Pubvp;
  DXk[5] = Qubap;
  DXk[6] = Pubap;
  DXk[7] = Plungp;
  DXk[8] = Pvpp;
  DXk[9] = Qapolp;
  DXk[10] = Ppap;
  DXk[11] = PAAp;

  // Get Auxiliary Results
  auxOut[0] = Psveff;
  auxOut[1] = Psa;
  auxOut[2] = LVOF; //QAo;
  auxOut[3] = Qvp;
  auxOut[4] = QAA;
  auxOut[5] = t;
  auxOut[6] = AA;
  auxOut[7] = AV;
  auxOut[8] = Qubv;
  auxOut[9] = Qvp;
  auxOut[10] = Qsh;
  auxOut[11] = Quba+Qsh;
  auxOut[12] = Psaactive;
  auxOut[13] = Psapassive;
  auxOut[14] = Psvactive;
  auxOut[15] = Psvpassive;
  auxOut[16] = Qreg;
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

// ========================
// FLUSH MODEL DATA TO FILE
// ========================
void flushHeartModelDataToFile(string outFileName,int start, int stop ,double* t,double** outVals,double** auxOutVals){

  FILE* outFile;
  outFile = fopen(outFileName.c_str(),"w");
  fprintf(outFile,"%20s %20s %20s %20s %20s %20s %20s %20s %20s %20s %20s\n","t","Qvp","Qvc","Qav","QAo","Psa","Psv","PAo","Ppa","PAA","QAA");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e %20e %20e %20e %20e %20e %20e %20e\n",
          t[loopA],auxOutVals[loopA][ipQvp],auxOutVals[loopA][ipQvc],outVals[loopA][ipQav],auxOutVals[loopA][ipQAo],
          auxOutVals[loopA][ipPsa],auxOutVals[loopA][ipPsv],outVals[loopA][ipPAo],outVals[loopA][ipPpa],outVals[loopA][ipPAA],auxOutVals[loopA][ipQAA]);
  }
  fclose(outFile);

  // FLUSH PV-LOOP CURVES
  outFile = fopen("Stage12BlocksModel_IND_PVLoops.txt","w");
  fprintf(outFile,"%20s %20s %20s %20s\n","Vsa","Psa","Vsv","Psv");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e %20e\n",outVals[loopA][ipVsa],auxOutVals[loopA][ipPsa],outVals[loopA][ipVsv],auxOutVals[loopA][ipPsv]);
  }
  fclose(outFile);

  // FLUSH AV CURVES
  outFile = fopen("Stage12Blocks_IND_ModelAV.txt","w");
  fprintf(outFile,"%20s %20s %20s\n","t","Qvp","Qvc");
  for(int loopA=start;loopA<stop;loopA++){    
    fprintf(outFile,"%20e %20e %20e\n",t[loopA]-t[start],auxOutVals[loopA][ipQvp],auxOutVals[loopA][ipQvc]);
  }
  fclose(outFile);
}

// POST PROCESS MODEL RESULTS
void postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  double cycleTime = timeStep*totalStepsOnSingleCycle;

  // Copy Auxiliary Variables From result for the last heart cycle
  double Psv[totalStepsOnSingleCycle];
  double Psa[totalStepsOnSingleCycle];
  double QAo[totalStepsOnSingleCycle];
  double t[totalStepsOnSingleCycle];
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
  double QAA[totalStepsOnSingleCycle];
  int globalInd = 0;
  for(int loopA=0;loopA<totalStepsOnSingleCycle;loopA++){
    globalInd = loopA+totalSteps-totalStepsOnSingleCycle;
    Psv[loopA]        = auxOutVals[globalInd][ipPsv];
    Psa[loopA]        = auxOutVals[globalInd][ipPsa];
    QAo[loopA]        = auxOutVals[globalInd][ipQAo];
    t[loopA]          = auxOutVals[globalInd][ipT];
    AA[loopA]         = auxOutVals[globalInd][ipAA];
    AV[loopA]         = auxOutVals[globalInd][ipAV];
    Qvc[loopA]        = auxOutVals[globalInd][ipQvc];
    Qvp[loopA]        = auxOutVals[globalInd][ipQvp];
    Qsh[loopA]        = auxOutVals[globalInd][ipQsh];
    QaftCao[loopA]    = auxOutVals[globalInd][ipQaftCao];
    Psaactive[loopA]  = auxOutVals[globalInd][ipPsaactive];
    Psapassive[loopA] = auxOutVals[globalInd][ipPsapassive];
    Psvactive[loopA]  = auxOutVals[globalInd][ipPsvactive];
    Psvpassive[loopA] = auxOutVals[globalInd][ipPsvpassive];
    Qreg[loopA]       = auxOutVals[globalInd][ipQreg];
    QAA[loopA]        = auxOutVals[globalInd][ipQAA];
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
  double PAA[totalStepsOnSingleCycle];
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
    PAA[loopA]   = outVals[globalInd][ipPAA];
  }

  // EVALUATE AVERAGE AND EXTREME VALUES
  // Integrals
  double CO      = trapz(0,totalStepsOnSingleCycle,t,QAo)/(double)cycleTime;   // Cardiac Output
  double mPsa    = trapz(0,totalStepsOnSingleCycle,t,Psa)/(double)cycleTime;   // Single Atrium
  double mPascAo = trapz(0,totalStepsOnSingleCycle,t,PAo)/(double)cycleTime;   // Aorta
  double mPpa    = trapz(0,totalStepsOnSingleCycle,t,Ppa)/(double)cycleTime;   // Pulmonary Arteries
  double mQapol  = trapz(0,totalStepsOnSingleCycle,t,Qapol)/(double)cycleTime;
  double mQsh    = trapz(0,totalStepsOnSingleCycle,t,Qsh)/(double)cycleTime;
  double mQreg   = trapz(0,totalStepsOnSingleCycle,t,Qreg)/(double)cycleTime;

  // Extreme Values
  double maxPAo = getMax(0,totalStepsOnSingleCycle,PAo);
  double minPAo = getMin(0,totalStepsOnSingleCycle,PAo);
  double maxPsv = getMax(0,totalStepsOnSingleCycle,Psv);
  double maxPsa = getMax(0,totalStepsOnSingleCycle,Psa);
  double maxVsv = getMax(0,totalStepsOnSingleCycle,Vsv);
  double minVsv = getMin(0,totalStepsOnSingleCycle,Vsv);
  double maxQAo = getMax(0,totalStepsOnSingleCycle,QAo);
  double maxQAA = getMax(0,totalStepsOnSingleCycle,QAA);
  double minPpa = getMin(0,totalStepsOnSingleCycle,Ppa);
  double maxPpa = getMax(0,totalStepsOnSingleCycle,Ppa);  
  // Min and Max of curves
  double minQvc = getMin(0,totalStepsOnSingleCycle,Qvc);
  double maxQvc = getMax(0,totalStepsOnSingleCycle,Qvc);
  double minQvp = getMin(0,totalStepsOnSingleCycle,Qvp);
  double maxQvp = getMax(0,totalStepsOnSingleCycle,Qvp);

  // ==============
  // PEAK DETECTION
  // ====================================
  // APPLY CIRCULAR SHIFT TO MODEL CURVES
  // ====================================
  double tc_S,tc_M,tc_D,tc_A;
  double Qvc_S,Qvc_M,Qvc_D,Qvc_A;
  bool fc_S,fc_M,fc_D,fc_A;
  int min_fc,max_fc;

  double tp_S,tp_M,tp_D,tp_A;
  double Qvp_S,Qvp_M,Qvp_D,Qvp_A;    
  bool fp_S,fp_M,fp_D,fp_A;
  int min_fp,max_fp;
  
  // Iterate on shifted Qvc Until Peaks are Detected
  int shiftValue = 0;
  int pfCount = 0;
  bool isValidQvc = false;
  while((!isValidQvc)&&(pfCount<10)){
    applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvc); 

    isValidQvc = detect_peak(totalStepsOnSingleCycle,t,Qvc,
                             tc_S,Qvc_S,fc_S,
                             tc_M,Qvc_M,fc_M,
                             tc_D,Qvc_D,fc_D,
                             tc_A,Qvc_A,fc_A,
                             min_fc,max_fc);
    if(isValidQvc != 0){
      shiftValue += floor(totalStepsOnSingleCycle/10);
      applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvc); 
    }
    pfCount++;
  }
 
  // Iterate on shifted Qvp Until Peaks are Detected
  shiftValue = 0;
  pfCount = 0;
  bool isValidQvp = false;
  while((!isValidQvp)&&(pfCount<10)){
    applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvp); 

    isValidQvp = detect_peak(totalStepsOnSingleCycle,t,Qvp,
                             tp_S,Qvp_S,fp_S,
                             tp_M,Qvp_M,fp_M,
                             tp_D,Qvp_D,fp_D,
                             tp_A,Qvp_A,fp_A,
                             min_fp,max_fp);

    if(isValidQvp != 0){
      shiftValue += floor(totalStepsOnSingleCycle/10);
      applyCircShift(shiftValue,totalStepsOnSingleCycle,Qvp); 
    }
    pfCount++;
  }

  // GET DELAY TIMES
  double vc_SD_time = fabs(tc_S-tc_D);
  double vc_MA_time = fabs(tc_M-tc_A);
  double vp_SD_time = fabs(tp_S-tp_D);
  double vp_MA_time = fabs(tp_M-tp_A);

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
  results[9] = Qvc_S;
  results[10] = Qvc_M;
  results[11] = Qvc_D;
  results[12] = Qvc_A;
  results[13] = Qvp_S;
  results[14] = Qvp_M;
  results[15] = Qvp_D;
  results[16] = Qvp_A;
  results[17] = vc_SD_time; 
  results[18] = vc_MA_time; 
  results[19] = vp_SD_time; 
  results[20] = vp_MA_time; 
  results[21] = maxQAA;
  // Additional Variables
  results[22] = minPpa;
  results[23] = maxPpa;
  results[24] = mQreg;
  results[25] = Psv[0];
  results[26] = maxPsa;
}

