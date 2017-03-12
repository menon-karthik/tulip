
# include "cmModelVTKExtract.h"

using namespace std;

// ================
// EXTRACT VTK DATA
// ================
double ExtractVTKData(int nodeID,int resultType,vtkPolyData* polydata){
  double res = 0.0;
  if(resultType == 0){
    res = polydata->GetPointData()->GetScalars("pressure_avg_mmHg")->GetComponent(0,nodeID);
  }else if(resultType == 1){
    res = polydata->GetPointData()->GetScalars("TAWSS")->GetComponent(0,nodeID);
  }else if(resultType == 2){
    res = polydata->GetPointData()->GetScalars("OSI")->GetComponent(0,nodeID);
  }else{
    throw cmException("Invalid Result Type.\n");
  }
  // Return
  return res;
}

// =====================================
// CHECK IF POINTDATAFILE ALREADY EXISTS
// =====================================
bool pointDataFileExists(){
  bool res = false;
  ifstream pointDataFile;
  pointDataFile.open("pointDataTable.dat");
  if(pointDataFile.is_open()){
    res = true;
    pointDataFile.close();
  }else{
    res = false;
  }
  return res;
}

// ===========
// CONSTRUCTOR
// ===========
cmModelVTKExtract::cmModelVTKExtract(string fileName, int fileStart, int fileEnd, int fileResultType){
  stdVec temp;

  vtkFileName = fileName;
  fileIDStart = fileStart;
  fileIDEnd   = fileEnd;
  fileResultType = fileResultType;

  // Check If you can find an existing file
  bool foundPointDataFile = pointDataFileExists();

  // Read the Table or the VTK files
  if(foundPointDataFile){
    printf("Point Data File Found.\n");
    fflush(stdout);

    string inTable("pointDataTable.dat");
    readTableFromFile(inTable,nodeValsTable);
  }else{
    printf("Point Data File Not Found.\n");
    fflush(stdout);

    // Loop on Files
    double res = 0.0;
    printf("Reading VTK Files...\n");
    fflush(stdout);
    for(int loopA=fileIDStart;loopA<=fileIDEnd;loopA++){
    
      // Write Message
      printf("Reading VTK File n. %d...\n",loopA);
      fflush(stdout);

      // Read All data into the Table
      vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
      string currFileName = vtkFileName + "_" + to_string(loopA) + ".vtp";
      reader->SetFileName(currFileName.c_str());
      reader->Update();
      vtkPolyData* polydata = reader->GetOutput();

      int totalPoints = polydata->GetNumberOfPoints();

      // Extract Data
      temp.clear();
      for(int loopB=0;loopB<totalPoints;loopB++){
        res = ExtractVTKData(loopB,fileResultType,polydata);
        temp.push_back(res);
      }
      nodeValsTable.push_back(temp);
    }
    printf("Saving Point Data Table for reuse...\n");
    fflush(stdout);
    // WRITE TABLE TO FILE
    string outTable("pointDataTable.dat");
    writeTableToFile(outTable,nodeValsTable);
  }
}

// ================================
// EVAL MODEL: ONE MODEL EVALUATION
// ================================
double cmModelVTKExtract::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
  // Get The inputs
  int fileNameID = inputs[0];
  int nodeID     = inputs[1];

  outputs.clear();
  outputs.push_back(nodeValsTable[fileNameID][nodeID]);

  // Return
  errorCode.push_back(0);
  return 0.0;
}

// =====================================
// SAVE A AUX MODEL WITH NODE STATISTICS
// =====================================
void cmModelVTKExtract::applyNodeStatistics(stdMat& nodeStats){
  // Read All data into the Table
  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  string currFileName = vtkFileName + "_" + to_string(fileIDStart) + ".vtp";
  reader->SetFileName(currFileName.c_str());
  reader->Update();
  vtkPolyData* polydata = reader->GetOutput();
  // Add New Scalars With Node Statistics
  
  // Add Average
  vtkDoubleArray* average = vtkDoubleArray::New();
  average->SetName("SCH_AVERAGE");
  average->SetNumberOfComponents(1);
  average->SetNumberOfValues(polydata->GetNumberOfPoints());
  for(int loopA=0;loopA<polydata->GetNumberOfPoints();loopA++){
    average->SetValue(loopA,nodeStats[loopA][0]);
  }
  polydata->GetPointData()->AddArray(average);
  
  // Add Standard Deviation
  vtkDoubleArray* standardDev = vtkDoubleArray::New();
  standardDev->SetName("SCH_STD");
  standardDev->SetNumberOfComponents(1);
  standardDev->SetNumberOfValues(polydata->GetNumberOfPoints());
  for(int loopA=0;loopA<polydata->GetNumberOfPoints();loopA++){
    standardDev->SetValue(loopA,nodeStats[loopA][1]);
  }
  polydata->GetPointData()->AddArray(standardDev);

  // Export the model
  // Write the file
  vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("schResult.vtp");
  writer->SetInputData(polydata);
  writer->SetDataModeToBinary();
  writer->Write();

}

string cmModelVTKExtract::getResultName(int resID){
  return string("qty");
}
 
// =====================================================
// FUNCTIONS THAT MUST BE IMPLEMENTED BY THE SUB CLASSES
// =====================================================
int cmModelVTKExtract::getParameterTotal(){return 0;};
string cmModelVTKExtract::getParamName(int parID){
  return string("");
}
int cmModelVTKExtract::getStateTotal(){return 0;}
int cmModelVTKExtract::getResultTotal(){return 1;}
void cmModelVTKExtract::getParameterLimits(stdVec& limits){}
void cmModelVTKExtract::getDefaultParams(stdVec& params){}
void cmModelVTKExtract::getPriorMapping(int priorModelType,int* prPtr){}
