import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter

fs=8
plt.rc('font', family='serif')
plt.rc('xtick', labelsize='x-small')
plt.rc('ytick', labelsize='x-small')
plt.rc('text', usetex=False)

data = np.loadtxt('paramTraces.txt',skiprows=1)[:,2:]

print(data.mean(axis=0),data.std(axis=0))

plt.figure(figsize=(5,2))
for loopA in range(data.shape[1]):
  plt.subplot(1,data.shape[1],loopA+1)
  plt.hist(data[:,loopA],bins=50)
  plt.xlabel('',fontsize=fs)
  plt.ylabel('PDF',fontsize=fs)      
  plt.tick_params(axis='both', labelsize=fs)
  plt.yticks([])

plt.tight_layout()
plt.show()


