#include <math.h>

#include "uq1DQuadratureRule_CC.h"
#include "uqException.h"
#include "uqConstants.h"
#include "uqUtils.h"

// Constructor Single Integration Rule
uq1DQuadratureRule_CC::uq1DQuadratureRule_CC(const int quadOrder,const int suppType): uq1DQuadratureRule(quadOrder,suppType){
  totalPoints = quadOrder;
  // Allocate the Point Vectors
  points.resize(totalPoints);
  weights.resize(totalPoints);
}

// Constructor Double Integration Rule
uq1DQuadratureRule_CCDouble::uq1DQuadratureRule_CCDouble(const int quadOrder, const int suppType): uq1DQuadratureRule(quadOrder,suppType){
  totalPoints = 2*quadOrder;
    // Allocate the Point Vectors
  points.resize(totalPoints);
  weights.resize(totalPoints);
}

// Generate Points and Weights for Clenshaw-Curtis quandrature
void uq1DQuadratureRule_CC::generatePointsAndWeights(){
  // Zero Points
  if(totalPoints < 1){
    throw uqException("Error: Not Enough Points for Quadrature.");
  } 
  // Only One Point
  if(totalPoints == 1){
    points[totalPoints] = 0.0;
    weights[totalPoints] = 2.0;
  }
  int N_Value = (totalPoints-1);
  int loopEnd = (int)(N_Value/2);
  // Allocation
  double V[loopEnd+1];
  double GX[loopEnd+1];
  double GY[loopEnd+1];
  // Evaluate the Location of the Quadrature Points
  for(int loopA=0;loopA<totalPoints;loopA++){
    points[loopA] = -cos(((loopA)*M_PI)/(totalPoints-1));
  }
  // Form Vector V
  for(int loopA=0;loopA<(loopEnd+1);loopA++){
    if(loopA == (loopEnd)){
      V[loopA] = ((N_Value-3.0)/(2.0*(((int)N_Value)/(2))-1.0))-1.0;
    }else{
      V[loopA] = 2.0/(1-4.0*(loopA)*(loopA));
    }
  }
  double w0 = (1.0/((N_Value*N_Value)-1.0+(((int)N_Value)%(2))));
  
  // Form Vector G
  for(int loopA=0;loopA<(loopEnd+1);loopA++){
    if (loopA == (loopEnd)){
      GX[loopA] = V[loopA] + w0*((2.0-(((int)N_Value)%(2)))*N_Value-1.0);
      GY[loopA] = 0.0;
    }else{
      GX[loopA] = V[loopA]-w0;
      GY[loopA] = 0.0;
    }
  }
    
  // Eval the Inverse DFT to Find the Weights
  double fftWeights[totalPoints];
  uqUtils::fft1DInv(totalPoints-1,loopEnd+1,GX,GY,fftWeights);
  
  // Assign final Weights
  for(int loopA=0;loopA<(totalPoints-1);loopA++){
    weights[loopA] = fftWeights[loopA];
  }
  weights[totalPoints-1] = fftWeights[0];
  
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

// Construct Points and Weights for Double quadrature Rule
void uq1DQuadratureRule_CCDouble::generatePointsAndWeights(){
  
  // Build Single Quadrature Rule
  uq1DQuadratureRule_CC* singleRule = new uq1DQuadratureRule_CC(quadOrder,supportType);
  singleRule->generatePointsAndWeights();
  int singleTotPoints = singleRule->getTotalPoints();
  stdVec singlePoints = singleRule->getPoints();
  stdVec singleWeights = singleRule->getWeights();

  // Build Double Quadrature Rule
  // Transform Points And Weights
  for(int loopA=0;loopA<2;loopA++){
    for(int loopB=0;loopB<singleTotPoints;loopB++){
      // Map Points
      if(supportType == kHaarRange){
        points[(loopA)*singleTotPoints+loopB] = (loopA)*0.5+0.5*singlePoints[loopB];
      }else{
        points[(loopA)*singleTotPoints+loopB] = -0.5+(loopA)*1.0+0.5*singlePoints[loopB];
      }
      // Nudge Points
      //if(loopB == 1){
      if(loopB == 0){
        points[(loopA)*singleTotPoints+loopB] = points[(loopA)*singleTotPoints+loopB] + kMathZero;
      }else if (loopB == singleTotPoints-1){
        points[(loopA)*singleTotPoints+loopB] = points[(loopA)*singleTotPoints+loopB] - kMathZero;
      }
      // Map Weights
      weights[(loopA)*singleTotPoints+loopB] = 0.5*singleWeights[loopB];
    }
  }
}


