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
  print(raw_s)
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
    print(self.samples.shape)

  def plotMsg(self,outFile,plotColor):
    
    for loopA in range(self.samples.shape[1]):
      plt.figure(figsize=(4,3))
      plt.hist(self.samples[:,loopA],density=True,color=plotColor,bins=30,alpha=0.4)
      # Plot Kernel Density Estimation
      density = gaussian_kde(self.samples[:,loopA])
      xs = np.linspace(self.samples[:,loopA].min(),self.samples[:,loopA].max(),1000)
      plt.plot(xs,density(xs),color='k',lw=2)
      # Set title, labels, etc.
      plt.xlabel(convertToTex(self.names[loopA]),fontsize=fs)
      plt.ylabel('PDF',fontsize=fs)      
      plt.tick_params(axis='both', labelsize=fs)
      plt.ticklabel_format(axis='x', style='sci',scilimits=(0,0),useOffset=False,useLocale=False,useMathText=False)
      plt.yticks([])
      # plt.xticks(rotation=80,ha='right')
      # ax.xaxis.set_major_formatter(FormatStrFormatter('%.1e'))
      plt.tight_layout()
      plt.savefig(outFile + '_' + str(loopA) + '.pdf')
    # plt.show()
      plt.close()

if __name__ == "__main__": 

  msgList = ['msg_node_0_from_factor_0.txt','msg_node_0_from_factor_3.txt',
             'msg_node_1_from_factor_1.txt','msg_node_1_from_factor_4.txt',
             'msg_node_2_from_factor_2.txt','msg_node_2_from_factor_5.txt',
             'msg_node_3_from_factor_3.txt','msg_node_3_from_factor_4.txt',
             'msg_node_4_from_factor_4.txt','msg_node_4_from_factor_5.txt',
             'msg_node_5_from_factor_5.txt',
             'node_0_marginal.txt','node_1_marginal.txt',
             'node_2_marginal.txt','node_3_marginal.txt',
             'node_4_marginal.txt','node_5_marginal.txt']

  typeList = ['forward','backward',
             'forward','backward',
             'forward','backward',
             'forward','backward',
             'forward','backward',
             'forward',
             'marginal','marginal',
             'marginal','marginal',
             'marginal','marginal']

  for loopA,msgItem in enumerate(msgList):
    msgNode = msg(msgItem)
    pre, ext = os.path.splitext(msgItem)
    if(typeList[loopA] == 'forward'):
      color = 'b'
    elif(typeList[loopA] == 'backward'):
      color = 'r'
    elif(typeList[loopA] == 'marginal'):
      color = 'gray'
    # 
    msgNode.plotMsg('/home/dschiava/Documents/02_Documents/02_Proposals/01_DARPA_YFA/05_Reports/09_Q2Y3_Report/imgs/noEvidence/' + pre,color)
  