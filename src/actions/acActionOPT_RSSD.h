#ifndef ACACTIONOPT_RSSD_H
#define ACACTIONOPT_RSSD_H

# include <stdio.h>
# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <cmath>

# include "acAction.h"
# include "cmModel.h"
# include "cmUtils.h"


// GENERIC CLASS FOR SAMPLES
class acActionOPT_RSSD: public acAction{
  protected:
    // Options
    double stepSizeCoeff;
    int itMax;
    double stepLength;
    double normTol;
    int numComponentsToUpdate;
    // Initial Parameter Location Options
    bool initialPointFromFile;
    bool startFromCenter;
    string initialPointFile;

    // AUX FUNCTIONS
    double runModel(double* params);
    void   perform_RSSD(int par_num, double* startPoint, double* xmin, double& ynewlo);
    void   evalFullGradient(int par_num, double* params, double stepSizeCoeff, double* grad, double& fval);
    void   updateGradient(int par_num, double* params, int numComponents, double stepSizeCoeff, double* grad, double& fval);
    void   LPN_OPT_RSSD();
    
  public:

  	// CONSTRUCTOR
  	acActionOPT_RSSD(double locStepSizeCoeff,
                      int locItMax,
                      double locStepLength,
                      double locNormTol,
                      int locNumComponentsToUpdate,
                      bool locInitialPointFromFile,
                      bool locStartFromCenter,
                      string locInitialPointFile);

  	// PERFORM ACTION
  	virtual int go();
};

#endif //ACACTIONOPT_RSSD_H
