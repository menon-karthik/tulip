# Imports
import sys
sys.path.insert(0, '../../../../bin/py')
# Import UQ Library
import tulipUQ as uq
# Import Computational Model Library
import tulipCM as cm
# Import Data Library
import tulipDA as da
# Import Action Library
import tulipAC as ac
# Other libraries
import numpy as np
from mpi4py import MPI
import matplotlib.pyplot as plt

# Perform data preparation
def prepare_data():

  # Get Parameter Set From Optimization
  inputs     = uq.stdVec()
  outputs    = uq.stdVec()
  errorCodes = uq.stdIntVec()

  # Solve Model
  model = cm.cmTutorial()
  model.getDefaultParams(inputs)
  
  # Dataset
  useSingleColumn = False
  columnID = 0  
  data = da.daData_multiple_Table(useSingleColumn,columnID)
  data.readFromFile('tutorial.csv')
  model.setData(data);

  # Evaluate model  
  ll = model.evalModelError(inputs,outputs,errorCodes)

  # Print results and LL
  print()
  print('Outputs: ',np.array(outputs))
  print('LL: ',ll)

# MAIN FUNCTION
def main_dream():

  # Init MPI
  comm = MPI.COMM_WORLD
  rank = comm.Get_rank()
  size = comm.Get_size()
  
  # Dataset
  useSingleColumn = False
  columnID = 0  
  data = da.daData_multiple_Table(useSingleColumn,columnID)
  data.readFromFile('tutorial.csv')

  # Assign model
  model = cm.cmTutorial()

  # Assign dataset
  model.setData(data);

  # Set DREAM Parameters
  totChains         = size
  totGenerations    = 30000
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
  dreamRestartReadFileName = '';
  # string dreamRestartReadFileName = "restart_read_GR.txt";
  # Write restart file just in case
  dreamRestartWriteFileName = 'restart_write_GR.txt'

  # Set Prior Information
  usePriorFromFile = False
  priorFileName = ''
  priorModelType = 0

  # Initialize DREAM Action
  dream = ac.acActionDREAMmpi(totChains,
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
  dream.setModel(model)

  # Run MCMC Simulation
  dream.go()

  # Add Barrier
  comm.Barrier()

  # Post Process the Results
  if(rank == 0):
    debugMode = False
    burnInPercent = 50.0 # half of the samples discarded
    dream.postProcess(debugMode,burnInPercent);

def post_process():
  params = np.loadtxt('paramTraces.txt',skiprows=1)[:,2:]
  plt.figure(figsize=(10,3))
  plt.subplot(1,3,1)
  plt.hist(params[:,0],bins='auto',alpha=0.5)
  plt.axvline(x = 1, color = 'r')
  plt.subplot(1,3,2)
  plt.hist(params[:,1],bins='auto',alpha=0.5)
  plt.axvline(x = 5, color = 'r')
  plt.subplot(1,3,3)
  plt.hist(params[:,2],bins='auto',alpha=0.5)
  plt.axvline(x = 60, color = 'r')
  plt.tight_layout()
  plt.show()

# ====
# MAIN
# ====
if __name__ == "__main__":

  # Perform data preparation
  # prepare_data()

  # run dream function
  main_dream()

  # post_process
  # post_process()
