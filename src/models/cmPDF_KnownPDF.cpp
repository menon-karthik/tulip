
# include "cmPDF_KnownPDF.h"

using namespace std;

cmPDFKnownPDF::cmPDFKnownPDF(int PDFType){
  this->PDFType = PDFType;
}

int cmPDFKnownPDF::getParameterTotal(){
  return 1;
}

int cmPDFKnownPDF::getResultTotal(){
  return 1;
}

void cmPDFKnownPDF::getDefaultParams(stdVec& params){

  // Resize Parameter Array
  params.resize(getParameterTotal());
  params[0] = 0.0;
}

void cmPDFKnownPDF::getDefaultParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  limits[0] = -10.0; limits[1] = 10.0;
}

double cmPDFKnownPDF::evalModelError(const stdVec& inputs,stdVec& outputs,stdIntVec& errorCode){

  // Get input values
  double currParam = inputs[0];
  double result = 0.0;

  if(PDFType == stdGaussianPDF){
    result = -0.5*(currParam - 0.0)*(currParam - 0.0)/1.0;
  }else if(PDFType == bimodalPDF){
    // Proportional to a bimodal distribution
    result = log(0.6*(1.0/sqrt(2.0 * M_PI * 2.0))*exp(-0.5*(currParam - 0.0)*(currParam - 0.0)/2.0) + 0.4*(1.0/sqrt(2.0 * M_PI * 1.0))*exp(-0.5*(currParam - 5.0)*(currParam - 5.0)/1.0));
  }else if(PDFType == uniformPDF){
    result = -1.0;
  }else if(PDFType == multimodalPDF){
    // To Finish
    result = -1.0;
  }

  // Store Error Code
  errorCode.push_back(0);

  // Return 
  return -1.0*result;
}
