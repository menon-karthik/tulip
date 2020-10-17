#ifndef NTUTILS_H
#define NTUTILS_H

# include "ntException.h"
# include "uqTypes.h"

namespace ntUtils{

void aggregateMsgs(stdMat& Msg,const stdMat& addMsg);

stdMat copySamples(const stdMat& samples);
  
}

#endif // NTUTILS_H
