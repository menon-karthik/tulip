import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import gaussian_kde

# Generate samples from two PDF
sample1 = np.random.normal(loc=10.0,scale=2.0,size=1000)
sample2 = np.random.normal(loc=8.0,scale=0.5,size=1000)
sample3 = np.concatenate((sample1,sample2))

plt.hist(sample1,bins=100,density=True,color='blue',alpha=0.4)
plt.hist(sample2,bins=100,density=True,color='red',alpha=0.4)
plt.hist(sample3,bins=100,density=True,color='gray',alpha=0.4)

density1 = gaussian_kde(sample1)
density2 = gaussian_kde(sample2)
density3 = gaussian_kde(sample3)

x1 = np.linspace(np.min(sample1),np.max(sample1),1000)
x2 = np.linspace(np.min(sample2),np.max(sample2),1000)
x3 = np.linspace(np.min(sample3),np.max(sample3),1000)

plt.plot(x1,density1(x1),color='k',lw=2)
plt.plot(x2,density2(x2),color='k',lw=2)
plt.plot(x3,density3(x3),color='k',lw=2)
plt.plot(x3,density1(x3)*density2(x3)*10,color='r',lw=2)
plt.show()
