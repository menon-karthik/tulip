#ifndef ACACTION_OBJPROFILE_H
#define ACACTION_OBJPROFILE_H

# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip>
# include "acAction.h"
# include "cmModel.h"
# include "cmUtils.h"
# include "uqUtils.h"

// GENERIC CLASS FOR SAMPLES
class acAction_OBJProfile: public acAction{

  protected:

    // Total Profiling Step
  	int totalProfileSteps;

    // Use Initial Point File
  	bool initialPointFromFile;
    bool startFromCenter;
	  string initialPointFile;
    
    // Run Model
    double runModel(double* params);
    int OBJ_Profile();

  public:

  	// CONSTRUCTOR
  	acAction_OBJProfile(int locTotalProfileSteps,
                        bool locInitialPointFromFile,
                        bool locStartFromCenter,
                        string locInitialPointFile);

  	// PERFORM ACTION
  	virtual int go();
};

#endif //ACACTION_OBJPROFILE_H
