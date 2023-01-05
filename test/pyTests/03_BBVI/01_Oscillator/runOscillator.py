# Imports
import sys,os
# Set library path
sys.path.insert(0, '../../../../../bin/py')
# Import UQ Library
import tulipUQ as uq
# Import Computational Model Library
import tulipCM as cm
# Import Data Library
import tulipDA as da
# Import Action Library
import tulipAC as ac
# Numpy and Matplotlib
import numpy as np
import matplotlib.pyplot as plt

# Test: Run One instance of the Oscillation Model
def runModel(param=181875.0):

  # Set Data Object
  # useSingleColumn = True
  # columnID = 0   
  # data = da.daData_multiple_Table(useSingleColumn,columnID);
  # data.readFromFile(dataFile);
    
  # Create new LPN model
  model = cm.cmOscillator()
  # Print all data etc.
  model.printLevel = 1
  
  # Assign Data Object To Model
  # model.setData(data)

  # Init inputs/outputs/errors
  inputs = uq.stdVec()
  inputs.push_back(param)
  outputs = uq.stdVec()
  errorCodes = uq.stdIntVec()

  # Solve First Model
  ll = model.evalModelError(inputs,outputs,errorCodes)

  # Write Results
  return outputs[0]

def genData(numDataPoints,std,dataFileName):
  '''
  Implement the data generation process
  '''
  stdGaussVals = np.random.normal(size=numDataPoints)
  
  # Run Model for the default parameter Value
  ob = runModel()

  # Constuct data
  data = ob + stdGaussVals * std

  outFile = open(dataFileName,'w')
  outFile.write("dmax,")
  for i in range(len(data)):    
    outFile.write('%8.3e,' %(data[i]))

  outFile.write('\n')


# Test: Run One instance of the Oscillation Model
def runBBVI(dataFile):

  # Set Data Object
  useSingleColumn = False
  columnID = 0
  data = da.daData_multiple_Table(useSingleColumn,columnID);
  data.readFromFile(dataFile);
    
  # Create new LPN model
  model = cm.cmOscillator()
  # Print all data etc.
  model.printLevel = 0

  # Assign Data Object To Model
  model.setData(data)

  bbvi = ac.acActionBBVI()
  bbvi.setModel(model)
  bbvi.initParameters()

  # opt_method;
  # adj;
  # stdVec a;
  # stdVec b;

  # double Lam[2] = {3.0, 0.5}; // For Normal
  bbvi.lam[0] = 3.0
  bbvi.lam[1] = 0.5
  # double adj[2] = {1.0, 1.0};
  # double a[1] = {kmax - kmin};
  bbvi.a[0] = 220000-140000
  bbvi.b[0] = 0.0
  # double b[1] = {0.0};

  # long numSaves;
  bbvi.totIt = 10000
  bbvi.batchSize = 30
  bbvi.go()

# ====
# MAIN
# ====
if __name__ == "__main__":
  runMode = 'bbvi'
  # runMode = 'gendata'
  dataFileName = 'data.txt'
  # Running mode
  if runMode == 'testing':
    # Test the model works
    res = runModel()
    print(res)
  elif runMode == 'gendata':
    # Set Params
    numDataPoints = 600
    dataStd = 0.1*0.007504
    # Generate Data
    genData(numDataPoints,dataStd,dataFileName)
  elif runMode == 'bbvi':
    # Run Main Function
    runBBVI(dataFileName)
  else:
    print('Error: Invalid Running Mode')





