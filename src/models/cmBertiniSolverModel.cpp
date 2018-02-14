# include "cmBertiniSolverModel.h"

# include <cstdio>
# include <string>

# include "cmException.h"
# include "exprtk.h"

using namespace std;

// Extract dimensions and number of equations from file
void extractDimensions(string inputFile,long& dims,long& numEq){

  string currExpression("");
  string line;
  size_t foundDim;
  size_t foundEqu;
  ifstream f(inputFile);
  if(f.is_open()){
    while(getline(f,line)){
      foundDim  = line.find("dims");
      foundEqu  = line.find("numEq");
      if((foundDim!=string::npos||foundEqu!=string::npos)){
        currExpression += line + " ";
      }
    }
    f.close();
  }

  printf("Current Expr: %s\n",currExpression.c_str());

  // Add the variables
  exprtk::parser_error::type   error_t;  
  exprtk::expression<double>   expression_t;
  exprtk::parser<double>       parser_t;    
  exprtk::symbol_table<double> symbol_table_t;

  double dimsTmp;
  double numEqTmp;

  if(!symbol_table_t.add_variable("dims",dimsTmp)){
    printf("dims not added\n");
  }
  if(!symbol_table_t.add_variable("numEq",numEqTmp)){
    printf("numEq not added\n");
  }

  // Register Symbol Table
  expression_t.register_symbol_table(symbol_table_t);

  // Compile Expression From File
  parser_t.compile(currExpression,expression_t);

  // Collect the inputs 
  expression_t.value();

  // Print the two values
  printf("Number of Dimensions: %d\n",int(dimsTmp));
  printf("Number of Equations: %d\n",int(numEqTmp));
}

// Extract Parameter Limits
void extractLimits(string inputFile,stdVec& limits){
  
  string currExpression("");
  string line;
  size_t found;
  ifstream f(inputFile);
  if(f.is_open()){
    while(getline(f,line)){
      line = trim(line);
      // Check if the string is a comment
      if(line[0] == "#"){

      }else{

      }
      found = line.find("limits");
      if(found!=string::npos){
        currExpression += line + " ";
      }
    }
    f.close();
  }

  printf("Current Expr: %s\n",currExpression.c_str());

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
  parser_t.compile(currExpression,expression_t);

  // Collect the inputs 
  long res = expression_t.value();

  // Print the two values
  for(int loopA=0;loopA<limits.size()/2;loopA++){
    printf("Min: %f, Max: %f\n",limits[2*loopA + 0],limits[2*loopA + 1]);
  }

}

cmBertiniSolverModel::cmBertiniSolverModel(string inputFile){

  // First Extract the number of dimensions and equations
  long dims = 0;
  long numEq = 0;
  extractDimensions(inputFile,dims,numEq);

  // Assign Dimensions and number of equations
  this->dims = dims;
  this->numEq = numEq;

  // Extract Parameter Limits
  stdVec limits(2*this->dims,0.0);
  extractLimits(inputFile,limits);

  // // Init Error Type
  // exprtk::parser_error::type error;

  // // Read an input non linear system from a text file
  // ifstream f(inputFile);
  // stringstream buffer;
  // buffer << f.rdbuf();
  // string fileContent = buffer.str();

  // // Add Symbols
  // x.resize(1);
  // eq.resize(1);
  // if(!symbol_table.add_vector("x",x)){
  //   printf("x not added\n");
  // }
  // if(!symbol_table.add_vector("eq",eq)){
  //   printf("eq not added\n");
  // }
  // if(!symbol_table.add_variable("dims",dims)){
  //   printf("dims not added\n");
  // }
  // if(!symbol_table.add_variable("numEq",numEq)){
  //   printf("numEq not added\n");
  // }

  // // Register Symbol Table
  // expression.register_symbol_table(symbol_table);

  // // Compile Expression From File
  // if(!parser.compile(fileContent,expression)){
  //   printf("Error: %s\n Input file: %s\n",parser.error().c_str(),fileContent.c_str());
  //   printf("\n");

  //   for (size_t i=0;i<parser.error_count();++i){
  //     // Include the specific nature of each error
  //     // and its position in the expression string.

  //     error = parser.get_error(i);

  //     printf("Error: %02d Position: %02d\n Type: [%s]\n Message: %s\n",
  //            static_cast<int>(i),
  //            static_cast<int>(error.token.position),
  //            exprtk::parser_error::to_str(error.mode).c_str(),
  //            error.diagnostic.c_str());
  //   }
  // 	throw cmException("ERROR: Invalid input file.\n");
  // }else{
  // 	printf("Input file successfully compiled.\n");
  // }

  // // Collect the inputs 
  // double res = expression.value();

  // printf("dims: %f\n",dims);
  // printf("numEq: %f\n",numEq);

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

}
void cmBertiniSolverModel::getDefaultParams(stdVec& params){

}
void cmBertiniSolverModel::getPriorMapping(int priorModelType,int* prPtr){

}
string cmBertiniSolverModel::getParamName(int parID){

}
string cmBertiniSolverModel::getResultName(int resID){
  string res;
  if(resID == 0){
    res = string("result");
  }
  return res;
}
double cmBertiniSolverModel::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  double res = 0.0;
  // Add res to the outputs
  outputs.clear();
  outputs.push_back(res);
  // No Error
  errorCode.clear();
  errorCode.push_back(0);
  // Return 
  return res;


}





