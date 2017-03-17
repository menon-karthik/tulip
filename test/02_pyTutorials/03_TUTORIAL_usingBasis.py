# Imports
import tulipUQ as uq
import numpy as np
import matplotlib.pyplot as plt

# =============
# MAIN FUNCTION
# =============
if __name__ == "__main__":

  # ==============
  # SET PARAMETERS
  # ==============

  steps     = 50
  currOrder = 3

  # Allocate Result
  results = np.zeros((steps,5))

  # ==================
  # SIMPLE POLYNOMIALS
  # ==================

  polyIntMonomial = uq.uqPolyBasis(uq.kPolyMonomials,currOrder+1)
  polyIntLegendre = uq.uqPolyBasis(uq.kPolyLegendre,currOrder+1)
  polyIntHermite  = uq.uqPolyBasis(uq.kPolyHermite,currOrder+1)

  currentLoc = 0.0
  for loopA in xrange(steps):
    currentLoc = loopA/float(steps - 1)
    results[loopA,0] = polyIntMonomial.evaluate(currentLoc,currOrder)
    results[loopA,1] = polyIntLegendre.evaluate(currentLoc,currOrder)
    results[loopA,2] = polyIntHermite.evaluate(currentLoc,currOrder)

  # ======================
  # ORTHOGONAL POLYNOMIALS
  # ======================

  # Define the number of quadrature points
  currLevel = 10

  # Define the Uniform Measure
  measureAtQuadPoints = uq.stdVec(currLevel)
  for loopA in xrange(currLevel):
    measureAtQuadPoints[loopA] = 1.0;

  orthoPolyInt = uq.uqOrthoPolyBasis(currOrder+1,currLevel,measureAtQuadPoints)
  
  currentLoc = 0.0;
  for loopA in xrange(steps):
    currentLoc = loopA/float(steps - 1)
    results[loopA,3] = orthoPolyInt.evaluate(currentLoc,currOrder)

  # =============
  # MULTIWAVELETS
  # =============

  mwInt = uq.uqMWBasis(currOrder+1,currLevel)

  currentLoc = 0.0
  for loopa in xrange(steps):
    currentLoc = loopA/float(steps - 1)
    results[loopA,4] = mwInt.EvalMotherMW(currentLoc,currOrder)

  # ==========
  # PLOT BASIS
  # ==========

  xVals = np.arange(steps)/float(steps - 1)
  print results
  plt.plot(xVals,results[:,0])
  plt.plot(xVals,results[:,1])
  plt.plot(xVals,results[:,2])
  #plt.plot(xVals,results[:,3])
  plt.plot(xVals,results[:,4])
  plt.show()

