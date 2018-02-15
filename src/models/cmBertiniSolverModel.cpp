# include "cmBertiniSolverModel.h"

# include <cstdio>
# include <string>

# include "cmException.h"
# include "cmUtils.h"
# include "exprtk.h"

using namespace std;

string extractSubExprFromFile(string inputFile,string key){
  string currExpression("");
  string line;
  size_t found;
  ifstream f(inputFile);
  if(f.is_open()){
    while(getline(f,line)){
      line = trim(line);
      // Check if the string is a comment
      if(line[0] != '#'){
        found = line.find(key);
        if(found!=string::npos){
          currExpression += line + " ";
        }
      }
    }
    f.close();
  }  
  return currExpression;
}

// Extract dimensions and number of equations from file
void extractNumDimensions(string inputFile,double& dims){

  string key("dims");
  string currExpression = extractSubExprFromFile(inputFile,key);

  // Add the variables
  exprtk::parser_error::type   error_t;  
  exprtk::expression<double>   expression_t;
  exprtk::parser<double>       parser_t;    
  exprtk::symbol_table<double> symbol_table_t;

  if(!symbol_table_t.add_variable("dims",dims)){
    printf("dims not added\n");
  }
  // Register Symbol Table
  expression_t.register_symbol_table(symbol_table_t);

  // Compile Expression From File
  if(!parser_t.compile(currExpression,expression_t)){
    printf("Error: %s\n Input file: %s\n",parser_t.error().c_str(),currExpression.c_str());
    printf("\n");

    for (size_t i=0;i<parser_t.error_count();++i){
      // Include the specific nature of each error
      // and its position in the expression string.

      error_t = parser_t.get_error(i);

      printf("Error: %02d Position: %02d\n Type: [%s]\n Message: %s\n",
             static_cast<int>(i),
             static_cast<int>(error_t.token.position),
             exprtk::parser_error::to_str(error_t.mode).c_str(),
             error_t.diagnostic.c_str());
    }
    throw cmException("ERROR: Invalid input file.\n");
  }



  // Collect the inputs 
  expression_t.value();
}

// Extract dimensions and number of equations from file
void extractNumEquations(string inputFile,double& numEq){

  string key("numEq");
  string currExpression = extractSubExprFromFile(inputFile,key);

  // Add the variables
  exprtk::parser_error::type   error_t;  
  exprtk::expression<double>   expression_t;
  exprtk::parser<double>       parser_t;    
  exprtk::symbol_table<double> symbol_table_t;

  if(!symbol_table_t.add_variable("numEq",numEq)){
    printf("numEq not added\n");
  }

  // Register Symbol Table
  expression_t.register_symbol_table(symbol_table_t);

  // Compile Expression From File
  if(!parser_t.compile(currExpression,expression_t)){
    printf("Error: %s\n Input file: %s\n",parser_t.error().c_str(),currExpression.c_str());
    printf("\n");

    for (size_t i=0;i<parser_t.error_count();++i){
      // Include the specific nature of each error
      // and its position in the expression string.

      error_t = parser_t.get_error(i);

      printf("Error: %02d Position: %02d\n Type: [%s]\n Message: %s\n",
             static_cast<int>(i),
             static_cast<int>(error_t.token.position),
             exprtk::parser_error::to_str(error_t.mode).c_str(),
             error_t.diagnostic.c_str());
    }
    throw cmException("ERROR: Invalid input file.\n");
  }


  // Collect the inputs 
  expression_t.value();
}

// Extract Parameter Limits
void extractLimits(string inputFile,stdVec& limits){
  
  string key("limits");
  string currExpression = extractSubExprFromFile(inputFile,key);

  // Add the variables
  exprtk::parser_error::type   error_t;  
  exprtk::expression<double>   expression_t;
  exprtk::parser<double>       parser_t;    
  exprtk::symbol_table<double> symbol_table_t;

  if(!symbol_table_t.add_vector("limits",limits)){
    printf("limits vector not added\n");
  }

  // Register Symbol Table
  expression_t.register_symbol_table(symbol_table_t);

  // Compile Expression From File
  if(!parser_t.compile(currExpression,expression_t)){
    printf("Error: %s\n Input file: %s\n",parser_t.error().c_str(),currExpression.c_str());
    printf("\n");

    for (size_t i=0;i<parser_t.error_count();++i){
      // Include the specific nature of each error
      // and its position in the expression string.

      error_t = parser_t.get_error(i);

      printf("Error: %02d Position: %02d\n Type: [%s]\n Message: %s\n",
             static_cast<int>(i),
             static_cast<int>(error_t.token.position),
             exprtk::parser_error::to_str(error_t.mode).c_str(),
             error_t.diagnostic.c_str());
    }
    throw cmException("ERROR: Invalid input file.\n");
  }


  // Collect the inputs 
  long res = expression_t.value();
}

// Extract Initial Point From File
void extractInitPoint(string inputFile,stdVec& initPoint){
  
  string key("initPoint");
  string currExpression = extractSubExprFromFile(inputFile,key);

  // Add the variables
  exprtk::parser_error::type   error_t;  
  exprtk::expression<double>   expression_t;
  exprtk::parser<double>       parser_t;    
  exprtk::symbol_table<double> symbol_table_t;

  if(!symbol_table_t.add_vector("initPoint",initPoint)){
    printf("limits vector not added\n");
  }

  // Register Symbol Table
  expression_t.register_symbol_table(symbol_table_t);

  // Compile Expression From File
  if(!parser_t.compile(currExpression,expression_t)){
    printf("Error: %s\n Input file: %s\n",parser_t.error().c_str(),currExpression.c_str());
    printf("\n");

    for (size_t i=0;i<parser_t.error_count();++i){
      // Include the specific nature of each error
      // and its position in the expression string.

      error_t = parser_t.get_error(i);

      printf("Error: %02d Position: %02d\n Type: [%s]\n Message: %s\n",
             static_cast<int>(i),
             static_cast<int>(error_t.token.position),
             exprtk::parser_error::to_str(error_t.mode).c_str(),
             error_t.diagnostic.c_str());
    }
    throw cmException("ERROR: Invalid input file.\n");
  }


  // Collect the inputs 
  long res = expression_t.value();
}

cmBertiniSolverModel::cmBertiniSolverModel(string inputFile){

  // Store Current input file
  this->inputFile = inputFile;

  // First Extract the number of dimensions and equations
  double dims = 0;
  double numEq = 0;
  extractNumDimensions(inputFile,dims);
  extractNumEquations(inputFile,numEq);

  // Assign Dimensions and number of equations
  this->dims = long(dims);
  this->numEq = long(numEq);

  // Extract Parameter Limits
  limits.resize(2*this->dims,0.0);
  extractLimits(inputFile,limits);

  // Extract Initial Point
  initPoint.resize(this->dims,0.0);
  extractInitPoint(inputFile,initPoint);

  // Extract Current Expression
  string key("eq");
  evalExpression = extractSubExprFromFile(inputFile,key);

}

cmBertiniSolverModel::~cmBertiniSolverModel(){
}

int cmBertiniSolverModel::getParameterTotal(){
  return dims;
}

int cmBertiniSolverModel::getStateTotal(){
  return 0;
}

int cmBertiniSolverModel::getResultTotal(){
  return 1;
}

void cmBertiniSolverModel::getParameterLimits(stdVec& limits){
  limits = this->limits;
}

void cmBertiniSolverModel::getDefaultParams(stdVec& params){
  params = this->initPoint;
}

void cmBertiniSolverModel::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: getPriorMapping not implemented.\n");
}

string cmBertiniSolverModel::getParamName(int parID){
  return string("x[" + to_string(parID) + "]");
}

string cmBertiniSolverModel::getResultName(int resID){
  string res;
  if(resID == 0){
    res = string("result");
  }else{
    throw cmException("ERROR: result id does not exists.\n");   
  }
  return res;
}

double cmBertiniSolverModel::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  // Add the variables
  exprtk::parser_error::type   error_t;  
  exprtk::expression<double>   expression_t;
  exprtk::parser<double>       parser_t;    
  exprtk::symbol_table<double> symbol_table_t;

  // Add Inputs to x
  stdVec x(inputs);
  if(!symbol_table_t.add_vector("x",x)){
    printf("x not added\n");
  }

  // Add the Equation vector
  stdVec eq(this->numEq,0.0);
  if(!symbol_table_t.add_vector("eq",eq)){
    printf("eq not added\n");
  }

  // Register Symbol Table
  expression_t.register_symbol_table(symbol_table_t);

  // Compile Expression From File
  if(!parser_t.compile(evalExpression,expression_t)){
    printf("Error: %s\n Input file: %s\n",parser_t.error().c_str(),evalExpression.c_str());
    printf("\n");

    for (size_t i=0;i<parser_t.error_count();++i){
      // Include the specific nature of each error
      // and its position in the expression string.

      error_t = parser_t.get_error(i);

      printf("Error: %02d Position: %02d\n Type: [%s]\n Message: %s\n",
             static_cast<int>(i),
             static_cast<int>(error_t.token.position),
             exprtk::parser_error::to_str(error_t.mode).c_str(),
             error_t.diagnostic.c_str());
    }
    throw cmException("ERROR: Invalid input file.\n");
  }

  // Collect the inputs 
  return expression_t.value();
}







