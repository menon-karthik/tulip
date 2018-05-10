#ifndef CMBERTINISOLVERMODEL_H
#define CMBERTINISOLVERMODEL_H

# include <string>

# include "cmModel.h"

# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmBertiniSolverModel: public cmModel{
  public:

    string inputFile;
    long dims;
    long numEq;
    stdVec limits;
    stdVec initPoint;
    string evalExpression;
    double exponent;
    stdMat deflationPoints;

  	cmBertiniSolverModel(string inputFile);
    virtual ~cmBertiniSolverModel();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
  	virtual void getDefaultParameterLimits(stdVec& limits);
  	virtual void getDefaultParams(stdVec& params);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);

    // Non Virtual Functions
    void setExponent(double exp);
    void addSolution(const stdVec& solution);
};

#endif // CMBERTINISOLVERMODEL_H
