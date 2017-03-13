#ifndef CMOPENLOOPSURROGATEMODEL_H
#define CMOPENLOOPSURROGATEMODEL_H

# include <string>
# include <exception>
# include <iostream>
# include <fstream>

# include "uqTypes.h"
# include "uqSamples.h"
# include "uqConstants.h"
# include "uqPolyMatrix.h"

# include "cmModel.h"
# include "cmUtils.h"
# include "cmResistanceModel.h"
# include "cm3DPQSurrogateModel.h"
# include "cmRCRModel.h"
# include "cmException.h"

using namespace std;

// R Record
struct resRecord{
  double resValue;
  stdMat distPressHistory;
};
typedef vector<resRecord*> resArray;
// RCR Record
struct rcrRecord{
  double rProxValue;
  double capValue;
  double rDistValue;
  stdMat distPressHistory;
};
typedef vector<rcrRecord*> rcrArray;

// GENERIC CLASS FOR SAMPLES
class cmOpenLoopSurrogateModel: public cmModel{
  public:

    // DATA MEMBERS
    double timeStep3DModel;
    int totSteps3DModel;
    double totalStepsRCR;

    // SET FILENAMES
    string model3DFileName;
    string rcrFileName;
    string resFileName;
    string mappingFileName;
    string iniFlowRatesFileName;
    string inletPressuresFileName;
    string inletFlowratesFileName;
    // SET TARGETS
    stdVec targets;

  	// CONSTRUCTOR
  	cmOpenLoopSurrogateModel(double localTimeStep3DModel, 
                             int localTotSteps3DModel, 
                             double localTotalStepsRCR, 
                             string localModel3DFileName,
                             string localRCRFileName,
                             string localRESFileName,
                             string localMappingFileName,
                             string localIniFlowRatesFileName,
                             string localInletPressuresFileName,
                             string localInletFlowratesFileName,
                             stdVec localTargets);

    // FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
    virtual int    getParameterTotal();
    virtual string getParamName(int parID);
    virtual int    getStateTotal();
    virtual int    getResultTotal();
  	virtual void   getParameterLimits(stdVec& limits);
  	virtual void   getDefaultParams(stdVec& params);
  	virtual void   getPriorMapping(int priorModelType,int* prPtr);

    // EVAL MODEL 
    virtual double evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCodes);
    virtual stdVec evalModelError(stdMat inputs,stdMat& outputs, stdIntVec& errorCodes);
};

#endif //CMOPENLOOPSURROGATEMODEL_H
