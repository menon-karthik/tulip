# include "odeCoronary.h"

using namespace std;

// Constructor
odeCoronary::odeCoronary(){
}

int odeCoronary::getParameterTotal(){
  return 35;
}

int odeCoronary::getStateTotal(){
  return 48; 
}

int odeCoronary::getAuxStateTotal(){
  return 52;
}

int odeCoronary::getResultTotal(){
  return 22;  
}

// GET PARAMETER NAME
string odeCoronary::getParamName(int index){
  string result;
  switch(index){
    case 0: {          
      result = "Tsa";
      break; }
    case 1: {      
      result = "tpwave";
      break; }
    case 2: {      
      result = "Erv";
      break; }
    case 3: {      
      result = "Elv";
      break; }
    case 4:  {      
      result = "Elvp";
      break; }
    case 5: {      
      result = "Lrv_a";
      break; }
    case 6: {      
      result = "Rrv_a";
      break; }
    case 7: {      
      result = "Lra_v";
      break; }
    case 8: {      
      result = "Rra_v";
      break; }
    case 9: {      
      result = "Lla_v";
      break; }    
    case 10: {      
      result = "Rla_v";
      break; }    
    case 11: {      
      result = "Rlv_ao";
      break; }    
    case 12: {      
      result = "Llv_a";
      break; }    
    case 13: {      
      result = "Vrv_u";
      break; }    
    case 14: {      
      result = "Vlv_u";
      break; }    
    case 15: {      
      result = "Rpd";
      break; }
    case 16: {
	    result = "Cp";
      break; }    
    case 17: {
	    result = "Cpa";
      break; }
    case 18: {
	    result = "Kxp_ra";
      break; }
    case 19: {
	    result = "Kxv_ra";
      break; }
    case 20: {
	    result = "Emax_ra";
      break; }
    case 21: {
	    result = "Vaso_ra";
      break; }
    case 22: {
	    result = "Kxp_la";
      break; }
    case 23: {
	    result = "Kxv_la";
      break; }
    case 24: {
	    result = "Emax_la";
      break; }
    case 25: {
	    result = "Vaso_la";
      break; }
    case 26: {
	    result = "Ram_cor";
      break; }
    case 27: {
	    result = "Rv_cor";
      break; }
    case 28: {
      result = "Cam_l";
      break; }
    case 29: {
	    result = "Ca_l";
      break; }
    case 30: {
	    result = "Cam_r";
      break; }
    case 31: {
	    result = "Ca_r";
      break; }
    case 32: {
	    result = "Rrcr";
      break; }
    case 33: {
	    result = "Crcr";
      break; }
    case 34: {
	    result = "dPdt_r";
      break; }
  }
  return result;
}

// ===============
// GET RESULT NAME
// ===============
string odeCoronary::getResultName(int index){
  string result;
  switch(index){
    case 0: {      
      result = "Pao-min";
      break; }
    case 1: {      
      result = "Pao-max";
      break; }
    case 2: {      
      result = "Pao-mean";
      break; }
    case 3: {      
      result = "Aor-Cor-split";
      break; }
    case 4: {      
      result = "ABSQinlet";
      break; }
    case 5: {      
      result = "Ppul-min";
      break; }
    case 6: {      
      result = "Ppul-max";
      break; }
    case 7: {      
      result = "EF-LV";
      break; }
    case 8: {      
      result = "Qla-ratio";
      break; }
    case 9: {      
      result = "mit-valve-time";
      break; }
    case 10: {      
      result = "aor-valve-time";
      break; }
    case 11: {      
      result = "pul-valve-time";
      break; }
    case 12: {      
      result = "Prv-Pra";
      break; }
    case 13: {      
      result = "Pra-mean";
      break; }
    case 14: {      
      result = "l-cor-max-ratio";
      break; }
    case 15: {      
      result = "l-cor-tot-ratio";
      break; }
    case 16: {      
      result = "l-third-FF";
      break; }   
    case 17: {      
      result = "l-half-FF";
      break; }   
    case 18: {      
      result = "r-cor-max-ratio";
      break; }   
    case 19: {      
      result = "r-cor-tot-ratio";
      break; }       
    case 20: {      
      result = "r-third-FF";
      break; }    
    case 21: {      
      result = "r-half-FF";
      break; } 
  }
  return result;
}

// ====================
// GET MODEL PARAMETERS
// ====================
void odeCoronary::getDefaultParams(stdVec& params){
  
  params.resize(getParameterTotal());

  params[0] = 0.4;  // Tsa
  params[1] = 8.50; // tpwave

  params[2] = 0.5;  // Erv   
  params[3] = 0.8;  // Elv
  params[4] = 5.5;  // Elvp

  params[5] = 0.55; // Lrv_a
  params[6] = 50.0; // Rrv_a

  params[7] = 1.0;  // Lra_v
  params[8] = 5.0;  // Rra_v

  params[9] = 0.002;// Lla_v
  params[10] = 5.0; // Rla_v

  params[11] = 100.0;  // Rlv_ao
  params[12] = 1.0;   // Llv_a

  params[13] = 0.0; // Vrv_u
  params[14] = 0.0; // Vlv_u

  params[15] = 150.0; // Rpd
  params[16] = 3.0;   // Cp
  params[17] = 0.85;   // Cpa

  params[18] = 4.0;   // Kxp_ra
  params[19] = 0.005; // Kxv_ra
  params[20] = 0.2;   // Emax_ra
  params[21] = 0.0;   // Vaso_ra

  params[22] = 8.0;   // Kxp_la
  params[23] = 0.008;// Kxv_la
  params[24] = 0.3;   // Emax_la
  params[25] = 0.0;   // Vaso_la

  params[26] = 1.0;  // Ram_cor
  params[27] = 1.0;   // Rv_cor
  params[28] = 1.0;   // Cam_l
  params[29] = 1.0;   // Ca_l
  params[30] = 1.0;  // Cam_r
  params[31] = 1.0;  // Ca_r
  params[32] = 1.0;  // Rrcr
  params[33] = 1.0;  // Crcr
  params[34] = 0.5;  // dPdt_r
}

// ====================
// GET PARAMETER RANGES
// ====================
void odeCoronary::getParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());

  // Constrained ranges
  limits[0]=0.3000;   limits[1]=0.5000; // Tsa
  limits[2]=7.0000;   limits[3]=12.000; // tpwave
  limits[4]=0.0100;   limits[5]=10.000; // Erv
  limits[6]=0.0100;   limits[7]=15.000; // Elv
  limits[8]=0.0100;   limits[9]=10.000; // Elvp
  limits[10]=0.0001;  limits[11]=5.0;   // Lrv_a
  limits[12]=0.0001;  limits[13]=100.0; // Rrv_a
  limits[14]=0.00001; limits[15]=5.00;  // Lra_v
  limits[16]=0.00001; limits[17]=100.0; // Rra_v
  limits[18]=0.00001; limits[19]=5.00;  // Lla_v
  limits[20]=0.00001; limits[21]=100.0; // Rla_v
  limits[22]=0.00001; limits[23]=200.0; // Rlv_ao
  limits[24]=0.00001; limits[25]=5.0;   // Llv_a
  limits[26]=-100.0;  limits[27]=100.0; // Vrv_u
  limits[28]=-100.0;  limits[29]=100.0; // Vlv_u
  limits[30]=0.0001;  limits[31]=100.0; // Rpd
  limits[32]=0.0001;  limits[33]=5.0;   //Cp
  limits[34]=0.0001;  limits[35]=5.0;   //Cpa
  limits[36]=0.001;   limits[37]=10.00; //Kxp_ra
  limits[38]=0.0001;  limits[39]=0.100; //Kxv_ra
  limits[40]=0.0001;  limits[41]=1.00;  //Emax_ra
  limits[42]=-20.00;  limits[43]=20.00; //Vaso_ra
  limits[44]=0.0001;  limits[45]=10.00; //Kxp_la
  limits[46]=0.0001;  limits[47]=0.100; //Kxv_la
  limits[48]=0.0001;  limits[49]=1.000; //Emax_la
  limits[50]=-20.00;  limits[51]=20.00; //Vaso_la
  limits[52]=0.0001;  limits[53]=20.0;  //Ram_cor
  limits[54]=0.0001;  limits[55]=20.0;  //Rv_cor
  limits[56]=0.0001;  limits[57]=20.0;  //Cam_l
  limits[58]=0.0001;  limits[59]=20.0;  //Ca_l
  limits[60]=0.0001;  limits[61]=20.0;  //Cam_r
  limits[62]=0.0001;  limits[63]=20.0;  //Ca_r
  limits[64]=0.0001;  limits[65]=20.0;  //Rrcr
  limits[66]=0.0001;  limits[67]=10.0;  //Crcr
  limits[68]=0.5000;  limits[69]=1.000; //dPdt_r

/*  // COMMON PARAMETER LIMITS
  limits[0]=0.1000; limits[1]=0.5000; // Tsa
  limits[2]=5.0000; limits[3]=20.000; // tpwave
  limits[4]=0.0100; limits[5]=30.000; // Erv
  limits[6]=0.0100; limits[7]=30.000; // Elv
  limits[8]=0.0100; limits[9]=30.000; // Elvp
  limits[10]=0.0001; limits[11]=50.0; // Lrv_a
  limits[12]=0.0001;  limits[13]=500.0; // Rrv_a
  limits[14]=0.00001; limits[15]=50.00; // Lra_v
  limits[16]=0.00001; limits[17]=500.0;   // Rra_v
  limits[18]=0.00001; limits[19]=50.00; // Lla_v
  limits[20]=0.00001; limits[21]=500.0; // Rla_v
  limits[22]=0.00001; limits[23]=1000.0; // Rlv_ao
  limits[24]=0.00001; limits[25]=50.0; // Llv_a
  limits[26]=-100.0; limits[27]=100.0; // Vrv_u
  limits[28]=-100.0; limits[29]=100.0; // Vlv_u
  limits[30]=0.0001; limits[31]=500.0; // Rpd
  limits[32]=0.0001; limits[33]=5.0; //Cp
  limits[34]=0.0001; limits[35]=5.0; //Cpa
  limits[36]=0.001;  limits[37]=50.00; //Kxp_ra
  limits[38]=0.0001; limits[39]=5.000; //Kxv_ra
  limits[40]=0.0001; limits[41]=5.00; //Emax_ra
  limits[42]=-50.00; limits[43]=50.00; //Vaso_ra
  limits[44]=0.0001; limits[45]=50.00; //Kxp_la
  limits[46]=0.0001; limits[47]=5.000; //Kxv_la
  limits[48]=0.0001; limits[49]=5.000; //Emax_la
  limits[50]=-50.00; limits[51]=50.00; //Vaso_la
  limits[52]=0.0001; limits[53]=500.0; //Ram_cor
  limits[54]=0.0001; limits[55]=500.0; //Rv_cor
  limits[56]=0.0001; limits[57]=100.0; //Cam_l
  limits[58]=0.0001; limits[59]=100.0; //Ca_l
  limits[60]=0.0001; limits[61]=100.0; //Cam_r
  limits[62]=0.0001; limits[63]=100.0; //Ca_r
  limits[64]=0.0001; limits[65]=250.0; //Rrcr
  limits[66]=0.0001; limits[67]=100.0; //Crcr
  limits[68]=0.5000; limits[69]=1.000; //dPdt_r */
}

// SOLVE SINGLE ODE STEP
void odeCoronary::eval(double t,const stdVec& Xk,const stdVec& params,const stdVec& fn, stdVec& DXk, stdVec& auxOut){

  double Tsa_s = params[0];
  double tpwave_s = params[1];  
  double Erv_s = params[2];
  double Elv_s = params[3];
  double Elvp_s = params[4];
  double Lrv_a = params[5]/pConv;
  double Rrv_a = params[6]/pConv;
  double Lra_v = params[7]/pConv;
  double Rra_v = params[8]/pConv;
  double Lla_v = params[9]/pConv;
  double Rla_v = params[10]/pConv; 
  double Rlv_ao = params[11]/pConv;
  double Llv_a = params[12]/pConv;
  double Vrv_u = params[13];
  double Vlv_u = params[14];
  double Rpd = params[15]/pConv;
  double Cp = params[16];
  double Cpa = params[17];
  double Kxp_ra = params[18];
  double Kxv_ra = params[19];
  double Emax_ra = params[20];
  double Vaso_ra = params[21];
  double Kxp_la = params[22];
  double Kxv_la = params[23];
  double Emax_la = params[24];
  double Vaso_la = params[25];
 
  // Assign Curve Factors

  double Ram_cor_Factor = params[26];
  double Rlv_cor_Factor = params[27];
  double Cam_l_Factor = params[28];
  double Ca_l_Factor = params[29];
  double Cam_r_Factor = params[30];
  double Ca_r_Factor = params[31];
  double Rrcr_Factor = params[32];
  double Crcr_Factor = params[33];
  double dPdt_r = params[34];

  // Coronary boundary condition parameters
  double Ram_l[nCOR_l];
  Ram_l[0]=Ram_cor_Factor*0.85E0*198.71;
  Ram_l[1]=Ram_cor_Factor*0.85E0*299.07;
  Ram_l[2]=Ram_cor_Factor*0.85E0*191.76;
  Ram_l[3]=Ram_cor_Factor*0.85E0*370.66;
  Ram_l[4]=Ram_cor_Factor*0.85E0*426.65;
  Ram_l[5]=Ram_cor_Factor*0.85E0*370.04;
  Ram_l[6]=Ram_cor_Factor*0.85E0*841.86;
  Ram_l[7]=Ram_cor_Factor*0.85E0*841.86;
  Ram_l[8]=Ram_cor_Factor*0.85E0*365.78;
  Ram_l[9]=Ram_cor_Factor*0.85E0*758.84;
  Ram_l[10]=Ram_cor_Factor*0.85E0*350.94;

  double Ram_r[nCOR_r];
  Ram_r[0]=Ram_cor_Factor*0.85E0*340.83;
  Ram_r[1]=Ram_cor_Factor*0.85E0*157.36;
  Ram_r[2]=Ram_cor_Factor*0.85E0*350.1;
  Ram_r[3]=Ram_cor_Factor*0.85E0*339.24;
  Ram_r[4]=Ram_cor_Factor*0.85E0*522.45;

  double Rv_l[nCOR_l];
  Rv_l[0]=Rlv_cor_Factor*1.0E0*37.849;
  Rv_l[1]=Rlv_cor_Factor*1.0E0*56.966;
  Rv_l[2]=Rlv_cor_Factor*1.0E0*36.525;
  Rv_l[3]=Rlv_cor_Factor*1.0E0*70.602;
  Rv_l[4]=Rlv_cor_Factor*1.0E0*81.267;
  Rv_l[5]=Rlv_cor_Factor*1.0E0*70.485;
  Rv_l[6]=Rlv_cor_Factor*1.0E0*160.35;
  Rv_l[7]=Rlv_cor_Factor*1.0E0*160.35;
  Rv_l[8]=Rlv_cor_Factor*1.0E0*69.672;
  Rv_l[9]=Rlv_cor_Factor*1.0E0*144.54;
  Rv_l[10]=Rlv_cor_Factor*1.0E0*66.846;
  
  double Rv_r[nCOR_r];
  Rv_r[0]=Rlv_cor_Factor*1.0E0*64.92;
  Rv_r[1]=Rlv_cor_Factor*1.0E0*29.973;
  Rv_r[2]=Rlv_cor_Factor*1.0E0*66.686;
  Rv_r[3]=Rlv_cor_Factor*1.0E0*64.617;
  Rv_r[4]=Rlv_cor_Factor*1.0E0*99.515;

  double Cam_l[nCOR_l];
  Cam_l[0]=Cam_l_Factor*1.0E0*0.0119;
  Cam_l[1]=Cam_l_Factor*1.0E0*0.0087;
  Cam_l[2]=Cam_l_Factor*1.0E0*0.0122;
  Cam_l[3]=Cam_l_Factor*1.0E0*0.0073;
  Cam_l[4]=Cam_l_Factor*1.0E0*0.0066;
  Cam_l[5]=Cam_l_Factor*1.0E0*0.0074;
  Cam_l[6]=Cam_l_Factor*1.0E0*0.0039;
  Cam_l[7]=Cam_l_Factor*1.0E0*0.0039;
  Cam_l[8]=Cam_l_Factor*1.0E0*0.0074;
  Cam_l[9]=Cam_l_Factor*1.0E0*0.0042;
  Cam_l[10]=Cam_l_Factor*1.0E0*0.0077;

  double Ca_l[nCOR_l];
  Ca_l[0]=Ca_l_Factor*5.0E-1*0.0015;
  Ca_l[1]=Ca_l_Factor*5.0E-1*0.0011;
  Ca_l[2]=Ca_l_Factor*5.0E-1*0.0015;
  Ca_l[3]=Ca_l_Factor*5.0E-1*0.0009;
  Ca_l[4]=Ca_l_Factor*5.0E-1*0.0008;
  Ca_l[5]=Ca_l_Factor*5.0E-1*0.0009;
  Ca_l[6]=Ca_l_Factor*5.0E-1*0.0005;
  Ca_l[7]=Ca_l_Factor*5.0E-1*0.0005;
  Ca_l[8]=Ca_l_Factor*5.0E-1*0.0009;
  Ca_l[9]=Ca_l_Factor*5.0E-1*0.0005;
  Ca_l[10]=Ca_l_Factor*5.0E-1*0.0009;

  double Cam_r[nCOR_r];
  Cam_r[0]=Cam_r_Factor*1.0E0*0.0119;
  Cam_r[1]=Cam_r_Factor*1.0E0*0.0215;
  Cam_r[2]=Cam_r_Factor*1.0E0*0.0116;
  Cam_r[3]=Cam_r_Factor*1.0E0*0.0119;
  Cam_r[4]=Cam_r_Factor*1.0E0*0.0085;

  double Ca_r[nCOR_r];
  Ca_r[0]=Ca_r_Factor*5.0E-1*0.0015;
  Ca_r[1]=Ca_r_Factor*5.0E-1*0.0027;
  Ca_r[2]=Ca_r_Factor*5.0E-1*0.0014;
  Ca_r[3]=Ca_r_Factor*5.0E-1*0.0015;
  Ca_r[4]=Ca_r_Factor*5.0E-1*0.0011;

  double Rrcr[nRCR];
  Rrcr[0]=Rrcr_Factor*1.25E0*1.6190;
  Rrcr[1]=Rrcr_Factor*1.25E0*11.7390;
  Rrcr[2]=Rrcr_Factor*1.25E0*59.5360;
  Rrcr[3]=Rrcr_Factor*1.25E0*14.2290;
  Rrcr[4]=Rrcr_Factor*1.25E0*9.3300;
  Rrcr[5]=Rrcr_Factor*1.25E0*15.9040;

  double Crcr[nRCR];
  Crcr[0]=Crcr_Factor*1.0E0*0.0151;
  Crcr[1]=Crcr_Factor*1.0E0*0.0021;
  Crcr[2]=Crcr_Factor*1.0E0*0.0004;
  Crcr[3]=Crcr_Factor*1.0E0*0.0017;
  Crcr[4]=Crcr_Factor*1.0E0*0.0026;
  Crcr[5]=Crcr_Factor*1.0E0*0.0015;

  // Set Time integration parameters
  double Tc = 0.8696;
  double Tsa = Tc * Tsa_s;
  double tpwave = Tc/(double)tpwave_s;
  double tInCycle = fmod(t,Tc);

  // Evaluate Activation Function based on Elastance
  double AA = 0.0;
  if(tInCycle <= tpwave){
    AA = (0.5)*(1.0-cos(2.0*M_PI*(tInCycle-tpwave+Tsa)/Tsa));
  }else if((tInCycle>=(Tc-Tsa)+tpwave)and(tInCycle<Tc)){
    AA = (0.5)*(1.0-cos(2.0*M_PI*(tInCycle-tpwave-(Tc-Tsa))/Tsa));
  }else{
    AA = 0.0;
  }

  // Perform computations on resistances
  double Erv_i = 0.0;
  double Elv_i = 0.0;
  double Elv_p = 0.0;
  for(int loopA=0;loopA<numElasModes;loopA++){
    Elv_i = Elv_i + (Ft_elastance[loopA][0])*cos(2.0*M_PI*(loopA)*tInCycle/(double)Tc) 
                  - (Ft_elastance[loopA][1])*sin(2.0*M_PI*(loopA)*tInCycle/(double)Tc);
  }
  for(int loopA=0;loopA<numElasModes;loopA++){
    Elv_p = Elv_p + (Ft_elasprime[loopA][0])*cos(2.0*M_PI*(loopA)*tInCycle/(double)Tc) 
                  - (Ft_elasprime[loopA][1])*sin(2.0*M_PI*(loopA)*tInCycle/(double)Tc);
  }

  // Default Set
  Erv_i = Elv_i * Erv_s;
  Elv_i = Elv_i * Elv_s;
  Elv_p = Elv_p * Elvp_s;

  double psi_ra = Kxp_ra*(exp((Xk[0]-Vaso_ra)*Kxv_ra)-1.0);
  double psi_la = Kxp_la*(exp((Xk[5]-Vaso_la)*Kxv_la)-1.0);

  double Pra = AA*(Emax_ra*(Xk[0]-Vaso_ra)-psi_ra)+psi_ra;
  double Pla = AA*(Emax_la*(Xk[5]-Vaso_la)-psi_la)+psi_la;

  double Plv = Elv_i * (Xk[7] - Vlv_u);
  double Prv = Erv_i * (Xk[2] - Vrv_u);

  // =======================================
  // Calculate total flow through the system
  // =======================================
  double Qtot = 0.0;
  int counter = ipRCRStart;
  for(int loopA=0;loopA<nRCR;loopA++){
    Qtot = Qtot+(Xk[counter]-Pra)/(double)Rrcr[loopA];
    counter++;
  }
  // Set counter to distal Pressure of COR BC
  counter++; 
  for(int loopA=0;loopA<nCOR_l;loopA++){
    Qtot = Qtot+(Xk[counter]-Pra)/(double)Rv_l[loopA];
    counter = counter + 2;
  }
  for(int loopA=0;loopA<nCOR_r;loopA++){
    Qtot = Qtot + (Xk[counter]-Pra)/(double)Rv_r[loopA];
    counter = counter + 2;
  }


  // Initialize flags
  double Ind[nUnknowns];
  for(int loopA=0;loopA<nUnknowns;loopA++){
    Ind[loopA] = 1.0;
  }

  // Heart model ODEs
  if(Pra <= Prv){
    Ind[1]=0.0;
  }

  // Integrate Model
  DXk[0] = Qtot - Xk[1] * Ind[1];
  DXk[1] = 1.0/(double)Lra_v * (Pra-Prv-Rra_v*Xk[1]*Ind[1]);
  if(Prv <= Xk[4]){
    Ind[3] = 0.0;
  }

  DXk[2] = Ind[1]*Xk[1] - Ind[3]*Xk[3];
  DXk[3] = 1.0/(double)Lrv_a * ((Prv-Xk[4])-(Rrv_a)*Xk[3]*Ind[3]);
  DXk[4] = 1.0/Cp * (Xk[3]*Ind[3] - (Xk[4]-Pla)/(Rpd));
  if(Pla <= Plv){
    Ind[6] = 0.0;
  }
   
  DXk[5] = Ind[3]*Xk[3] - Ind[6]*Xk[6];
  DXk[6] = 1.0/Lla_v * (Pla-Plv-Rla_v*Xk[6]*Ind[6]);
  if(Plv <= Xk[9]){
    Ind[8] = 0.0;
  }
      
  DXk[7] = Ind[6]*Xk[6] - Ind[8]*Xk[8];
  DXk[8] = (1.0/(double)Llv_a)*(Plv-Xk[9]-Rlv_ao*Xk[8]*Ind[8]);  
  DXk[9] = (1.0/Cpa) * (Xk[8] + fn[nCOR+nRCR]);

  double dPvdt = (Elv_p*(Xk[7] - Vlv_u) + Elv_i * DXk[7]);

  // RCR equations start from here
  counter = ipRCRStart;
  for(int loopA=0;loopA<nRCR;loopA++){    
    DXk[counter]=(1.0/(double)Crcr[loopA]) * (fn[loopA] - (Xk[counter]-Pra)/(double)Rrcr[loopA]);
    counter++;
  }
  
  // Coronary equations start from here 
  // Left Coronary equations
  counter = ipRCRStart + nRCR;
  for(int loopA=0;loopA<nCOR_l;loopA++){    
    // 1st Coronary ODE - attached to outlet
    DXk[counter] = (1.0/Ca_l[loopA])*(fn[loopA+nRCR] -(Xk[counter]- Xk[counter+1])/(double)Ram_l[loopA]);
    counter = counter + 1;
    // 2nd Coronary ODE - not attached to outlet
    DXk[counter] = (1.0/Cam_l[loopA]) * ( ( Xk[counter-1]-Xk[counter] )/(double)Ram_l[loopA] - ( Xk[counter] - Pra )/(double)Rv_l[loopA]) + dPdt_r*dPvdt;
    // Update Counter
    counter++;
  }
  
  // Right Coronary equations 
/********************************************ALSO NEED TO BE FIXED***************************************************************/

  counter = ipRCRStart + nRCR + 2*nCOR_l;
  for(int loopA=0;loopA<nCOR_r;loopA++){
    // 1st Coronary ODE - attached to outlet
    DXk[counter] = (1.0/Ca_r[loopA])*( fn[loopA+nRCR+nCOR_l]-(Xk[counter]-Xk[counter+1])/(double)Ram_r[loopA]);
    // printf("Q[%d] = %e\n",loopA+nRCR+nCOR_l,Q[loopA]+nRCR+nCOR_l);
    counter = counter + 1;
    // 2nd Coronary ODE - not attached to outlet
    DXk[counter] = (1.0/Cam_r[loopA]) * ( ( Xk[counter-1]-Xk[counter] )/(double)Ram_r[loopA] - ( Xk[counter] - Pra )/(double)Rv_r[loopA]) + dPdt_r*dPvdt;
    counter = counter + 1;
  }

/*******************************************************************************************************************************/

  // Get Auxiliary Results
  auxOut[0] = t; // 
  auxOut[1] = tInCycle; // 
  auxOut[2] = Elv_i; // 
  auxOut[3] = Elv_p; // 
  auxOut[4] = fn[0]; // RCR-1
  auxOut[5] = fn[1]; // RCR-2
  auxOut[6] = fn[2]; // RCR-3
  auxOut[7] = fn[3]; // RCR-4 
  auxOut[8] = fn[4]; // RCR-5 
  auxOut[9] = fn[5]; // RCR-6 
  auxOut[10] = fn[6]; // LCA-1
  auxOut[11] = fn[7]; // LCA-2
  auxOut[12] = fn[8]; // LCA-3 
  auxOut[13] = fn[9]; // LCA-4 
  auxOut[14] = fn[10]; // LCA-5 
  auxOut[15] = fn[11]; // LCA-6 
  auxOut[16] = fn[12]; // LCA-7
  auxOut[17] = fn[13]; // LCA-8 
  auxOut[18] = fn[14]; // LCA-9 
  auxOut[19] = fn[15]; // LCA-10
  auxOut[20] = fn[16]; // LCA-11
  auxOut[21] = fn[17]; // RCA-1
  auxOut[22] = fn[18]; // RCA-2
  auxOut[23] = fn[19]; // RCA-3
  auxOut[24] = fn[20]; // RCA-4
  auxOut[25] = fn[21]; // RCA-5
  auxOut[26] = fn[22]; // INLET 
  auxOut[27] = dPvdt; // 
  auxOut[28] = Qtot; // 
  auxOut[29] = Prv; // 
  auxOut[30] = Plv; // 
  auxOut[31] = 0.0;
  auxOut[32] = 0.0;
  auxOut[33] = 0.0;
  auxOut[34] = 0.0;
  auxOut[35] = 0.0;
  auxOut[36] = 0.0;
  auxOut[37] = 0.0;
  auxOut[38] = 0.0;
  auxOut[39] = AA;
  auxOut[40] = Pra; // 
  auxOut[41] = Pla; // 
  auxOut[42] = 0.0;
  auxOut[43] = 0.0;
  auxOut[44] = 0.0;
  auxOut[45] = 0.0;
  auxOut[46] = Ind[6]; // Mitral Valve
  auxOut[47] = Ind[8]; // Aortic Valve
  auxOut[48] = Ind[1]; // Tricuspid Valve
  auxOut[49] = Ind[3]; // Pulmonary Valve
  auxOut[50] = 0.0;
  auxOut[51] = 0.0;
}

// POST-PROCESS CORONARY RESULTS
void odeCoronary::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  // Get the total rows in auxOutVals
  int totOutputSteps = auxOutVals.size();
  double t[totOutputSteps];
  for(int loopA=0;loopA<totOutputSteps;loopA++){
    t[loopA] = auxOutVals[loopA][0];
  }
    
  // Set values for result inquiry
  int ao_valve = 47;
  int mit_valve = 46;
  int tri_valve = 48;
  int pulm_valve = 49;
  int rcr_st = 4;
  int l_cor_st = 10;
  int r_cor_st = 21;

  // SUM RCR FLUX
  double temp = 0.0;
  double Q_rcr = 0.0;
  for(int loopA=0;loopA<nRCR;loopA++){
    temp = trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,auxOutVals[loopA+rcr_st]);
    Q_rcr += temp;
  }
  
  // SUM LEFT CORONARY FLUX
  double left_check[nCOR_l];
  double Q_lcor = 0.0;
  for(int loopA=0;loopA<nCOR_l;loopA++){  
    temp = trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,auxOutVals[l_cor_st+loopA]);
    left_check[loopA] = temp;
    Q_lcor = Q_lcor + temp;
  }

  // INTEGRATE MAIN FLOW
  double lmain_flow = trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,auxOutVals[l_cor_st]);

  // SUM RIGHT CORONARY FLUX
  double right_check[nCOR_r];
  double Q_rcor = 0.0;
  for(int loopA=0;loopA<nCOR_r;loopA++){    
    temp = trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,auxOutVals[r_cor_st+loopA]);
    right_check[loopA] = temp;
    Q_rcor = Q_rcor + temp;
  }

  // SUM RIGHT CORONARY FLUX
  double rmain_flow = trapz(totOutputSteps-totalStepsOnSingleCycle-1,totOutputSteps,t,auxOutVals[r_cor_st]);

  // FIND THE END OF SYSTOLE
  int systole_end = totOutputSteps-totalStepsOnSingleCycle/2 - 1;
  for(int count = totOutputSteps-totalStepsOnSingleCycle-1; count < totOutputSteps-1; count++) {
     if(auxOutVals[ao_valve][count] > 0.0 && auxOutVals[ao_valve][count+1] == 0.0) {
        systole_end = count;
 //       printf("SYSTOLE END FOUND TO BE: %d\n",systole_end);
        break;
     }
  }

  // FIND THE START OF SYSTOLE
  int systole_start = totOutputSteps - totalStepsOnSingleCycle - 1;
  for(int count = totOutputSteps - 1; count > totOutputSteps-totalStepsOnSingleCycle - 1; count--) {
     if(auxOutVals[mit_valve][count] == 0.0 && auxOutVals[mit_valve][count-1] > 0.0) {
        systole_start = count;
        break;
     }
  }

  // FIND WHEN AORTIC VALVE OPENS
  int ao_open = totOutputSteps - totalStepsOnSingleCycle - 1;
  for(int count = totOutputSteps - totalStepsOnSingleCycle - 1; count < totOutputSteps - 1; count++) {
     if(auxOutVals[ao_valve][count] == 0.0 && auxOutVals[ao_valve][count+1] > 0.0) {
        ao_open = count;
        break;
     }
  }

  // FIND WHEN MITRAL VALVE OPENS    
  int mit_open = totOutputSteps - totalStepsOnSingleCycle - 1;
  for(int count = totOutputSteps - totalStepsOnSingleCycle - 1; count < totOutputSteps-1; count++) {
     if(auxOutVals[mit_valve][count] == 0.0 && auxOutVals[mit_valve][count+1] > 0.0) {
        mit_open = count;
        break;
     }
  }
   
  int mit_half = round((mit_open+systole_start)/(double)2.0);
  int aor_half = round((ao_open+systole_end)/(double)2.0);
    
  // CALCULATE MAX AND TOTAL CORONARY FLOW DURING SYSTOLE
  double l_cor_qmax_s = max(getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end,auxOutVals[l_cor_st]),getMax(systole_start, totOutputSteps,auxOutVals[l_cor_st]));
  double l_cor_qtot_s = trapz(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end,t,auxOutVals[l_cor_st]) + trapz(systole_start, totOutputSteps,t,auxOutVals[l_cor_st]);
  double r_cor_qmax_s = max(getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end,auxOutVals[r_cor_st]),getMax(systole_start, totOutputSteps,auxOutVals[r_cor_st]));
  double r_cor_qtot_s = trapz(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end,t,auxOutVals[r_cor_st]) + trapz(systole_start, totOutputSteps,t,auxOutVals[r_cor_st]);
    
  // CALCULATE MAX AND TOTAL CORONARY FLOW DURING DIASTOLE
  double l_cor_qmax_d = getMax(systole_end, systole_start,auxOutVals[l_cor_st]);
  double l_cor_qtot_d = trapz(systole_end, systole_start,t,auxOutVals[l_cor_st]);  
  double r_cor_qmax_d = getMax(systole_end, systole_start,auxOutVals[r_cor_st]);
  double r_cor_qtot_d = trapz(systole_end, systole_start,t,auxOutVals[r_cor_st]);
    
  // CALCULATE RATIOS (DIASTOLE TO SYSTOLE)
  double r_cor_max_ratio = r_cor_qmax_d/r_cor_qmax_s;
  double r_cor_tot_ratio = r_cor_qtot_d/r_cor_qtot_s;
  double l_cor_max_ratio = l_cor_qmax_d/l_cor_qmax_s;
  double l_cor_tot_ratio = l_cor_qtot_d/l_cor_qtot_s;

  // CALCULATE 1/3 FF AND 1/2 FF
  double r_third_FF, r_half_FF, l_third_FF, l_half_FF;
  int thirdcyc = round(totalStepsOnSingleCycle/3);
  int halfcyc = round(totalStepsOnSingleCycle/2);
  if(systole_end+thirdcyc-1 < totOutputSteps) {
     r_third_FF = trapz(systole_end-1, systole_end+thirdcyc,t,auxOutVals[r_cor_st])/rmain_flow;
     l_third_FF = trapz(systole_end-1, systole_end+thirdcyc,t,auxOutVals[l_cor_st])/lmain_flow;
  } else {
     r_third_FF = 0;
     l_third_FF = 0;
  }

  if(systole_end+halfcyc-1 < totOutputSteps) {
     r_half_FF = trapz(systole_end-1, systole_end+halfcyc,t,auxOutVals[r_cor_st])/rmain_flow;
     l_half_FF = trapz(systole_end-1, systole_end+halfcyc,t,auxOutVals[l_cor_st])/lmain_flow;
  } else {
     r_half_FF = 0;
     l_half_FF = 0;
  }

//  double result = 0.0;
//  for(int loopA=totalSteps - totalStepsOnSingleCycle - 1;loopA<(totalSteps-1);loopA++){
//    result += 0.5*(auxOutVals[27][loopA+1] + auxOutVals[27][loopA])*(t[loopA+1] - t[loopA]);
 // }

  // COMPUTE QUANTITIES
  double Qinlet = trapz(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps,t,auxOutVals[26]);
  double Aor_Cor_split = ((Q_lcor+Q_rcor)/(Q_lcor+Q_rcor+Q_rcr))*100.0;
  double Pao_max = getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[9]);
  double Pao_min = getMin(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[9]);
  double Pao_mean = getMean(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[9]);
  double Ppul_max = getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[4]);
  double Ppul_min = getMin(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[4]);
  double EF_LV = (getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[7]) - getMin(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[7]))/getMax(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, outVals[7]);
  double Prv_Pra = getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end, auxOutVals[29]) - getMax(totOutputSteps - totalStepsOnSingleCycle - 1, systole_end, auxOutVals[40]);
  // double Ppul_Prv = getMax(systole_end-1, systole_start, auxOutVals[29]) - getMax(systole_end-1, systole_start, outVals[4]);
  double mit_valve_time = getSum(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, auxOutVals[mit_valve])/(double)totalStepsOnSingleCycle;
  double aor_valve_time = getSum(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, auxOutVals[ao_valve])/(double)totalStepsOnSingleCycle;
  double pul_valve_time = getSum(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, auxOutVals[pulm_valve])/(double)totalStepsOnSingleCycle;
  // QLA_Ratio
  double Qla_ratio_num = getMax(mit_open-1,mit_half,outVals[6]);
  double Qla_ratio_den = getMax(mit_half-1,systole_start,outVals[6]);
  double Qla_ratio = Qla_ratio_num/Qla_ratio_den;
  double Pra_mean = getMean(totOutputSteps - totalStepsOnSingleCycle - 1, totOutputSteps, auxOutVals[40]);
  if(r_cor_max_ratio < 0 || l_cor_max_ratio < 0) {
      r_cor_max_ratio = 9001.0;
  }
  
  // WRITE RESULT
  results[0] = Pao_min;
  results[1] = Pao_max;
  results[2] = Pao_mean;
  results[3] = Aor_Cor_split;
  results[4] = fabs(Qinlet);
  results[5] = Ppul_min;
  results[6] = Ppul_max;
  results[7] = EF_LV;
  results[8] = Qla_ratio;
  results[9] = mit_valve_time;
  results[10] = aor_valve_time;
  results[11] = pul_valve_time;
  results[12] = Prv_Pra;
  results[13] = Pra_mean;
  results[14] = l_cor_max_ratio;
  results[15] = l_cor_tot_ratio;
  results[16] = l_third_FF;
  results[17] = l_half_FF;
  results[18] = r_cor_max_ratio;
  results[19] = r_cor_tot_ratio;
  results[20] = r_third_FF;
  results[21] = r_half_FF;
  
  // KEYS
  vector<string> keys;
  keys.push_back(string("PaoMin"));
  keys.push_back(string("PaoMax"));
  keys.push_back(string("PaoMean"));
  keys.push_back(string("AorCorSplit"));
  keys.push_back(string("AbsQin"));
  keys.push_back(string("PpulMin"));
  keys.push_back(string("PpulMax"));
  keys.push_back(string("EFLV"));
  keys.push_back(string("QlaRatio"));
  keys.push_back(string("mitValveTime"));
  keys.push_back(string("aorValveTime"));
  keys.push_back(string("pulValveTime"));
  keys.push_back(string("PrvPra"));
  keys.push_back(string("PraMean"));
  keys.push_back(string("LCorMaxRatio"));
  keys.push_back(string("LCorTotRatio"));
  keys.push_back(string("LThirdFF"));
  keys.push_back(string("LHalfFF"));
  keys.push_back(string("RCorMaxRatio"));
  keys.push_back(string("RCorTotRatio"));
  keys.push_back(string("RThirdFF"));
  keys.push_back(string("RHalfFF"));

  stdVec computedValues;
  computedValues.push_back(results[0]);
  computedValues.push_back(results[1]);
  computedValues.push_back(results[2]);
  computedValues.push_back(results[3]);
  computedValues.push_back(results[4]);
  computedValues.push_back(results[5]);
  computedValues.push_back(results[6]);
  computedValues.push_back(results[7]);
  computedValues.push_back(results[8]);
  computedValues.push_back(results[9]);
  computedValues.push_back(results[10]);
  computedValues.push_back(results[11]);
  computedValues.push_back(results[12]);
  computedValues.push_back(results[13]);
  computedValues.push_back(results[14]);
  computedValues.push_back(results[15]);
  computedValues.push_back(results[16]);
  computedValues.push_back(results[17]);
  computedValues.push_back(results[18]);
  computedValues.push_back(results[19]);
  computedValues.push_back(results[20]);
  computedValues.push_back(results[21]);

  stdVec stdFactors;
  stdFactors.push_back(9.0/83.242);
  stdFactors.push_back(14.0/132.26);
  stdFactors.push_back(10.7/107.36);
  stdFactors.push_back(0.4/4.616);
  stdFactors.push_back(15.25/69.67);
  stdFactors.push_back(2.0/11.18);
  stdFactors.push_back(5.0/22.18);
  stdFactors.push_back(0.053/0.4726);
  stdFactors.push_back(0.13/0.734);
  stdFactors.push_back(0.06/0.405);
  stdFactors.push_back(0.036/0.233);
  stdFactors.push_back(0.05/0.2794);
  stdFactors.push_back(5.0/22.713);
  stdFactors.push_back(1.0/3.291);
  stdFactors.push_back(0.8/1.985);
  stdFactors.push_back(1.73/4.73);
  stdFactors.push_back(0.02/0.4825);
  stdFactors.push_back(0.03/0.717);
  stdFactors.push_back(0.3/1.154);
  stdFactors.push_back(0.24/1.964);
  stdFactors.push_back(0.07/0.373);
  stdFactors.push_back(0.07/0.567);
  
  stdVec weigths;
  weigths.push_back(1.0);  // PaoMin
  weigths.push_back(1.0);  // PaoMax
  weigths.push_back(10.0); // PaoMean
  weigths.push_back(1.0);  // AorCorSplit
  weigths.push_back(1.0);  // AbsQin
  weigths.push_back(10.0); // PpulMin
  weigths.push_back(10.0); // PpulMax
  weigths.push_back(1.0);  // EFLV
  weigths.push_back(1.0);  // QlaRatio
  weigths.push_back(1.0);  // mitValveTime
  weigths.push_back(1.0);  // aorValveTime
  weigths.push_back(1.0);  // pulValveTime
  weigths.push_back(10.0); // PrvPra
  weigths.push_back(10.0); // PraMean
  weigths.push_back(10.0); // LCorMaxRatio
  weigths.push_back(10.0); // LCorTotRatio
  weigths.push_back(10.0); // LThirdFF
  weigths.push_back(10.0); // LHalfFF
  weigths.push_back(10.0); // RCorMaxRatio
  weigths.push_back(10.0); // RCorTotRatio
  weigths.push_back(10.0); // RThirdFF
  weigths.push_back(10.0); // RHalfFF

  // Print and compare
  double result = 0.0;
  if(data != NULL){

    // Print Info
    if(options.flushData){
      data->printAndCompare(datasetColumn,keys,computedValues,weigths);  
    }
  
    // Evaluate Log Likelihood
    result = data->evalLogLikelihood(datasetColumn,keys,computedValues,stdFactors,weigths);

    printf("LL: %.5f\n",result);
    
  }
  return result;
}
