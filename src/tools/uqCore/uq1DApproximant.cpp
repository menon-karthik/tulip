#include "uq1DApproximant.h"

uq1DApproximant::uq1DApproximant(){
}

uq1DApproximant::~uq1DApproximant(){
}

void uq1DApproximant::evaluate(const stdVec& XValue,stdVec& results){
  results.clear();
  for(int loopA=0;loopA<XValue.size();loopA++){
    results.push_back(evaluate(XValue[loopA]));
  }	
}
