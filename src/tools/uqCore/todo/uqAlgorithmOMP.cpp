# include "uqAlgorithmOMP.h"

// ===========
// CONSTRUCTOR
// ===========
uqAlgorithmOMP::uqAlgorithmOMP(){
}

// ====================================================================================
// Initialize Weights From Measurement Matrix: Returns the Squared of the Weight Matrix
// ====================================================================================
uqVector InitializeWeights(uqOperator AMat, uqVector rhs){

  // Get Rows and Columns
  int rowCount = AMat.getRowCount();
  int colCount = AMat.getColCount();

  // Allocate and Initialize
  uqVector weightMat(colCount);

  // Initialize Weights
  double diagValue = 0.0;
  for(int loopA=0;loopA<colCount;loopA++){
    diagValue = 0.0;
    for(int loopB=0;loopB<rowCount;loopB++){
      diagValue += AMat(loopB,loopA)*rhs(loopB);
    }
    // Take Absolute Value
    diagValue = diagValue*diagValue;
    // Assign To Diagonal Element
    weightMat.setValue(loopA,diagValue);
  }

  // Stabilization
  //const = 2500/S/log10(N);
  //delta = const*exp((max(log(w))+min(log(w)))/2);

  // Return Matrix
  return weightMat;
}

// =======================
// FORM SENSING DICTIONARY
// =======================
uqOperator FormSensingDictionary(uqOperator AMat,uqVector rhs,int maxIterations, bool writeIterations){

  // INITIALIZE WEIGHTS
  uqVector SqWeightMat = InitializeWeights(AMat,rhs);
  uqOperator rMat();

  // START ITERATION LOOP
  bool converged = false;
  int currentIt = 0;
  while((!(converged))&&(currentIt<maxIterations)){

    // Increment Iteration Number
    currentIt++;

    // Calculate R
    rMat = ComputeRMat(AMat,SqWeightMat);

    // Update Sensing Basis set
    sensingMat = UpdateSensingMat(rMat,AMat);

    // Check Beta
    betaNorm = ComputeBetaNorm(AMat,SensingMat,zeroProdCount);
    printf("Beta Norm: %e, Zero Products: %e\n",betaNorm,zeroProdCount);

    // Update Weights
    SqNewWeightMat = UpdateWeights(SensingMat,rhs);

    // Eval l_2 Relative Difference for the Weights
    weightDiffRelNorm = EvalWeightRelDiff(colCount,SqWeightMat,SqNewWeightMat);

    if(writeIterations){
      ShowMessage("Iteration: %d, RelNorm: %e\n",currentIt,WeightDiffRelNorm);
    }

    // Update Weights
    for(int loopA=0;loopA<colCount;loopA++){
      SqWeightMat[LoopA] = SqNewWeightMat[LoopA];
    }

    // Check Convergence
    converged = (betaNorm<1.0e-10);

  }

  // Delete
  delete [] SqWeightMat;
}

// ========================
// MAIN OMP SOLVER FUNCTION
// ========================
int uqAlgorithmOMP::solve(uqOperator* AMat, uqVector* rhs,
                          uqOptionsOMP* ompOptions,
                          bool useSensingDictionary,
                          uqVector* solution){

  // ALLOCATE TIME VARIABLES
  clock_t sweep_start;
  clock_t updateSupport_start;
  clock_t ls_start;
  clock_t resUpdate_start;
  time_t now;

  // Write Starting Message
  if(ompOptions->writeMsgs){
    time(&now);
    printf("OMP Solution Started: %s",ctime(&now));
  }

  // Store Column Norm for later use
  double* colNorms = AMat->getColumnNorms();
  // Trasform to Unit Matrix
  AMat->convertToUnitaryColums();

  // FORM SENSING DICTIONARY IF REQUIRED
  if(ompOptions->useSensingDictionary){
    if(ompOptions->writeMsgs){
      uqUtils::uqWriteMessage("Forming Sensing Dictionary...");
    }
    double** SensingMat = formSensingDictionary(AMat,RHS);
    if(OMPOptions->WriteMsgs){
      uqUtils::uqWriteMessage("Done.\n");
    }
  }

  //Write Iteration Header
  if(OMPOptions->writeMsgs){
    uqUtils::uqWriteMessage('\n');
    if(OMPOptions->DebugMode){
      printf("%-12s %-12s %-8s %-8s %-8s %-8s","IT","RES NORM","SWEEP","UPD SUP","LS","UPD RES");
    }else{
      printf("%-12s %-12s","IT","RES NORM");
    }
  }

  // Prepare file for printing the residual
  if (ompOptions->printResidual){
    // Open Output File
    FILE* outFile;
    outFile = fopen(OMPOptions->residualFileName.c_str(),"w");
  }

  //Allocation
  // Solution Vector
  double mrItSol[ColCount] = {0.0};
  // Residual
  double itRes[RowCount];
  // Set Initial Residual Equal to RHS
  for(int loopA=0;loopA<RowCount;loopA++){
    ItRes[LoopA] = RHS[LoopA];
  }

  // Initialize Time
  if(OMPOptions->DebugMode){
    ompTime.sweep = 0.0;
    ompTime.updateSupport = 0.0;
    ompTime.ls = 0.0;
    ompTime.resUpdate = 0.0;
  }

  // Init Support Size
  int totalInsertedCols = 0;
  // Start Loop
  int intConverged = 0;
  int currentIt = 0;
  double resNorm = 1.0;

  // =====================
  // START OMP ITERARTIONS
  // =====================
  while((intConverged == 0)&&(currentIt<ompOptions.maxIterations)){

    // Increment Iteration Count
    currentIt++;

    // Write Progress
    if(OMPOptions.WriteMsgs){
      if ((currentIt % OMPOptions.iterationPrintStep) == 0){
        if(OMPOptions.debugMode){
          printf("%-12d %-12.3e %-8.1f %-8.1f %-8.1f %-8.1f",currentIt,resNorm,Sweep_TotalTime/1000.0,UpdateSupport_TotalTime/1000.0,LS_TotalTime/1000.0,ResUpdate_TotalTime/1000.0);
        }else{
          printf("%-12d %-12.3e",currentIt,resNorm);
        }

      }
    }

    // Initialize Time
    if(OMPOptions.debugMode){
      sweep_start = clock();
    }

    // ==================
    // SWEEP/SENSING STEP
    // ==================
    if(useSensingDictionary)
      // Sweep Step With Sensing Matrix
      int bestIDX = SensingMat->Sweep(rowCount,colCount,totalInsertedCols,insertedCols,itRes);
    }else{
      // Sweep Step With Measurement Matrix
      int bestIDX = AMat->Sweep(rowCount,colCount,totalInsertedCols,insertedCols,itRes);
    }

    // Check Time
    if(ompOptions.debugMode){
      ompTime.sweep += (double)(clock() - sweep_start) / CLOCKS_PER_SEC;
    }

    // Initialize Time
    if(ompOptions.debugMode){
      updateSupport_start = clock();
    }

    // ===================
    // UPDATE SUPPORT STEP
    // ===================
    updateSupport(colCount,bestIDX,totalInsertedCols,insertedCols);

    // Check Time
    if(ompOptions.debugMode){
      ompTime.updateSupport += (double)(clock() - updateSupport_start) / CLOCKS_PER_SEC;
    }

    // Initialize Time for Least Squares Solution
    if(ompOptions.debugMode){
      ls_start = clock();
    }

    // ==================================
    // SOLVER FOR LEAST SQUARES WITH LSMR
    // ==================================
    uqAlgorithmLSMR algLSMR = new uqAlgorithmLSMR();
    int intConv = algLSMR->solve(rowCount,colCount,AMat,rhs,totalInsertedCols,insertedCols,resNorm,BMat,mrItSol);

    // CHECK CONVERGENCE
    if(intConv == 1){
      intConverged = 1;
      if(ompOptions.WriteMsgs){
        uqWriteMessage("LSMR Solution NOT CONVERGED!");
      }
      // Copy the Solution
      for(int loopA=0;loopA<colCount;loopA++){
        if (colNorms[loopA]>uqMathZero){
          solution[LoopA] = MRItSol[loopA]/colNorms[loopA];
        }else{
          solution[LoopA] = 0.0;
        }
        // Exit
        Exit;
      }

      // CHECK TIME
      if(ompOptions.DebugMode){
        ompTime.ls += (double)(clock() - ls_start) / CLOCKS_PER_SEC;
      }

      // Initialize Time
      if(ompOptions.DebugMode){
        resUpdate_start = clock();
      }

      // ====================
      // UPDATE RESIDUAL STEP
      // ====================
      oldResNorm = resNorm;
      UpdateOMPResidual(rowCount,colCount,AMat,rhs,mrItSol,itRes,resNorm);

      // CHECK TIME
      if(ompOptions.DebugMode){
        ompTime.resUpdate += (double)(clock() - resUpdate_start) / CLOCKS_PER_SEC;
      }

      // Residual Didn't Change
      if(fabs(oldResNorm)< kMathZero){
        resDidntChange = false;
      }else{
        resDidntChange= ((fabs(resNorm-oldResNorm)/(oldResNorm))<1.0e-10);
      }


      // CHECK CONVERGENCE
      isConverged = (resNorm<ompOptions.convergenceTol)||(resDidntChange);
    End;

    // Close Residual File
    if(ompOptions->printResidual){
      fopen(outFile);
    }

    // WRITE CONVERGECE MESSAGES
    if(ompOptions.WriteMsgs){
      if(isConverged){
        if (resNorm<ompOptions.convergenceTol){
          uqWriteMessage("CONVERGED By Residual Norm.");
          uqWriteMessage("Norm: "+to_string(resNorm)+"; Tol: "+to_string(ompOptions.convergenceTol)+".");
        }else{
          uqWriteMessage("CONVERGED By Small Residual Change.");
        }
      }else{
        uqWriteMessage("OMP Solution NOT CONVERGED!");
      }
    }

    // Print The Index Set
    if(ompOptions->printColumnSet){
      FILE *fp;
      fp=fopen("OMPColumnSet.csv", "w");
      for(int loopA=0;loopA<totalInsertedCols;loopA++){
        fprintf(fp,"%d\n",insertedCols[LoopA]);
      }
      fclose(fp);
    }

    // COPY THE SOLUTION
    for(int loopA=0;loopA<colCount;loopA++){
      if(colNorms[LoopA] > kMathZero){
        solution[LoopA] = mrItSol[LoopA]/(double)colNorms[LoopA];
      }else{
        solution[LoopA] = 0.0;
      }
    }

    // WRITE FINAL MESSAGE
    if(ompOptions.WriteMsgs){
      time(&now);
      printf("OMP Solution Finished: %s",ctime(&now));
    }
}


// =================
// COMPUTE BETA NORM
// =================
/*double ComputeBetaNorm(uqOperator* AMat, uqOperator* SensingMat, int &ZeroProducts){
  ZeroProducts = 0;
  double betaNorm = 0.0;
  for(int loopA=0;loopA<AMat->getCols();loopA++){
    currentProduct = 0.0;
    for(int loopB=0;loopB<AMat->getRows;loopA++){
      currentProduct = currentProduct + AMat->getValue[LoopB,LoopA] * SensingMat->getValue[LoopB,LoopA];
    }
  }
  // If Zero Product then Increment the counter
  if(fabs(currentProduct)<MathZero){
    zeroProducts += 1;
  }
  if(fabs(currentProduct)>MathZero){
    betaNorm = betaNorm + (currentProduct-1.0) * (currentProduct-1.0);
  }
  betaNorm = sqrt(betaNorm);
}

{Update Weights}
Procedure UpdateWeights(RowCount,ColCount: Integer4;
                        SensingMat: Double2DArray;
                        RHS: DoubleArray;
                        Var WeightMat: DoubleArray);
Var
  LoopA,LoopB: Integer4;
  DiagValue: Double;
Begin
  {Allocate and Initialize}
  SetLength(WeightMat,ColCount+1);
  For LoopA:=1 To ColCount Do WeightMat[LoopA]:=0.0;
  {Initialize Weights}
  For LoopA:=1 To ColCount Do
  Begin
    DiagValue:=0.0;
    For LoopB:=1 To RowCount Do DiagValue:=DiagValue+SensingMat[LoopB,LoopA]*RHS[LoopB];
    {Take Absolute Value}
    DiagValue:=Sqr(DiagValue);
    {Assign To Diagonal Element}
    WeightMat[LoopA]:=DiagValue;
  End;
End;

{Solve Linear System to Find Rphi}
Function ComputeRPhi(RowCount: Integer4;
                     CurrentColumn: Integer4;
                     RMat: Double2DArray;
                     AMat: Double2DArray;
                     Var Rphi: DoubleArray): String;
Var
  LoopA,LoopB: Integer4;
  TempMat: TReal2DArray;
  TempRHS,TempSol: TReal1DArray;
Begin
  {Init Result}
  Result:='';

  {Allocate TempMatrix and TempRHS}
  SetLength(TempMat,RowCount);
  For LoopA:=0 To RowCount-1 Do SetLength(TempMat[LoopA],RowCount);
  SetLength(TempRHS,RowCount);

  {Copy R Matrix}
  For LoopA:=1 To RowCount Do
  Begin
    For LoopB:=1 To RowCount Do
    Begin
      TempMat[LoopA-1,LoopB-1]:=RMat[LoopA,LoopB];
    End;
  End;

  {Copy RHS}
  For LoopA:=1 To RowCount Do
  Begin
    TempRHS[LoopA-1]:=AMat[LoopA,CurrentColumn];
  End;

  {Solve Linear Equation Set}
  If Not(SMatrixSolve(TempMat,TempRHS,RowCount,TRUE{IsUpper},TempSol)) Then
  Begin
    Result:='Error in ComputeRPhi: Cannot Solve Linear Equation Set';
  End;

  {Copy Solution Back}
  SetLength(Rphi,RowCount+1);
  For LoopA:=1 To RowCount Do Rphi[LoopA]:=TempSol[LoopA-1];

  {DeAllocate TempMatrix and TempRHS}
  For LoopA:=0 To RowCount-1 Do SetLength(TempMat[LoopA],0);
  SetLength(TempMat,0);
  SetLength(TempRHS,0);
  FreeMemory(TempMat);
  FreeMemory(TempRHS);
End;

{Update Sensing Matrix}
Procedure UpdateSensingMat(RowCount,ColCount: Integer4;
                           RMat: Double2DArray;
                           AMat: Double2DArray;
                           Var SensingMat: Double2DArray);
Var
  LoopA,LoopB: Integer4;
  Rphi: DoubleArray;
  Denominator: Double;
Begin
  {Allocate Sensing Mat}
  SetLength(SensingMat,RowCount+1);
  For LoopA:=1 To RowCount Do SetLength(SensingMat[LoopA],ColCount+1);
  {Build Matrix}
  For LoopA:=1 To ColCount Do
  Begin
    {Find R-1phi}
    ComputeRPhi(RowCount,LoopA,RMat,AMat,Rphi);
    {Find Sensing Column}
    Denominator:=0.0;
    For LoopB:=1 To RowCount Do
    Begin
      Denominator:=Denominator+AMat[LoopB,LoopA]*Rphi[LoopB];
    End;
    For LoopB:=1 To RowCount Do
    Begin
      If ABS(Denominator)>MathZero Then SensingMat[LoopB,LoopA]:=(Rphi[LoopB]/Denominator)
      Else SensingMat[LoopB,LoopA]:=0.0;
    End;
  End;
End;


{Compute Matrix R}
Procedure ComputeRMat(RowCount,ColCount: Integer4;
                      AMat: Double2DArray;
                      SqWeightMat: DoubleArray;
                      Var RMat: Double2DArray);
Var
  LoopA,LoopB,LoopC: Integer4;
  TempMat: Double2DArray;
Begin
  {Allocate TempMat}
  SetLength(TempMat,RowCount+1);
  For LoopA:=1 To RowCount Do SetLength(TempMat[LoopA],ColCount+1);

  {Allocate and Initialize R Matrix}
  SetLength(RMat,RowCount+1);
  For LoopA:=1 To RowCount Do SetLength(RMat[LoopA],RowCount+1);
  For LoopA:=1 To RowCount Do
  Begin
    For LoopB:=1 To RowCount Do RMat[LoopA,LoopB]:=0.0;
  End;
  {Compute the R Matrix}
  For LoopA:=1 To RowCount Do
  Begin
    For LoopB:=1 To ColCount Do TempMat[LoopA,LoopB]:=AMat[LoopA,LoopB]*SqWeightMat[LoopB];
  End;
  {Second Multiplication}
  For LoopA:=1 To RowCount Do
  Begin
    For LoopB:=1 To RowCount Do
    Begin
      RMat[LoopA,LoopB]:=0.0;
      For LoopC:=1 To ColCount Do
      Begin
        RMat[LoopA,LoopB]:=RMat[LoopA,LoopB]+TempMat[LoopA,LoopC]*AMat[LoopB,LoopC];
      End;
    End;
  End;
  {DeAllocate TempMat}
  For LoopA:=1 To RowCount Do SetLength(TempMat[LoopA],0);
  SetLength(TempMat,0);
  FreeMemory(TempMat);
End;

{Eval The Relative Difference Norm For the Weights}
Function EvalWeightRelDiff(ColCount: Integer4;
                           SqWeightVector: DoubleArray;
                           SqNewWeightVector: DoubleArray): Double;
Var
  LoopA: Integer4;
Begin
  {Init Result}
  Result:=0.0;
  For LoopA:=1 To ColCount Do
  Begin
    //If ABS(SqWeightVector[LoopA])>MathZero Then Result:=Result+Sqr((SqNewWeightVector[LoopA]-SqWeightVector[LoopA])/SqWeightVector[LoopA]);
    Result:=Result+Sqr(SqNewWeightVector[LoopA]-SqWeightVector[LoopA]);
  End;
  Result:=Sqrt(Result);
End;


(*function [phi delta w]=huang(psi,u,S,N)
const = 2500/S/log10(N);
w = abs(psi'*u);
delta = const*exp((max(log(w))+min(log(w)))/2);
W = diag(abs(psi'*u))+delta*eye(length(w));
phi = zeros(size(psi));
res = 1;
while res > 1e-10
  R = psi*W^2*psi';
  for i = 1:size(psi,2)
    phi(:,i) = (inv(R)*psi(:,i))./(psi(:,i)'*inv(R)*psi(:,i));
  end
  W = diag(abs(phi'*u))+delta*eye(size(W));
  w = diag(W);
  delta = const*exp((max(log(w))+min(log(w)))/2);
  res = norm(diag(phi'*psi)-diag(eye(size(psi,2))));
end *)

{Solve Problem With LSMR}
Function SolveWithLSMR(RowCount,ColCount: Integer4;
                       AMat: Double2DArray;
                       RHS: DoubleArray;
                       TotalInsertedCols: Integer4;
                       InsertedCols: Integer4Array;
                       ResNorm: Double;
                       Var BMat: Double2DArray;
                       Var MRItSol: DoubleArray;
                       Var Converged: Boolean): String;
Var
  LoopA,LoopB: Integer4;
  ErrorString: String;
  CurrentIndex: Integer4;
  LSMROptions: LSMROptionRecord;
  ItSol: DoubleArray;
Begin
  {Allocate for the Last Column}
  For LoopA:=1 To RowCount Do SetLength(BMat[LoopA],TotalInsertedCols+1);

  {Form the LS Matrix Accounting For the Support}
  For LoopA:=1 To RowCount Do
  Begin
    {Assign Index}
    CurrentIndex:=InsertedCols[TotalInsertedCols];
    {Copy Value}
    BMat[LoopA,TotalInsertedCols]:=AMat[LoopA,CurrentIndex];
  End;

  {Solve With LSMR}
  LSMROptions.MaxIt:=1000;
  LSMROptions.ANormTol:=ResNorm*1.0e-2;
  LSMROptions.BNormTol:=ResNorm*1.0e-2;
  LSMROptions.WriteMsgs:=FALSE;
  ErrorString:=LSMRSolve(mtExplicitMat,LSMROptions,RowCount,TotalInsertedCols,
                         0,0,Nil,
                         Nil,Nil,Nil,Nil,
                         BMat,RHS,Converged,ItSol);
  If (ErrorString<>'') Then
  Begin
    Result:=ErrorString;
    Exit;
  End;

  {Update Solution to original support}
  SetLength(MRItSol,ColCount+1);
  For LoopA:=1 To ColCount Do MRItSol[LoopA]:=0.0;
  For LoopA:=1 To TotalInsertedCols Do
  Begin
    CurrentIndex:=InsertedCols[LoopA];
    MRItSol[CurrentIndex]:=ItSol[LoopA];
  End;
End;

Function PerformSweepStep(RowCount,ColCount: Integer4;
                          TotalInsertedCols: Integer4;
                          InsertedCols: Integer4Array;
                          ItRes: DoubleArray;
                          AMat: Double2DArray;
                          Var BestIDX: Integer4): String;
Var
  LoopA,LoopB: Integer4;
  CurrentProduct,MaxProduct: Double;
Begin
  {Init Result}
  Result:='';

  {Eval Epsilon For All Remaining Columns}
  MaxProduct:=0.0;
  For LoopA:=1 To ColCount Do
  Begin
    If Not(IsInTempList(LoopA,TotalInsertedCols,InsertedCols)) Then
    Begin
      {Eval Zj}
      CurrentProduct:=0.0;
      For LoopB:=1 To RowCount Do
      Begin
        CurrentProduct:=CurrentProduct+ItRes[LoopB]*AMat[LoopB,LoopA];
      End;
      CurrentProduct:=ABS(CurrentProduct);
      If (CurrentProduct>MaxProduct) Then
      Begin
        MaxProduct:=CurrentProduct;
        BestIDX:=LoopA;
      End;
    End;
  End;
End;


Function UpdateSupport(ColCount: Integer4;
                       BestIDX: Integer4;
                       Var TotalInsertedCols: Integer4;
                       Var InsertedCols: Integer4Array): String;
Var
  LoopA: Integer4;
  MinEps,MaxEps: Double;
  EpsIdx: Integer4;
Begin
  {Init Result}
  Result:='';

  {Add to the List Of Checked Columns}
  PutInTempList(BestIDX,TotalInsertedCols,InsertedCols,FALSE);
End;

Function UpdateSolutionWithLS(RowCount: Integer4;ColCount: Integer4;
                              AMat: Double2DArray;
                              Rhs: DoubleArray;
                              TotalInsertedCols: Integer4;
                              InsertedCols: Integer4Array;
                              Var ItSol: DoubleArray): String;
Var
  LoopA,LoopB,LoopC: Integer4;
  LSMat: Double2DArray;
  LSRHS: DoubleArray;
  LSSol: DoubleArray;
  CurrentIndex: Integer4;
  FirstIndex,SecondIndex: Integer4;
Begin
  {Init Result}
  Result:='';

  {Allocation}
  SetLength(LSMat,TotalInsertedCols);
  SetLength(LSRHS,TotalInsertedCols);
  For LoopA:=0 To (TotalInsertedCols-1) Do SetLength(LSMat[LoopA],TotalInsertedCols);

  {Form the LS Matrix Accounting For the Support}
  For LoopA:=1 To TotalInsertedCols Do
  Begin
    FirstIndex:=InsertedCols[LoopA];
    For LoopB:=1 To TotalInsertedCols Do
    Begin
      SecondIndex:=InsertedCols[LoopB];
      {Init Matrix}
      LSMat[LoopA-1,LoopB-1]:=0.0;
      For LoopC:=1 To RowCount Do
      Begin
        LSMat[LoopA-1,LoopB-1]:=LSMat[LoopA-1,LoopB-1]+AMat[LoopC,FirstIndex]*AMat[LoopC,SecondIndex];
      End;
    End;
  End;

  {Form The LS RHS Accounting for the Support}
  For LoopA:=1 To TotalInsertedCols Do
  Begin
    FirstIndex:=InsertedCols[LoopA];
    {Init RHS}
    LSRHS[LoopA-1]:=0.0;
    For LoopB:=1 To RowCount Do
    Begin
      LSRHS[LoopA-1]:=LSRHS[LoopA-1]+AMat[LoopB,FirstIndex]*Rhs[LoopB];
    End;
  End;

  {Solve To Find The New Solution}
  If Not(SMatrixSolve(TReal2DArray(LSMat),
                      TReal1DArray(LSRHS),
                      TotalInsertedCols,
                      TRUE,
                      TReal1DArray(LSSol))) Then
  Begin
    Result:='SMatrixSolve Error: Cannot Solve Symmetric Linear System';
    Exit;
  End;

  {Update Solution to original support}
  For LoopA:=1 To ColCount Do ItSol[LoopA]:=0.0;
  For LoopA:=1 To TotalInsertedCols Do
  Begin
    CurrentIndex:=InsertedCols[LoopA];
    ItSol[CurrentIndex]:=LSSol[LooPA-1];
  End;

  {DeAllocation}
  For LoopA:=0 To (TotalInsertedCols-1) Do SetLength(LSMat[LoopA],0);
  SetLength(LSMat,0);
  FreeMemory(LSMat);
  SetLength(LSRHS,0);
  FreeMemory(LSRHS);

End;

{Update Residual}
Function UpdateOMPResidual(RowCount,ColCount: Integer4;
                           AMat: Double2DArray;
                           RHS: DoubleArray;
                           ItSol: DoubleArray;
                           Var ItRes: DoubleArray;
                           Var ResNorm: Double): String;
Var
  LoopA,LoopB: Integer4;
  BNorm: Double;
Begin
  {Init Result}
  Result:='';

  {Update The Residual}
  ResNorm:=0.0;
  BNorm:=0.0;
  For LoopA:=1 To RowCount Do
  Begin
    {Init}
    ItRes[LoopA]:=RHS[LoopA];
    BNorm:=BNorm+Sqr(ItRes[LoopA]);
    For LoopB:=1 To ColCount Do
    Begin
      ItRes[LoopA]:=ItRes[LoopA]-AMat[LoopA,LoopB]*ItSol[LoopB];
    End;
    ResNorm:=ResNorm+Sqr(ItRes[LoopA]);
  End;
  {Make The Squared Root}
  BNorm:=Sqrt(BNorm);
  If ABS(BNorm)<MathZero Then  ResNorm:=Sqrt(ResNorm)
  Else ResNorm:=(Sqrt(ResNorm)/BNorm);
End;*/


end.
