'''
Export Python Optimization Algorithm to tulip 

This file exposes the wrappers for the most popular
optimizers, i.e.: 

fmin(func, x0[, args, xtol, ftol, maxiter, ...])	
Minimize a function using the downhill simplex algorithm.

fmin_powell(func, x0[, args, xtol, ftol, ...])	
Minimize a function using modified Powell’s method.

fmin_cg(f, x0[, fprime, args, gtol, norm, ...])	
Minimize a function using a nonlinear conjugate gradient algorithm.

fmin_bfgs(f, x0[, fprime, args, gtol, norm, ...])	
Minimize a function using the BFGS algorithm.

fmin_ncg(f, x0, fprime[, fhess_p, fhess, ...])	
Unconstrained minimization of a function using the Newton-CG method.
'''

# Import Computational Models from tulip
import tulipUQ as uq

# Import Optimizers
from scipy.optimize import fmin
from scipy.optimize import fmin_powell
from scipy.optimize import fmin_cg
from scipy.optimize import fmin_bfgs
from scipy.optimize import fmin_ncg

class acAction_OPT_fmin():
  # Constructor
  def __init__(self, x0, args=(), xtol=0.0001, ftol=0.0001, maxiter=None, maxfun=None, full_output=0, disp=1, retall=0, callback=None, initial_simplex=None):
    # Computational Model
    self.model = None
    # Optimizer Arguments
    self.x0 = x0
    self.args = args
    self.xtol = xtol
    self.ftol = ftol
    self.maxiter = maxiter
    self.maxfun = maxfun
    self.full_output = full_output 
    self.disp = disp
    self.retall = retall
    self.callback = callback
    self.initial_simplex = initial_simplex

  def runModel(x):
    inputs = uq.stdVec()
    for loopA in xrange(len(x)):
      inputs.push_back(x[loopA])
    outputs = uq.stdVec()
    errorCode = uq.stdIntVec()
    res = self.model.evalModelError(inputs,outputs,errorCode)
    return res;

  # Set Model
  def setModel(model):
    self.model = model

  # Run Optimizer
  def go():
    scipy.optimize.fmin(runModel, \
                        self.x0, \
                        self.args, \
                        self.xtol, \
                        self.ftol, \
                        self.maxiter=None, \
                        self.maxfun=None, \
                        self.full_output=0, \
                        self.disp=1, \
                        self.retall=0, \
                        self.callback=None, \
                        self.initial_simplex=None)




