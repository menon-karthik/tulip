#include "uq1DQuadratureRule_Regular.h"

// Constructor
uq1DQuadratureRule_Regular::uq1DQuadratureRule_Regular(const int order, const int suppType):uq1DQuadratureRule(order,suppType){
  // Total number of points decided by the order
  totalPoints = order + 1;
  // Allocate the Point Vectors
  points.resize(totalPoints);
  weights.resize(totalPoints);
}

// Distructor
uq1DQuadratureRule_Regular::~uq1DQuadratureRule_Regular(){
  // Delete Point Vectors
  points.clear();
  weights.clear();
}

// Generate Points and Weights for Regular Grid on [0,1]
void uq1DQuadratureRule_Regular::generatePointsAndWeights(){  

  // Zero Points
  if(totalPoints < 1){
    throw uqException("Error: Not Enough Points for Quadrature.");
  }else if(totalPoints == 1){
    // Only One Point
    if(supportType == kHaarRange){
      points[totalPoints] = 0.5;
      weights[totalPoints] = 1.0;
    }else{
      points[totalPoints] = 0.0;
      weights[totalPoints] = 2.0;
    }
  }else{

    // Multiple Points
    for(int loopA=0;loopA<totalPoints;loopA++){
      points[loopA] = -1.0 + (2.0/(double)(totalPoints-1))*loopA;
      weights[loopA] = (2.0/(double)(totalPoints-1));
    }
    // Half weight in First and Last Point
    weights[0] /= 2.0;
    weights[totalPoints-1] /= 2.0;    
  }
  // Scale Point if Haar Support is Used
  if(supportType == kHaarRange){
    double currentValue = 0.0;
    // Scale Points to [0,1]
    for(int loopA=0;loopA<totalPoints;loopA++){
      currentValue = points[loopA];
      currentValue = 0.5*(currentValue+1.0);
      points[loopA] = currentValue;
      weights[loopA] = 0.5*weights[loopA];
    }
  }  
}

