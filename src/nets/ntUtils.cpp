# include "ntUtils.h"

namespace ntUtils{

void aggregateMsgs(stdMat& Msg,const stdMat& addMsg){
  // Check the number of samples to be the same 
  // otherwise merging is not possible
  if(Msg.size() != addMsg.size()){
    throw ntException("ERROR: Incompatible messages in ntUtils::aggregateMsgs.");
  }
  // Since these are different variables add to the dimensionality
  for(int loopA=0;loopA<Msg.size();loopA++){
    for(int loopB=0;loopB<addMsg[loopA].size();loopB++){
      Msg[loopA].push_back(addMsg[loopA][loopB]);
    }
  }
}

stdMat copySamples(const stdMat& samples){
  stdMat copy;
  stdVec tmp;
  for(int loopA=0;loopA<samples.size();loopA++){
    for(int loopB=0;loopB<samples[loopA].size();loopB++){
      tmp.push_back(samples[loopA][loopB]);
    }
    copy.push_back(tmp);
  }
  return copy;
}

}

