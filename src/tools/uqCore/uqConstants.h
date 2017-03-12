#ifndef UQCONSTANTS_H
#define UQCONSTANTS_H

// NUMERICAL CONSTANTS
//! Numerical approximation of zero.
const double kMathZero = 1.0e-7;

// QUADRATURE RANGES
//! Legendre quadrature range [-1,1].
const int kLegendreRange = 0;
//! Haar wavelet range [0,1].
const int kHaarRange = 1;

// TYPES OF POLYNOMIALS
//! Simple monomials.
const int kPolyMonomials = 0;
//! Polynomials from the Legendre family.
const int kPolyLegendre  = 1;
//! Polynomials from the Hermite family.
const int kPolyHermite   = 2;

// TYPE OF MULTI-INDEX
//! Full tensor product multi-index for multi-d polynomials
const int kMIFullOrder = 0;
//! Total order multi-index for multi-d polynomials
const int kMIPartialOrder = 1;

#endif // UQCONSTANTS
