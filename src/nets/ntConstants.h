#ifndef NTCONSTANTS_H
#define NTCONSTANTS_H

enum nodeDependenceTypes{ntRoot, ntDeterministic, ntProbabilistic};
enum nodeSupportTypes{stDiscrete,stContinuous};
enum nodePDFTypes{pdfUnknown,pdfUniform,pdfGaussian};
enum nodePMFTypes{pmfUnknown,pmfUniform,pmfPoisson};
// Model Type
enum modelTypes{mtModel,mtApproximant};

#endif // NTCONSTANTS_H

