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

double genOneSample(const stdVec& bins,const stdVec& df,uqPDF* uSampler){

  // Generate random integer
  int count = 0;  
  double u = uSampler->sample(0.0,1.0);
  if(u <= df[0]){
    return bins[0];
  }else{
    int count = 1;
    bool found = false;
    while((!found)&&(count<df.size())){
      found = ((u > df[count-1])&&(u <= df[count]));
      if(!found){
        count++;
      }        
    }
    return bins[count];
  }
}

stdVec genSampleFromPMF(const stdVec& bins,const stdVec& pmf,int n,uqPDF* uSampler){
  // Compute df from pmf
  stdVec res;
  stdVec df;
  double cumSum = 0.0;
  for(int loopA=0;loopA<pmf.size();loopA++){
    cumSum += pmf[loopA];
    df.push_back(cumSum);
  }

  // Check PMF is valid
  if(fabs(cumSum-1.0) > 1.0e-3){
    throw ntException("ERROR: PMF is not valid in ntUtils::genSampleFromPMF."); 
  }

  double oneSample = 0.0;
  for(int loopA=0;loopA<n;loopA++){
    oneSample = genOneSample(bins,df,uSampler);
    res.push_back(oneSample);
  }
  return res;
}

}

