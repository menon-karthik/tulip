import sys
sys.path.insert(0, '../../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
import tulipAC as ac 
import tulipUQ as uq 
import tulipCM as cm
sys.path.insert(0, '../../../../bin/py')
from tulipPyUtils import numpyToTulip

fs=8
plt.rc('font',  family='serif')
plt.rc('xtick', labelsize='x-small')
plt.rc('ytick', labelsize='x-small')
plt.rc('text',  usetex=False)

# MAIN FUNCTION
if __name__ == "__main__":

  # Read in and out samples
  inSamples = np.loadtxt('debug_samples_incomingMsg.txt',skiprows=1)
  outSamples = np.loadtxt('debug_samples_outcomingMsg.txt',skiprows=1)

  print('inSamples dimension: ',inSamples.shape[1])
  print('outSamples dimension: ',outSamples.shape[1])

  maxCols = max(inSamples.shape[1],outSamples.shape[1])

  plt.figure(figsize=(10,4))

  for loopA in range(inSamples.shape[1]):
    plt.subplot(2,maxCols,loopA+1)
    plt.hist(inSamples[:,loopA],bins=50)
    plt.xlabel('Observation '+str(loopA),fontsize=fs)
    plt.ylabel('PDF',fontsize=fs)      
    plt.tick_params(axis='both', labelsize=fs)
    plt.yticks([])

  for loopA in range(outSamples.shape[1]):
    plt.subplot(2,maxCols,maxCols+loopA+1)
    plt.hist(outSamples[:,loopA],bins=50)
    plt.xlabel('Inference '+str(loopA),fontsize=fs)
    plt.ylabel('PDF',fontsize=fs)      
    plt.tick_params(axis='both', labelsize=fs)
    plt.yticks([])
  
  plt.tight_layout()
  plt.show()

