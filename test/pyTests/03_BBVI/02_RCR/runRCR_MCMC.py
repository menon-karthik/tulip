# Usage: running estimate.py to get the traces file.  The parameter traces are required to run the MCMC in step 3

# Imports
import sys
# sys.path.insert(0, '/Users/karlynharrod/Documents/Documents/UND/Research_Projects/Heart/tulip_v07/tulipBIN/py')
sys.path.insert(0,'../../../../../bin/py')
# sys.path.insert(0,'/home/dschiava/Documents/01_Development/01_PythonApps/05_tulip/bin/py')
# Import UQ Library
import tulipUQ as uq
# Import Computational Model Library
import tulipCM as cm
# Import Data Library
import tulipDA as da
# Import Action Library
import tulipAC as ac
# Import Mat File From MATLAB
#import scipy.io
import numpy as np
from mpi4py import MPI


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

# MAIN FUNCTION
def runMCMC(dataFile, comm):
  # MPI Init
  rank = comm.Get_rank()
  size = comm.Get_size()
  # Set Data Object
  useSingleColumn = False
  columnID = 0
  data = da.daData_multiple_Table(useSingleColumn,columnID)
  data.readFromFile(dataFile)

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
  lpnModel.printLevel = 0
  # Assign Data Object To Model
  lpnModel.setData(data)


  # Set DREAM Parameters
  totChains         = size  # size
  totGenerations    = 30000  #30000
  totalCR           = 3
  totCrossoverPairs = 5
  dreamGRThreshold  = 1.2
  dreamJumpStep     = 10
  dreamGRPrintStep  = 10

  # Set OUTPUT Files
  dreamChainFileName = 'chain_GR_000000.txt'
  dreamGRFileName    = 'gr_GR.txt'

  # Set Restart File
  # No restart Simulation
  dreamRestartReadFileName = ''
  # string dreamRestartReadFileName = "restart_read_GR.txt";
  # Write restart file just in case
  dreamRestartWriteFileName = 'restart_write_GR.txt'

  # Set Prior Information
  usePriorFromFile = False
  priorFileName = ''
  priorModelType = 0

  # Initialize DREAM Action
  dream = ac.acActionDREAM(totChains,
                           totGenerations,
                           totalCR,
                           totCrossoverPairs,
                           dreamChainFileName,
                           dreamGRFileName,
                           dreamGRThreshold,
                           dreamJumpStep,
                           dreamGRPrintStep,
                           dreamRestartReadFileName,
                           dreamRestartWriteFileName,
                           usePriorFromFile,
                           priorFileName,
                           priorModelType)

  # Set Model
  dream.setModel(lpnModel)

  # Run MCMC Simulation
  dream.go()

# ====
# MAIN
# ====
if __name__ == "__main__":

  runMode = "mcmc"
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
  elif runMode == 'mcmc':
    # Init MPI
    comm = MPI.COMM_WORLD
    # Run Main Function
    runMCMC(dataFileName,comm)
  else:
    print('Error: Invalid Running Mode')

