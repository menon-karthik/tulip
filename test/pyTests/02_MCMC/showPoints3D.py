# Imports
import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d
from matplotlib import cm
from skimage import measure

def plotGraphs():

  # Set Number of Samples
  totSamples = 20000
  # levels = np.array([0.0,0.01,0.2,0.4,0.6])
  levels = np.array([1.0,2.0,3.0,4.0,5.0])

  # Read MCMC Sample Data
  data1 = np.loadtxt('paramTraces_13.txt',skiprows=1)
  data1 = data1[:totSamples,2:]
  data2 = np.loadtxt('paramTraces_14.txt',skiprows=1)
  data2 = data2[:totSamples,2:]
  data3 = np.loadtxt('paramTraces_15.txt',skiprows=1)
  data3 = data3[:totSamples,2:]

  #  Read Optimal Points
  opt1 = np.loadtxt('OptimalPoints_13.txt')
  opt2 = np.loadtxt('OptimalPoints_14.txt')
  opt3 = np.loadtxt('OptimalPoints_15.txt')

  # Create Exact Surfaces
  delta = 0.2
  x = np.arange(-3.0, 3.0, delta)
  y = np.arange(-3.0, 3.0, delta)
  z = np.arange(-3.0, 3.0, delta)
  X,Y,Z= np.meshgrid(x, y, z)
  V = X**2 - Z*(Y**2)
  

  fig = plt.figure()
  ax = fig.add_subplot(111, projection='3d')

  verts, faces, extra1, extra2 = measure.marching_cubes_lewiner(V, 0, spacing=(0.2, 0.2, 0.2))
  ax.plot_trisurf(verts[:, 0], verts[:,1], faces, verts[:, 2], cmap='Spectral', lw=1)
  ax.set_xlim([-1.0,6.0])
  ax.set_ylim([-1.0,6.0])
  ax.set_zlim([-6.0,-3.0])

  plt.show()
  sys.exit(-1)

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