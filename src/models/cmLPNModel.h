#ifndef CMLPNMODEL_H
#define CMLPNMODEL_H

# include <string>

# include "cmModel.h"
# include "cmException.h"
# include "odeIntegrator.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmLPNModel: public cmModel{
  public:

    // ODE Integrator that contains the model
    odeIntegrator* integrator;

    //! Default constructor
    cmLPNModel(odeIntegrator* integrator);
    //! Virtual destructor
    virtual ~cmLPNModel();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
    virtual void getDefaultParameterLimits(stdVec& limits);
    virtual void getDefaultParams(stdVec& params);
    virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif // CMLPNMODEL_H
