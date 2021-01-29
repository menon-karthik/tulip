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
  inSamples = np.loadtxt('debug_samples_in.txt',skiprows=1)
  outSamples = np.loadtxt('debug_samples_out.txt',skiprows=1)

  # Create input sample table
  sMat_py = np.zeros((inSamples.shape[0],3))
  sMat_py[:,0] = inSamples[:,0]
  sMat_py[:,1] = inSamples[:,1]
  sMat_py[:,2] = outSamples[:,2]
  numSamples = sMat_py.shape[0]

  # Convert to stdMat
  sampleMat = numpyToTulip(sMat_py)

  # Create Samples
  samples = uq.uqSamples(sampleMat)

  # Initialize Model
  model = cm.cm_darpaSimple_code2()

  # Initialize UP Action
  mc = ac.acActionUP_MC(samples)
  # Set Model
  mc.setModel(model)
  # Set UP options
  mc.opts.numberOfRepeats = 1
  mc.opts.sampleGroups.clear()
  mc.opts.sampleGroups.push_back(numSamples)
  mc.opts.storeSamples = True

  # Run Propagation
  mc.go()
  
  # Conver results to numpy
  res = np.array(mc.all_outputs)

  plt.figure(figsize=(5,2))

  plt.subplot(1,2,1)
  plt.hist(res[:,0],bins=50)
  plt.xlabel('',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([])

  plt.subplot(1,2,2)
  plt.hist(res[:,1],bins=50)
  plt.xlabel('',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([])
  
  plt.tight_layout()
  plt.savefig('check_UP_node_4.pdf')

