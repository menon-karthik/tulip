# Imports
import sys,os
# Set library path
sys.path.insert(0,'/home/dschiava/Development/CApps/tulipApp/libBin/py')
# Import UQ Library
import tulipUQ as uq
# Import Computational Model Library
import tulipCM as cm
# Import Data Library
import tulipDA as da
# Import Action Library
import tulipAC as ac
# Import Mat File From MATLAB
import scipy.io
import numpy as np
from mpi4py import MPI
from constants import *

# MAIN FUNCTION
def main(modelType,comm):
  
  # MPI Init
  rank = comm.Get_rank()
  size = comm.Get_size()

  # Set Model
  if(modelType == kPointAndCircle):
    model = cm.cmAnalyticalExpressionModel(cm.kPointAndCircle)
  elif(modelType == kAlphaCurve):
    model = cm.cmAnalyticalExpressionModel(cm.kAlphaCurve)
  elif(modelType == kKuramoto):
    model = cm.cmAnalyticalExpressionModel(cm.kKuramoto)    
  else:
    print('Error: Invalid Model.')
    sys.exit(-1)

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

    # Rename File
    if(modelType == kPointAndCircle):
      os.rename('paramTraces.txt','paramTraces_01.txt')
    elif(modelType == kAlphaCurve):
      os.rename('paramTraces.txt','paramTraces_02.txt')
    elif(modelType == kKuramoto):
      os.rename('paramTraces.txt','paramTraces_03.txt')
    else:
      print('Error: Invalid Model.')
      sys.exit(-1)

# ====
# MAIN
# ====
if __name__ == "__main__":

  # Init MPI
  comm = MPI.COMM_WORLD
  rank = comm.Get_rank()
  size = comm.Get_size()

  # Run Main Function
  main(int(sys.argv[1]),comm)

