#ifndef ODENORMALADULTSIMPLEPA_H
#define ODENORMALADULTSIMPLEPA_H

# include <stdio.h>
# include <string>
# include "stdlib.h"
# include "math.h"
# include "uqTypes.h"
# include "cmModel.h"
# include "lpnUtils.h"
# include "cmException.h"

// Prior Mapping Types
const int ipPriorMapIdentity = 0;

// Target Configurations
const int ipTargetConfig_AllTargets = 0;
const int ipTargetConfig_ExcludePAP = 1;
const int ipTargetConfig_HRBPCVP    = 2;

// GENERIC CLASS FOR SAMPLES
class odeNormalAdultSimplePA: public odeModel{
  protected:

    // Keep model version for backward compatibility
    // --- 0001 for old data format - One file, multiple patients
    // --- 0001 for old data format - One file, one patient
    static const int modelVersion = 0002;    
    static const bool useNewRanges = true;

    // Select various target settings - DES APR 2016
    int targetConfigMode;

    //! Total number of heart cycles to be computed
    int numCycles;
    //! Total number of time step per heart Cycle
    int totalStepsOnSingleCycle;

  private:
    /*! 
    \verbatim embed:rst
    **Purpose**
    Checks if a given set of parameters is inside the admissible limits. 

    **Discussion**
    None
    
    **Modified**
    2 May 2015 - Daniele Schiavazzi
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] params the vector with the parameters to test result boolean test: true if the parameters are inside the admissible ranges, false otherwise.
    */
    bool areParamsInsideLimits(stdVec params);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Check if sign of parameter is consistent with associated limits.
    
    **Discussion**
    None
    
    **Modified**
    2 May 2015 - Daniele Schiavazzi
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] params (stdVec): Vector containing the parameter set to test. 
    */
    bool isParamsSignCorrect(stdVec params);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Implement the 4th order Runge-Kutta method to solve ordinary first order differential equations with constant time steps.

    **Discussion**
    None
    
    **Modified**
    2 May 2015 - Daniele Schiavazzi
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] totalSteps (integer): Total number of steps for time integration.
    \param[in] timeStep (double): Amplitude of the time step.
    \param[in] iniVals (stdVec): Vector of initial values for state variables.
    \param[in] params (stdVec): Vector of LPN parameters.
    \param[out] outVals (stdMat): State variable outputs outVals[i][j] containing the value of the i-th state variable at the j-th time step.
    \param[out] auxOutVals (stdMat): Auxiliary state variable outputs auxOutVals[i][j] containing the value of the i-th auxiliary state variable at the j-th time step.
    */
    int PerformRK4Steps(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Main function to solve the LPN model.
    
    **Discussion**
    It determines the time step, sets up the arrays and runs the RK4 ODE routine. 
    
    **Modified**
    2 May 2015 - Daniele Schiavazzi
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] options (lpnOptions): The stuct containing the program options.
    \param[in] params (stdVec): Double array of parameter values.
    \param[out] results (stdVec): LPN Model results.
    */
    int solveLPN(lpnOptions options, stdVec params, stdVec& results);
    // Eval Elastance
    double evalElastance(double tr);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Implements the model equations for the Runge Kutta Scheme.
    
    **Discussion**
    None

    **Modified**
    2 May 2015 - Daniele Schiavazzi
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] tn (double): Current time instant in simulation. 
    \param[in] Xn (stdVec): State variables for the last step
    \param[in] params (stdVec): list of parameters
    \param[out] Xn1 (stdVec): State variable derivatives needed by RK4 
    \param[out] out (stdVec): Post processing outputs
    \param[out] Ind (stdVec): Valve state vector
    */
    void lpnODE(double tn, stdVec Xn, stdVec params, stdVec& Xn1, stdVec& out, stdVec& Ind);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Flush state variables and auxiliary results to file
    
    **Discussion**
    Results are saved in a space-separated table. Rows are different timesteps, columns are different variables. State variables are saved first, followed by auxiliary state variables.  
    
    **Modified**
    2 May 2015 - Daniele Schiavazzi
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] fileName (string): Name of output file.
    \param[in] totalSteps (integer): Total number of time steps.
    \param[in] totalStates (integer): Total number of state variables.
    \param[in] outVals (stdMat): Matrix with state variables at all time steps. 
    \param[in] totAuxStates (integer): Total number of auxiliary state variables. 
    \param[in] auxOutVals (stdMat): Matrix with auxiliary state variables at all time steps.
    */
    void writeAllDataToFile(string fileName,int totalSteps,int totalStates,stdMat outVals,int totAuxStates,stdMat auxOutVals);
    // Set model results (handles various model versions)
    void setModelResults(stdVec outputs,stdStringVec& keys,stdVec& computedValues,stdVec& stdFactors,stdVec& weigths);

  public:

  	/*! 
    \verbatim embed:rst
    **Purpose**
    Constructor
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] cycles number of heart cycles to solve
    \param[in] stepsPerCycle number of time steps per heart cycle to be solved
    */
  	odeNormalAdultSimplePA(int cycles, int stepsPerCycle);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Get the default parameter limits taking into account the frozen parameters
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[out] limits vector of parameter limits. For parameter i the lower bound is limits[2*i] and upper bound limits[2*i+1]
    */
    void getDefaultParameterLimits(stdVec& limits);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Set an integer value to select an appropriate set of target quantities for output.
    
    **Discussion**
    This is used in situations when we want to perform blind prediction of the patient's pulmonary pressures. 
    Also it is used to reduce the number of targets in situation of limited data availability
    
    **Modified**
    APR 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] mode integer value. The following modalities are supported : 
               ipTargetConfig_AllTargets (0) allows to include all targets. 
               ipTargetConfig_ExcludePAP (1) includes all targets but excludes the pulmonary pressures (SPAP,DPAP,PWP)
               ipTargetConfig_HRBPCVP (2) includes only targets such as HR,SBP,DBP,CVP,RAP,RVED

    */    
    void setTargetConfigMode(int mode);
    
    /*! 
    \verbatim embed:rst
    **Purpose**
    Return a list of target keys the the model produces as outputs
    
    **Discussion**
    No Discussion
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[out] modelResultKeys std vector with list of target quantity keys
    */      
    void getModelResultKeys(stdStringVec& modelResultKeys);

    // Virtual functions from cmModel
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual string getResultName(int index);
    virtual int    getStateTotal();
    virtual int    getAuxStateTotal();
    virtual int    getResultTotal();
    virtual void   getParameterLimits(stdVec& limits);
    virtual void   getDefaultParams(stdVec& params);
    virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCode);

};

#endif // ODENORMALADULTSIMPLEPA_H
