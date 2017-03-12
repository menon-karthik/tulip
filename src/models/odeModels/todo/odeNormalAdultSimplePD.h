#ifndef ODENORMALADULTSIMPLEPD_H
#define ODENORMALADULTSIMPLEPD_H

# include <string>
# include <stdio.h>
# include <string>
# include "uqTypes.h"
# include "lpnUtils.h"
# include "stdlib.h"
# include "math.h"
# include "cmModel.h"
# include "cmException.h"

// GENERIC CLASS FOR SAMPLES
class odeNormalAdultSimplePD: public odeModel{
  protected:

    // Keep model version for backward compatibility
    static const int modelVersion = 0001;

    //! Total number of heart cycles to be computed
    int numCycles;
    //! Total number of time step per heart Cycle
    int totalStepsOnSingleCycle;
    //! List of fixed parameters
    stdIntVec fixedParams;

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
    Implement the 4th order Runge-Kutta method to solve ordinary first order differential equations with constant time step.

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
    Flush state variables and auxiliary results to file.

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

  	// Constructor
    odeNormalAdultSimplePD(int cycles, int stepsPerCycle);

    // Functions that need implementation
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual string getResultName(int index);
    virtual int    getStateTotal();
    virtual int    getAuxStateTotal();
    virtual int    getResultTotal();
  	virtual void   getParameterLimits(stdVec& limits);
            void   getDefaultParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
    virtual void   getPriorMapping(int priorModelType,int* prPtr);
    virtual double evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCode);
};

#endif // ODENORMALADULTSIMPLEPD_H
