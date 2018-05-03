# Imports
import sys
sys.path.insert(0, '../../../py')

import tulipUQ as uq
import numpy as np
import matplotlib.pyplot as plt

# =============
# MAIN FUNCTION
# =============
if __name__ == "__main__":

  # Construct samples
  samples = uq.uqSamples()
  samples.addVariable('Var1',uq.kSAMPLEUniform,-1.0,1.0)
  samples.addVariable('Var2',uq.kSAMPLEUniform,-1.0,1.0)
  samples.generateCartesianGrid(10,uq.kCC,uq.kHaarRange)

  # Construct Polynomial Matrix on samples
  polyMat = uq.uqPolyMatrix(samples,5,uq.kPolyLegendre,uq.kMIPartialOrder)

  test = np.array(polyMat.getMatrix())
  print(np.linalg.norm(test,axis=0))

  # Eval functional value with simple function
  rhs = uq.stdVec()
  rhs.resize(polyMat.getRowCount());
  for loopA in range(polyMat.getRowCount()):
    rhs[loopA] = samples.getValuesAt(loopA,0) + samples.getValuesAt(loopA,1)

  # Construct the regression algorithm
  bcs = uq.uqAlgorithmBCS();

  # Set options: Print Progress to screen
  bcs.opts.printProgressToScreen = False;
  bcs.opts.printDBGMessages      = False;

  bcsCoeffs = uq.stdVec()
  coeffPrec = uq.stdMat()
  resNorm = 0.0
  bcsReturn = bcs.run(polyMat.getRowCount(),polyMat.getColCount(), 
                      rhs,polyMat.getMatrix(),
                      bcsCoeffs,coeffPrec,resNorm);

  # Print coefficients on screen
  coeffs = np.array(bcsCoeffs)
  print('--- BCS Regression Coefficients')
  for loopA in range(len(coeffs)):
    print('%5d %10f' % (loopA,coeffs[loopA]))
  print('Residual Norm: %8e' % (resNorm))
    

