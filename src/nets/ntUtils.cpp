# include "ntUtils.h"

namespace ntUtils{

stdMat copySamples(const stdMat& samples){
  stdMat copy;
  stdVec tmp;
  for(int loopA=0;loopA<samples.size();loopA++){
  	tmp.clear();
    for(int loopB=0;loopB<samples[loopA].size();loopB++){
      tmp.push_back(samples[loopA][loopB]);
    }
    copy.push_back(tmp);
  }
  return copy;
}

void appendSamples(const stdMat& msg,stdMat& marginal){
  stdVec tmp;
  for(int loopA=0;loopA<msg.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<msg[0].size();loopB++){
      tmp.push_back(msg[loopA][loopB]);
    }
    marginal.push_back(tmp);
  }
}

bool sameStringVec(const stdStringVec& v1,const stdStringVec& v2){
  if(v1.size() != v2.size()){
    return false;
  }
  bool same = true;
  for(int loopA=0;loopA<v1.size();loopA++){
    same = same && (v1[loopA] == v2[loopA]);
  }
  return same;
}
  
bool sameVec(const stdVec& v1,const stdVec& v2){
  if(v1.size() != v2.size()){
    return false;
  }
  bool same = true;
  for(int loopA=0;loopA<v1.size();loopA++){
    same = same && (fabs(v1[loopA]-v2[loopA])<1.0e-8);
  }
  return same;
}

}

