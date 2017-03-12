#ifndef GRMODEL_H
#define GRMODEL_H

# include "uqTypes.h"

// GENERIC CLASS FOR SAMPLES
class grModel{
  public:
    
    // CONSTRUCTOR AND DISTRUCTOR
    grModel();
    
    // EVAL STORED ENERGY GIVEN PARAMS
    virtual double evalStoredEnergy(double axialStretch, double thetaStretch, double radialStretch, stdVec matParams);
};

#endif //GRMODEL_H
