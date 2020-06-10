import sys
sys.path.insert(0, '../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
# Import tulip UQ library
import tulipUQ as uq
import tulipNT as nt


def prepareNodeFiles():

file = open("theta1.txt","w") 
# File is comma separated
# Number of Samples, Number of Variables
# Name of variables 
# Standard Deviation of all variables
# Limits of all variables: first row min and second row max
    // Variable limits
    stdVec limits;
# Matrix of variable realizations

file.write('# Define Nodes') 


    // Evidence: variable ID and evidence value
    stdIntVec evidenceVarID;
    stdVec evidenceVarValue;
    // List of pointers to the factors
    vector<ntFactor*> nodeFactors;
    stdBoolVec isDownstreamFactor;
    // List of messages to factors
    vector<ntMessage*> messages;






def genNetInputFile(fileName):
  file = open(fileName,"w") 
  file.write('# Define Nodes') 
  file.write('# Root Nodes') 
  file.write('# Node, ID, nodeType, variable file (contains names, stds, samples)') 
  file.write('Node,0,root,theta1.txt') 
  file.write('Node,1,root,theta2.txt') 
  file.write('Node,2,root,theta3.txt') 
  file.write('# Deterministic Nodes') 
  file.write('# Node, ID, nodeType, file with functional approximants') 
  file.write('Node,3,deterministic,approx_L.txt') 
  file.write('Node,4,deterministic,approx_S.txt') 
  file.write('Node,5,deterministic,approx_F.txt') 
  file.write('') 
  file.write('# Define Edges') 
  file.write('Edge,0,3') 
  file.write('Edge,3,4') 
  file.write('Edge,1,4') 
  file.write('Edge,2,5') 
  file.write('Edge,4,5') 
  file.write('') 
  file.write('# Define Evidence') 
  file.write('# Evidence, node ID, VarID1, value1, varID2, value2, ...') 
  file.write('Evidence,4,') 
  file.close() 






# MAIN FUNCTION
if __name__ == "__main__":

  # Prepare files with nodal information
  prepareNodeFiles()

  # Set Network file, all the file 
  netFile = 'netFile.txt'
  genNetInputFile()
  
  # Create new Model Network
  net = nt.ntNet(netFile)

  # Change the evidence directly in the network
  # 

  # Pass messages and perform inference
  net.runBP()
