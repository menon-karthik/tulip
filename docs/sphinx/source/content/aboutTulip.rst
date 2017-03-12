.. _codebase:

.. |logo| image:: imgs/tulip.png
   :width: 4%

==================
About tulip |logo|
==================

**tulip** ( Tools for Uncertainty quantification, Lumped modeling and Identification of Parameters) is a framework designed to simplify the task of optimizing and estimating parameters of lumped models for human circulation. 

**tulip** is written in OOP c++ and contains classes for:

* **Generic computational models** (:ref:`cmModel <codeModels>` class) - This class contains a generic implementation of a computational model. A library of models (lumped parameter ) is provided, but the user can wrap his own solver, by creating a child class of **cmModel** and implementing the following methods (plus a number of optional methods):

  * **getParameterTotal**. Retrieves the total number of parameters (including initial conditions for state variables).    
  * **getResultTotal**. Determines the total number of scalar results in the output.
  * **getDefaultParams**. Retrieves the default model parameters.     
  * **getParameterLimits**. Determines the admissible parameter ranges.        
  * **evalModelError**. Evaluate the model at the selected parameter values, store the results and returns the value of the objective function or negative log-likelihood.

* **Generic operation performed on computational models** (:ref:`acAction <codeActions>` class) - these are operations that can be performed on the model.
* **Generic data operation performed on computational models** (:ref:`daData <codeData>` class) - This provides a way to 

Available LPN Models
""""""""""""""""""""

A library of lumped parameter models is already implemented for the following physiologies:

* **Adult physiologies** either complete or simplified and with multiple pulmonary layouts.
* **Coronary physiologies**, allowing assimilation of patient data for coronary artery disease.
* **Single-ventricle** ventricle physiologies (under-development). These physiologies are associated with severe form of congenital heart disease. 

Available Actions
"""""""""""""""""

An existing library of tools is provided including:

* **acActionOPT_NM**. Nelder-Mead simplex optimization :cite:`nelder1965simplex`. 
* **acActionDREAM**. MPI implementation of Differential Evolution adaptive Metropolis algorithm :cite:`vrugt2009accelerating`. This performs Bayesian estimation using an adaptive MCMC technique.
* **acActionLSI_FD**. MPI implementation of Differential Evolution adaptive Metropolis algorithm. 

other functionalities for UQ are under-development and will be available soon.
