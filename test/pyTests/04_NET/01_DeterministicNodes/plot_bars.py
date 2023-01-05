import numpy as np
import matplotlib.pyplot as plt

# data = np.loadtxt("el_modulus")
# data = np.loadtxt("heatflux")
# data = np.loadtxt("pressure")
# data = np.loadtxt("sigma_vm")
# data = np.loadtxt("temperature")
# data = np.loadtxt("temperature_limit")
data = np.loadtxt("velocity")

plt.bar(np.arange(len(data)),data)
plt.show()