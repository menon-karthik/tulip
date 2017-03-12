#ifndef UQMWTESTING_H
#define UQMWTESTING_H

#include "uqPolyInterpolator.h"
#include "uqPDF.h"

namespace mwTesting
{

// -------------------------
// Perform Multiwavelet TEST
// -------------------------
void PerformMWTest(std::string fileName, int mwOrder, int mwIntLevel){
  
  // Open Output File
	FILE* outFile;
	outFile = fopen(fileName.c_str(),"w");

  // Allocate Product Matrix}
  double** monomMat = new double*[mwOrder];
  double** crossMat = new double*[mwOrder];
  for(int loopA=0;loopA<mwOrder;loopA++){
    monomMat[loopA] = new double[2*mwOrder];
    crossMat[loopA] = new double[mwOrder];
  }

  // Initialize
  for(int loopA=0;loopA<mwOrder;loopA++){
    for(int loopB=0;loopB<(2*mwOrder);loopB++){
      monomMat[loopA][loopB] = 0.0;
    }
    for(int loopB=0;loopB<mwOrder;loopB++){
      crossMat[loopA][loopB] = 0.0;
    }    
  }
  
  // Eval Integration Points
  uqClenshawCurtis1DQuadratureRuleDouble* quadRule = new uqClenshawCurtis1DQuadratureRuleDouble(mwIntLevel,kHaarRange);
  quadRule->generatePointsAndWeights();
  int totalPoints = quadRule->getTotalPoints();
  double* intPoints = quadRule->getPoints();
  double* intWeights = quadRule->getWeights();
  
  // USE A Different Probability Measure
  // Initialzie Measure: Truncated Gaussian PDF
  uqRescaledTruncatedGaussianPDF* pdf = new uqRescaledTruncatedGaussianPDF(6.0);
  double measure[totalPoints];
  for(int loopA=0;loopA<totalPoints;loopA++){
    //measure[loopA] = pdf->eval(intPoints[loopA]);
    measure[loopA] = 1.0;
  }
  
  // Form Multiwavelet Matrix
  uqMWInterpolator* mw = new uqMWInterpolator(mwOrder,mwIntLevel,measure);
  
  // Form Monomial Interpolator
  uqPolyInterpolator* poly = new uqPolyInterpolator(kPolyMonomials,mwIntLevel);
    
  // ------------------------
  // PERFORM INTEGRATION TEST
  // ------------------------
  double intValue = 0.0;
  double currentPoint = 0.0;
  double currentWeight = 0.0;
  double currentValue = 0.0;
  for(int loopC=0;loopC<totalPoints;loopC++){
    currentPoint = intPoints[loopC];
    currentWeight = intWeights[loopC];
    currentValue = currentPoint;
    // Form Matrix
    intValue = intValue + currentValue * currentWeight;
  }
  
  // Print Test Result Value
  fprintf(outFile,"INTEGRAL TEST\n");
  fprintf(outFile,"Current Integral: %e\n",intValue);

  // Perform Products with Monomials to Check Moments
  double mwValue = 0.0;
  double polyValue = 0.0;
  for(int loopA=0;loopA<mwOrder;loopA++){
    for(int loopB=0;loopB<(loopA+mwOrder);loopB++){
      monomMat[loopA][loopB] = 0.0;
      for(int loopC=0;loopC<totalPoints;loopC++){
        currentPoint = intPoints[loopC];
        currentWeight = intWeights[loopC];
        // First Polynomial
        mwValue = mw->EvalMW(currentPoint,0,0,loopA);
        // Second Polynomial
        polyValue = poly->eval(currentPoint,loopB);
        // Form Matrix
        monomMat[loopA][loopB] += mwValue * polyValue * measure[loopC] * currentWeight;
      }
    }
  }
  
  // Print Monomial Test Results
  fprintf(outFile,"\n");
  fprintf(outFile,"MONOMIAL TEST\n");
  for(int loopA=0;loopA<mwOrder;loopA++){
    for(int loopB=0;loopB<(loopA+mwOrder);loopB++){
      fprintf(outFile,"%e ",monomMat[loopA][loopB]);
    }
    fprintf(outFile,"\n");
  }
  
  // Perform Cross Products
  double firstMWValue = 0.0;
  double secondMWValue = 0.0;
  for(int loopA=0;loopA<mwOrder;loopA++){
    for(int loopB=0;loopB<mwOrder;loopB++){
      for(int loopC=0;loopC<totalPoints;loopC++){
        currentPoint = intPoints[loopC];
        currentWeight = intWeights[loopC];
        // First Polynomial
        firstMWValue = mw->EvalMW(currentPoint,0,0,loopA);
        // Second Polynomial
        secondMWValue = mw->EvalMW(currentPoint,1,1,loopB);
        // Form Matrix
        crossMat[loopA][loopB] += firstMWValue * secondMWValue * measure[loopC] * currentWeight;
      }
    }
  }
  
  // Print Monomial Test Results
  fprintf(outFile,"\n");
  fprintf(outFile,"CROSS PRODUCT TEST\n");
  for(int loopA=0;loopA<mwOrder;loopA++){
    for(int loopB=0;loopB<mwOrder;loopB++){
      fprintf(outFile,"%e ",crossMat[loopA][loopB]);
    }
    fprintf(outFile,"\n");
  }

  // Allocate Product Matrix}
  for(int loopA=0;loopA<mwOrder;loopA++){
    delete [] monomMat[loopA];
    delete [] crossMat[loopA];
  }
  delete [] monomMat;
  delete [] crossMat; 
  
  // Close Output file
	fclose(outFile);	 
}

}

#endif //UQMWTESTING_H