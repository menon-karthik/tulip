#ifndef GRFOURFIBERFAMILYMODEL_H
#define GRFOURFIBERFAMILYMODEL_H

# include "grModel.h"
# include "uqTypes.h"

// GENERIC CLASS FOR SAMPLES
class grFourFiberFamilyModel: public grModel{
  public:
    // CONSTRUCTOR AND DISTRUCTOR
    grFourFiberFamilyModel();
    
    // EVAL STORED ENERGY GIVEN PARAMS
    virtual double evalStoredEnergy(double axialStretch, double thetaStretch, double radialStretch, stdVec matParams);
};

#endif //GRFOURFIBERFAMILYMODEL_H
