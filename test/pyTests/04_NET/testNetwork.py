import sys
sys.path.insert(0, '../../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
# Import tulip NET library
import tulipNT as nt

def prepareRootNodeFiles():

  numSamples = 100

  ### Theta 1- Vehicle Velocity - avg = 6 km/s, std = 0.5 km/s
  file = open("theta1.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples) + '\n') 
  # Name of variables 
  file.write('Vehicle velocity\n') 
  # Standard Deviation of all variables
  file.write('500.0\n') 
  # Limits of all variables: first row min and second row max
  file.write('4000.0\n') 
  file.write('800.0\n') 
  # Matrix of variable realizations
  theta1Samples = np.random.normal(loc=6000.0,scale=500.0,size=(numSamples,1))
  for loopA in range(numSamples):
    file.write('%.3f \n' % (theta1Samples[loopA])) 
  file.close()

  ### Theta 2 - TPS Elastic Modulus - avg = 50 MPa, std = 10 MPa
  file = open("theta2.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples) + '\n') 
  # Name of variables 
  file.write('TPS Elastic module\n') 
  # Standard Deviation of all variables
  file.write('10.0e6\n') 
  # Limits of all variables: first row min and second row max
  file.write('20.0e6\n') 
  file.write('80.0e6\n') 
  # Matrix of variable realizations
  theta2Samples = np.random.normal(loc=50.0e6,scale=10.0e6,size=(numSamples,1))
  for loopA in range(numSamples):
    file.write('%.3f \n' % (theta2Samples[loopA])) 
  file.close()

  # Theta 3 - TPS Max Allowable Temperature - avg = 1533 K, std = 100 K
  file = open("theta3.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples) + '\n') 
  # Name of variables 
  file.write('TPS Max allowable temperature\n') 
  # Standard Deviation of all variables
  file.write('100.0\n') 
  # Limits of all variables: first row min and second row max
  file.write('1200.0\n') 
  file.write('1900.0\n') 
  # Matrix of variable realizations
  theta3Samples = np.random.normal(loc=1533.0,scale=100.0,size=(numSamples,1))
  for loopA in range(numSamples):
    file.write('%.3f \n' % (theta3Samples[loopA])) 
  file.close()

# Set Evidence 
def prepareEvidenceFiles():

  # Evidence on temperature in the TPS tile - avg = 1000 K, std = 50 K
  file = open("evidence.txt","w") 
  # File is comma separated
  # Node IDs, Number of Variables with evidence, (variableID, avg, std)*num
  file.write('4,1,1,1000.0,50.0\n') 
  file.close()

# Set Approximants for Deterministic Nodes
def prepareDeterministicNodeFiles():

  ### Thermo Mechanical Loads
  file = open("node_L.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('2,100\n') 
  # Name of variables 
  file.write('pressure,heatflux\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('1000.0,0.10e6\n') # Pressure [Pa], Heat Flux [W/m2]
  # Approximant Type for each variable
  file.write('model\n') # can be MODEL, FILE
  # Approximant Name or file name for each variable
  file.write('darpaSimpleCode1\n') # name of the model/file
  file.close()

  ### Thermo Mechanical Stress and Temperature
  file = open("node_S.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('2,100\n') 
  # Name of variables 
  file.write('sigma_vm,temperature\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('0.01e6,50.0\n') # Stress [Pa], Temperature [W/m2]
  # Approximant Type for each variable
  file.write('model\n') # can be MODEL, FILE
  # Approximant Name or file name for each variable
  file.write('darpaSimpleCode2\n') # name of the model/file
  file.close()

  ### Thermo Mechanical Stress and Temperature
  file = open("node_F.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,100\n') 
  # Name of variables 
  file.write('failure_ratio\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('0.1\n') # Stress [Pa], Temperature [W/m2]
  # Approximant Type for each variable
  file.write('model\n') # can be MODEL, FILE
  # Approximant Name or file name for each variable
  file.write('darpaSimpleCode3\n') # name of the model/file
  file.close()

def prepareFiles():
  '''
  Function to prepare all required files
  '''
  # Set Node Files
  prepareRootNodeFiles()
  # Set Approximants for Deterministic Nodes
  prepareDeterministicNodeFiles()
  # Set Evidence 
  prepareEvidenceFiles()

def genNetInputFile(fileName,addEvidence=False):
  file = open(fileName,"w") 
  file.write('# Define Nodes\n') 
  file.write('# Root Nodes\n') 
  file.write('# Node, ID, nodeType, variable file (contains names, stds, samples)\n') 
  file.write('Node,0,root,theta1.txt\n') 
  file.write('Node,1,root,theta2.txt\n') 
  file.write('Node,2,root,theta3.txt\n') 
  file.write('# Deterministic Nodes\n') 
  file.write('# Node, ID, nodeType, file with functional approximants\n') 
  file.write('Node,3,deterministic,node_L.txt\n') 
  file.write('Node,4,deterministic,node_S.txt\n') 
  file.write('Node,5,deterministic,node_F.txt\n') 
  file.write('') 
  file.write('# Define Edges\n') 
  file.write('Edge,0,3\n') 
  file.write('Edge,3,4\n') 
  file.write('Edge,1,4\n') 
  file.write('Edge,2,5\n') 
  file.write('Edge,4,5\n') 
  file.write('') 
  if(addEvidence):
    file.write('# Define Evidence File\n') 
    file.write('# Evidence File, node ID, VarID1, value1, std1, varID2, value2, std2, ...\n') 
    file.write('Evidence,File,evidence.txt\n') 
  file.close() 

# MAIN FUNCTION
if __name__ == "__main__":

  # Prepare files with nodal information
  prepareFiles()

  # Set Network file, all the file 
  netFile = 'netFile.txt'
  genNetInputFile(netFile)
  
  # Create new Model Network
  net = nt.ntNet(netFile)

  # Change the evidence directly in the network
  # evidenceFile = 'evidence.txt'
  # net.getEvidence(evidenceFile)

  # Pass messages and perform inference
  net.runBP()
