import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
from scipy.stats import gaussian_kde

numMarginals = 6
fs=14
plt.rc('font', family='serif')
plt.rc('xtick', labelsize='x-small')
plt.rc('ytick', labelsize='x-small')
plt.rc('text', usetex=False)

def convertToTex(text):
  text.replace("_", "\_")
  raw_s = r'{}'.format(text)
  # print(raw_s)
  return raw_s

class msg(object):
  
  def __init__(self,fileName):
    f = open(fileName, 'r') 
    for loopA in range(4):
      line = f.readline()
      if(loopA == 1):
        # Read variable names - row 1
        self.names = line.split(',')
      elif(loopA == 2):
        # Read variable standard deviation - row 2
        self.stdsnames = [float(i) for i in line.split(',')]
      elif(loopA == 3):
       # Read variable limits  - row 4
       self.limits = [float(i) for i in line.split(',')]
    f.close()
    # Read samples
    self.samples = np.loadtxt(fileName,skiprows=4,delimiter=',')
    if(len(self.samples.shape) == 1):
      self.samples = np.resize(self.samples,(-1,1))
    # print(self.samples.shape)

  def __str__(self):
    res = ''
    res += 'Variable names: '
    for loopA in range(len(self.names)):
      res += self.names[loopA].rstrip() + ','
    res += '\n'

    res += "Standard Deviations: " 
    for loopA in range(len(self.stdsnames)):
      res += str(self.stdsnames[loopA]) + ','
    res += '\n'
    
    res += "Limits Min: "
    for loopA in range(len(self.limits)//2):
      res += str(self.limits[2*loopA]) + ','
    res += '\n'

    res += "Limits Max: "
    for loopA in range(len(self.limits)//2):
      res += str(self.limits[2*loopA+1]) + ','
    res += '\n'

    res += "--- Sample Table" + '\n'
    # Print Headers
    for loopA in range(self.samples.shape[0]):
      for loopB in range(self.samples.shape[1]):
        res += "%-15.3e " % (self.samples[loopA,loopB])
      res += '\n'
    return res

  def evalJoint(self):
    if(len(self.names) > 1):
      print("ERROR: More than one variable per node: NOT IMPLEMENTED.")
      exit(-1)
    else:
      binarySamples = np.round(self.samples)
      return np.array([1.0-binarySamples.sum()/len(binarySamples),binarySamples.sum()/len(binarySamples)])

  def plotMsg(self,outFile,plotColor,plotDensity=False):
    
    for loopA in range(self.samples.shape[1]):
      plt.figure(figsize=(4,3))
      plt.hist(self.samples[:,loopA],density=True,color=plotColor,bins=2,alpha=0.4)
      # Plot Kernel Density Estimation
      if(plotDensity):
        density = gaussian_kde(self.samples[:,loopA])
        xs = np.linspace(self.samples[:,loopA].min(),self.samples[:,loopA].max(),1000)
        plt.plot(xs,density(xs),color='k',lw=2)
      # Set title, labels, etc.
      plt.xlabel(convertToTex(self.names[loopA]),fontsize=fs)
      plt.ylabel('PDF',fontsize=fs)      
      plt.tick_params(axis='both', labelsize=fs)
      plt.ticklabel_format(axis='x', style='sci',scilimits=(0,0),useOffset=False,useLocale=False,useMathText=False)
      plt.axvline(x=self.limits[loopA*2+0],c='k',ls='--')
      plt.axvline(x=self.limits[loopA*2+1],c='k',ls='--')
      plt.yticks([])
      # plt.xticks(rotation=80,ha='right')
      # ax.xaxis.set_major_formatter(FormatStrFormatter('%.1e'))
      plt.tight_layout()
      plt.savefig(outFile + '_' + str(loopA) + '.pdf')
      plt.show()
      plt.close()

if __name__ == "__main__": 

  msgList = ['msg_node_0_from_factor_0.txt',
             'msg_node_0_from_factor_1.txt',
             'msg_node_0_from_factor_2.txt',
             'msg_node_1_from_factor_1.txt',
             'msg_node_1_from_factor_3.txt',
             'msg_node_2_from_factor_2.txt',
             'msg_node_2_from_factor_3.txt',
             'msg_node_3_from_factor_3.txt',
             'node_0_marginal.txt',
             'node_1_marginal.txt',
             'node_2_marginal.txt',
             'node_3_marginal.txt']

  for loopA,msgItem in enumerate(msgList):

    # Create Message
    msgNode = msg(msgItem)
    
    # Print Message
    # print(msgNode)

    # Plot Message
    if(False):
      folderName = './out/'
      print("Plotting File: ",folderName + pre)
      msgNode.plotMsg(folderName + pre,color)

    print(msgList[loopA],msgNode.evalJoint())
  