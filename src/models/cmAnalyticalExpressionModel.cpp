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
    case kPointAndCircle:
      result = 2;
      break;
    case kAlphaCurve:
      result = 2;
      break;
    case kKuramoto:
      result = 2;
      break;
    case kWhitneyUmbrella:
      result = 3;
      break;
    case kPosdim:
      result = 3;
      break;
    case kSeller:
      result = 3;
      break;
    case kEnergyLandscape:
      result = 9;
      break;
    case kKuramotoReform:
      result = 4;
      break;
    case kEnergyLandscapeopt:
      result = 9;
      break;
  }
  // Return
  return result;
}

// Get Parameter Names
string cmAnalyticalExpressionModel::getParamName(int parID){
  if(parID<getParameterTotal()){
    return string("Par" + to_string(parID));  
  }else{
    throw new cmException("ERROR: Invalid Parameter ID");
  }
}

int cmAnalyticalExpressionModel::getStateTotal(){
  return 0;
}
int cmAnalyticalExpressionModel::getResultTotal(){
  return 1;
}

// Get parameter Limits
void cmAnalyticalExpressionModel::getDefaultParameterLimits(stdVec& limits){
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
  }else if(modelType == kPointAndCircle){
    limits[0] = -3.0; limits[1] = 3.0;
    limits[2] = -3.0; limits[3] = 3.0;
    //limits[4] = 0.1; limits[5] = 0.5;
  }else if(modelType == kAlphaCurve){
    limits[0] = -3.0; limits[1] = 3.0;
    limits[2] = -3.0; limits[3] = 3.0;
    //limits[4] = 0.1; limits[5] = 0.5;  
  }else if(modelType == kKuramoto){
    limits[0] = 0.0; limits[1] = 2.0*M_PI;
    limits[2] = 0.0; limits[3] = 2.0*M_PI;
   // limits[4] = 0.1; limits[5] = 0.5;
  }else if(modelType == kWhitneyUmbrella){
    limits[0] = -10; limits[1] = 10;
    limits[2] = -10; limits[3] = 10;
    limits[4] = -2; limits[5] =  2;
   // limits[6] = 0.1; limits[7] = 0.5;
  }else if(modelType == kPosdim){
    limits[0] = -10; limits[1] = 10;
    limits[2] = -10; limits[3] = 10;
    limits[4] = -10; limits[5] = 10;
   // limits[6] = 0.1; limits[7] = 0.5;
  }else if(modelType == kSeller){
    limits[0] = -10; limits[1] = 10;
    limits[2] = -10; limits[3] = 10;
    limits[4] = -10; limits[5] = 10;
   // limits[6] = 0.1; limits[7] = 0.5;
  }else if(modelType == kEnergyLandscape){
    limits[0] = -10; limits[1] = 10;
    limits[2] = -10; limits[3] = 10;
    limits[4] = -10; limits[5] =  10;
    limits[6] = -10; limits[7] = 10;
    limits[8] = -10; limits[9] = 10;
    limits[10] = -10; limits[11] =  10;
    limits[12] = -10; limits[13] = 10;
    limits[14] = -10; limits[15] = 10;
    limits[16] = -10; limits[17] =  10;
   // limits[18] = 0.1; limits[19] = 0.5;
  }else if(modelType == kKuramotoReform){
    limits[0] = -1; limits[1] = 1;
    limits[2] = -1; limits[3] = 1;
    limits[4] = -1; limits[5] = 1;
    limits[6] = -1; limits[7] = 1;
   // limits[8] = 0.1; limits[9] = 0.5;
  }else if(modelType == kEnergyLandscapeopt){
    limits[0] = -10; limits[1] = 10;
    limits[2] = -10; limits[3] = 10;
    limits[4] = -10; limits[5] =  10;
    limits[6] = -10; limits[7] = 10;
    limits[8] = -10; limits[9] = 10;
    limits[10] = -10; limits[11] =  10;
    limits[12] = -10; limits[13] = 10;
    limits[14] = -10; limits[15] = 10;
    limits[16] = -10; limits[17] =  10;
   // limits[18] = 0.1; limits[19] = 0.5;
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
  }else if(modelType == kPointAndCircle){
    params[0] = 0.0;
    params[1] = 0.0;
   // params[2] = 0.5;    
  }else if(modelType == kAlphaCurve){
    params[0] = 0.0;
    params[1] = 0.0;  
   // params[2] = 0.5;   
  }else if(modelType == kKuramoto){
    params[0] = M_PI;
    params[1] = M_PI;
   // params[2] = 0.5; 
  }else if(modelType == kWhitneyUmbrella){
    params[0] = 0;
    params[1] = 0;
    params[2] = -1;
   // params[3] = 0.5; 
  }else if(modelType == kPosdim){
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    //params[3] = 0.5; 
  }else if(modelType == kSeller){
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
   // params[3] = 0.5; 
  }else if(modelType == kEnergyLandscape){
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    params[5] = 0;
    params[6] = 0;
    params[7] = 0;
    params[8] = 0;
   // params[9] = 0.5;
  }else if(modelType == kEnergyLandscapeopt){
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
    params[4] = 0;
    params[5] = 0;
    params[6] = 0;
    params[7] = 0;
    params[8] = 0;
   // params[9] = 0.5;
  }else if(modelType == kKuramotoReform){
    params[0] = 0;
    params[1] = 0;
    params[2] = 0;
    params[3] = 0;
   // params[4] = 0.5;
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
    // F^-1(p) = G^-1(1-p)
    // F^-1(p) = - G^-1(p)
    return -RationalApproximation( sqrt(-2.0*log(p)) );
  }else{
    return RationalApproximation( sqrt(-2.0*log(1-p)) );
  }
}

// Get prior mapping
void cmAnalyticalExpressionModel::getPriorMapping(int priorModelType,int* prPtr){
  throw new cmException("Error: getPriorMapping not Implemented!");
}
    
// Evaluate Model
double cmAnalyticalExpressionModel::evalModelError(const stdVec& inputs, stdVec& outputs, stdIntVec& errorCode){
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
    //outputs.push_back(inputs[0]);
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
  }else if(modelType == kPointAndCircle){
    double res = pow(50*(fabs(inputs[1]*inputs[1] + inputs[0]*inputs[0]*(inputs[0]-1.0)*(inputs[0]-2.0))),5);
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
  }else if(modelType == kAlphaCurve){
    double res = pow(50*(fabs(inputs[1]*inputs[1] - inputs[0]*inputs[0]*(inputs[0]+1.0))),5);
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
  }else if(modelType == kKuramoto){
    double res = pow(50*(fabs(1.0/12.0 - 1.0/3.0*(sin(inputs[0]-inputs[1])+sin(inputs[0]-0.0))) + 
                     fabs(-1.0/12.0-1.0/3.0*(sin(inputs[1]-inputs[0])+sin(inputs[1]-0.0)))),3.0);
    //double res = pow(50*(fabs(1.0/12.0 - 1.0/3.0*(sin(inputs[0]-inputs[1])+sin(inputs[0]-0.0))) + 
    //                fabs(-1.0/12.0-1.0/3.0*(sin(inputs[1]-inputs[0])+sin(inputs[1]-0.0)))),inputs[2]);
   
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
  }else if(modelType == kWhitneyUmbrella){
    double res = pow(50*(fabs(inputs[0]*inputs[0]-inputs[2]*inputs[1]*inputs[1])),3.0);
    //double res = pow((fabs(inputs[0]*inputs[0]-inputs[2]*inputs[1]*inputs[1])),inputs[3]);
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
}else if(modelType == kPosdim){
    double res = pow(50*(fabs(inputs[0]*inputs[1]*inputs[2])+
                     fabs(inputs[2]*(inputs[2]*inputs[2]+inputs[1]*inputs[1]+inputs[0]*inputs[0]+inputs[1])) + 
                     fabs(inputs[1]*(inputs[1]+inputs[2]))),3.0);
    //double res = pow(50*(fabs(inputs[0]*inputs[1]*inputs[2])+
    //                 fabs(inputs[2]*(inputs[2]*inputs[2]+inputs[1]*inputs[1]+inputs[0]*inputs[0]+inputs[1])) + 
    //                 fabs(inputs[1]*(inputs[1]+inputs[2]))),inputs[3]);
    
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
}else if(modelType == kSeller){
    double res = pow(50*(fabs(inputs[2]*inputs[2]+inputs[1]*inputs[1]-inputs[0]*inputs[0])+
                     fabs(inputs[0]*inputs[2]+inputs[0]*inputs[1]-inputs[2])+
                     fabs(inputs[1]*inputs[2]+inputs[1]*inputs[1]+inputs[0]*inputs[0]-inputs[0])),3.0);
    
    //double res = pow(50*(fabs(inputs[2]*inputs[2]+inputs[1]*inputs[1]-inputs[0]*inputs[0])+
    //                 fabs(inputs[0]*inputs[2]+inputs[0]*inputs[1]-inputs[2])+
    //                 fabs(inputs[1]*inputs[2]+inputs[1]*inputs[1]+inputs[0]*inputs[0]-inputs[0])),inputs[3]);
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
}else if(modelType == kEnergyLandscape){
    double res = pow(50*(fabs(inputs[0]*inputs[0]*inputs[0]/10.0+2.0*inputs[0]-inputs[1]-inputs[2]-inputs[3]-inputs[6])+
                     fabs(inputs[1]*inputs[1]*inputs[1]/10.0+2.0*inputs[1]-inputs[0]-inputs[2]-inputs[4]-inputs[7])+
                     fabs(inputs[2]*inputs[2]*inputs[2]/10.0+2.0*inputs[2]-inputs[0]-inputs[1]-inputs[5]-inputs[8])+
                     fabs(inputs[3]*inputs[3]*inputs[3]/10.0+2.0*inputs[3]-inputs[0]-inputs[4]-inputs[5]-inputs[6])+
                     fabs(inputs[4]*inputs[4]*inputs[4]/10.0+2.0*inputs[4]-inputs[1]-inputs[3]-inputs[5]-inputs[7])+
                     fabs(inputs[5]*inputs[5]*inputs[5]/10.0+2.0*inputs[5]-inputs[2]-inputs[3]-inputs[4]-inputs[8])+
                     fabs(inputs[6]*inputs[6]*inputs[6]/10.0+2.0*inputs[6]-inputs[0]-inputs[3]-inputs[7]-inputs[8])+
                     fabs(inputs[7]*inputs[7]*inputs[7]/10.0+2.0*inputs[7]-inputs[1]-inputs[4]-inputs[6]-inputs[8])+
    //                 fabs(inputs[8]*inputs[8]*inputs[8]/10.0+2.0*inputs[8]-inputs[2]-inputs[5]-inputs[6]-inputs[7])),inputs[9]);
                     fabs(inputs[8]*inputs[8]*inputs[8]/10.0+2.0*inputs[8]-inputs[2]-inputs[5]-inputs[6]-inputs[7])),1.0/5.0);
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
}else if(modelType == kEnergyLandscapeopt){
    double res = pow(50*(fabs(inputs[0]*inputs[0]*inputs[0]/10.0+2.0*inputs[0]-inputs[1]-inputs[2]-inputs[3]-inputs[6])+
                     fabs(inputs[1]*inputs[1]*inputs[1]/10.0+2.0*inputs[1]-inputs[0]-inputs[2]-inputs[4]-inputs[7])+
                     fabs(inputs[2]*inputs[2]*inputs[2]/10.0+2.0*inputs[2]-inputs[0]-inputs[1]-inputs[5]-inputs[8])+
                     fabs(inputs[3]*inputs[3]*inputs[3]/10.0+2.0*inputs[3]-inputs[0]-inputs[4]-inputs[5]-inputs[6])+
                     fabs(inputs[4]*inputs[4]*inputs[4]/10.0+2.0*inputs[4]-inputs[1]-inputs[3]-inputs[5]-inputs[7])+
                     fabs(inputs[5]*inputs[5]*inputs[5]/10.0+2.0*inputs[5]-inputs[2]-inputs[3]-inputs[4]-inputs[8])+
                     fabs(inputs[6]*inputs[6]*inputs[6]/10.0+2.0*inputs[6]-inputs[0]-inputs[3]-inputs[7]-inputs[8])+
                     fabs(inputs[7]*inputs[7]*inputs[7]/10.0+2.0*inputs[7]-inputs[1]-inputs[4]-inputs[6]-inputs[8])+
    //                 fabs(inputs[8]*inputs[8]*inputs[8]/10.0+2.0*inputs[8]-inputs[2]-inputs[5]-inputs[6]-inputs[7])),inputs[9]);
                     fabs(inputs[8]*inputs[8]*inputs[8]/10.0+2.0*inputs[8]-inputs[2]-inputs[5]-inputs[6]-inputs[7])),5);
    outputs.push_back(res);
    errorCode.push_back(0);
    return res;
}else if(modelType == kKuramotoReform){
    double res = (pow(100*fabs(1.0/12.0-1.0/3.0*(inputs[0]*inputs[3]-inputs[1]*inputs[2]+inputs[0]))+
                     100*fabs(-1.0/12.0-1.0/3.0*(inputs[1]*inputs[2]-inputs[0]*inputs[3]+inputs[1]))+
                     100*fabs(1-inputs[0]*inputs[0]-inputs[2]*inputs[2])+
    //                 100*fabs(1-inputs[1]*inputs[1]-inputs[3]*inputs[3]),inputs[4]));
                     100*fabs(1-inputs[1]*inputs[1]-inputs[3]*inputs[3]),3));
    outputs.push_back(res);
    errorCode.push_back(0);
}
    else{
    throw new cmException("Error: Invalid Model Type");
  }
}

// Get Name of result quantity
string cmAnalyticalExpressionModel::getResultName(int resID){
  return string("qty");
}


