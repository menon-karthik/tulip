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
# Import Mat File From MATLAB
import scipy.io
import numpy as np
from mpi4py import MPI

# Available Models
ipGOH  = 0
ipSHRA = 1
ipFR   = 2

# MAIN FUNCTION
def main():
  
  # MPI Init
  rank = comm.Get_rank()
  size = comm.Get_size()

  # Dataset
  useSingleColumn = False
  columnID = 0  
  data = da.daData_Scalar_MultiplePatients(useSingleColumn,columnID);
  data.readFromFile('../datasets/tutorial.csv');

  model = cm.cmTutorial()

  # ASSIGN DATASET
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
  main()
