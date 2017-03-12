#ifndef ACACTIONGSI_H
#define ACACTIONGSI_H

# include <stdio.h>
# include <cstdlib>
# include <iostream>
# include <cstring>
# include <cmath>
# include <vector>
# include "acAction.h"
# include "cmUtils.h"
# include "polynomial.h"
# include "spgrid.h"
# include "pdflib.h"
# include "uqConstants.h"
# include "uqPolyBasis.h"

// =========================================
// COMPUTATION OF GLOBAL SENSITIVITY INDEXES
// =========================================
class acActionGSI: public acAction{
  protected:
    // Data Members
    bool   useDefaultRange;
    string priorParamFile;

    // Member Functions
    void printSensitivityTablesToLatex(int index, int res_num,int par_num,vector<vector<double> > dirTable,vector<vector<double> > totTable);
    void printSensitivityTables(int index, int res_num,int par_num,vector<vector<double> > dirTable,vector<vector<double> > totTable);
    void printSigmaCoeffs(int index, int res_num,int basis_num,vector<vector<double> > sigmaCoeffs);
  public:
    // Constructor
    acActionGSI();
    // Perform Action
    virtual int go();
};

#endif //ACACTION_H
