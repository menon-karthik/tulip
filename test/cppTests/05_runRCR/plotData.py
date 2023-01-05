# Imports
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('allData.dat')

plt.plot(data[:,1],data[:,2]/1333.22)
plt.show()