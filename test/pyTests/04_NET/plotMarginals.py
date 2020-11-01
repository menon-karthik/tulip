import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter

numMarginals = 6
fs=12
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

  def plotMsg(self,outFile,plotRed=False):
    plt.figure(figsize=(4*self.samples.shape[1],3))
    for loopA in range(self.samples.shape[1]):
      ax = plt.subplot(1,self.samples.shape[1],loopA+1)
      if(plotRed):
        ax.hist(self.samples[:,loopA],density=True,color='red')
      else:
        ax.hist(self.samples[:,loopA],density=True)
      ax.set_xlabel(convertToTex(self.names[loopA]),fontsize=fs)
      ax.set_ylabel('PDF',fontsize=fs)      
      ax.tick_params(axis='both', labelsize=fs)
      ax.ticklabel_format(axis='x', style='sci',scilimits=(0,0),useOffset=False,useLocale=False,useMathText=False)
      ax.set_yticks([])
      plt.xticks(rotation=80,ha='right')
      ax.xaxis.set_major_formatter(FormatStrFormatter('%.1e'))
    plt.tight_layout()
    plt.savefig(outFile)

if __name__ == "__main__": 

  # Check Incoming Node Messages
  msgNode = msg('msg_node_0_from_factor_0.txt')
  msgNode.plotMsg('msg_node_0_from_factor_0.pdf')

  msgNode = msg('msg_node_0_from_factor_3.txt')
  msgNode.plotMsg('msg_node_0_from_factor_3.pdf',plotRed=True)

  msgNode = msg('msg_node_1_from_factor_1.txt')
  msgNode.plotMsg('msg_node_1_from_factor_1.pdf')

  msgNode = msg('msg_node_1_from_factor_4.txt')
  msgNode.plotMsg('msg_node_1_from_factor_4.pdf',plotRed=True)

  msgNode = msg('msg_node_2_from_factor_2.txt')
  msgNode.plotMsg('msg_node_2_from_factor_2.pdf')

  msgNode = msg('msg_node_2_from_factor_5.txt')
  msgNode.plotMsg('msg_node_2_from_factor_5.pdf',plotRed=True)

  msgNode = msg('msg_node_3_from_factor_3.txt')
  msgNode.plotMsg('msg_node_3_from_factor_3.pdf')

  msgNode = msg('msg_node_3_from_factor_4.txt')
  msgNode.plotMsg('msg_node_3_from_factor_4.pdf',plotRed=True)

  msgNode = msg('msg_node_4_from_factor_4.txt')
  msgNode.plotMsg('msg_node_4_from_factor_4.pdf')

  msgNode = msg('msg_node_4_from_factor_5.txt')
  msgNode.plotMsg('msg_node_4_from_factor_5.pdf',plotRed=True)

  msgNode = msg('msg_node_5_from_factor_5.txt')
  msgNode.plotMsg('msg_node_5_from_factor_5.pdf')

# node_0_marginal.txt
# node_1_marginal.txt
# node_2_marginal.txt
# node_3_marginal.txt
# node_4_marginal.txt
# node_5_marginal.txt

#  marg = msg('node_3_marginal.txt')
#  marg.plotMsg()
