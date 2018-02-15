#!/bin/bash

# Run MCMC
mpiexec -n 3 python3 example_runMCMConModel.py input.txt

# Run Optimization
python3 example_optMCMConModel.py input.txt

# Plot Results for 2D Problems
# python3 showPoints2D.py

# Plot Results for 3D Problems
# python3 showPoints3D.py