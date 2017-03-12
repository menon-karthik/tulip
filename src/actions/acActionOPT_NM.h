#ifndef ACACTIONOPT_NM_H
#define ACACTIONOPT_NM_H

# include <stdio.h>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <cmath>

# include "acAction.h"
# include "cmModel.h"
# include "rnglib.h"
# include "cmUtils.h"

/*! 
This class performs Nelder-Mead optimization on the generic cmModel.
*/
class acActionOPT_NM: public acAction{
  protected:
    // OPTIONS
    //! Convergence Tolerance
    double convTol;
    //! Check Convergence every convUpdateIt iterations
    int convUpdateIt;
    //! Maximum Iterations
    int maxOptIt;
    //! Coefficient for Step increments
    double stepCoefficient;
    //! Use starting point from file
    bool initialPointFromFile;
    //! Start Simulations From the Centre
    bool startFromCenter;
    //! File with initial starting point
    string initialPointFile;
    //! Optimal Parameter Set
    double* optParams;
    //! Initial Guess Defined Check
    bool haveInitialGuess;
    //! Print Messages
    bool doPrintMessages;

    // AUXILARY FUNCTIONS
    void LPN_opt ();
    double runModel(double* params);
    void nelmin (int n, double start[], double xmin[], 
    double *ynewlo, double reqmin, double step[], int konvge, int kcount, 
    int *icount, int *numres, int *ifault );
    
  public:
    /*! 
    \verbatim embed:rst
    **Purpose**
    Constructor
    
    **Discussion**
    Sets the tolerances and number of iterations.
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] localConvTol the converge tolerance. This is equal to the minimum variance of the 
    objective function evaluations on the simplex. If the variance is smaller than this value the 
    algorithm converges. 
    \param[in] localConvUpdateIt maximum number of iterations between successive convergence checks. 
    \param[in] localMaxOptIt Maximum number of model evaluations.
    \param[in] stepCoefficient determines the size and shape of the initial simplex. At initialization, 
    the initial guess is perturbed in each direction using the quantity stepCoefficient*(ul[i]-ll[i]) where ul and ll
    are the upper and lower bounds on parameter i (admissible parameter ranges).
    */
    acActionOPT_NM(double localConvTol, int localConvUpdateIt, int localMaxOptIt,
                   double stepCoefficient);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Tells the optimized to read the initial parameter guess from file
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] value read from file if value==true otherwise use default parameter set.
    */
    void setInitialPointFromFile(bool value){initialPointFromFile = value;};
    
    /*! 
    \verbatim embed:rst
    **Purpose**
    Set the initial guess parameter file
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] file file name where to read the initial parameter guess. In this file,
    there's one parameter per line.
    */
    void setInitialPointFile(string file){initialPointFile = file;};
    
    /*! 
    \verbatim embed:rst
    **Purpose**
    Get optimal parameter set
    
    **Discussion**
    After optimization, this function allows to recover the optimal parameter set.
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] idx the index of the optimal parameter vector component to be gathered.
    */
    double getOptimalParameter(int idx){return optParams[idx];}

    /*! 
    \verbatim embed:rst
    **Purpose**
    Set initial parameter guess
    
    **Discussion**
    Unlike previous function, here both the flag and the file name are set.
    Optionally the initial guess can be located in the center of the parameter space.
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] localUseStartingParameterFromFile flag to read the initial parameter guess from file.
    \param[in] localStartFromCenter use the center of the parameter space as initial guess.
    \param[in] localStartParameterFile name of the file containing the initial parameter guess.
    */
    void setInitialParamGuess(bool localUseStartingParameterFromFile,
                              bool localStartFromCenter,
                              string localStartParameterFile);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Activate/deactivate message printing to stdout
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] flag print messages to stdout if flag==true, otherwise no information is printed.
    */
    void setMessagesPrinting(bool flag);

  	// Virtual functions from acAction
  	virtual int go();
};

#endif //ACACTIONOPT_NM_H
