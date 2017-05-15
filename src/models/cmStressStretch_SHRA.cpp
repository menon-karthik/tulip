# include "cmStressStretch_SHRA.h"

using namespace std;

cmStressStretch_SHRA::cmStressStretch_SHRA(){
}

cmStressStretch_SHRA::~cmStressStretch_SHRA(){
}

int cmStressStretch_SHRA::getParameterTotal(){

}
int cmStressStretch_SHRA::getStateTotal(){

}
int cmStressStretch_SHRA::getResultTotal(){

}
void cmStressStretch_SHRA::getParameterLimits(stdVec& limits){

}
void cmStressStretch_SHRA::getDefaultParams(stdVec& params){

}
void cmStressStretch_SHRA::getPriorMapping(int priorModelType,int* prPtr){

}
string cmStressStretch_SHRA::getParamName(int parID){

}
string cmStressStretch_SHRA::getResultName(int resID){

}
double cmStressStretch_SHRA::evalModelError(stdVec inputs,stdVec& outputs, stdIntVec& errorCode){
// function [tzz_th_SHR_a] = stress_SHR_a(x_SHR_a)
// global Cqq Czz I4_a y_a l2_a beta_a ki_a
// =====================================================================================
// NOTE: the parameters are collected in x_SHR_a in the following order for SHR_a SEF
//   inputs[0] = (1-phi)*mu         Ground state shear modulus of ligament/tendon matrix
//   inputs[1] = phi*E              Fibril Young's modulus
//   inputs[2] = theta0             Crimp angle of outermost fibrils
//   inputs[3] = psi                Fascicle helix angle
// =====================================================================================                                                    

// Pre-allocation
ki_a          = vector<>(length(Czz),0.0);
tzz_th_SHR_a  = vector<>(length(Czz),0.0);

// Right Cauchy-Green Strain Tensor 4th Invariant
I4_a    = Cqq.*(sin(x_SHR_a(4)))^2 + Czz.*(cos(x_SHR_a(4)))^2;
y_a     = I4_a;                                    
// (lambda*)^2, transition stretch^2
l2_a    = (cos(x_SHR_a(3)))^-2;
    
for i=1:length(Czz);
        if I4_a(i) < 1          
%       Compression
            ki_a(i)       =     0;                     
        elseif I4_a(i) <= l2_a && I4_a(i) >= 1
%       Toe Region
            ki_a(i)       =     (x_SHR_a(2)/(6*sqrt(I4_a(i))*(sin(x_SHR_a(3)))^2))*(2-(3/sqrt(y_a(i)))+(y_a(i))^-1.5);             
        elseif I4_a(i) > l2_a
%       Linear Region
            beta_a(i)     =     2*(1-(cos(x_SHR_a(3)))^3)/(3*(sin(x_SHR_a(3)))^2);
            ki_a(i)       =     (x_SHR_a(2)/(2*sqrt(I4_a(i))))*(beta_a(i)-(1/sqrt(y_a(i))));                        
        end
        tzz_th_SHR_a(i)   =     x_SHR_a(1)*(Czz(i)-Cqq(i)) + 2*ki_a(i)*(Czz(i)*(cos(x_SHR_a(4)))^2- 0.5*Cqq(i)*(sin(x_SHR_a(4)))^2);
end



}



