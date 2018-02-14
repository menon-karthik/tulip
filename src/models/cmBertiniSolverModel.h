#ifndef CMBERTINISOLVERMODEL_H
#define CMBERTINISOLVERMODEL_H

# include <string>

# include "cmModel.h"

# include "uqTypes.h"

using namespace std;

// GENERIC CLASS FOR MODELS
class cmBertiniSolverModel: public cmModel{
  private:
    //exprtk::parser_error::type error;  
    //exprtk::expression<long> expression;
    //exprtk::parser<long> parser;    
    //exprtk::symbol_table<long> symbol_table;
    
    double dims;
    double numEq;
    vector<double> x;
    vector<double> eq;

  public:

  	cmBertiniSolverModel(string inputFile);
    virtual ~cmBertiniSolverModel();

    virtual int getParameterTotal();
    virtual int getStateTotal();
    virtual int getResultTotal();
  	virtual void getParameterLimits(stdVec& limits);
  	virtual void getDefaultParams(stdVec& params);
  	virtual void getPriorMapping(int priorModelType,int* prPtr);
    virtual string getParamName(int parID);
    virtual string getResultName(int resID);
    virtual double evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode);
};

#endif // CMBERTINISOLVERMODEL_H
