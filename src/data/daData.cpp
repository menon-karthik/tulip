
# include "daData.h"

using namespace std;

daData::daData(){

}

daData::~daData(){

}

void daData::clear(){
  dict.clear();
}

void daData::extractKeyValues(const stdStringVec& refKeys,const stdBoolVec& unUsedKeys,stdStringVec& outKeys,stdVec& outValues){
  outKeys.clear();
  outValues.clear();
  for(int loopA=0;loopA<refKeys.size();loopA++){
    if((dict.find(refKeys[loopA]) != dict.end())&&(!unUsedKeys[loopA])){
      // Found Key
      outKeys.push_back(refKeys[loopA]);
      // Carefull: Only first Patient
      outValues.push_back(atof(dict[refKeys[loopA]][0].c_str()));
    }
  }
}

void daData::addKeyValue(string key,double value){
  ostringstream sstream;
  sstream << value;
  string valueAsString(sstream.str());
  stdStringVec valuesAsString;
  valuesAsString.push_back(valueAsString);
  dict[key] = valuesAsString;
}

void daData::removeKeyValue(string key){
  for(dataMap::iterator it = dict.begin(); it!=dict.end();){
    if(it->first == key){
      dict.erase(it);
    }else{
      it++;	
    }
  }
}

// Print Dictionary to stdout
void daData::printToScreen(int patientIndex){
  printf("---\n");
  printf("%30s %30s\n","key","value");
  for(dataMap::iterator it = dict.begin(); it!=dict.end();it++){
  	printf("%30s %30f\n",it->first.c_str(),atof(it->second[patientIndex].c_str()));
  }
  printf("---\n");
}