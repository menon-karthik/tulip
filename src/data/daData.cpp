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
      if(dict[refKeys[loopA]][0].compare("none") != 0){
        // Found Key
        outKeys.push_back(refKeys[loopA]);
        // Carefull: Only first Patient
        outValues.push_back(atof(dict[refKeys[loopA]][0].c_str()));
      }
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
  this->measured.push_back(value);
}

void daData::removeKeyValue(string key){
  dataMap::iterator it;
  it=dict.find(key);
  if(it != dict.end()){
    // Remove the element pointed by iterator
    dict.erase(it);
  }
}

void daData::addCovariance(std::vector<std::vector<double>> covariance_mat_inv, double determinant) {
  this->cov_det = determinant;
  int dim;
  dim = covariance_mat_inv.size();
  this->cov_inv.resize(dim);
  for (int i=0; i<dim; i++) {
    this->cov_inv[i].resize(dim);
    for (int j=0; j<dim; j++) {
      this->cov_inv[i][j] = covariance_mat_inv[i][j];
    }
  }
}

double daData::evalLikelihood(const stdStringVec& keys,const stdVec& avValues,const stdVec& stdFactors,const stdVec& weights){
  double ll = evalLogLikelihood(keys,avValues,stdFactors,weights);
  return exp(-ll);  
}

void daData::printUserSTDs(){
  printf("### User-defined Standard Deviations\n");
  typedef doubleMap::iterator it_type;
  for(it_type iterator = userStdDict.begin(); iterator != userStdDict.end(); iterator++) {
    printf("Key %s, Value %f\n",iterator->first.c_str(),iterator->second);
  }
  printf("###\n");
}
