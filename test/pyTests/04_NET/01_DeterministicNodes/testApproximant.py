import sys
sys.path.insert(0, '../../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
# Import tulip UQ library
import tulipUQ as uq
import tulipAC as ac

def numpyToStdMat(npMat):
  if(len(npMat.shape) == 1):
    npMat = npMat.reshape(-1,1)
  res = uq.stdMat()
  tmp = uq.stdVec()
  for loopA in range(npMat.shape[0]):
    tmp.clear()
    for loopB in range(npMat.shape[1]):
      tmp.push_back(npMat[loopA,loopB])
    res.push_back(tmp)
  return res

def evalModel(samples1):
  # Simple sum across dimensions
  return np.sum(samples1,axis=1)

def genAndSaveApproximant(inTableFile,outTableFile,addSampleLocs,approxFileName):

  # Read file with input samples 
  inputs = np.loadtxt(inTableFile)
  # Read file with output samples
  outputs = np.loadtxt(outTableFile)

  # Create uqSamples for inputs
  ins = uq.uqSamples(numpyToStdMat(inputs))
  outs = uq.uqSamples(numpyToStdMat(outputs))

  # Set the parameters for PC surrogate creation
  order = 2
  polyType = uq.kPolyLegendre
  completeOrderType = uq.kMIPartialOrder
  # completeOrderType = uq.kMIFullOrder

  # Initialize Polynomial Chaos Approximant
  pc = ac.acActionUP_PC(ins,outs,order,polyType,completeOrderType)
  # Train Surrogate
  pc.go()
  # Get Approximant
  approx = pc.getApproximant()

  # Generate new realizations with the approximant
  addRes = uq.stdMat()
  addRes = approx.evaluate(numpyToStdMat(addSampleLocs));

  # Save the approximant to file
  approx.exportToFile(approxFileName)

  # Return the results
  return np.array(addRes).T

# Pick Approximant from file and evaluate additional samples
def readAndEvalApproximant(fileName,samples):

  approx = uq.uqApproximant_SE()
  approx.importFromFile(fileName)
  res = uq.stdMat()
  res = approx.evaluate(numpyToStdMat(samples))

  # Return the results
  return np.array(res).T



# MAIN FUNCTION
if __name__ == "__main__":

  # Set Table Files
  inTableFile = 'inputTable.txt'
  outTableFile = 'outputTable.txt'
  nDim = 1

  # Generate Samples
  ins1 = np.random.uniform(low=0.0, high=10.0, size=(100,nDim))
  ins2 = np.random.uniform(low=0.0, high=10.0, size=(100,nDim))

  # Evaluate Model
  res1 = evalModel(ins1)

  # Save Tables
  np.savetxt(inTableFile,ins1)
  np.savetxt(outTableFile,res1)

  # Construct and save approximant
  approxFileName = 'approx1.txt'
  res2 = genAndSaveApproximant(inTableFile,outTableFile,ins2,approxFileName)

  # Pick Approximant from file and evaluate additional samples
  res22 = readAndEvalApproximant(approxFileName,ins2)

  # Plot and see
  print('ins1',ins1.shape)
  print('res1',res1.shape)
  print('ins2',ins2.shape)
  print('res2',res2.shape)
  print('res22',res22.shape)

  plt.plot(ins1,res1,'bo',markersize=5,alpha=0.6)
  plt.plot(ins2,res2,'go',markersize=5,alpha=0.6)
  plt.plot(ins2,res22,'ro',markersize=5,alpha=0.6)
  plt.show()

