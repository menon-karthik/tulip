import sys
sys.path.insert(0, '../../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
# Import tulip NET library
import tulipNT as nt

numSamples = 2000

def prepareRootNodeFiles():

  ### Theta 1- Vehicle Velocity - avg = 6 km/s, std = 0.5 km/s
  file = open("theta1.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('velocity\n') 
  # Standard Deviation of all variables
  file.write('1000.0\n') 
  # Limits of all variables: first row min and second row max
  file.write('4000.0\n') 
  file.write('8000.0\n') 
  # Matrix of variable realizations
  theta1Samples = np.random.normal(loc=6000.0,scale=500.0,size=(numSamples,1))
  for loopA in range(numSamples):
    file.write('%.3f \n' % (theta1Samples[loopA])) 
  file.close()

  ### Theta 2 - TPS Elastic Modulus - avg = 50 MPa, std = 10 MPa
  file = open("theta2.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('el_modulus\n') 
  # Standard Deviation of all variables
  file.write('2.0e7\n') 
  # Limits of all variables: first row min and second row max
  file.write('2.0e7\n') 
  file.write('8.0e7\n') 
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
  file.write('temperature_limit\n') 
  # Standard Deviation of all variables
  file.write('200.0\n') 
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
  file.write('2,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('pressure,heatflux\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('5000.0,1.0e5\n') # Pressure [Pa], Heat Flux [W/m2]
  # Limits of all variables: first row min and second row max
  file.write('1.0e2,1.0e5\n') 
  file.write('1.0e6,5.0e6\n') 
  # Approximant Type for each variable
  file.write('model\n') # can be MODEL, FILE
  # Approximant Name or file name for each variable
  file.write('darpaSimpleCode1\n') # name of the model/file
  file.close()

  ### Thermo Mechanical Stress and Temperature
  file = open("node_S.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('2,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('sigma_vm,temperature\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('1.0e4,50.0\n') # Stress [Pa], Temperature [K]
    # Limits of all variables: first row min and second row max
  file.write('1.0e2,200.0\n') 
  file.write('1.0e6,5000.0\n') 
  # Approximant Type for each variable
  file.write('model\n') # can be MODEL, FILE
  # Approximant Name or file name for each variable
  file.write('darpaSimpleCode2\n') # name of the model/file
  file.close()

  ### Thermo Mechanical Stress and Temperature
  file = open("node_F.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('failure_ratio\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('0.1\n') # Failure ratio
    # Limits of all variables: first row min and second row max
  file.write('0.5\n') 
  file.write('2.0\n') 
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
    file.write('Evidence,4,1,1,1000.0,50.0\n') 
  file.close() 

# MAIN FUNCTION
if __name__ == "__main__":

  # Prepare files with nodal information
  prepareFiles()

  # Set Network file, all the file 
  netFile = 'netFile.txt'
  genNetInputFile(netFile,addEvidence=True)
  
  # Create new Model Network
  net = nt.ntNet(netFile)

  # Change the evidence directly in the network
  # net.removeEvidence()
  # evidenceFile = 'evidence.txt'
  # net.assignEvidence(evidenceFile)

  # Pass messages and perform inference
  net.runBP()
