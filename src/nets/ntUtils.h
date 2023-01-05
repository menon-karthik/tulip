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

  bool isInStringVec(string name,const stdStringVec& vec);

  bool sameVec(const stdVec& v1,const stdVec& v2);  

  stdVec genSampleFromPMF(const stdVec& bins,const stdVec& pmf,int n,uqPDF* uSampler);

  double discretizeSample(double sample, int numSubdiv, double lb, double ub);

  int getSampleState(double sample, int numSubdiv, double lb, double ub);

  stdVec getBins(int numSubdiv, double lb, double ub);

  bool sameArray(const stdVec& v1,const stdVec& v2);

}

#endif // NTUTILS_H
