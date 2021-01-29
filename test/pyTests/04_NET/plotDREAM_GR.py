# Imports
import sys,math
import numpy as np
import matplotlib.pyplot as plt

def main():
  # Read GR Stat Data and LSI Tables
  grData = np.loadtxt('gr_GR.txt',skiprows=1)

  # Create Figure
  fig = plt.figure(figsize=(8,5))
  ax = plt.subplot(1,1,1)
  for loopA in range(1,grData.shape[1]):
    ax.plot(grData[:,0],grData[:,loopA])
  ax.set_xlabel('MCMC Steps',fontsize=8)
  ax.set_ylabel('Gelman-Rubin Convergence Metric',fontsize=8)

  plt.tight_layout()
  plt.show()

# ====
# MAIN
# ====
if __name__ == "__main__":
  main()
