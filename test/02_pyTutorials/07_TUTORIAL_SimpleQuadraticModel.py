// ================================
// TUTORIAL EXCERCISE I
// Running a Simple Quadratic Model
// ================================

# include "uqTypes.h"
# include "cmModel.h"
# include "cmTutorial.h"
# include "daData.h"
# include "daData_Scalar_MultiplePatients.h"

using namespace std;

// =============
// MAIN FUNCTION
// =============
int main(int argc, char* argv[]){
  try{

    // Assign Dataset
    daData* data = new daData_Scalar_MultiplePatients();
    string fileName("../datasets/tutorial.csv");
    data->readFromFile(fileName);

    // Declare generic cmModel
    cmModel* myModel;
      
    // Construct Specific Model
    myModel = new cmTutorial();

    // Assign Dataset
    int currentColumn = 1;
    myModel->setData(data,currentColumn);

    // Get Default Input Parameter Values
    stdVec inputs;
    myModel->getDefaultParams(inputs);

    // Solve Model
    stdVec outputs;
    stdIntVec errorCodes;    
    double ll = myModel->evalModelError(inputs,outputs,errorCodes);

    printf("Model Results\n");
    printf("\n");
    printf("Final Location: %f\n",outputs[0]);
    printf("Total Time: %f\n",outputs[1]);
    printf("Maximum Height: %f\n",outputs[2]);
    printf("\n");
    printf("Resulting Log-likelihod: %f\n",ll);

  }catch(exception &e){
    // ERROR: TERMINATED!
    printf("\n");
    printf("TERMINATED!\n");
    return 0;
  }

  // COMPLETED
  printf("\n");
  printf("COMPLETED!\n");
  return 0;
}
