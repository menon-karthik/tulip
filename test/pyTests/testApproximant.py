import sys
sys.path.insert(0, '../../../bin/py')
import numpy as np
import matplotlib.pyplot as plt
# Import tulip UQ library
import tulipUQ as uq

# Read file with input samples 
inputs = np.loadtxt('inputTable.txt')
# Read file with output samples
outputs = np.loadtxt('outputTable.txt')

# Create uqSamples for inputs
uqs = uqSamples(numpyToStdMat(inputs))

# Create a polynomial matrix
order = 2
polyType = uq.kPolyLegendre
completeOrderType = uq.kMIPartialOrder
# completeOrderType = uq.kMIFullOrder

# Form polynomial matrix
polymat = uqPolyMatrix(uqs, order, polyType, completeOrderType);



