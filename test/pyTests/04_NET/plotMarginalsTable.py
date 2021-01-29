import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
from scipy.stats import gaussian_kde

fs=8
plt.rc('font',  family='serif')
plt.rc('xtick', labelsize='x-small')
plt.rc('ytick', labelsize='x-small')
plt.rc('text',  usetex=False)

numBins = 100

nodeMessageList = [['msg_node_0_from_factor_0.txt','msg_node_0_from_factor_3.txt'],
                   ['msg_node_1_from_factor_1.txt','msg_node_1_from_factor_4.txt'],
                   ['msg_node_2_from_factor_2.txt','msg_node_2_from_factor_5.txt'],
                   ['msg_node_3_from_factor_3.txt','msg_node_3_from_factor_4.txt'],
                   ['msg_node_4_from_factor_4.txt','msg_node_4_from_factor_5.txt'],
                   ['msg_node_5_from_factor_5.txt']]

factorMessageList = [['msg_factor_0_from_node_0.txt'],
                     ['msg_factor_1_from_node_1.txt'],
                     ['msg_factor_2_from_node_2.txt'],
                     ['msg_factor_3_from_node_0.txt','msg_factor_3_from_node_3.txt'],
                     ['msg_factor_4_from_node_1.txt','msg_factor_4_from_node_3.txt','msg_factor_4_from_node_4.txt'],
                     ['msg_factor_5_from_node_2.txt','msg_factor_5_from_node_4.txt','msg_factor_5_from_node_5.txt']]

def getMarginalTableSize(msgList):
  numRows = len(msgList)
  colSize = []

  for loopA in range(len(msgList)):
    currColSize = 0
    for loopB in range(len(msgList[loopA])):
      # Read Message
      currMsg = msg(msgList[loopA][loopB])
      currColSize += currMsg.samples.shape[1]
    # Append Total Size
    colSize.append(currColSize)

  return numRows,np.array(colSize).max()

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

  def plotMsg(self,title,idx):
    plt.hist(self.samples[:,idx],bins=numBins,density=True,color='gray',alpha=0.4)
    density = gaussian_kde(self.samples[:,idx])
    xs = np.linspace(self.samples[:,idx].min(),self.samples[:,idx].max(),1000)
    plt.plot(xs,density(xs),color='k',lw=2)
    plt.xlabel(convertToTex(self.names[idx]),fontsize=fs)
    plt.ylabel('PDF',fontsize=fs)      
    plt.title(title,fontsize=fs)      
    plt.tick_params(axis='both', labelsize=fs)
    plt.axvline(x=self.limits[idx*2+0],c='k',ls='--')
    plt.axvline(x=self.limits[idx*2+1],c='k',ls='--')
    plt.yticks([])
    plt.locator_params(axis='x', nbins=3)

def plotMarginalTable(msgList,outFile):

  # Get the size of the marginal table
  numRows,numCols = getMarginalTableSize(msgList)

  plt.figure(figsize=(10,10))
  # Loop over the receiving node
  for loopNode in range(len(msgList)):
    # Loop over the sending factor
    colCount = 0
    for loopFactor in range(len(msgList[loopNode])):      
      currMsg = msg(msgList[loopNode][loopFactor])
      print("Processing File: ",msgList[loopNode][loopFactor])
      for loopIdx in range(currMsg.samples.shape[1]):
        colCount += 1
        plt.subplot(numRows,numCols,loopNode*numCols+colCount)
        currMsg.plotMsg(msgList[loopNode][loopFactor],loopIdx)
  plt.tight_layout()
  plt.savefig(outFile)
  plt.show()
    
if __name__ == "__main__": 

  # Plot Node Messages
  plotMarginalTable(nodeMessageList,'nodeMsg.pdf')

  # Plot Factor Messages
  # plotMarginalTable(factorMessageList,'factorMsg.pdf')