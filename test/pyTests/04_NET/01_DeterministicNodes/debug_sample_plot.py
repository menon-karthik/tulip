import numpy as np
import matplotlib.pyplot as plt

fs=8
plt.rc('font',  family='serif')
plt.rc('xtick', labelsize='x-small')
plt.rc('ytick', labelsize='x-small')
plt.rc('text',  usetex=False)

if __name__ == "__main__": 

  # Read Samples
  inSamples = np.loadtxt('debug_samples_in.txt',skiprows=1)
  outSamples = np.loadtxt('debug_samples_out.txt',skiprows=1)
  
  plt.figure(figsize=(8,5))
  
  plt.subplot(2,3,1)
  plt.hist(inSamples[:,0],bins=50)
  plt.xlabel('inSample 0',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([])   

  plt.subplot(2,3,2)
  plt.hist(inSamples[:,1],bins=50)
  plt.xlabel('inSample 1',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([])   

  plt.subplot(2,3,4)
  plt.hist(outSamples[:,0],bins=50)
  plt.xlabel('outSample 0',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([])   

  plt.subplot(2,3,5)
  plt.hist(outSamples[:,1],bins=50)
  plt.xlabel('outSample 1',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([]) 

  plt.subplot(2,3,6)
  plt.hist(outSamples[:,2],bins=50)
  plt.xlabel('outSample 3',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([]) 


  plt.tight_layout()  
  plt.show()  

