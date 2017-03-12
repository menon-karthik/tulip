#ifndef GRSIMPLEEXPONENTIALCOSTITUTIVEMODEL_H
#define GRSIMPLEEXPONENTIALCOSTITUTIVEMODEL_H

# include "grModel.h"

// GENERIC CLASS FOR SAMPLES
class grSimpleExponentialCostitutiveModel: public grModel{
  public:
    // CONSTRUCTOR AND DISTRUCTOR
    grSimpleExponentialCostitutiveModel();

    // EVAL STORED ENERGY GIVEN PARAMS
    virtual double evalStoredEnergy(double axialStretch, double thetaStretch, double radialStretch, stdVec matParams);

};

#endif //GRSIMPLEEXPONENTIALCOSTITUTIVEMODEL_H
