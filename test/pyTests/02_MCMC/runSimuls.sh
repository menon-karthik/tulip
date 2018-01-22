#!/bin/bash

# Run MCMC
mpiexec -n 3 python3 example_runMCMConModel.py 10
mpiexec -n 3 python3 example_runMCMConModel.py 11
mpiexec -n 3 python3 example_runMCMConModel.py 12
mpiexec -n 3 python3 example_runMCMConModel.py 13
mpiexec -n 3 python3 example_runMCMConModel.py 14
mpiexec -n 3 python3 example_runMCMConModel.py 15
mpiexec -n 3 python3 example_runMCMConModel.py 16

# Run Optimization
python3 example_optMCMConModel.py 10
python3 example_optMCMConModel.py 11
python3 example_optMCMConModel.py 12
python3 example_optMCMConModel.py 13
python3 example_optMCMConModel.py 14
python3 example_optMCMConModel.py 15
python3 example_optMCMConModel.py 16

# Plot Results for 2D Problems
python3 showPoints2D.py

# Plot Results for 3D Problems
python3 showPoints3D.py