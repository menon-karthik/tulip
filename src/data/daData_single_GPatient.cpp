# include "daData_single_GPatient.h"

using namespace std;

daData_single_GPatient::daData_single_GPatient(int keyColumn,int timeStampColumn){
  this->keyColumn = keyColumn;
  this->timeStampColumn = timeStampColumn;
}

daData_single_GPatient::~daData_single_GPatient(){

}

void daData_single_GPatient::getTable(stdMat& table){
  throw daException("ERROR: getTable not implemented for daData_single_GPatient.\n");
}

void daData_single_GPatient::readFromFile(string fileName){
  stdStringMat stringTable;
  stdStringVec temp;
  int error = readCSStringTableFromFile(fileName,stringTable);
  if(error != 0){
    throw daException("Error: in Data.readFromFile, invalid file.\n");
  }
  for(int loopA=0;loopA<stringTable.size();loopA++){
    if(stringTable[loopA][0].compare("REDCap") == 0){
      temp.clear();
      // 0-th column is the RedCap string: avoid starting from 1
      for(int loopB=1;loopB<stringTable[loopA].size();loopB++){
        if((loopB != keyColumn)&&(loopB != timeStampColumn)){
          temp.push_back(stringTable[loopA][loopB]);  
        }     
      }
      if(temp.size() > 1){
        throw daException("ERROR: REDCap data is not a scalar.\n");
      }
      dict[stringTable[loopA][keyColumn]] = temp;
    }
  }
}

double daData_single_GPatient::evalOBJ(const stdStringVec& keys,const stdVec& values,const stdVec& weights){
  
  // Check The Size of keys and values
  if((keys.size() != values.size())||(keys.size() != weights.size())){
    throw daException("Error in evalOBJ size of keys and values are not consistent.\n");
  }
  
  // Simple Cost Objective: Sum of Squared Percent Change
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
      measuredString = dict[keys[loopA]][0];
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

double daData_single_GPatient::evalLogLikelihood(stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights){
  
  // Check The Size of keys and values
  if((keys.size() != avValues.size())||(keys.size() != stdFactors.size())||(keys.size() != weights.size())){
    throw daException("Error in evalPosterior size of keys and values are not consistent.\n");
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
  for(int loopA=0;loopA<keys.size();loopA++){
    if(dict.find(keys[loopA]) != dict.end()){
      // Found Key
      computed = avValues[loopA];  
      stdFactor = stdFactors[loopA];
      weightVal = weights[loopA];
      measuredString = dict[keys[loopA]][0];
      if(measuredString.compare("none") != 0){
        try{
          measured = atof(measuredString.c_str());
          // Use Absolute STDs
          stdDev = fabs(stdFactor);
          perc = 0.5*(computed - measured)*(computed - measured)/((stdDev * stdDev) * weightVal);
          result += perc;
          count++;
        }catch(...){
        }
      }
    }
  }
  //printf("Total checked targets: %d\n",count);
  // Return Value
  return result;
}

double daData_single_GPatient::evalLikelihood(stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights){
  double ll = evalLogLikelihood(keys,avValues,stdFactors,weights);
  return log(ll);
}

void daData_single_GPatient::printAndCompare(stdStringVec keys,stdVec values,stdVec weigths){
  
  // Check The Size of keys and values
  if((keys.size() != values.size())||(keys.size() != weigths.size())){
    throw daException("Error in evalOBJ size of keys and values are not consistent.\n");
  }

  // PRINT MAP
  //typedef map<string,stdVec>::iterator it_type;
  //for(it_type iterator = dict.begin(); iterator != dict.end(); iterator++) {
  //  printf("Key %s, Value %f\n",iterator->first.c_str(),iterator->second[1]);
  //}

  if(dict.begin()->second.size() == 0){
    throw daException("Error in evalOBJ invalid data index.\n");
  }
  
  // Simple Cost Objective: Sum of Percent Change
  FILE* fp;
  fp = fopen("outputTargets.out","w");
  double computed = 0.0;
  string measuredString;
  double measured = 0.0;
  double weight = 0.0;
  fprintf(fp,"%30s %15s %15s %15s\n","Key", "Measured", "Computed", "Weight");
  for(int loopA=0;loopA<keys.size();loopA++){
    if(dict.find(keys[loopA]) != dict.end()){
      // Found Key
      computed = values[loopA];      
      measuredString = dict[keys[loopA]][0];
      if(measuredString.compare("none") != 0){
        try{
          measured = atof(measuredString.c_str());
          weight = weigths[loopA];
          fprintf(fp,"%30s %15.3f %15.3f %15.3f\n",keys[loopA].c_str(),measured,computed,weight);
        }catch(...){
        }
      }
    }
  }
  fprintf(fp,"\n");
  fclose(fp);
}

int daData_single_GPatient::getPatientValue(string key,double &result){
  int retVal = 0;
  if(dict.find(key) != dict.end()){
    try{
      result = atof(dict[key][0].c_str());
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

// Show Available Keys for a given timeStamp
void daData_single_GPatient::getAvailableKeys(stdStringVec& foundKeys){
  foundKeys.clear();
  typedef map<string,stdStringVec>::iterator it_type;
  string currKey;
  string currTimeStamp;
  int count = 0;
  for(it_type iterator = dict.begin(); iterator != dict.end(); iterator++) {
    count++;
    // Get Key
    currKey = iterator->first;
    // Add Key if you have the same time stamp
    foundKeys.push_back(currKey);
  }
}

void daData_single_GPatient::printToScreen(){
  printf("%30s %15s\n","Key", "Value");
  typedef map<string,stdStringVec>::iterator it_type;
  string currKey;
  string currTimeStamp;
  int count = 0;
  for(it_type iterator = dict.begin(); iterator != dict.end(); iterator++){
    count++;
    // Get Couple Key,Value
    printf("%30s %15.3e\n",iterator->first.c_str(), atof(iterator->second[0].c_str()));
  }
}
