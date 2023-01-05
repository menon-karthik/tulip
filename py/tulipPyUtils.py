import sys
import numpy as np
# Import tupil core UQ library
import tulipUQ as uq

def numpyToTulip(pyInput):
  '''
  Convert numpy matrix to tulip format
  '''
  if(len(pyInput.shape) > 2):
    print('ERROR: Cannot convert numpy tensors (dim > 2) to tulip')
    exit(-1)

  if(len(pyInput.shape) > 1):
    
    # Is matrix
    if(pyInput.dtype == np.int32):
      res = uq.stdIntMat()
      tmp = uq.stdIntVec()
    elif(pyInput.dtype == np.float64):
      res = uq.stdMat()
      tmp = uq.stdVec()
    for loopA in range(len(pyInput)):
      tmp.clear()
      for loopB in range(len(pyInput[loopA])):
        tmp.push_back(pyInput[loopA,loopB])
      res.push_back(tmp)

  else:
    
    # Is vector
    if(pyInput.dtype == np.int32):
      res = uq.stdIntVec()
    elif(pyInput.dtype == np.float64):
      res = uq.stdVec()
    for loopA in range(len(pyInput)):
      res.push_back(pyInput[loopA])

  return res
