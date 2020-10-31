#ifndef NTUTILS_H
#define NTUTILS_H

# include <math.h>

# include "ntException.h"
# include "uqTypes.h"

namespace ntUtils{

  stdMat copySamples(const stdMat& samples);

  void appendSamples(const stdMat& msg,stdMat& marginal);

  bool sameStringVec(const stdStringVec& v1,const stdStringVec& v2);

  bool sameVec(const stdVec& v1,const stdVec& v2);  
}

#endif // NTUTILS_H
