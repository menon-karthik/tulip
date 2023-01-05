# Imports
import numpy as np
import matplotlib.pyplot as plt

def plotGraphs():

  # Set Number of Samples
  totSamples = 20000
  levels = np.array([0.0,0.01,0.2,0.4,0.6])

  # Read MCMC Sample Data
  data1 = np.loadtxt('paramTraces_10.txt',skiprows=1)
  data1 = data1[:totSamples,2:]
  data2 = np.loadtxt('paramTraces_11.txt',skiprows=1)
  data2 = data2[:totSamples,2:]
  data3 = np.loadtxt('paramTraces_12.txt',skiprows=1)
  data3 = data3[:totSamples,2:]

  #  Read Optimal Points
  opt1 = np.loadtxt('OptimalPoints_10.txt')
  opt2 = np.loadtxt('OptimalPoints_11.txt')
  opt3 = np.loadtxt('OptimalPoints_12.txt')

  # Create Exact Surfaces
  delta = 0.025
  x = np.arange(-3.0, 3.0, delta)
  y = np.arange(-3.0, 3.0, delta)
  X, Y = np.meshgrid(x, y)
  Z1 = Y**2 + X**2*(X-1.0)*(X-2.0)
  Z2 = Y**2 - X**2*(X+1.0)
  t1 = np.arange(0.0, 2.0*np.pi, delta)
  t2 = np.arange(0.0, 2.0*np.pi, delta)
  T1, T2 = np.meshgrid(t1, t2)
  Z3 = np.abs(1.0/12.0 - 1.0/3.0*(np.sin(T1-T2)+np.sin(T1-0))) + np.abs(-1.0/12.0-1.0/3.0*(np.sin(T2-T1)+np.sin(T2-0.0)))

  plt.figure(figsize=(8,6))
  ax = plt.subplot(2,3,1)
  ax.plot(data1[:,0],data1[:,1],'bo',markersize=1,alpha=0.5)
  ax.contour(X, Y, Z1, levels)
  ax.set_title('First Curve')
  ax.set_xlim([-3.0,3.0])
  ax.set_ylim([-3.0,3.0])

  ax = plt.subplot(2,3,2)
  ax.plot(data2[:,0],data2[:,1],'bo',markersize=1,alpha=0.5)
  ax.contour(X, Y, Z2, levels)
  ax.set_title('Alpha Curve')
  ax.set_xlim([-3.0,3.0])
  ax.set_ylim([-3.0,3.0])

  ax = plt.subplot(2,3,3)
  ax.plot(data3[:,0],data3[:,1],'bo',markersize=1,alpha=0.5)
  ax.contour(t1, t2, Z3, levels)
  ax.set_title('Kuramoto')
  ax.set_xlim([0,2*np.pi])
  ax.set_ylim([0,2*np.pi])

  ax = plt.subplot(2,3,4)
  ax.plot(opt1[:,0],opt1[:,1],'ro',markersize=4,alpha=0.6)
  ax.contour(X, Y, Z1, levels)
  ax.set_xlim([-3.0,3.0])
  ax.set_ylim([-3.0,3.0])

  ax = plt.subplot(2,3,5)
  ax.plot(opt2[:,0],opt2[:,1],'ro',markersize=4,alpha=0.6)
  ax.contour(X, Y, Z2, levels)
  ax.set_xlim([-3.0,3.0])
  ax.set_ylim([-3.0,3.0])

  ax = plt.subplot(2,3,6)
  ax.plot(opt3[:,0],opt3[:,1],'ro',markersize=4,alpha=0.6)
  ax.contour(t1, t2, Z3, levels)
  ax.set_xlim([0,2*np.pi])
  ax.set_ylim([0,2*np.pi])

  plt.tight_layout()
  plt.savefig('sample_Distribution.pdf')
  # plt.show()

# =============
# MAIN FUNCTION
# =============
if __name__ == "__main__":
  plotGraphs()