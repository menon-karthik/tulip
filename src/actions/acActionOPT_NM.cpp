
# include "acActionOPT_NM.h"

using namespace std;

//****************************************************************************80

void acActionOPT_NM::nelmin (int n, double start[], double xmin[], 
  double *ynewlo, double reqmin, double step[], int konvge, int kcount, 
  int *icount, int *numres, int *ifault)

//****************************************************************************80
//
//  Purpose:
//
//    NELMIN minimizes a function using the Nelder-Mead algorithm.
//
//  Discussion:
//
//    This routine seeks the minimum value of a user-specified function.
//
//    Simplex function minimisation procedure due to Nelder+Mead(1965),
//    as implemented by O'Neill(1971, Appl.Statist. 20, 338-45), with
//    subsequent comments by Chambers+Ertel(1974, 23, 250-1), Benyon(1976,
//    25, 97) and Hill(1978, 27, 380-2)
//
//    The function to be minimized must be defined by a function of
//    the form
//
//      function fn ( x, f )
//      double fn
//      double x(*)
//
//    and the name of this subroutine must be declared EXTERNAL in the
//    calling routine and passed as the argument FN.
//
//    This routine does not include a termination test using the
//    fitting of a quadratic surface.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    27 February 2008
//
//  Author:
//
//    Original FORTRAN77 version by R ONeill.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    John Nelder, Roger Mead,
//    A simplex method for function minimization,
//    Computer Journal,
//    Volume 7, 1965, pages 308-313.
//
//    R ONeill,
//    Algorithm AS 47:
//    Function Minimization Using a Simplex Procedure,
//    Applied Statistics,
//    Volume 20, Number 3, 1971, pages 338-345.
//
//  Parameters:
//
//    Input, double FN ( double x[] ), the name of the routine which evaluates
//    the function to be minimized.
//
//    Input, int N, the number of variables.
//
//    Input/output, double START[N].  On input, a starting point
//    for the iteration.  On output, this data may have been overwritten.
//
//    Output, double XMIN[N], the coordinates of the point which
//    is estimated to minimize the function.
//
//    Output, double YNEWLO, the minimum value of the function.
//
//    Input, double REQMIN, the terminating limit for the variance
//    of function values.
//
//    Input, double STEP[N], determines the size and shape of the
//    initial simplex.  The relative magnitudes of its elements should reflect
//    the units of the variables.
//
//    Input, int KONVGE, the convergence check is carried out 
//    every KONVGE iterations.
//
//    Input, int KCOUNT, the maximum number of function 
//    evaluations.
//
//    Output, int *ICOUNT, the number of function evaluations 
//    used.
//
//    Output, int *NUMRES, the number of restarts.
//
//    Output, int *IFAULT, error indicator.
//    0, no errors detected.
//    1, REQMIN, N, or KONVGE has an illegal value.
//    2, iteration terminated because KCOUNT was exceeded without convergence.
//
{
  double ccoeff = 0.5;
  double del;
  double dn;
  double dnn;
  double ecoeff = 2.0;
  double eps = 0.001;
  int i;
  int ihi;
  int ilo;
  int j;
  int jcount;
  int l;
  int nn;
  double *p;
  double *p2star;
  double *pbar;
  double *pstar;
  double rcoeff = 1.0;
  double rq;
  double x;
  double *y;
  double y2star;
  double ylo;
  double ystar;
  double z;
  // Additional Parameters
  stdVec inputs;
  stdVec outputs;
  int errorCode = 0;

//
//  Check the input parameters.
//
  if ( reqmin <= 0.0 )
  {
    *ifault = 1;
    return;
  }

  if ( n < 1 )
  {
    *ifault = 1;
    return;
  }

  if ( konvge < 1 )
  {
    *ifault = 1;
    return;
  }

  p = new double[n*(n+1)];
  pstar = new double[n];
  p2star = new double[n];
  pbar = new double[n];
  y = new double[n+1];

  *icount = 0;
  *numres = 0;

  jcount = konvge; 
  dn = ( double ) ( n );
  nn = n + 1;
  dnn = ( double ) ( nn );
  del = 1.0;
  rq = reqmin * dn;
//
//  Initial or restarted loop.
//
  for ( ; ; )
  {
    for ( i = 0; i < n; i++ )
    { 
      p[i+n*n] = start[i];
    }
    y[n] = runModel(start);
    //y[n] = fn ( start );
    *icount = *icount + 1;

    for ( j = 0; j < n; j++ )
    {
      x = start[j];
      start[j] = start[j] + step[j] * del;
      // printf("Step Coeff %d: %f\n",j,start[j]);
      for ( i = 0; i < n; i++ )
      {
        p[i+j*n] = start[i];
      }
      y[j] = runModel(start);
      //y[j] = fn ( start );
      *icount = *icount + 1;
      start[j] = x;
    }
//                    
//  The simplex construction is complete.
//                    
//  Find highest and lowest Y values.  YNEWLO = Y(IHI) indicates
//  the vertex of the simplex to be replaced.
//                
    ylo = y[0];
    ilo = 0;

    for ( i = 1; i < nn; i++ )
    {
      if ( y[i] < ylo )
      {
        ylo = y[i];
        ilo = i;
      }
    }
//
//  Inner loop.
//
    for ( ; ; )
    {
      if ( kcount <= *icount )
      {
        break;
      }
      *ynewlo = y[0];
      ihi = 0;

      for ( i = 1; i < nn; i++ )
      {
        if ( *ynewlo < y[i] )
        {
          *ynewlo = y[i];
          ihi = i;
        }
      }
//
//  Calculate PBAR, the centroid of the simplex vertices
//  excepting the vertex with Y value YNEWLO.
//
      for ( i = 0; i < n; i++ )
      {
        z = 0.0;
        for ( j = 0; j < nn; j++ )
        { 
          z = z + p[i+j*n];
        }
        z = z - p[i+ihi*n];  
        pbar[i] = z / dn;
      }
//
//  Reflection through the centroid.
//
      for ( i = 0; i < n; i++ )
      {
        pstar[i] = pbar[i] + rcoeff * ( pbar[i] - p[i+ihi*n] );
      }
      ystar = runModel(pstar);
      //ystar = fn ( pstar );
      *icount = *icount + 1;
//
//  Successful reflection, so extension.
//
      if ( ystar < ylo )
      {
        for ( i = 0; i < n; i++ )
        {
          p2star[i] = pbar[i] + ecoeff * ( pstar[i] - pbar[i] );
        }
        y2star = runModel(p2star);
        //y2star = fn ( p2star );
        *icount = *icount + 1;
//
//  Check extension.
//
        if ( ystar < y2star )
        {
          for ( i = 0; i < n; i++ )
          {
            p[i+ihi*n] = pstar[i];
          }
          y[ihi] = ystar;
        }
//
//  Retain extension or contraction.
//
        else
        {
          for ( i = 0; i < n; i++ )
          {
            p[i+ihi*n] = p2star[i];
          }
          y[ihi] = y2star;
        }
      }
//
//  No extension.
//
      else
      {
        l = 0;
        for ( i = 0; i < nn; i++ )
        {
          if ( ystar < y[i] )
          {
            l = l + 1;
          }
        }

        if ( 1 < l )
        {
          for ( i = 0; i < n; i++ )
          {
            p[i+ihi*n] = pstar[i];
          }
          y[ihi] = ystar;
        }
//
//  Contraction on the Y(IHI) side of the centroid.
//
        else if ( l == 0 )
        {
          for ( i = 0; i < n; i++ )
          {
            p2star[i] = pbar[i] + ccoeff * ( p[i+ihi*n] - pbar[i] );
          }
          y2star = runModel(p2star);
          //y2star = fn ( p2star );
          *icount = *icount + 1;
//
//  Contract the whole simplex.
//
          if ( y[ihi] < y2star )
          {
            for ( j = 0; j < nn; j++ )
            {
              for ( i = 0; i < n; i++ )
              {
                p[i+j*n] = ( p[i+j*n] + p[i+ilo*n] ) * 0.5;
                xmin[i] = p[i+j*n];
              }
              y[j] = runModel(xmin);
              //y[j] = fn ( xmin );
              *icount = *icount + 1;
            }
            ylo = y[0];
            ilo = 0;

            for ( i = 1; i < nn; i++ )
            {
              if ( y[i] < ylo )
              {
                ylo = y[i];
                ilo = i;
              }
            }
            continue;
          }
//
//  Retain contraction.
//
          else
          {
            for ( i = 0; i < n; i++ )
            {
              p[i+ihi*n] = p2star[i];
            }
            y[ihi] = y2star;
          }
        }
//
//  Contraction on the reflection side of the centroid.
//
        else if ( l == 1 )
        {
          for ( i = 0; i < n; i++ )
          {
            p2star[i] = pbar[i] + ccoeff * ( pstar[i] - pbar[i] );
          }
          y2star = runModel(p2star);
          //y2star = fn ( p2star );
          *icount = *icount + 1;
//
//  Retain reflection?
//
          if ( y2star <= ystar )
          {
            for ( i = 0; i < n; i++ )
            {
              p[i+ihi*n] = p2star[i];
            }
            y[ihi] = y2star;
          }
          else
          {
            for ( i = 0; i < n; i++ )
            {
              p[i+ihi*n] = pstar[i];
            }
            y[ihi] = ystar;
          }
        }
      }
//
//  Check if YLO improved.
//
      if ( y[ihi] < ylo )
      {
        ylo = y[ihi];
        ilo = ihi;
      }
      jcount = jcount - 1;

      if ( 0 < jcount )
      {
        continue;
      }
//
//  Check to see if minimum reached.
//
      if ( *icount <= kcount )
      {
        jcount = konvge;

        z = 0.0;
        for ( i = 0; i < nn; i++ )
        {
          z = z + y[i];
        }
        x = z / dnn;

        z = 0.0;
        for ( i = 0; i < nn; i++ )
        {
          z = z + pow ( y[i] - x, 2 );
        }

        if ( z <= rq )
        {
          break;
        }
      }
    }
//
//  Factorial tests to check that YNEWLO is a local minimum.
//
    for ( i = 0; i < n; i++ )
    {
      xmin[i] = p[i+ilo*n];
    }
    *ynewlo = y[ilo];

    if ( kcount < *icount )
    {
      *ifault = 2;
      break;
    }

    *ifault = 0;

    for ( i = 0; i < n; i++ )
    {
      del = step[i] * eps;
      xmin[i] = xmin[i] + del;
      z = runModel(xmin);
      //z = fn ( xmin );
      *icount = *icount + 1;
      if ( z < *ynewlo )
      {
        *ifault = 2;
        break;
      }
      xmin[i] = xmin[i] - del - del;
      z = runModel(xmin);
      //z = fn ( xmin );
      *icount = *icount + 1;
      if ( z < *ynewlo )
      {
        *ifault = 2;
        break;
      }
      xmin[i] = xmin[i] + del;
    }

    if ( *ifault == 0 )
    {
      break;
    }
//
//  Restart the procedure.
//
    for ( i = 0; i < n; i++ )
    {
      start[i] = xmin[i];
    }
    del = eps;
    *numres = *numres + 1;
  }
  delete [] p;
  delete [] pstar;
  delete [] p2star;
  delete [] pbar;
  delete [] y;

  return;
}

// WRAPPER TO RUN THE MODEL
double acActionOPT_NM::runModel(double* params){
  
  // SET HEART MODEL PARAMETERS
  int par_num = model->getParameterTotal();

  // CHECK LIMITS
  stdVec limits;
  model->getParameterLimits(limits);

  // Copy Inputs
  stdVec inputs;
  stdVec outputs;
  double valToInsert = 0.0;
  for(int loopA=0;loopA<par_num;loopA++){
    // CHECK LIMITS
    if(params[loopA]<limits[2*loopA + 0]){
      // Smaller than Min
      valToInsert = limits[2*loopA + 0] + (limits[2*loopA + 0] - params[loopA]);
    }else if(params[loopA]>limits[2*loopA + 1]){
      // Bigger than Max
      valToInsert = limits[2*loopA + 1] - (params[loopA] - limits[2*loopA + 1]);
    }else{
      valToInsert = params[loopA];
    }
    if(fabs(limits[2*loopA + 1] - limits[2*loopA + 0])<kMathZero){
      valToInsert = limits[2*loopA + 1];
    }
    params[loopA] = valToInsert;
    // Add to Inputs
    //printf("Original %f, New %f\n",params[loopA],valToInsert);
    inputs.push_back(valToInsert);
  }
  stdIntVec errorCodes;
  try{    
    double ynewlo = model->evalModelError(inputs,outputs,errorCodes);
    if(errorCodes[0] == 0){
      if(doPrintMessages){
        printf("   LL =  %f\n",ynewlo);  
      }      
      return ynewlo;  
    }else{
      if(doPrintMessages){
        printf("   ERROR!");
      }
      return 10000.0;
    }
  }catch(exception &e){
    if(doPrintMessages){
      printf("Msg: %s\n",e.what());
      printf("   ERROR!");
    }
    return 10000.0;
  }
}

acActionOPT_NM::acActionOPT_NM(double localConvTol,int localConvUpdateIt,int localMaxOptIt,
                               double localStepCoefficient){
  // Set Optimization Options
  convTol = localConvTol;
  convUpdateIt = localConvUpdateIt;
  maxOptIt = localMaxOptIt;
  stepCoefficient = localStepCoefficient;
  // No Initial Guess yet
  haveInitialGuess = false;
  doPrintMessages = true;
}

// WRITE OPTIMAL PARAMETER SET TO FILE
void writeOptimumToFile(string fileName,int par_num,double* opt){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  for(int loopA=0;loopA<par_num;loopA++){
    fprintf(f,"%f\n",opt[loopA]);
  }
  fclose(f);	
}

// WRITE OPTIMAL VALUE TO FILE
void writeOptimumValueToFile(string fileName,double optVal){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  fprintf(f,"%e\n",optVal);
  fclose(f);  
}

void acActionOPT_NM::setInitialParamGuess(bool localUseStartingParameterFromFile,
                                          bool localStartFromCenter,
                                          string localStartParameterFile){

  // Assign Data Members
  initialPointFromFile = localUseStartingParameterFromFile;
  initialPointFile =  localStartParameterFile;
  startFromCenter = localStartFromCenter;

  // Get parameter number from model
  int par_num = model->getParameterTotal();

  // Allocate
  optParams = new double[par_num];

  // GET STARTING PARAMETER SET
  if(initialPointFromFile){
    // Read Parameters From File
    if(doPrintMessages){
      printf("\n");
      printf(" READING PARAMETERS FROM FILE: %s\n",initialPointFile.c_str());
    }
    int error = cmUtils::readParamsFromFile(initialPointFile,optParams);
    if(error != 0){
      exit(1);
    }
  }else if (startFromCenter){
    // Start From the Center of the Parameter Space
    if(doPrintMessages){
      printf("\n");
      printf(" USING CENTRE PARAMETERS\n");
    }
    stdVec currLimits;
    model->getParameterLimits(currLimits);
    for(int loopA=0;loopA<par_num;loopA++){
      optParams[loopA] = 0.5 * (currLimits[2*loopA + 0] + currLimits[2*loopA + 1]);
    }
  }else{
    // Use Default Parameter Values    
    if(doPrintMessages){
      printf("\n");
      printf(" USING DEFAULT PARAMETERS\n");
    }
    stdVec tempStart;
    model->getDefaultParams(tempStart);
    for(int loopA=0;loopA<par_num;loopA++){
      optParams[loopA] = tempStart[loopA];
    }
  }

  // Initial Guess Defined
  haveInitialGuess = true;

  return;
}

// OPTIMIZE LPN MODEL
void acActionOPT_NM::LPN_opt (){

  // Initial Guess Must be defined
  if(!haveInitialGuess){
    printf("Error: No Initial Guess Defined in NM Optimizer.");
    exit(1);
  }

  // Declare
  int icount;
  int ifault;
  int numres;
  double reqmin;
  double *step;
  double *xmin;
  double ynewlo;

  // Get parameter number from model
  int par_num = model->getParameterTotal();

  // Allocate
  step = new double[par_num];
  xmin = new double[par_num];

  // GET STEPS FROM VARIABLE LIMITS
  stdVec limits;
  model->getParameterLimits(limits);
  for(int loopA=0;loopA<par_num;loopA++){
    step[loopA] = stepCoefficient*fabs(limits[loopA*2+0]-limits[loopA*2+1]);
  }
  
  // PRINT STARTING PARAMETER SET
  if(doPrintMessages){
    cout << "\n";
    cout << "  Starting Parameter Set:\n";
    cout << "\n";
    for (int loopA=0;loopA<par_num;loopA++){
      cout << "  " << setw(14) << optParams[loopA] << "\n";
    }
  }

  // ===========================
  // EVAL INITIAL FUNCTION VALUE
  // ===========================
  // Copy Inputs
  stdVec inputs;
  stdVec outputs;
  for(int loopA=0;loopA<par_num;loopA++){
    inputs.push_back(optParams[loopA]);
  }

  stdIntVec errorCodes;
  try{
    ynewlo = model->evalModelError(inputs,outputs,errorCodes);
    // printf("Errore: %d\n",errorCodes[0]);
    if(errorCodes[0] == 0){
      if(doPrintMessages){
        printf("   LL =  %f\n",ynewlo);
      }
    }else{
      if(doPrintMessages){
        printf("   ERROR!");
      }
      ynewlo = 10000.0;
    }
  }catch(exception &e){
    if(doPrintMessages){
      printf("   ERROR!");
    }
    ynewlo = 10000.0;
  }

  // PRINT FUNCTION VALUE FOR INITIAL POINT
  if(doPrintMessages){
    cout << "\n";
    cout << "  F(X) = " << ynewlo << "\n";
  }

  // RUN NELDER-MEAD OPTIMIZATION
  nelmin (par_num, optParams, xmin, &ynewlo, convTol, step,
          convUpdateIt, maxOptIt, &icount, &numres, &ifault);

  // PRINT OUTPUT REPORT
  if(doPrintMessages){
    cout << "\n";
    cout << "  Return code IFAULT = " << ifault << "\n";
    cout << "\n";
    cout << "  Estimate of minimizing value X*:\n";
    cout << "\n";
    for (int loopA=0;loopA<par_num;loopA++){
      cout << "  " << setw(14) << xmin[loopA] << "\n";
    }
    cout << "\n";
    cout << "  F(X*) = " << ynewlo << "\n";
    cout << "\n";
    cout << "  Number of iterations = " << icount << "\n";
    cout << "  Number of restarts =   " << numres << "\n";
  }

  // WRITE OPTIMAL PARAMETER SET TO FILE
  writeOptimumToFile("optParams.txt",par_num,xmin);
  writeOptimumValueToFile("optValue.txt",ynewlo);

  // STORE OPTIMAL PARAMETER SET
  for(int loopA=0;loopA<par_num;loopA++){
    optParams[loopA] = xmin[loopA];
  }

  // FREE MEMORY
  delete [] step;
  delete [] xmin;

  return;
}

void acActionOPT_NM::setMessagesPrinting(bool flag){
  doPrintMessages = flag;
}

int acActionOPT_NM::go(){

  if(doPrintMessages){
    uqUtils::printTimestamp();
    cout << "\n";
    cout << "OPT_NM:\n";
  }

  // SET OPTIONS
  if(doPrintMessages){
    printf("\n");
    printf("  Nelder-Mead Options\n");
    printf("  -------------------\n");
    printf("  Convergence Tolerance: %e\n",convTol);
    printf("  Iterations between convergence check: %d\n",convUpdateIt);
    printf("  Maximum Iterations: %d\n",maxOptIt);
  }

  // RUN OPTIMIZATION CODE
  LPN_opt();  
  
  // PRINT END OF EXECUTION
  if(doPrintMessages){
    cout << "\n";
    cout << "OPT_NM:\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    uqUtils::printTimestamp();
  }
  
  return 0;
}
