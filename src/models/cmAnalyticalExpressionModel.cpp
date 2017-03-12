# include <string>
# include <math.h>

# include "cmException.h"
# include "cmAnalyticalExpressionModel.h"
# include "uqTypes.h"

using namespace std;

// Constructor
cmAnalyticalExpressionModel::cmAnalyticalExpressionModel(int localModelType){
  modelType = localModelType;
}

// Get the number of parameters
int cmAnalyticalExpressionModel::getParameterTotal(){
  int result = 0;
  switch(modelType){
    case kAnalyticalModelUnity2D:
      result = 2;
      break;
    case kAnalyticalModelLinear2D:
      result = 2;
      break;
    case kModelDiscAgarwal:
      result = 2;
      break;
    case kModelZabarasSP:
      result = 2;
      break;
    case kAnalyticalPiecewiseLinear1D:
      result = 1;
      break;
    case kAnalyticalConstant:
      result = -1;
      break;
    case kAnalyticalLinear:
      result = -1;
      break;
  }
  // Return
  return result;
}

// Get Parameter Names
string cmAnalyticalExpressionModel::getParamName(int parID){
  return string("");
}

int cmAnalyticalExpressionModel::getStateTotal(){
  return 0;
}
int cmAnalyticalExpressionModel::getResultTotal(){
  return 1;
}

// Get parameter Limits
void cmAnalyticalExpressionModel::getParameterLimits(stdVec& limits){
  limits.resize(2*getParameterTotal());
  if(modelType == kAnalyticalModelUnity2D){
    limits[0] = 0.0; limits[1] = 1.0;
    limits[2] = 0.0; limits[3] = 1.0;
  }else if(modelType == kAnalyticalModelLinear2D){
    limits[0] = 0.0; limits[1] = 1.0;
    limits[2] = 0.0; limits[3] = 1.0;
  }else if(modelType == kModelDiscAgarwal){
    limits[0] = 0.0; limits[1] = 1.0;
    limits[2] = 0.0; limits[3] = 1.0;
  }else if(modelType == kModelZabarasSP){
    limits[0] = 0.0; limits[1] = 1.0;
    limits[2] = 0.0; limits[3] = 1.0;
  }else if(modelType == kAnalyticalPiecewiseLinear1D){
    limits[0] = 0.0; limits[1] = 1.0;
  }else{
    throw new cmException("Error: Invalid Model Type");
  }
}

// Get Default parameters
void cmAnalyticalExpressionModel::getDefaultParams(stdVec& params){
  params.resize(getParameterTotal());
  if(modelType == kAnalyticalModelUnity2D){
    params[0] = 0.5;
    params[1] = 0.5;
  }else if(modelType == kAnalyticalModelLinear2D){
    params[0] = 0.5;
    params[1] = 0.5;
  }else if(modelType == kModelDiscAgarwal){
    params[0] = 0.5;
    params[1] = 0.5;
  }else if(modelType == kModelZabarasSP){
    params[0] = 0.5;
    params[1] = 0.5;    
  }else if(modelType == kAnalyticalPiecewiseLinear1D){
    params[0] = 0.5;
  }else{
    throw new cmException("Error: Invalid Model Type");
  }
}

// Inverse Gaussian CDF from: http://www.johndcook.com/blog/normal_cdf_inverse/
double RationalApproximation(double t){
  // Abramowitz and Stegun formula 26.2.23.
  // The absolute value of the error should be less than 4.5 e-4.
  double c[] = {2.515517, 0.802853, 0.010328};
  double d[] = {1.432788, 0.189269, 0.001308};
  return t - ((c[2]*t + c[1])*t + c[0]) /
             (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}
 // Inverse Gaussian CDF from: http://www.johndcook.com/blog/normal_cdf_inverse/
double NormalCDFInverse(double p){
  if(p<=0.0){
    p=1.0e-12;
  } 
  if(p>=1.0){
    p=1.0-1.0e-12;
  } 
  // See article above for explanation of this section.
  if (p < 0.5){
    // F^-1(p) = - G^-1(p)
    return -RationalApproximation( sqrt(-2.0*log(p)) );
  }else{
    // F^-1(p) = G^-1(1-p)
    return RationalApproximation( sqrt(-2.0*log(1-p)) );
  }
}

// Get prior mapping
void cmAnalyticalExpressionModel::getPriorMapping(int priorModelType,int* prPtr){
}
    
// Evaluate Model
double cmAnalyticalExpressionModel::evalModelError(stdVec inputs, stdVec& outputs, stdIntVec& errorCode){
  outputs.clear();
  errorCode.clear();
  if(modelType == kAnalyticalModelUnity2D){
    // No Error
    errorCode.push_back(0);
    // Evaluate Function
    outputs.push_back(1.0);
    return 0.0;
  }else if(modelType == kAnalyticalModelLinear2D){
    // No Error
    errorCode.push_back(0);
    // Evaluate Function
    outputs.push_back(inputs[0]*inputs[1]);
    return 0.0;
  }else if(modelType == kModelDiscAgarwal){
    // No Error
    errorCode.push_back(0);
    // Evaluate Function
    double functionVal = 0.0;
    if((inputs[0]<=0.5)&&(inputs[1]<=0.5)){
      functionVal = sin(M_PI*inputs[0])*sin(M_PI*inputs[1]);
    }else{
      functionVal = 0.0;
    }
    // Return Value
    outputs.push_back(functionVal);
    return 0.0;
  }else if(modelType == kModelZabarasSP){
    errorCode.push_back(0);
    double functionVal = 0.0;
    functionVal = 1.0/(fabs(0.3-inputs[0]*inputs[0]-inputs[1]*inputs[1]) + 0.1);
    //functionVal = sin(2.0 * M_PI * inputs[0] * 2.0) * sin(2.0 * M_PI * inputs[1] * 2.0);
    //printf("x1 %f, x2, %f, y %f\n",inputs[0],inputs[1],functionVal);
    // Return Value

    // Simple Biliear Function
    //if(inputs[0]<0.5){
    //  functionVal = 10.0 * inputs[0];
    //}else{
    //  functionVal = 10.0 - 10.0 * inputs[0];
    //}
    
    outputs.push_back(functionVal);
    return 0.0; 
  }else if(modelType == kAnalyticalPiecewiseLinear1D){
    errorCode.push_back(0);
    double functionVal = 0.0;
    //if(inputs[0] < 0.35){
      //functionVal = 1.0 + 0.5*sin(2.0 * M_PI * 10.0 * inputs[0]);  
    //}else{
      functionVal = 2.0 + 0.3*sin(2.0 * M_PI * 3.0 * inputs[0]);  
    //}    
    outputs.push_back(functionVal);
    return 0.0; 
  }else if(modelType == kAnalyticalConstant){
    errorCode.push_back(0);
    outputs.push_back(0.3);
    return 0.0;
  }else if(modelType == kAnalyticalLinear){
    errorCode.push_back(0);
    double result = 0.0;
    for(int loopA=0;loopA<inputs.size();loopA++){
      result += inputs[loopA];
    }
    outputs.push_back(result);
    return 0.0;
  }else if(modelType == kAnalyticalQuadratic){
    errorCode.push_back(0);
    double result = 0.0;
    for(int loopA=0;loopA<inputs.size();loopA++){
      result += inputs[loopA] * inputs[loopA];
    }
    outputs.push_back(result);
    return 0.0;
  }else if(modelType == kIdentity){
    errorCode.push_back(0);
    for(int loopA=0;loopA<inputs.size();loopA++){
      outputs.push_back(inputs[loopA]);
    }
    return 0.0;      
  }else if(modelType == kTransformedGaussian){
    if(inputs.size() > 1){
      throw new cmException("Error: Gaussian CDF transformation available in 1D only\n");
    }else{
      errorCode.push_back(0);
      for(int loopA=0;loopA<inputs.size();loopA++){
        outputs.push_back(NormalCDFInverse(inputs[loopA]));
      }
    }
    return 0.0;    
  }else{
    throw new cmException("Error: Invalid Model Type");
  }

}

// Get Name of result quantity
string cmAnalyticalExpressionModel::getResultName(int resID){
  return string("qty");
}


