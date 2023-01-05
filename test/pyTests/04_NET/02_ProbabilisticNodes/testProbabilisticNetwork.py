import sys
sys.path.insert(0, '../../../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
# Import tulip NET library
import tulipNT as nt

# IMPLEMENTATION OF A SIMPLE WET GRASSS
# BAYESIAN NETWORK AS A TEST

numSamples = 100000

def prepareRootNodeFiles():

  ### Theta 1- Vehicle Velocity - avg = 6 km/s, std = 0.5 km/s
  file = open("cloudy.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('cloudy\n') 
  # Standard Deviation of all variables
  file.write('0.25\n') 
  # Limits of all variables: first row min and second row max
  file.write('0.0\n') 
  file.write('1.0\n') 
  # Matrix of variable realizations
  theta1Samples = np.random.uniform(size=(numSamples,1))
  for loopA in range(numSamples):
    file.write('%.3f \n' % (theta1Samples[loopA])) 
  file.close()

# Set Evidence 
def prepareEvidenceFiles():

  # Evidence on temperature in the TPS tile - avg = 1000 K, std = 50 K
  file = open("evidence.txt","w") 
  # File is comma separated
  # Node IDs, Number of Variables with evidence, (variableID, avg, std)*num
  # Evidence the grass is wet
  file.write('3,1,0,1.0,0.0\n') 
  file.close()

# Set Approximants for Deterministic Nodes
def prepareProbabilisticNodeFiles():

  ### Thermo Mechanical Loads
  file = open("rainy.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('rainy\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('0.25\n') 
  # Limits of all variables: first row min and second row max
  file.write('0.0\n') 
  file.write('1.0\n') 
  # Subdivisions of the resulting variable: 2 it is binary: rainy/not rainy
  file.write('2\n')
  # Conditional Probability Table
  file.write('0 0.8 0.2\n')
  file.write('1 0.2 0.8\n')
  file.close()
  ### Thermo Mechanical Stress and Temperature
  file = open("sprinklers.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('sprinklers\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('0.25\n')   
    # Limits of all variables: first row min and second row max
  file.write('0.0\n') 
  file.write('1.0\n') 
  # Subdivisions of the resulting variable: 2 it is binary: rainy/not rainy
  file.write('2\n')
  # Conditional Probability Table
  file.write('0 0.5 0.5\n')
  file.write('1 0.9 0.1\n')
  file.close()

  ### Thermo Mechanical Stress and Temperature
  file = open("wetgrass.txt","w") 
  # File is comma separated
  # Number of Variables, Number of Samples
  file.write('1,'+str(numSamples)+'\n') 
  # Name of variables 
  file.write('wetgrass\n') # These are mapped to the model results is type is MODEL
  # Standard Deviation of all variables
  file.write('0.25\n')   
    # Limits of all variables: first row min and second row max
  file.write('0.0\n') 
  file.write('1.0\n') 
  # Subdivisions of the resulting variable: 2 it is binary: rainy/not rainy
  file.write('2\n')
  # Conditional Probability Table
  # First input rainy, second sprinklers
  file.write('0 0 1.0 0.0\n')
  file.write('1 0 0.1 0.9\n')
  file.write('0 1 0.1 0.9\n')
  file.write('1 1 0.01 0.99\n')
  file.close()

def prepareFiles():
  '''
  Function to prepare all required files
  '''
  # Set Node Files
  prepareRootNodeFiles()
  # Set Approximants for Deterministic Nodes
  prepareProbabilisticNodeFiles()
  # Set Evidence 
  prepareEvidenceFiles()

def genNetInputFile(fileName,addEvidence=False):
  file = open(fileName,"w") 
  file.write('# Define Nodes\n') 
  file.write('# Root Nodes\n') 
  file.write('# Node, ID, nodeType, variable file (contains names, stds, samples)\n') 
  file.write('Node,0,root,cloudy.txt\n') 
  file.write('# Deterministic Nodes\n') 
  file.write('# Node, ID, nodeType, file with functional approximants\n') 
  file.write('Node,1,probabilistic,rainy.txt\n') 
  file.write('Node,2,probabilistic,sprinklers.txt\n') 
  file.write('Node,3,probabilistic,wetgrass.txt\n') 
  file.write('') 
  file.write('# Define Edges\n') 
  file.write('Edge,0,1\n') 
  file.write('Edge,0,2\n') 
  file.write('Edge,1,3\n') 
  file.write('Edge,2,3\n') 
  file.write('') 
  if(addEvidence):
    file.write('# Define Evidence File\n') 
    file.write('# Evidence File, node ID, VarID1, value1, std1, varID2, value2, std2, ...\n') 
    file.write('Evidence,3,1,0,1.0,0.0\n') 
  file.close() 

# MAIN FUNCTION
if __name__ == "__main__":

  # Prepare files with nodal information
  prepareFiles()

  # Set Network file, all the file 
  netFile = 'netProbFile.txt'
  genNetInputFile(netFile,addEvidence=True)
  
  # Create new Model Network
  net = nt.ntNet(netFile)

  # Pass messages and perform inference - First Step
  net.runBP()
