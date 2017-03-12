# include <math.h>

# include "grFourFiberFamilyModel.h"

// COSNTRUCTOR
grFourFiberFamilyModel::grFourFiberFamilyModel(){

}

// FUNCTION TO EVALUATE THE STORED ENERGY
double grFourFiberFamilyModel::evalStoredEnergy(double axialStretch, double thetaStretch, double radialStretch, stdVec matParams){
  
  // Declare
  double c1[3];
  double c2[3];
  double alpha[3];

  // Copy parameters
  // Axial
  c1[0] = matParams[0];
  c2[0] = matParams[1];
  // Circumferential
  c1[1] = matParams[2];
  c2[1] = matParams[3];
  // Diagonal
  c1[2] = matParams[4];
  c2[2] = matParams[5];
  // Angle
  alpha[0] = 0.0;
  alpha[1] = M_PI/2.0;
  alpha[2] = matParams[6]*(M_PI/180.0);

  // Compute total energy
  double totalEnergy = 0.0;
  double firstTerm = 0.0;
  double secondTerm = 0.0;
  double currStretch = 0.0;
  double currentEn = 0.0;
  for(int loopA=0;loopA<3;loopA++){
    // Compute equivalent stretch
    firstTerm = thetaStretch*sin(alpha[loopA]);
    secondTerm = axialStretch*cos(alpha[loopA]);
    currStretch = sqrt(firstTerm*firstTerm + secondTerm*secondTerm);
    currentEn = (c1[loopA]/(4.0*c2[loopA]))*(exp(c2[loopA]*(currStretch*currStretch-1.0)*(currStretch*currStretch-1.0))-1.0);
    totalEnergy += currentEn;
  }

  // Return total energy
  return totalEnergy;

}