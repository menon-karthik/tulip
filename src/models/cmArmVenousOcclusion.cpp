# include "cmArmVenousOcclusion.h"

// ====================================
// COMPUTE EXPLICIT SOLUTION FOR CASE 1
// ====================================
void computeQtyCase1(int totalSteps, double timeStep,
                     double Vext, double t1, double t2,
                     double tau1,
                     double Ra,double Rv,
                     double map, double cvp,
                     double IaRest, double IvRest, 
                     double IaOccl, double IvOccl,
                     stdMat& outMatrix){
  double t = 0.0;
  bool firstTime = true;
  double iaIni = 0.0;
  double ivIni = 0.0;
  double vcIni = 0.0;
  for(int loopA=0;loopA<totalSteps;loopA++){

    if(t < t1){
      
      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = IaRest;
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = IaRest;
      // Venous Pressure
      outMatrix[loopA][ipVenousPressure] = map - Ra*IaRest;

    }else if((t >= t1)&&(t < t2)){

      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = (IaRest - IaOccl)*exp(-(t-t1)/tau1) + IaOccl;
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = (IaRest - Vext/Rv - IaOccl)*exp(-(t-t1)/tau1) + IaOccl;
      // Vanous Pressure
      outMatrix[loopA][ipVenousPressure] = Ra*(IaOccl - IaRest)*exp(-(t-t1)/tau1) + map - Ra*IaOccl;

    }else{

      if(firstTime){
        iaIni = outMatrix[loopA-1][ipArterialFlux];
        ivIni = outMatrix[loopA-1][ipVenousFlux];
        vcIni = outMatrix[loopA-1][ipVenousPressure];
        firstTime = false;
      }

      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = (iaIni - IaRest)*exp(-(t-t2)/tau1) + IaRest;
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = (ivIni + Vext/Rv - IvRest)*exp(-(t-t2)/tau1) + IvRest;
      // Venous Pressure
      outMatrix[loopA][ipVenousPressure] = (vcIni - map + Ra*IaRest)*exp(-(t-t2)/tau1) + map - Ra*IaRest;

    }

    // Update Time
    t += timeStep;
  }
}

// ====================================
// COMPUTE EXPLICIT SOLUTION FOR CASE 2
// ====================================
void computeQtyCase2(int totalSteps, double timeStep,
                     double Vext, double t1, double t2, double tStar,
                     double tau1, double tau2,
                     double Ra,double Rv,
                     double map, double cvp,
                     double IaRest, double IvRest, 
                     double IaOccl, double IvOccl,
                     stdMat& outMatrix){
  double t = 0.0;
  bool firstTimeCase3 = true;
  bool firstTimeCase4 = true;
  double iaIniCase3 = 0.0;
  double ivIniCase3 = 0.0;
  double vcIniCase3 = 0.0;
  double iaIniCase4 = 0.0;
  double ivIniCase4 = 0.0;
  double vcIniCase4 = 0.0;  
  for(int loopA=0;loopA<totalSteps;loopA++){

    printf("Current Time: %f\n",t);

    if(t < t1){
      
      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = IaRest;
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = IvRest;
      // Venous Pressure
      outMatrix[loopA][ipVenousPressure] = map - Ra*IaRest;

    }else if((t >= t1)&&(t < tStar)){

      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = IaRest*exp(-(t-t1)/tau2);
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = 0.0;
      // Vanous Pressure
      outMatrix[loopA][ipVenousPressure] = -Ra*IaRest*exp(-(t-t1)/tau2) + map;

    }else if((t >= tStar)&&(t < t2)){

      if(firstTimeCase3){
        iaIniCase3 = outMatrix[loopA-1][ipArterialFlux];
        ivIniCase3 = outMatrix[loopA-1][ipVenousFlux];
        vcIniCase3 = outMatrix[loopA-1][ipVenousPressure];
        firstTimeCase3 = false;
      }


      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = (iaIniCase3 - IaOccl)*exp(-(t-tStar)/tau1) + IaOccl;
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = -IvOccl*exp(-(t-tStar)/tau1) + IvOccl;
      // Vanous Pressure
      outMatrix[loopA][ipVenousPressure] = (vcIniCase3 - map + Ra*IaOccl)*exp(-(t-tStar)/tau1) + map - Ra*IaOccl;

    }else{

      if(firstTimeCase4){
        iaIniCase4 = outMatrix[loopA-1][ipArterialFlux];
        ivIniCase4 = outMatrix[loopA-1][ipVenousFlux];
        vcIniCase4 = outMatrix[loopA-1][ipVenousPressure];
        firstTimeCase4 = false;
      }

      // Arterial Flux
      outMatrix[loopA][ipArterialFlux]   = (iaIniCase4 - IaRest)*exp(-(t-t2)/tau1) + IaRest;
      // Venous Flux
      outMatrix[loopA][ipVenousFlux]     = (ivIniCase4 + Vext/Rv - IvRest)*exp(-(t-t2)/tau1) + IvRest;
      // Venous Pressure
      outMatrix[loopA][ipVenousPressure] = (vcIniCase4 - map + Ra*IaRest)*exp(-(t-t2)/tau1) + map - Ra*IaRest;

    }

    // Update Time
    t += timeStep;
  }
}

// ===========
// CONSTRUCTOR
// ===========
cmArmVenousOcclusion::cmArmVenousOcclusion(){
}

// ==============================
// GET TOTAL NUMBER OF PARAMETERS
// ==============================
int cmArmVenousOcclusion::getParameterTotal(){
  return 9;
}

// ====================
// GET PARAMETERS NAMES
// ====================
string cmArmVenousOcclusion::getParamName(int parID){
  if(parID == 0){
    return string("Vext");
  }else if(parID == 1){
    return string("t1");
  }else if(parID == 2){
    return string("t2");
  }else if(parID == 3){
    return string("map");
  }else if(parID == 4){
    return string("cvp");
  }else if(parID == 5){
    return string("Ra");
  }else if(parID == 6){
    return string("fbf");
  }else if(parID == 7){
    return string("ftv");
  }else if(parID == 8){
    return string("C");
  }else{
    throw cmException("ERROR: Invalid Parameter ID in getParamName.\n");
  }
}

// GET TOTAL NUMBER OF STATES
int cmArmVenousOcclusion::getStateTotal(){
  // The only state variable is the 
  // pressure in the capacitor
  return 0;
}

int cmArmVenousOcclusion::getResultTotal(){
  // Three Pressures and two flowrates
  return 1;
}

string cmArmVenousOcclusion::getResultName(int resID){
  throw cmException("ERROR: cmRCRModel::getResultName not implemented.\n");
}

// =====================
// GET PARAMETERS LIMITS
// =====================
void cmArmVenousOcclusion::getParameterLimits(stdVec& limits){
  limits.resize(18);
  limits[2*0 + 0] = 0.0;  limits[2*0 + 1] = 50.0;
  limits[2*1 + 0] = 5.0;  limits[2*1 + 1] = 20.0;
  limits[2*2 + 0] = 50.0; limits[2*2 + 1] = 80.0;
  limits[2*3 + 0] = 60.0; limits[2*3 + 1] = 120.0;
  limits[2*4 + 0] = 2.0;  limits[2*4 + 1] = 15.0;
  limits[2*5 + 0] = 50.0; limits[2*5 + 1] = 500.0;
  limits[2*6 + 0] = 1.0;  limits[2*6 + 1] = 8.0;
  limits[2*7 + 0] = 0.1;  limits[2*7 + 1] = 5.0;
  limits[2*8 + 0] = 0.01; limits[2*8 + 1] = 2.0;
}

// ========================
// DEFAULT MODEL PARAMETERS
// ========================
void cmArmVenousOcclusion::getDefaultParams(stdVec& params){
  params.resize(9);
  // External Cuff Pressure
  params[0] = 5.5;
  // Time of cuff pressure increase
  params[1] = 10.0;
  // Time of cuff pressure decrease
  params[2] = 70.0;
  // Mean Arterial Pressure
  params[3] = 100.0;
  // Central Venous Pressure
  params[4] = 5.0;
  // Arterial Resistance
  params[5] = 99.0;
  // Forearm Blood Flow
  params[6] = 3.0;
  // ForeArm tissue volume
  params[7] = 1.8;
  // Venous Capacitance
  params[8] = 0.8;
}

// PRIOR MAPPING - NOT USED IN THIS CASE
void cmArmVenousOcclusion::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping not implemented.\n"); 
}

// ==========
// EVAL MODEL
// ==========
double cmArmVenousOcclusion::evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCodes){

  // Copy Parameters
  // External Cuff Pressure
  double Vext = inputs[0];
  // Time of cuff pressure increase
  double t1   = inputs[1];
  // Time of cuff pressure decrease
  double t2   = inputs[2];
  // Mean Arterial Pressure
  double map  = inputs[3];
  // Central Venous Pressure
  double cvp  = inputs[4];
  // Arterial Resistance
  double Ra   = inputs[5];
  // Forearm Blood Flow
  double fbf  = inputs[6];
  // ForeArm tissue volume
  double ftv  = inputs[7];
  // Venous Capacitance
  double C    = inputs[8];
  
  // Write Parameters To File
  printf("Vext: %f\n",Vext);
  printf("t1: %f\n",t1);
  printf("t2: %f\n",t2);
  printf("map: %f\n",map);
  printf("cvp: %f\n",cvp);
  printf("Ra: %f\n",Ra);
  printf("fbf: %f\n",fbf);
  printf("ftv: %f\n",ftv);
  printf("C: %f\n",C);

  // Set Time Step Parameters
  double timeStep = 0.1;
  int totalSteps  = 1000;

  // Compute Total Resistance
  double Rtot = (map-cvp)/((fbf/60.0)*10.0*(ftv));

  // Compute Venous Resistance
  double Rv = Rtot - Ra;
  printf("Rv: %f\n",Rv);

  // Compute Initial Quantities
  double Vcrit  = (map - cvp)*Rv/(Ra + Rv);
  double IaRest = (map - cvp)/(Ra + Rv);
  double IvRest = IaRest;
  double IaOccl = (map - cvp - Vext)/(Ra + Rv);
  double IvOccl = IaOccl;
  printf("Vcrit: %f\n",Vcrit);
  printf("IaRest: %f\n",IaRest);
  printf("IaOccl: %f\n",IaOccl);

  // Initialize Result Matrix
  stdMat outMatrix;
  outMatrix.resize(totalSteps);
  for(int loopA=0;loopA<totalSteps;loopA++){
    outMatrix[loopA].resize(3);
  }

  // First Condition Vext < Vcrit
  if(Vext < Vcrit){

    printf("Vext < Vcrit\n");

    // Compute the unique time constant
    double tau1 = (Ra*Rv/(Ra+Rv))*C;

    // Compute Quantities over time
    computeQtyCase1(totalSteps,timeStep,
                    Vext,t1,t2,
                    tau1,
                    Ra,Rv,
                    map,cvp,
                    IaRest,IvRest, 
                    IaOccl,IvOccl,
                    outMatrix);

  }else{

    printf("Vext >= Vcrit\n");

    // Compute the first time constant
    double tau1 = (Ra*Rv/(Ra+Rv))*C;
    printf("tau1: %f\n",tau1);

    // Compute the second time constant
    double tau2 = Ra*C;
    printf("tau2: %f\n",tau2);

    // Determine Tau*
    double tStar = t1 + tau2 * log((IaRest*Ra)/(IaOccl*(Ra+Rv)));
    printf("tStar: %f\n",tStar);

    // Compute Quantities over time
    computeQtyCase2(totalSteps, timeStep,
                    Vext, t1, t2, tStar,
                    tau1, tau2,
                    Ra, Rv,
                    map, cvp,
                    IaRest, IvRest, 
                    IaOccl, IvOccl,
                    outMatrix);

  }

  // Print Matrix To File
  writeTableToFile("allData_B.txt",outMatrix);
    
  // Return the results
  outputs.clear();
  outputs.push_back(0.0);
  errorCodes.clear();
  errorCodes.push_back(0);
}

// ===========================
// EVAL MODEL: MULTIPLE INPUTS
// ===========================
stdVec cmArmVenousOcclusion::evalModelError(stdMat inputs,stdMat& outputs, stdIntVec &errorCodes){
    throw cmException("ERROR: evalModelError not implemented.\n");
}
