#ifndef ACACTIONLSI_FD_H
#define ACACTIONLSI_FD_H

# include <stdio.h>
# include <cstdlib>
# include <iostream>
# include <cstring>
# include <cmath>

# include "acAction.h"
# include "cmModel.h"
# include "cmUtils.h"

// Generic Class for Samples
class acActionLSI_FD: public acAction{
  protected:

    // Read Parameter From File
    bool specifyParamLocation;
    
    // Parameter File Name
    string initialParamFile;

    // Finite Difference Perturbation
    double perturbFactor;

    // Eval Function Derivative/Sensitivity Factor
    bool evalDeriv;

    // Write Sensitivity Table To File
    void WriteSentitivityTableToFile(string fileName,int par_num,int res_num,vector<vector<double> > &sensTable);

    // Solve Model
    int solveModel(int par_num, double* allParams, int res_num, double* results);

  public:

    // Constructor
    acActionLSI_FD(bool specifyParamLocation,string initialParamFile,double perturbFactor,bool evalDeriv);

    // Perform Action
    virtual int go();
};

#endif //ACACTIONLSI_FD_H
