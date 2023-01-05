#ifndef TULIPUQ_H
#define TULIPUQ_H

// One-dimensional approximant
# include "uq1DApproximant.h"
// Global approximant on a single element
# include "uq1DApproximant_SE.h"
// One-dimensional multi-element approximant
# include "uq1DApproximant_ME.h"

// Numerical quadrature
# include "uq1DQuadratureRule.h"
// Tensor grid trapeziodal quadrature
# include "uq1DQuadratureRule_Regular.h"
// Clenshaw Curtis quadrature
# include "uq1DQuadratureRule_CC.h"

// N-dim Approximant
# include "uqApproximant.h"
// Single Element Approximant
# include "uqApproximant_SE.h"
// Multi Element Approximant
# include "uqApproximant_ME.h"


// Algoritmhs
# include "uqAlgorithm.h"
// Relevance vector machine regression
# include "uqAlgorithmBCS.h"

// Basis functions
# include "uqBasis.h"
// Polynomials
# include "uqPolyBasis.h"
// Orthogonal polynomials
# include "uqOrthoPolyBasis.h"
// Generalized Alpert Multiwavelets
# include "uqMWBasis.h"

// Mult-index for multi-variate polynomials
# include "uqMultiIndex.h"

// Measurement matrices
# include "uqMatrix.h"
// Polynomial matrices
# include "uqPolyMatrix.h"
// Generalized Multiwavelet matrices
# include "uqMWMatrix.h"

// Parameter space partitioning
# include "uqPartition.h"
// Binary tree partitioning
# include "uqPartitionBinaryTree.h"

// Close-form PDFs
# include "uqPDF.h"

// Class for samples
# include "uqSamples.h"

// Exceptions
# include "uqException.h"

// Constants and types
# include "uqConstants.h"
# include "uqTypes.h"

// Utils
# include "uqUtils.h"

#endif // TULIPUQ_H

