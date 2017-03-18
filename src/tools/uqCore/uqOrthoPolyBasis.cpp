#include <math.h>

#include "uqConstants.h"
#include "uqException.h"
#include "uqOrthoPolyBasis.h"
#include "uq1DQuadratureRule_CC.h"

// Constructor
uqOrthoPolyBasis::uqOrthoPolyBasis(int order, int level,const stdVec& measureAtQuadPoints){
  basisOrder = order;
  quadratureOrder = level;

  // Check Measures Size
  if(measureAtQuadPoints.size() != 2*level){
    throw uqException("ERROR: Not enough points in the measure, using double rule for othogonal polynomials.\n");
  }

  probabilityMeasure.clear();
  for(int loopA=0;loopA<measureAtQuadPoints.size();loopA++){
    probabilityMeasure.push_back(measureAtQuadPoints[loopA]);
  }
  alphaCoeff.resize(basisOrder);
  betaCoeff.resize(basisOrder);
  normCoeff.resize(basisOrder);
  // Build Coefficients
  buildRecursiveCoefficients();
}

// Destructor
uqOrthoPolyBasis::~uqOrthoPolyBasis(){
}

// Build Recorsive Coefficients for Orthogonal Polynomials
void uqOrthoPolyBasis::buildRecursiveCoefficients(){
  
  // Get Integration Points
  uq1DQuadratureRule* quadRule = new uq1DQuadratureRule_CCDouble(quadratureOrder,kHaarRange);
  quadRule->generatePointsAndWeights();
  int totalPoints = quadRule->getTotalPoints();
  stdVec intPoints = quadRule->getPoints();
  stdVec intWeights = quadRule->getWeights();
  
  // Initialize currPoly and prevPoly at quadrature Points
  double currPoly[totalPoints];
  double prevPoly[totalPoints];
  double tempPoly[totalPoints];
  for(int loopA=0;loopA<totalPoints;loopA++){
    currPoly[loopA] = 1.0;
    prevPoly[loopA] = 0.0;
    tempPoly[loopA] = 0.0;
  }

  // MAIN LOOP
  double integral1;
  double integral2;
  double integral3;
  for(int loopA=0;loopA<basisOrder;loopA++){
    // Eval Integrals
    integral1 = 0.0;
    integral2 = 0.0;
    integral3 = 0.0;
    for(int loopB=0;loopB<totalPoints;loopB++){
      integral1 += intPoints[loopB]*currPoly[loopB]*currPoly[loopB]*probabilityMeasure[loopB]*intWeights[loopB];
      integral2 += currPoly[loopB]*currPoly[loopB]*probabilityMeasure[loopB]*intWeights[loopB];
      integral3 += prevPoly[loopB]*prevPoly[loopB]*probabilityMeasure[loopB]*intWeights[loopB];
    }
    // Store Alphas and Betas
    alphaCoeff[loopA] = (integral1/integral2);
    if(loopA == 0){
      betaCoeff[loopA] = 0.0;
    }else{
      betaCoeff[loopA] = (integral2/integral3);
    }
    
    // Eval Norm Coefficient
    normCoeff[loopA] = 0.0;
    for(int loopB=0;loopB<totalPoints;loopB++){
      normCoeff[loopA] += currPoly[loopB]*currPoly[loopB]*probabilityMeasure[loopB]*intWeights[loopB];
    }
    normCoeff[loopA] = sqrt(normCoeff[loopA]);
    
    // Update Current and Previous Polynomials
    for(int loopB=0;loopB<totalPoints;loopB++){
      tempPoly[loopB] = prevPoly[loopB];
      prevPoly[loopB] = currPoly[loopB];
      currPoly[loopB] = (intPoints[loopB]-alphaCoeff[loopA])*currPoly[loopB]-betaCoeff[loopA]*tempPoly[loopB];
    }
  }
}

// Eval Orthogonal Polynomials
double uqOrthoPolyBasis::evaluate(double XValue, int order){
  if(order > basisOrder-1){
    throw uqException("ERROR: Order exceeds basis order.\n");
  }
  double currPoly = 1.0;
  double prevPoly = 0.0;
  double tempPoly = 0.0;
  // MAIN LOOP
  for(int loopA=0;loopA<order;loopA++){
    tempPoly = prevPoly;
    prevPoly = currPoly;
    currPoly = (XValue-alphaCoeff[loopA])*currPoly-betaCoeff[loopA]*tempPoly;
  }
  return currPoly/normCoeff[order];
}

// Get Polynomial Type for Basis
int uqOrthoPolyBasis::getPolynomialType(){
  return -1;
}

