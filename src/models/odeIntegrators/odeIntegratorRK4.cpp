# include "odeIntegratorRK4.h"

using namespace std;

// Constructor
odeIntegratorRK4::odeIntegratorRK4(odeModel* odeModel):odeIntegrator(odeModel){
}

int odeIntegratorRK4::run(int totalSteps,double timeStep,stdVec iniVals,stdVec params,stdMat& outVals,stdMat& auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = 0;
  bool printIC = false;
  bool isValidDouble = true;

  // Get Total number of States
  int totalStates = ode->getStateTotal();
  int totAuxStates = ode->getAuxStateTotal();

  // Allocate Temporary Variables
  stdVec k1;
  k1.resize(totalStates);
  stdVec k2;
  k2.resize(totalStates);
  stdVec k3;
  k3.resize(totalStates);
  stdVec k4;
  k4.resize(totalStates);
  stdVec k1AuxOut;
  k1AuxOut.resize(totAuxStates);
  stdVec k2AuxOut;
  k2AuxOut.resize(totAuxStates);
  stdVec k3AuxOut;
  k3AuxOut.resize(totAuxStates);
  stdVec k4AuxOut;
  k4AuxOut.resize(totAuxStates);
  stdVec Xk2;
  Xk2.resize(totalStates);
  stdVec Xk3;
  Xk3.resize(totalStates);
  stdVec Xk4;
  Xk4.resize(totalStates);
  stdVec Ind;
  Ind.resize(totalStates);

  // Initialize State Vectors and Copy Initial Conditions
  stdVec Xn;
  Xn.resize(totalStates);
  stdVec Xn1;
  Xn1.resize(totalStates);

  // Initialize Variables
  for(int loopA=0;loopA<totalStates;loopA++){
    k1[loopA] = 0.0;
    k2[loopA] = 0.0;
    k3[loopA] = 0.0;
    k4[loopA] = 0.0;

    Xk2[loopA] = 0.0;
    Xk3[loopA] = 0.0;
    Xk4[loopA] = 0.0;
    Ind[loopA] = 1.0;
    Xn[loopA] = 0.0;
    Xn1[loopA] = 0.0;
  }
  for(int loopA=0;loopA<totAuxStates;loopA++){
    k1AuxOut[loopA] = 0.0;
    k2AuxOut[loopA] = 0.0;
    k3AuxOut[loopA] = 0.0;
    k4AuxOut[loopA] = 0.0;
  }

  // Set Initial Conditions
  for(int loopA=0;loopA<ode->getStateTotal();loopA++){
    Xn[loopA] = iniVals[loopA];
  }

  // =========
  // TIME LOOP
  // =========
  for(int loopA=0;loopA<totalSteps;loopA++){

    // Increment Time Step
    stepId++;

    // Eval K1
    ode->eval(currTime,Xn,params,k1,k1AuxOut,Ind);

    // Eval K2
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk2[loopB] = Xn[loopB] + ((1.0/3.0)*timeStep) * k1[loopB];
    }
    ode->eval(currTime + (1.0/3.0) * timeStep,Xk2,params,k2,k2AuxOut,Ind);
    
    // Eval K3
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk3[loopB] = Xn[loopB] - (1.0/3.0)*timeStep * k1[loopB] + (1.0*timeStep) * k2[loopB];
    }
    ode->eval(currTime + (2.0/3.0) * timeStep,Xk3,params,k3,k3AuxOut,Ind);
    
    // Eval K4
    for(int loopB=0;loopB<totalStates;loopB++){
      Xk4[loopB] = Xn[loopB] + timeStep*k1[loopB] - timeStep*k2[loopB] + timeStep * k3[loopB];
    }
    ode->eval(currTime + timeStep,Xk4,params,k4,k4AuxOut,Ind);

    // Eval Xn1
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn1[loopB] = Xn[loopB] + (1.0/8.0)*timeStep*(k1[loopB] + 3.0 * k2[loopB] + 3.0 * k3[loopB] + k4[loopB]);
    }

    // CHECK IF SOLUTION IS VALID
    if((stepId % 1) == 0){
      for(int loopB=0;loopB<totalStates;loopB++){
        isValidDouble = (Xn1[loopB] != Xn1[loopB]);
        if(isValidDouble){
          // cout << "FOUND SOMETHING THAT IS NOT VALID AT " << loopB << "\n";
          return 1;
        }
      }
    }

    // Update Xn
    for(int loopB=0;loopB<totalStates;loopB++){
      Xn[loopB] = Xn1[loopB] * Ind[loopB];
    }    

    // Copy back k1AuxOut for each time step.
    for(int loopB=0;loopB<totAuxStates;loopB++){
      auxOutVals[loopB][stepId] = k4AuxOut[loopB];
    }

    // Copy The solution back for each time step
    for(int loopB=0;loopB<totalStates;loopB++){
      outVals[loopB][stepId] = Xn1[loopB];
    }  

    // Update Current Time
    currTime += timeStep;
  }
  
  // RETURN OK
  return 0;
}

/*
int odeCoronary::PerformRK4Steps(int modelID, int total3DSteps, int total0DSteps,
                                   double timeStep,int totalStates,int totAuxStates,double* iniVals,
                                   int totalParams, double* params,double** outVals,double** auxOutVals){

  // Intialize Current Time
  double currTime = 0.0;
  int stepId = -1;
  int stepId3D = -1;
  bool isValidDouble = true;

  // Allocate Temporary Variables
  double k1[totalStates];
  double k2[totalStates];
  double k3[totalStates];
  double k4[totalStates];
  double k1AuxOut[totAuxStates];
  double k2AuxOut[totAuxStates];
  double k3AuxOut[totAuxStates];
  double k4AuxOut[totAuxStates];
  double Xk2[totalStates];
  double Xk3[totalStates];
  double Xk4[totalStates];
  double Ind[nUnknowns];
  double P[nFaces];
  double Q[nFaces];

  // Initialize State Vectors and Copy Initial Conditions
  double Xn[totalStates];
  double Xn1[totalStates];

  // INITIALIZE HEART MODEL PARAMETERS
  Xn[0] = 82.93E0;
  Xn[1] = 81.5E0;
  Xn[2] = 148.55E0;
  Xn[3] = 0.0E0;
  Xn[4] = 1.71E0;
  Xn[5] = 77.093E0;
  Xn[6] = 11.31E0;
  Xn[7] = 83.2E0;
  Xn[8] = 0.0E0;
  Xn[9] = 93.9E0;
  // INITIALIZE RCR OUTLETS
  for(int loopA=ipRCRStart;loopA<ipRCRStart+nRCR;loopA++){
    Xn[loopA] = 100.0;
  }
  // INITIALIZE CORONARY FACES ATTACHED TO OUTLET
  Xn[16] = 93.0;  
  Xn[18] = 93.0;
  Xn[20] = 93.0;
  Xn[22] = 93.0;
  Xn[24] = 93.0;
  Xn[26] = 93.0;
  Xn[28] = 93.0;
  Xn[30] = 93.0;
  Xn[32] = 93.0;
  Xn[34] = 93.0;
  Xn[36] = 93.0;
  Xn[38] = 93.0;
  Xn[40] = 93.0;
  Xn[42] = 93.0;
  Xn[44] = 93.0;
  Xn[46] = 93.0;
  // INITIALIZE CORONARY OUTLET VARIABLES NOT ATTACHED TO OUTLET
  Xn[17] = 1.5;
  Xn[19] = 1.5;
  Xn[21] = 1.5;
  Xn[23] = 1.5;
  Xn[25] = 1.5;
  Xn[27] = 1.5;
  Xn[29] = 1.5;
  Xn[31] = 1.5;
  Xn[33] = 1.5;
  Xn[35] = 1.5;
  Xn[37] = 1.5;
  Xn[39] = 1.5;
  Xn[41] = 1.5;
  Xn[43] = 1.5;
  Xn[45] = 1.5;
  Xn[47] = 1.5;
  for(int loopA = 0; loopA < nFaces; loopA++) {
     P[loopA] = 0.0;
     Q[loopA] = 0.0;
  }

  // =============
  // 3D MODEL LOOP
  // =============
  for(int loop3D=0;loop3D<total3DSteps;loop3D++){

    //if((loop3D % 1000) == 0){
    //  printf("Iteration %d\n",loop3D);
    //}

    stepId3D++;


    // EVAL 3D MODEL
    evalSurrogate3DModel(P,Q,loop3D);   

    // Print Q
    //for(int loopQ=0;loopQ<nFaces;loopQ++){
      //printf("Q %d, %.3f\n",loopQ+1,Q[loopQ]);
    //} 
    //getchar();

    // =========
    // TIME LOOP
    // =========
    for(int loopA=0;loopA<total0DSteps;loopA++){

      // Increment Time Step
      stepId++;

      // Eval K1
      evalModel(modelID,currTime,totalStates,Xn,params,k1,k1AuxOut,Ind,Q,P);

      // Eval K2
      for(int loopB=0;loopB<totalStates;loopB++){
        Xk2[loopB] = Xn[loopB] + ((1.0/3.0)*timeStep) * k1[loopB];
      }
      evalModel(modelID,currTime + (1.0/3.0) * timeStep,totalStates,Xk2,params,k2,k2AuxOut,Ind,Q,P);

      // Eval K3
      for(int loopB=0;loopB<totalStates;loopB++){
        Xk3[loopB] = Xn[loopB] - (1.0/3.0)*timeStep * k1[loopB] + (1.0*timeStep) * k2[loopB];
      }
      evalModel(modelID,currTime + (2.0/3.0) * timeStep,totalStates,Xk3,params,k3,k3AuxOut,Ind,Q,P);

      // Eval K4
      for(int loopB=0;loopB<totalStates;loopB++){
        Xk4[loopB] = Xn[loopB] + timeStep*k1[loopB] - timeStep*k2[loopB] + timeStep * k3[loopB];
      }
      evalModel(modelID,currTime + timeStep,totalStates,Xk4,params,k4,k4AuxOut,Ind,Q,P);

      // Eval Xn1
      for(int loopB=0;loopB<totalStates;loopB++){
        if(Ind[loopB] > 0) {
           Xn1[loopB] = Xn[loopB] + (1.0/8.0)*timeStep*(k1[loopB] + 3.0 * k2[loopB] + 3.0 * k3[loopB] + k4[loopB]);
        }
        else {
           Xn1[loopB] = 0.0;
        }
      }

      for(int loopB = 0; loopB < nFaces; loopB++) {
        P[loopB] = Xn1[faceToStateMapping[loopB]]*pConv;
      }

      // CHECK IF SOLUTION IS VALID
      if((stepId % 100) == 0){
        for(int loopB=0;loopB<totalStates;loopB++){
          isValidDouble = (Xn1[loopB] != Xn1[loopB]);
          if(isValidDouble){
            // cout << "FOUND SOMETHING THAT IS NOT VALID AT " << loopB << "\n";
            return 1;
          }
        }
      }

      // Update Xn
      for(int loopB=0;loopB<totalStates;loopB++){
        Xn[loopB] = Xn1[loopB];
      }

      // Update Current Time
      currTime += timeStep;
    }

    // SAVE OUTPUT FOR EVERY 3D MODEL SOLUTION
    // Copy back k1AuxOut for each time step.
    for(int loopB=0;loopB<totAuxStates;loopB++){
      auxOutVals[loopB][stepId3D] = k4AuxOut[loopB];
    }

    // Copy The solution back for each time step
    for(int loopB=0;loopB<totalStates;loopB++){
      outVals[loopB][stepId3D] = Xn1[loopB];
    }
  }

  // RETURN OK
  return 0;
}
*/
