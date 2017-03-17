#ifndef UQQUADRATURERULE_H
#define UQQUADRATURERULE_H

#include <string>
#include "uqTypes.h"

class uq1DQuadratureRule{
  public:
    // Data Members
    int quadOrder;
    int totalPoints;
    int supportType;
    stdVec points;
    stdVec weights;
    // Constructor and Destructor
    uq1DQuadratureRule(const int quadOrder, const int suppType);
    virtual ~uq1DQuadratureRule();
    // Member Functions
    int getTotalPoints(){return totalPoints;};
    stdVec getPoints();
    stdVec getWeights();
    // Printing Facilities
    void printToFile(std::string fileName);
    void printToScreen();
    // Points and weights generation
    virtual void generatePointsAndWeights() = 0;
};

#endif // UQQUADRATURERULE_H
