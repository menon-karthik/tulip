// Testing Non linear model parser
// DES, Feb 2018

# include "uqTypes.h"
# include "uqConstants.h"

# include "cmException.h"
# include "cmUtils.h"
# include "cmModel.h"
# include "cmBertiniSolverModel.h"

using namespace std;


int main(int argc, char* argv[]){

  double ll = 0.0;
    
  try{

    // Create new data object

    // Create a ODE Model
    string fileName("input.txt");
    cmModel* model = new cmBertiniSolverModel(fileName);

    // Get Default parameter set
    stdVec inputs;
    model->getDefaultParams(inputs);

    // Solve Model
    stdVec outputs;
    stdIntVec errorCodes;    
    ll = model->evalModelError(inputs,outputs,errorCodes);
    
  }catch(exception& e){
    // ERROR: TERMINATED!
    printf("\n");
    printf("Msg: %s\n",e.what());
    printf("TERMINATED!\n");
    return 0;
  }

  // COMPLETED
  printf("\n");
  printf("Obj: %f\n",ll);
  printf("COMPLETED!\n");
  return 0;
}





