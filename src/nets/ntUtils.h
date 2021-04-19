#ifndef NTUTILS_H
#define NTUTILS_H

# include <math.h>

# include "ntException.h"
# include "uqTypes.h"
# include "uqPDF.h"

namespace ntUtils{

  stdMat copySamples(const stdMat& samples);

  void appendSamples(const stdMat& msg,stdMat& marginal);

  bool sameStringVec(const stdStringVec& v1,const stdStringVec& v2);

  bool sameVec(const stdVec& v1,const stdVec& v2);  

  stdVec genSampleFromPMF(const stdVec& bins,const stdVec& pmf,int n,uqPDF* uSampler);
}

#endif // NTUTILS_H
