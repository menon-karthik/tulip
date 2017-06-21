# include "daData_multiple_Table.h"

using namespace std;

daData_multiple_Table::daData_multiple_Table(){

}

daData_multiple_Table::~daData_multiple_Table(){

}

void daData_multiple_Table::readFromFile(string fileName){
  stdStringMat stringTable;
  stdStringVec temp;
  int error = readCSStringTableFromFile(fileName,stringTable);
  if(error != 0){
    throw daException("Error: in Data.readFromFile, invalid file.\n");
  }
  for(int loopA=0;loopA<stringTable.size();loopA++){
  	temp.clear();
  	for(int loopB=1;loopB<stringTable[0].size();loopB++){
      // Float Values
      // printf("Current String: %s\n",stringTable[loopA][loopB].c_str());
  	  temp.push_back(stringTable[loopA][loopB]);
  	}
    // printf("String: %s\n",stringTable[loopA][0].c_str());
  	dict[stringTable[loopA][0]] = temp;
  }
  
}

double daData_multiple_Table::evalOBJ(int dataIndex,stdStringVec keys,stdVec values,stdVec weights){
  
  // Check The Size of keys and values
  if((keys.size() != values.size())||(keys.size() != weights.size())){
    throw daException("Error in evalOBJ size of keys and values are not consistent.\n");
  }
  if(dict.begin()->second.size() < dataIndex){
    throw daException("Error in evalOBJ invalid data index.\n");
  }
  
  // Simple Cost Objective: Sum of Percent Change
  double result = 0.0;
  double perc = 0.0;
  int count = 0;
  double computed = 0.0;
  string measuredString;
  double measured = 0.0;
  for(int loopA=0;loopA<keys.size();loopA++){
    if(dict.find(keys[loopA]) != dict.end()){
      // Found Key
      computed = values[loopA];      
      measuredString = dict[keys[loopA]][dataIndex];
      if(measuredString.compare("none") != 0){
        try{
          measured = atof(measuredString.c_str());
          perc = (100.0*fabs(computed - measured)/(double)measured) * weights[loopA];
          result += perc * perc;
          count++;
        }catch(...){
        }
      }
    }
  }
  result /= (double)count;
  return result;
}

double daData_multiple_Table::evalLogLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights){
  
  // Check The Size of keys and values
  if((keys.size() != avValues.size())||(keys.size() != stdFactors.size())||(keys.size() != weights.size())){
    throw daException("Error in evalPosterior size of keys and values are not consistent.\n");
  }
  if(dict.begin()->second.size() < dataIndex + 1){
    throw daException("Error in evalPosterior invalid data index.\n");
  }
  
  // Eval log-likelihood
  double result = 0.0;
  double perc = 0.0;
  int    count = 0;
  double computed = 0.0;
  string measuredString;
  double measured = 0.0;
  double stdDev = 0.0;
  double weightVal = 0.0;
  double stdFactor = 0.0;
  // printf("Test\n");
  for(int loopA=0;loopA<keys.size();loopA++){
    if(dict.find(keys[loopA]) != dict.end()){
      // Found Key
      computed = avValues[loopA];  
      stdFactor = stdFactors[loopA];
      weightVal = weights[loopA];
      measuredString = dict[keys[loopA]][dataIndex];
      if(measuredString.compare("none") != 0){
        try{
          measured = atof(measuredString.c_str());
          stdDev = fabs(stdFactor);          
          perc = 0.5*(computed - measured)*(computed - measured)/((stdDev * stdDev) * weightVal);
          // printf("%f\n",perc);          
          result += perc;
          count++;
        }catch(...){
        }
      }
    }
  }
  // printf("\n");
  // printf("Result: %f\n",result);
  // Return Value
  return result;
}

double daData_multiple_Table::evalLikelihood(int dataIndex,stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights){
  double ll = evalLogLikelihood(dataIndex,keys,avValues,stdFactors,weights);
  return log(ll);
}

void daData_multiple_Table::printAndCompare(int datasetColumn,stdStringVec keys,stdVec values,stdVec weigths){
  
  // Check The Size of keys and values
  if((keys.size() != values.size())||(keys.size() != weigths.size())){
    throw daException("Error in evalOBJ size of keys and values are not consistent.\n");
  }

  // PRINT MAP
  //typedef map<string,stdStringVec>::iterator it_type;
  //for(it_type iterator = dict.begin(); iterator != dict.end(); iterator++) {
  //  printf("Key %s, Value %s\n",iterator->first.c_str(),iterator->second[1].c_str());
  //}

  if(dict.begin()->second.size() < (datasetColumn + 1)){
    throw daException("Error in evalOBJ invalid data index.\n");
  }

  // Simple Cost Objective: Sum of Percent Change
  FILE* fp;
  fp = fopen("outputTargets.out","w");
  double computed = 0.0;
  string measuredString;
  double measured = 0.0;
  double weight = 0.0;
  fprintf(fp,"%15s %15s %15s %15s\n","Key", "Measured", "Computed", "Weight");
  for(int loopA=0;loopA<keys.size();loopA++){
    if(dict.find(keys[loopA]) != dict.end()){
      // Found Key
      computed = values[loopA];      
      measuredString = dict[keys[loopA]][datasetColumn];      
      if(measuredString.compare("none") != 0){        
        try{
          measured = atof(measuredString.c_str());
          weight = weigths[loopA];
          fprintf(fp,"%15s %15.3f %15.3f %15.3f\n",keys[loopA].c_str(),measured,computed,weight);
        }catch(...){
        }
      }
    }
  }
  fprintf(fp,"\n");
  fclose(fp);
}

int daData_multiple_Table::getPatientValue(int patientID,string key,double &result){
  int retVal = 0;
  if(dict.find(key) != dict.end()){
    try{
      result = atof(dict[key][patientID - 1].c_str());
    }catch(...){
      result = 0.0;
      retVal = 1;
      return retVal;
    }    
    retVal = 0;
  }else{
    result = 0.0;
    retVal = 1;
  }
  return retVal;
}
