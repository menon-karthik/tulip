# include "odeArmVenousOcclusion.h"

// CONSTRUCTOR
odeArmVenousOcclusion::odeArmVenousOcclusion(const stdMat& pressureHist){
  stdVec temp;
  for(int loopA=0;loopA<pressureHist.size();loopA++){
    temp.clear();
    for(int loopB=0;loopB<pressureHist[0].size();loopB++){
      temp.push_back(pressureHist[loopA][0]);
      temp.push_back(pressureHist[loopA][1]);
    }
    cuffPressureHistory.push_back(temp);
  }
}

// EVAL FORCING
double evalCuffPressure(double currTime,const stdMat& cuffPressureHistory){
  return linInterp(cuffPressureHistory,0,1,currTime);
}

// NON LINEAR FUNCTION TO SOLVE
double evalFunction(double a, double area, double perim){
  double b = area/(M_PI * a);
  return M_PI * (3.0*(a+b) - sqrt((3*a+b)*(a+3*b))) - perim;
}

// DERIVATIVE OF THE NON LINEAR FUNCTION TO SOLVE
double evalDeriv(double a, double area, double perim){
  double res = 0.0;
  res += 3.0*M_PI - 3.0*area/(a*a);
  res -= (M_PI/2.0)*(1.0/sqrt(3*a*a + 9.0*area/(M_PI) + (area/M_PI) + (3.0*area*area)/(M_PI*M_PI*a*a)))*(6*a - (6*area*area)/(M_PI*M_PI*a*a*a));
  return res;
}

// ==================================================
// SOLVE NEWTON-PARHSON ITERATION TO FIND THE ELLIPSE
// SEMI-AXIS GIVE ITS AREA AND PERIMETER
// ==================================================
double solveForEllipseAxis(double area, double perim, double iniVal){
  //printf("Initial Value: %f\n",iniVal);
  double res = iniVal;
  double currIncrement = 0.0;
  bool converged = false;
  while(!converged){
    // Compute increment
    currIncrement = - evalFunction(res,area,perim)/evalDeriv(res,area,perim);
    // printf("Current Increment: %f\n",currIncrement);
    // Update semi-axis value
    res += currIncrement;
    // printf("New semi-axis: %f\n",res);
    // getchar();
    // Check convergence
    converged = fabs(currIncrement) < 1.0e-5;
  }
  return res;
}

// SOLVE SINGLE ODE STEP
void odeArmVenousOcclusion::evalDeriv(double t,const stdVec& Xk,const stdVec& params,const stdVec& fn, stdVec& DXk, stdVec& auxOut){
  // Store local copies of the parameters
  // Mean Arterial Pressure
  double map = params[0];
  // Central Venous Pressure
  double cvp = params[1];
  // Forearm Blood Flow ml/min/100 ml tissue
  double fbf = params[2];
  // Fore tissue volume in liters
  double ftv = params[3];
  // Arterial arm resistance
  double Ra  = params[4];
  // First Venous Resistance
  double Rv1 = params[5];
  // Vein Capacitance
  double C   = params[6];
  // Blood viscosity
  double visc = params[7];
  // Length of the venous segment
  double vLength = params[8];
  // Unstressed Venous Radius
  double r0 = params[9];
  // Threshold transmural pressure
  double pt0 = params[10];

  // Forcing - Cuff Pressure
  double cuffP = fn[0];

  // Unpack State Variables
  double Pc = Xk[0]; // Vein Pressure
  
  // Compute intermediate quantities
  double Rtot = (map-cvp)/((fbf/60.0)*10.0*(ftv));
  double Pt = cuffP - Pc;
  
  // Determine the radius given the external pressure
  int crossSectionShape = 0;
  double Aratio = 0.0;
  double Vseg = 0.0;
  double rSeg = 0.0;
  double sectionType = 0.0;

  if((Pt<0.0)){
    crossSectionShape = cssCircular;
    // Local Vanous compliance significantly less than C
    double Cseg = 0.01;
    Vseg = M_PI*r0*r0*vLength - Cseg*Pt;
    rSeg = sqrt(Vseg/(M_PI*vLength));
    Aratio = (Vseg/vLength)/(M_PI*r0*r0);
    sectionType = 1.0;
  }else if((Pt>=0.0)&&(Pt<pt0)){
    crossSectionShape = cssCircular;
    Aratio = 1.0-0.2208*(Pt/pt0);
    sectionType = 2.0;
  }else if((Pt>=pt0)&&(Pt<pt0 + 8.5)){
    crossSectionShape = cssElliptical;
    Aratio = 0.1777 - 0.070767*(Pt - pt0 - 8.5);
    sectionType = 3.0;
  }else if((Pt>=pt0 + 8.5)){
    crossSectionShape = cssCollapsed;
    Aratio = 0.1777 * exp(-0.399*(Pt-pt0-8.5));
    sectionType = 4.0;
  }

  // Determine the various forms of Poissiuille Low 
  // with circular, elliptical and collapsed cross section
  double Rnl = 0.0;
  if(crossSectionShape == cssCircular){
    // Determine the radius
    rSeg = r0 * sqrt(Aratio);
    // Use the Poiseuille Law for circular cross sections
    Rnl = (8.0 * visc * vLength)/(M_PI * rSeg * rSeg * rSeg * rSeg);
  }else if(crossSectionShape == cssElliptical){
    // Compute Circular Perimeter at Pt0
    double ellipsePerim = 2.0 * M_PI * r0;

    // Compute Area: CHECK THIS IS OK, I.E., 
    // THE RADIUS AT THE 0 INTRAMURAL PRESSURE r0 
    // IS EQUAL TO THE UNSTRESSED RADIUS
    double ellipseArea = Aratio * (M_PI * r0 * r0);

    // Compute Circular Radius, to start the Newton-Raphson
    // Iterations
    double circRad = sqrt(ellipseArea/M_PI);

    // Solve for the semi-axis a
    double a = solveForEllipseAxis(ellipseArea,ellipsePerim,2.0*circRad);

    // Compute the other semi-axis b
    double b = ellipseArea/(M_PI * a);

    // Determine the Modified Poiseiulle Resistance
    // for Elliptical cross section
    Rnl = ((4.0 * visc * vLength)/(M_PI)) * (a * a + b * b)/(a * a * a * b * b * b);

  }else if(crossSectionShape == cssCollapsed){
    // Area of one cylinder only
    // double cylArea = Aratio * (M_PI * r0 * r0)/2.0;
    //double cylArea = Aratio * (M_PI * r0 * r0) * 8.0;
    double cylArea = Aratio * (M_PI * r0 * r0);
    // Radius of one cilinder only
    double cylRad = sqrt(cylArea/M_PI);
    // total resistance of 2 cyliders in parallel
    Rnl = 0.5 * (8.0 * visc * vLength)/(M_PI * cylRad * cylRad * cylRad * cylRad);
  }
      
  // Assemble the two parallel contribution of basilic and chephalic veins
  // Therefore divide the resistance by half
  Rnl = 0.5 * Rnl;

  // Eval other quantities
  double Qc_IN = (map - Pc)/Ra;
  double Qc_OUT = (Pc - cvp)/(Rv1 + Rnl);
  // Eval derivatives
  double dPc = (Qc_IN - Qc_OUT)/(double)C;
  
  // Store time derivatives
  DXk[0] = dPc;

  // Store secondary output
  auxOut[0] = Qc_IN;
  auxOut[1] = Qc_OUT;
  auxOut[2] = Rtot;
  auxOut[3] = Pt;
  auxOut[4] = Aratio;
  auxOut[5] = Rnl;
  auxOut[6] = sectionType;
}


// MAIN FUNCTION FOR HEART SUBMODEL
void odeArmVenousOcclusion::postProcess(double timeStep, int totalStepsOnSingleCycle, int totalSteps, const stdMat& outVals,const stdMat& auxOutVals, stdVec& results){

  results.push_back(0.0);
}


// GET TOTAL NUMBER OF PARAMETERS
int odeArmVenousOcclusion::getParameterTotal(){
  return 11;
}

// GET PARAMETERS NAMES
string odeArmVenousOcclusion::getParamName(int parID){
  if(parID == 0){
    return string("map");
  }else if(parID == 1){
    return string("cvp");
  }else if(parID == 2){
    return string("fbf");
  }else if(parID == 3){
    return string("ftv");
  }else if(parID == 4){
    return string("Ra");
  }else if(parID == 5){
    return string("Rv1");
  }else if(parID == 6){
    return string("C");
  }else if(parID == 7){
    return string("visc");
  }else if(parID == 8){
    return string("vLength");
  }else if(parID == 9){
    return string("r0");
  }else if(parID == 10){
    return string("pt0");
  }else{
    throw cmException("ERROR: Invalid Parameter ID in getParamName.\n");
  }
}

// GET TOTAL NUMBER OF STATES
int odeArmVenousOcclusion::getStateTotal(){
  // The only state variable is the 
  // pressure in the capacitor
  return 1;
}

int odeArmVenousOcclusion::getResultTotal(){
  // Three Pressures and two flowrates
  return 1;
}

string odeArmVenousOcclusion::getResultName(int resID){
  throw cmException("ERROR: cmRCRModel::getResultName not implemented.\n");
}

void odeArmVenousOcclusion::getParameterLimits(stdVec& limits){
  limits.resize(22);
  limits[2*0 + 0] = 50.0; limits[2*0 + 1] = 120.0;
  limits[2*1 + 0] = 2.0; limits[2*1 + 1] = 15.0;
  limits[2*2 + 0] = 1.0; limits[2*2 + 1] = 8.0;
  limits[2*3 + 0] = 0.1; limits[2*3 + 1] = 5.0;
  limits[2*4 + 0] = 50.0; limits[2*4 + 1] = 500.0;
  limits[2*5 + 0] = 1.0; limits[2*5 + 1] = 20.0;
  limits[2*6 + 0] = 0.01; limits[2*6 + 1] = 2.0;
  limits[2*7 + 0] = 3.0e-5; limits[2*7 + 1] = 7.0e-5;
  limits[2*8 + 0] = 2.0; limits[2*8 + 1] = 10.0;
  limits[2*9 + 0] = 0.05; limits[2*9 + 1] = 0.5;
  limits[2*10 + 0] = 3.0; limits[2*10 + 1] = 8.0;
}

// DEFAULT MODEL PARAMETERS
void odeArmVenousOcclusion::getDefaultParams(stdVec& params){
  params.resize(11);
  // Mean Arterial Pressure
  params[0] = 100.0;
  // Central Venous Pressure
  params[1] = 5.0;
  // Forearm Blood Flow ml/min/100 ml tissue
  params[2] = 3.0;
  // Fore tissue volume in liters
  params[3] = 1.8;
  // Arterial arm resistance
  params[4] = 99.0;
  // First Venous Resistance
  params[5] = 7.0;
  // Vein Capacitance
  params[6] = 0.8;
  // Blood viscosity
  params[7] = 5.0e-5;
  // Length of the venous segment
  params[8] = 5.0;
  // Unstressed Venous Radius
  params[9] = 0.2;
  // Threshold transmural pressure
  params[10] = 5.0;
}
