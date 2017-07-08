
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

double daData::evalLikelihood(stdStringVec keys,stdVec avValues,stdVec stdFactors,stdVec weights){
  double ll = evalLogLikelihood(keys,avValues,stdFactors,weights);
  return log(ll);  
}

