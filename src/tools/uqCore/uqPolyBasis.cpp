#include "uqPolyBasis.h"

// Constructor
uqPolyBasis::uqPolyBasis(const int Type, int order){
  polyType = Type;
  basisOrder = order;
}

// Constructor 2
uqPolyBasis::uqPolyBasis(const int Type){
  polyType = Type;
  basisOrder = -1;
}

// Destructor
uqPolyBasis::~uqPolyBasis(){
}

// Get Polynomial Type for Basis
int uqPolyBasis::getPolynomialType(){
  return polyType;
}

// Eval Hermite Polynomials
double evalHermite(int order, double XValue){
  // Decrease Order
  if(order == 0){
    return 1.0;
  }else{
    order++;
  }
  // Hermite Polynomial Recursion
  double H_Granpa = 0.0;
  double H_Old = 1.0;
  double H_New = 0.0;
  for(int loopA=2;loopA<(order+1);loopA++){
    H_New = XValue*H_Old-(loopA-2)*H_Granpa;
    H_Granpa = H_Old;
    H_Old = H_New;
  }
  for(int loopA=2;loopA<(order);loopA++){
    H_New = H_New/sqrt(loopA);
  }
  return H_New;
}

// Eval Monomials
double evalMonomial(int order, double XValue){
  return pow(XValue,order);
}
  
// Eval Legendre Polynomials
double evalLegendre(int order, double XValue){
  double newValue = 0.0;
  if (order == 0){
    newValue = 1.0;
  }else if(order == 1){
    newValue = XValue;
  }else{
    double poly_Granpa = 1.0;
    double poly_Old = XValue;
    for(int loopA=2;loopA<(order+1);loopA++){
      newValue = (1.0/((loopA-1)+1.0))*((2.0*(loopA-1)+1.0)*XValue*poly_Old-(loopA-1)*poly_Granpa);
      // Update
      poly_Granpa = poly_Old;
      poly_Old = newValue;
    }
  }
  return (sqrt(2.0)*newValue)/sqrt(2.0/(2.0*order+1.0));
}

// Eval Generic Polynomials at requested order
double uqPolyBasis::evaluate(double XValue, int order){
  if(order > basisOrder-1){
    throw uqException("ERROR: Order exceeds basis order.\n");
  }
  double result = 0.0;
  switch(polyType){
    case kPolyMonomials:
      result = evalMonomial(order,XValue);
      break;
    case kPolyLegendre:
      result = evalLegendre(order,XValue);
      break;
    case kPolyHermite:
      result = evalHermite(order,XValue);
      break;
  }
  return result;
}



