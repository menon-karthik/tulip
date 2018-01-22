# Imports
import sys
sys.path.insert(0, '/home/dschiava/Development/CApps/tulipApp/libBin/py')
# Import UQ Library
import tulipUQ as uq
# Import Computational Model Library
import tulipCM as cm
# Import Data Library
import tulipDA as da
# Import Action Library
import tulipAC as ac
# Import numpy and matplotlib
import numpy as np
import matplotlib.pyplot as plt

# MAIN FUNCTION
def main(modelType):

  # Set Model
  model = cm.cmAnalyticalExpressionModel(modelType)

  # Set Optimizer Parameters
  # Convergence Tolerance
  convTol = 1.0e-10
  # Check Convergence every convUpdateIt iterations
  convUpdateIt = 1
  # Maximum Iterations
  maxOptIt = 5000
  # Coefficient for Step increments
  stepCoefficient = 0.01
  # Init Nelder-Mead Optimizer
  nm = ac.acActionOPT_NM(convTol,convUpdateIt,maxOptIt,stepCoefficient)
  # Set Model for Nelder-Mead Optimizer
  nm.setModel(model)

  # Total Number of iterations
  totInitialGuess = 100
  totRestarts     = 20

  # Read MCMC Samples and sub-samples using totInitialGuess
  data = np.loadtxt('paramTraces_' + str(modelType) + '.txt',skiprows=1)

  # SubSample
  print(np.random.choice(data.shape[0], totInitialGuess))
  data = data[np.random.choice(data.shape[0], totInitialGuess),2:]

  # Loop over the randomly selected initial guess
  results = np.zeros((totInitialGuess,model.getParameterTotal()))
  for loopA in range(totInitialGuess):
    # Get Model Limits
    x0 = data[loopA,:]
    np.savetxt('optParams.txt',x0)

    # Set Initial Guess Parameters
    useStartingParameterFromFile = True
    startFromCentre = False
    startParameterFile = 'optParams.txt'
    nm.setInitialParamGuess(useStartingParameterFromFile,startFromCentre,startParameterFile)

    # Nelder-Mead Optimiation with restart
    for loopB in range(totRestarts):      
      # Start Optimization    
      nm.go()
      # Restart
      if(loopB == 0):
        nm.setInitialPointFromFile(True)
        nm.setInitialPointFile('optParams.txt')

    # Read Optimal Parameters From File
    results[loopA,:] = np.loadtxt('optParams.txt')

  # Save Final Result
  np.savetxt('OptimalPoints_' + str(modelType)+ '.txt',results)    

# ====
# MAIN
# ====
if __name__ == "__main__":
	
  main(int(sys.argv[1]))
