
# include "cmOpenLoopSurrogateModel.h"

using namespace std;

// CONSTRUCTOR
cmOpenLoopSurrogateModel::cmOpenLoopSurrogateModel(double localTimeStep3DModel, 
	                                                 int localTotSteps3DModel, 
	                                                 double localTotalStepsRCR, 
                                                   string localModel3DFileName,
                                                   string localRCRFileName,
                                                   string localRESFileName,
                                                   string localMappingFileName,
                                                   string localIniFlowRatesFileName,
                                                   string localInletPressuresFileName,
                                                   string localInletFlowratesFileName,
                                                   stdVec localTargets){
  // DATA
  timeStep3DModel = localTimeStep3DModel;
  totSteps3DModel = localTotSteps3DModel;
  totalStepsRCR = localTotalStepsRCR;
  // FILENAMES
  model3DFileName = localModel3DFileName;
  rcrFileName = localRCRFileName;
  resFileName = localRESFileName;
  mappingFileName = localMappingFileName;
  iniFlowRatesFileName = localIniFlowRatesFileName;
  inletPressuresFileName = localInletPressuresFileName;
  inletFlowratesFileName = localInletFlowratesFileName;
  // TARGETS
  targets.clear();
  for(int loopA=0;loopA<localTargets.size();loopA++){
    targets.push_back(localTargets[loopA]);
  }

}

// ==========================
// TOTAL NUMBER OF PARAMETERS
// ==========================
int cmOpenLoopSurrogateModel::getParameterTotal(){
  return 19;
}

// ==================
// GET PARAMETER NAME
// ==================
string cmOpenLoopSurrogateModel::getParamName(int parID){
  if(parID == 0){
    return string("");	
  }else{
    throw cmException("ERROR: Not Implemented.\n");
  }
}

// ==========================
// GET TOTAL NUMBER OF STATES
// ==========================
int cmOpenLoopSurrogateModel::getStateTotal(){
  return 0;
}

// ==========================
// GET TOTAL NUMBER OF RESULT
// ==========================
int cmOpenLoopSurrogateModel::getResultTotal(){
  // MIN,AV,MAX PRESSURE
  return 3;
}

// ====================
// GET PARAMETER LIMITS
// ====================
void cmOpenLoopSurrogateModel::getParameterLimits(stdVec& limits){

  limits.resize(2*getParameterTotal());

  limits[0] = 0.1; limits[1] = 10.0;
  limits[2] = 0.1; limits[3] = 10.0;
  limits[4] = 0.1; limits[5] = 10.0;
  limits[6] = 0.1; limits[7] = 10.0;
  limits[8] = 0.1; limits[9] = 10.0;
  limits[10] = 0.1; limits[11] = 10.0;
  limits[12] = 0.1; limits[13] = 10.0;
  limits[14] = 0.1; limits[15] = 10.0;
  limits[16] = 0.1; limits[17] = 10.0;
  limits[18] = 0.1; limits[19] = 10.0;
  limits[20] = 0.1; limits[21] = 10.0;
  limits[22] = 0.1; limits[23] = 10.0;
  limits[24] = 0.1; limits[25] = 10.0;
  limits[26] = 0.1; limits[27] = 10.0;
  limits[28] = 0.1; limits[29] = 10.0;
  limits[30] = 0.1; limits[31] = 10.0;
  limits[32] = 0.1; limits[33] = 10.0;
  limits[34] = 0.1; limits[35] = 10.0;
  limits[36] = 0.1; limits[37] = 10.0;
}

// ======================
// GET DEFAULT PARAMETERS
// ======================
void cmOpenLoopSurrogateModel::getDefaultParams(stdVec& params){

  params.resize(getParameterTotal());

  // Resistance Factors
  params[0] = 1.0;
  params[1] = 1.0;
  params[2] = 1.0;
  params[3] = 1.0;
  params[4] = 1.0;
  params[5] = 1.0;
  params[6] = 1.0;
  params[7] = 1.0;
  params[8] = 1.0; // To change
  params[9] = 1.0;
  params[10] = 1.0;
  params[11] = 1.0;
  params[12] = 1.0;
  params[13] = 1.0;
  params[14] = 1.0;
  params[15] = 1.0;
  params[16] = 1.0;
  params[17] = 1.0;
  params[18] = 1.0;

}

// =============
// PRIOR MAPPING
// =============
void cmOpenLoopSurrogateModel::getPriorMapping(int priorModelType,int* prPtr){
  throw cmException("ERROR: Not Implemented.\n");
}

// ======================================
// EVAL MODEL: MULTIPLE MODEL EVALUATIONS
// ======================================
stdVec cmOpenLoopSurrogateModel::evalModelError(stdMat inputs,stdMat& outputs, stdIntVec& errorCodes){
  throw new cmException("ERROR: Not Implemented.\n");
}

// READ INITIAL FLOW RATES
void readIniFlowRateFile(string iniFlowRatesFileName, stdMat &iniFlowRates){
  // Read Table From File
  int error = readTableFromFile(iniFlowRatesFileName,iniFlowRates);
  if(error != 0){
    throw cmException("ERROR: Invalid Outlet Mapping File.\n");
  }
}  

// =============================
// READ INLET PRESSURE HISTORIES
// =============================
void readInletPressureTimeHistFile(string inletPressuresFileName,stdMat &inletPressures){
  // Read Table From File
  int error = readTableFromFile(inletPressuresFileName,inletPressures);
  if(error != 0){
    throw cmException("ERROR: Invalid Inlet Pressure File.\n");
  }
}

// =============================
// READ INLET FLOWRATE HISTORIES
// =============================
void readInletFlowrateTimeHistFile(string inletFlowratesFileName,stdMat &inletFlowrates){
  // Read Table From File
  int error = readTableFromFile(inletFlowratesFileName,inletFlowrates);
  if(error != 0){
    throw cmException("ERROR: Invalid Inlet Flowrate File.\n");
  }
}

// ========================
// READ OUTLET MAPPING FILE
// ========================
void readOutletMappingFile(string mappingFileName,stdIntVec &outletMap){
  // Read Table From File
  int error = readIntVectorFromFile(mappingFileName,outletMap);
  if(error != 0){
    throw cmException("ERROR: Invalid Outlet Mapping File.\n");
  }
}

// ===========
// READ R FILE
// ===========
void read_SV_RInputFile(string resFileName,resArray &ress){
  
  int numData = 0;
  resRecord* res;
  stdVec temp;

  // Declare input File
  ifstream infile;
  infile.open(resFileName);

  // Read Data From File
  string buffer;
  vector<string> tokenizedString;

  // SKIP FIRT LINE: Max time history data size
  std::getline(infile,buffer);

  while (std::getline(infile,buffer)){
    // Trim String
    boost::trim(buffer);
    // Tokenize String
    boost::split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);

    // Create new resitance object
    res = new resRecord;

    try{

      // Read the total number of entries
      numData = atoi(tokenizedString[0].c_str());

      for(int loopA=0;loopA<numData+1;loopA++){
        std::getline(infile,buffer);
        boost::trim(buffer);
        boost::split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
        if(loopA == 0){
          // Proximal Resistance Value
          res->resValue = atof(tokenizedString[0].c_str());
        }else if(loopA >= 1){
          temp.push_back(atof(tokenizedString[0].c_str()));
          temp.push_back(atof(tokenizedString[1].c_str()));
          res->distPressHistory.push_back(temp);
        }
      }
      // Add rcr Record
      ress.push_back(res);
    }catch(...){
      throw cmException("ERROR: Invalid Input File Format in read3DModelSurrogateFile.\n");
    }
  }
  // Close File
  infile.close();
}


// READ RCR FILE
void read_SV_RCRInputFile(string fileName,rcrArray &rcrs){

  int numData = 0;
  rcrRecord* rcr;
  stdVec temp;

  // Declare input File
  ifstream infile;
  infile.open(fileName);

  // Read Data From File
  string buffer;
  vector<string> tokenizedString;

  // SKIP FIRT LINE: Max time history data size
  std::getline(infile,buffer);

  while (std::getline(infile,buffer)){
    // Trim String
    boost::trim(buffer);
    // Tokenize String
    boost::split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);

    // Create new resitance object
    rcr = new rcrRecord;

    try{

      // Read the total number of entries
      numData = atoi(tokenizedString[0].c_str());

      for(int loopA=0;loopA<numData+3;loopA++){
        std::getline(infile,buffer);
        boost::trim(buffer);
        boost::split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);        
        if(loopA == 0){
          // Proximal Resistance Value
          rcr->rProxValue = atof(tokenizedString[0].c_str());
        }else if(loopA == 1){
          // Capacitance
          rcr->capValue = atof(tokenizedString[0].c_str());
        }else if(loopA == 2){
          // Distal Resistance Value
          rcr->rDistValue = atof(tokenizedString[0].c_str());
        }else if(loopA >= 3){
          temp.clear();
          temp.push_back(atof(tokenizedString[0].c_str()));
          temp.push_back(atof(tokenizedString[1].c_str()));
          rcr->distPressHistory.push_back(temp);
        }
      }
      // Add rcr Record
      rcrs.push_back(rcr);
    }catch(...){
      throw cmException("ERROR: Invalid Input File Format in read3DModelSurrogateFile.\n");
    }
  }
  // Close File
  infile.close();
}


// ===========
// FIND INLETS
// ===========
void findInlets(int totOutlets, stdIntVec outletMap, stdBoolVec &isInlet, int& inletIDX){
  // Init Array
  for(int loopA=0;loopA<totOutlets;loopA++){
    isInlet.push_back(true);
  }
  for(int loopA=0;loopA<outletMap.size();loopA++){
    isInlet[outletMap[loopA]] = false;
  }
  for(int loopA=0;loopA<totOutlets;loopA++){
    if(isInlet[loopA]){
      inletIDX = loopA;
    }
  }
}


// ================
// READ INPUT FILES
// ================
void readInputs(string model3DFileName,model3DRecord &model,
                string rcrFileName,rcrArray &rcrs,
                string resFileName,resArray &ress,
                string mappingFileName, stdIntVec &outletMap,
                string iniFlowRatesFileName, stdMat &iniFlowRates,
                string inletPressuresFileName,stdMat &inletPressures,
                string inletFlowratesFileName,stdMat &inletFlowrates){
  
  // READ INPUT FILE
  //printf("Reading Files...\n");
  bool printReport = false;
  read3DModelSurrogateFromFile(model3DFileName,model,printReport);
  //printf("Surrogate file read.\n");

  // READ RCR FILE
  if(rcrFileName != ""){
    read_SV_RCRInputFile(rcrFileName,rcrs);	
    //printf("RCR file read.\n");
  }
  
  // READ R FILE
  if(resFileName != ""){
    read_SV_RInputFile(resFileName,ress);	
    //printf("Resistance file read.\n");
  }
  
  // READ OUTLET MAPPING FILE
  readOutletMappingFile(mappingFileName,outletMap);  
  //printf("Outlet mapping file read.\n");

  // READ INITIAL FLOW RATES
  readIniFlowRateFile(iniFlowRatesFileName,iniFlowRates);  
  //printf("Initial flow rate file read.\n");

  // READ INLET PRESSURE HISTORIES
  readInletPressureTimeHistFile(inletPressuresFileName,inletPressures);
  //printf("Inlet pressure time history file read.\n");

  // READ INLET FLOWRATE HISTORIES
  readInletFlowrateTimeHistFile(inletFlowratesFileName,inletFlowrates);
  //printf("Inlet flow rate time history file read.\n");
}


// ====================================================
// CHANGE RCR VALUES FOR SELECTED PARAMETER COMBINATION
// ====================================================
void modifyRCRValues(rcrArray& rcrs,stdVec inputs){
  // Modulate Resistance Values
  for(int loopA=0;loopA<inputs.size();loopA++){
    rcrs[loopA]->rProxValue = rcrs[loopA]->rProxValue*inputs[loopA];
    rcrs[loopA]->rDistValue = rcrs[loopA]->rDistValue*inputs[loopA];
  }
}

// ================================
// EVAL MODEL: ONE MODEL EVALUATION
// ================================
double cmOpenLoopSurrogateModel::evalModelError(stdVec inputs,stdVec& outputs,stdIntVec& errorCodes){

  // SET TIME INTEGRATION PARAMETERS
  double timeStepRCR = timeStep3DModel/(double)totalStepsRCR;
  double paTommHg = 1.0/133.22;

  // DECLARE QUANTITIES
  model3DRecord model;
  rcrArray rcrs;
  resArray ress;
  stdIntVec outletMap;
  stdMat iniFlowRates;
  stdVec model3DInputs;
  stdMat inletPressures;
  stdMat inletFlowrates;
  stdBoolVec isInlet;

  // MODELS ARRAY
  vector<cmModel*> resModels;
  vector<cmModel*> rcrModels;

  // READ INPUT FILES
  readInputs(model3DFileName,model,
             rcrFileName,rcrs,
             resFileName,ress,
             mappingFileName,outletMap,
             iniFlowRatesFileName,iniFlowRates,
             inletPressuresFileName,inletPressures,
             inletFlowratesFileName,inletFlowrates);

  // GET TOTAL OUTLETS
  int totOutlets = iniFlowRates.size();  

  // INSERT THE PARAMETERS IN THE RCR VALUES
  printf("\n");
  for(int loopA=0;loopA<inputs.size();loopA++){
    printf("Input %d: %f\n",loopA,inputs[loopA]);
  }
  modifyRCRValues(rcrs,inputs);

  // CREATE 3D MODEL PQ APPROXIMANT
  cmModel* model3D;
  model3D = new cm3DPQSurrogateModel(model.polyType,model.polyOrder,model.normalizeData,
                                     model.limits,model.expCoeffs);

  
  // CREATE RESISTANCE AND RCR MODELS
  // RESISTANCE
  for(int loopA=0;loopA<ress.size();loopA++){
    cmModel* res;
    res = new cmResistanceModel(ress[loopA]->resValue,ress[loopA]->distPressHistory);
    resModels.push_back(res);
  }
  // RCR
  for(int loopA=0;loopA<rcrs.size();loopA++){
    cmModel* rcr;
    rcr = new cmRCRModel(rcrs[loopA]->rProxValue,rcrs[loopA]->capValue,
                         rcrs[loopA]->rDistValue,rcrs[loopA]->distPressHistory);
    rcrModels.push_back(rcr);
    //((cmRCRModel*)rcrModels[loopA])->show();
  }

  // ===========
  // FIND INLETS
  // ===========
  int inletIDX = 0;
  findInlets(totOutlets,outletMap,isInlet,inletIDX);
  // Print Inlets
  //for(int loopA=0;loopA<isInlet.size();loopA++){
  //  if(isInlet[loopA]){
  //    printf("Outlet %d is an inlet\n",loopA);
  //  }
  //}

  // =========================
  // ASSIGN INITIAL CONDITIONS
  // =========================
  stdVec currOutletFlowRates;
  stdVec currOutletPressure;
  double currentTime = 0.0;
  // Assign Init model flow rates
  //printf("Outlets: %d\n",totOutlets);
  for(int loopA=0;loopA<totOutlets;loopA++){
    currOutletFlowRates.push_back(iniFlowRates[loopA][0]);
    currOutletPressure.push_back(0.0);
  }  

  // =====================
  // TIME INTEGRATION LOOP
  // =====================
  stdMat outPress;
  stdMat outFlow;
  try{
    for(int loopA=0;loopA<totSteps3DModel;loopA++){
      // Update Current Time
      if(loopA>0){
        currentTime += timeStep3DModel;
      }   

      // =================
      // SOLVE RESISTANCES
      // =================
      // Give flow rate to the R and RCR MODEL and SOLVE for One 3D Model TimeStep
      // Get Pressure at the next 3D model time step
      stdVec inputs;
      stdVec outputs;
      stdIntVec errorCodes;
      for(int loopB=0;loopB<ress.size();loopB++){
        inputs.clear();
        // Assign Current Time
        inputs.push_back(currentTime + timeStep3DModel);
        // Assign Flow Rate as Input
        inputs.push_back(currOutletFlowRates[outletMap[loopB]]);
        // Solve R Model
        resModels[loopA]->evalModelError(inputs,outputs,errorCodes);
        if(errorCodes[0] != 0){
          throw cmException("ERROR: Cannot Solve Resistance Model.\n");
        }
        // Read Pressure Back
        currOutletPressure[outletMap[loopB]] = outputs[0];
      }          

      // =========
      // SOLVE RCR
      // =========
      for(int loopB=0;loopB<rcrs.size();loopB++){
        // Assign Inputs for RCR Model
        inputs.clear();      
        inputs.push_back(currentTime);
        inputs.push_back(timeStepRCR);
        inputs.push_back(totalStepsRCR);
        inputs.push_back(currOutletFlowRates[outletMap[ress.size()+loopB]]);        

        // Solve RCR Model
        rcrModels[loopB]->evalModelError(inputs,outputs,errorCodes);      
        if(errorCodes[0] != 0){
          throw cmException("ERROR: Cannot Solve RCR Model.\n");
        }          

        // Read Pressure Back        
        currOutletPressure[outletMap[ress.size()+loopB]] = outputs[0];
        //printf("Pressure From RCR: %e\n",currOutletPressure[outletMap[ress.size()+loopA]]);
      }      
      
      // ==========================================
      // GET PRESSURE AT INLETS USING INTERPOLATION
      // ==========================================
      // NOTE: MUST BE CONSIDERED WITH THE QUANTITY USED 
      // IN THE PQ INTERPOLATON SCHEME!
      int currInlet = 0;
      for(int loopB=0;loopB<totOutlets;loopB++){
        if(isInlet[loopB]){
          currInlet++;
          // Here is called Pressure but can be also the flowrate
          // In general any independent variable
          //currOutletPressure[loopB] = linInterp(inletPressures,0,currInlet,currentTime);
          //currOutletPressure[loopB] = 1.0e4;
          currOutletPressure[loopB] = linInterp(inletFlowrates,0,currInlet,currentTime);
        }
      }      

      // ================================
      // SOLVE 3D MODEL TO GET FLOW RATES
      // ================================
      // Update Flow Rates based on 3D Model Interpolant
      // Assign Current Pressure As Inputs
      model3DInputs.clear();
      for(int loopB=0;loopB<totOutlets;loopB++){
        model3DInputs.push_back(currOutletPressure[loopB]);
      }

      model3D->evalModelError(model3DInputs,outputs,errorCodes);
      if(errorCodes[0] != 0){
        throw cmException("ERROR: Cannot Solve 3D Surrogate Model.\n");
      }    

      // Update Flow Rates
      for(int loopB=0;loopB<totOutlets;loopB++){
        currOutletFlowRates[loopB] = outputs[loopB];
        //currOutletFlowRates[loopB] = 10.0;
        //currOutletFlowRates[loopB] = 10.0*sin(2.0*M_PI*(1.5)*currentTime);
        //currOutletFlowRates[loopB] = modelFlowRates[loopA+1][loopB];
      }

      // ===========================================
      // GET FLOW RATE AT INLETS USING INTERPOLATION
      // ===========================================
      currInlet = 0;
      for(int loopB=0;loopB<totOutlets;loopB++){
        if(isInlet[loopB]){
          currInlet++;
          currOutletFlowRates[loopB] = linInterp(inletFlowrates,0,currInlet,currentTime);
        }
      }

      // =======================
      // ADD PRESSURE/FLOW RATES
      // =======================
      outFlow.push_back(currOutletFlowRates);
      outPress.push_back(currOutletPressure);      
      
    }

    // WRITE TABLE TO FILE
    writeTableToFile(string("flowOut.dat"),outFlow);
    
    // ==============
    // CREATE RESULTS
    // ==============
    // GET AVERAGE FLOW RATE AT ALL OUTLETS
    int startLastCycle = 400;
    int endLastCycle = 500;
    stdVec avFlowSplit;
    for(int loopA=0;loopA<totOutlets;loopA++){
      if(loopA != inletIDX){
        avFlowSplit.push_back(0.0);
        for(int loopB=startLastCycle;loopB<endLastCycle;loopB++){
          avFlowSplit[avFlowSplit.size()-1] += fabs(outFlow[loopB][loopA]/outFlow[loopB][inletIDX]);
        }
        avFlowSplit[avFlowSplit.size()-1] = 100.0*avFlowSplit[avFlowSplit.size()-1]/(double)(endLastCycle-startLastCycle);
      }
    }
    
    // STORE IN OUTPUT
    outputs.clear();
    for(int loopA=0;loopA<avFlowSplit.size();loopA++){
      outputs.push_back(avFlowSplit[loopA]);
    }
    
    // =======================
    // EVAL OBJECTIVE FUNCTION
    // =======================
    double ll = 0.0;
    for(int loopA=0;loopA<avFlowSplit.size();loopA++){
      ll += (targets[loopA] - avFlowSplit[loopA]) * (targets[loopA] - avFlowSplit[loopA]);
    }
    //ll = sqrt(ll/(double)avFlowSplit.size());
    if(ll != ll){
      throw cmException("ERROR: Not Converged.\n");
    }


    std::string outNames[] = {"Inferior mesenteric artery",
                              "Right interior iliac artery",
                              "Right interior iliac artery branch 1",
                              "Right interior iliac artery branch 2",
                              "Left interior iliac artery branch 1",
                              "Left interior iliac artery",
                              "Left interior iliac artery branch 2",
                              "Aortic Inlet",
                              "Right exterior iliac artery",
                              "Left exterior iliac artery",
                              "Superior mesenteric artery",
                              "Left inferior renal artery",
                              "Left superior renal artery",
                              "Superior mesenteric artery branch 1",
                              "Right superior renal artery",
                              "Right inferior renal artery",
                              "Splenic artery branch 1",
                              "Splenic artery branch 2",
                              "Hepatic artery branch 2",
                              "Hepatic artery branch 1"};
    // PRINT FLOW SPLITS
    printf("Exact ");
    for(int loopA=0;loopA<avFlowSplit.size();loopA++){
      printf("%7.3f ",targets[loopA]);
    }
    printf("\n");
    printf("Got   ");
    for(int loopA=0;loopA<avFlowSplit.size();loopA++){
      printf("%7.3f ",avFlowSplit[loopA]);
    }
    printf("\n");

    //printf("Cost Function Value: %f\n",ll);

    // COMPLETED
    errorCodes.push_back(0);
    printf("Error State: %d\n",errorCodes[0]);
    return ll;

  }catch(exception &e){
  	// ERROR
  	errorCodes.push_back(1);
    printf("Error State: %d\n",errorCodes[0]);
  	return 0;
  }
}








