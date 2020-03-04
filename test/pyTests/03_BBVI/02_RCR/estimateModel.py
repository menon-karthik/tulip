# Usage: running estimate.py to get the traces file.  The parameter traces are required to run the MCMC in step 3

# Imports
import sys
# sys.path.insert(0, '/Users/karlynharrod/Documents/Documents/UND/Research_Projects/Heart/tulip_v07/tulipBIN/py')
sys.path.insert(0, '/afs/crc.nd.edu/group/tulip/01_code/01_tulip/bin/py')
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

# MAIN FUNCTION
def main(dataFile,comm,argv,includeDataStdAsParam = False):

  # MPI Init
  rank = comm.Get_rank()
  size = comm.Get_size()

  # Dataset
  useSingleColumn = True
  columnID = 0
  #print("columnID set to ", columnID, "in estimate.py")
  data = da.daData_multiple_Table(useSingleColumn, columnID)
  data.readFromFile('singlePatientData.txt')

  # CREATE ODE MODEL
  # USING MODEL VERSION 1 !!!!
  modelVersion = 1
  configMode = 0
  ode = cm.odeNormalAdultSimplePA(modelVersion,configMode)

  # CREATE ODE INTEGRATOR
  timeStep = 0.005
  totalCycles = 20
  rk4 = cm.odeIntegratorRK4(ode,timeStep,totalCycles)

  # Create new LPN model
  model = cm.cmLPNModel(rk4)

  # ASSIGN DATA OBJECT TO MODEL
  model.setData(data)


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
  dreamRestartReadFileName = '';
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
  dream.setModel(model)

  # Run MCMC Simulation
  dream.go()

  # Add Barrier
  comm.Barrier()

  # Post Process the Results
  if(rank == 0):
    debugMode = False
    burnInPercent = 0.1
    dream.postProcess(debugMode,burnInPercent);

# ====
# MAIN
# ====
if __name__ == "__main__":

  # Init MPI
  comm = MPI.COMM_WORLD
  rank = comm.Get_rank()
  size = comm.Get_size()

  # Run Main Function
  # Dataset
  useSingleColumn = True
  columnID = 0
  data = da.daData_multiple_Table(useSingleColumn, columnID)
  data.readFromFile('singlePatientData.txt')
  
  main(data,comm,sys.argv)

