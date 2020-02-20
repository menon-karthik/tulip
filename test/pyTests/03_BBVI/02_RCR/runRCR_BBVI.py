# Imports
import sys,os
# Set library path
sys.path.insert(0,'../../../../../bin/py')
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
def runModel():

  # Set Data Object
  # useSingleColumn = True
  # columnID = 0   
  # data = da.daData_multiple_Table(useSingleColumn,columnID);
  # data.readFromFile(dataFile);
    
  # Create a ODE Model
  ode = cm.odeRCR()

  # Define forcing
  forcing = uq.stdMat()

  # Read Focing in cgs from file
  cm.readTableFromFile('inlet.flow',forcing)

  # Create a ODE Model Integrator
  timeStep = 0.005
  totalCycles = 20
  rk4 = cm.odeIntegratorRK4(ode,timeStep,totalCycles,forcing)

  # Create new LPN model
  lpnModel = cm.cmLPNModel(rk4)

  # Print all data etc.
  lpnModel.printLevel = 1

  # Assign Data Object To Model
  # lpnModel.setData(data)

  # Init inputs/outputs/errors
  inputs = uq.stdVec()
  outputs = uq.stdVec()
  errorCodes = uq.stdIntVec()

  # Set the dafult parameters
  ode.getDefaultParams(inputs)

  # Solve First Model
  ll = lpnModel.evalModelError(inputs,outputs,errorCodes)
    
  # Return ll
  return np.array(outputs)

def genData(numDataPoints,std,dataFileName):
  '''
  Implement the data generation process
  '''
  stdGaussVals = np.random.normal(size=(3,numDataPoints))
  
  # Run Model for the default parameter Value
  ob = runModel()

  # Constuct data
  data = ob.reshape(3,-1) + stdGaussVals * std.reshape(3,-1)

  # Create Data File
  outFile = open(dataFileName,'w')
  outFile.write("min_P_0,")
  for i in range(data.shape[1]):    
    outFile.write('%8.3e,' %(data[0,i]))
  outFile.write('\n')    
  outFile.write("max_P_0,")
  for i in range(data.shape[1]):    
    outFile.write('%8.3e,' %(data[1,i]))
  outFile.write('\n')    
  outFile.write("av_P_0,")
  for i in range(data.shape[1]):    
    outFile.write('%8.3e,' %(data[2,i]))
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
  
  # runMode = 'testing'
  runMode = 'gendata'
  # runMode = 'bbvi'
  
  dataFileName = 'data.txt'
  # Running mode
  if runMode == 'testing':
    # Test the model works
    res = runModel()
    print('outputs: ',res)
  elif runMode == 'gendata':
    # Set Params
    numDataPoints = 600
    dataStd =  5.0*np.ones(3)
    # Generate Data
    genData(numDataPoints,dataStd,dataFileName)
  elif runMode == 'bbvi':
    # Run Main Function
    runBBVI(dataFileName)
  else:
    print('Error: Invalid Running Mode')





