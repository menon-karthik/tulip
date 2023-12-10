# include "acActionDREAMseq.h"

using namespace std;

// Constructor
// Constructor
acActionDREAMseq::acActionDREAMseq(int totChains,
                                   int locTotGenerations,
                                   int locTotalCR,
                                   int loctTotCrossoverPairs,
                                   string locDreamChainFileName,
                                   string locDreamGRFileName,
                                   double locDreamGRThreshold,
                                   int locDreamJumpStep,
                                   int locDreamGRPrintStep,
                                   string locDreamRestartReadFileName,
                                   string locDreamRestartWriteFileName,
                                   // Prior Options
                                   bool usePriorFromFile, 
                                   string priorFileName, 
                                   int priorModelType)
  :acActionDREAM(totChains,locTotGenerations,locTotalCR,loctTotCrossoverPairs,
                 locDreamChainFileName,locDreamGRFileName,locDreamGRThreshold,
                 locDreamJumpStep,locDreamGRPrintStep,locDreamRestartReadFileName,
                 locDreamRestartWriteFileName,usePriorFromFile,priorFileName,priorModelType){
}

acActionDREAMseq::~acActionDREAMseq(){
}

void acActionDREAMseq::chain_init (int chain_num, double fit[], int gen_num, int par_num, 
                                   double z[], int prior_num, int* prPtr, 
                                   stdVec prAv, stdVec prSd)
//  Parameters:
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Output, double FIT[CHAIN_NUM*GEN_NUM], the likelihood of
//    each sample.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Output, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int c;
  int p;
  double *zp;
  stdVec outputs;

  for ( c = 0; c < chain_num; c++ ){
    zp = prior_sample ( par_num ,prior_num, prPtr, prAv, prSd );

    for ( p = 0; p < par_num; p++ ){
      z[p+c*par_num+0*par_num*chain_num] = zp[p];
    }

    fit[c+0*chain_num] = sample_likelihood ( par_num, zp, outputs );

    delete [] zp;
  }
  return;
}

void acActionDREAMseq::chain_init_print (int chain_num, double fit[], int gen_num, 
                                         int par_num, string restart_read_filename, double z[])
//  Parameters:
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Input, double FIT[CHAIN_NUM*GEN_NUM], the likelihood of
//    each sample.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, string RESTART_READ_FILENAME, the name of the 
//    restart file.
//
//    Input, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int i;
  int j;

  if(printLevel > 0){

    cout << "\n";
    cout << "CHAIN_INIT_PRINT\n";
    cout << "  Display initial values of Markov chains.\n";

    if(!restart_read_filename.empty()){
      cout << "  Initialization from restart file \"" 
           << restart_read_filename << "\"\n";
    }else{
      cout << "  Initialization by sampling prior density.\n";
    }
    for(j = 0; j < chain_num; j++){
      cout << "\n";
      cout << "  Chain " << j << "\n";
      cout << "  Fitness " << fit[j+0*chain_num] << "\n";
      for(i = 0; i < par_num; i++){
        cout << "  " << setw(14) << z[i+j*par_num+0*par_num*chain_num];
        if ( ( i + 1 ) % 5 == 0 || i == par_num - 1 ){
          cout << "\n";
        }
      }
    }  
  }
  return;
}

double* acActionDREAMseq::std_compute(int chain_num, int gen_index, int gen_num, int par_num, double z[])
//
//  Parameters:
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Input, int GEN_INDEX, the current generation.
//    0 <= GEN_INDEX < GEN_NUM.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
//    Output, double STD_COMPUTE[PAR_NUM], the standard deviations.
//
{
  int i;
  int j;
  int k;
  double mean;
  double *std;

  std = new double[par_num];

  for ( i = 0; i < par_num; i++ )
  {
    mean = 0.0;
    for ( k = 0; k <= gen_index; k++ )
    {
      for ( j = 0; j < chain_num; j++ )
      {
        mean = mean + z[i+j*par_num+k*par_num*chain_num];
      }
    }
    mean = mean / ( double ) ( chain_num ) / ( double ) ( gen_index );

    std[i] = 0.0;
    for ( k = 0; k <= gen_index; k++ )
    {
      for ( j = 0; j < chain_num; j++ )
      {
        std[i] = std[i] + pow ( z[i+j*par_num+k*par_num*chain_num] - mean, 2 );
      }
    }
    std[i] = sqrt ( std[i] / ( double ) ( chain_num * gen_index - 1 ) );
  }

  return std;
}


void acActionDREAMseq::cr_dis_update (int chain_index, int chain_num, double cr_dis[], 
                                      int cr_index, int cr_num, int cr_ups[], int gen_index, int gen_num, 
                                      int par_num, double z[])
//
//  Purpose:
//
//    CR_DIS_UPDATE updates the CR distance.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    01 May 2013
//
//  Author:
//
//    Original FORTRAN90 version by Guannan Zhang.
//    C++ version by John Burkardt.
//
//  Parameters:
//
//    Input, int CHAIN_INDEX, the index of the chain.
//    0 <= CHAIN_INDEX < CHAIN_NUM.
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Input/output, double CR_DIS[CR_NUM], the CR distances.
//
//    Input, int CR_INDEX, the index of the CR.
//    0 <= CR_INDEX < CR_NUM.
//
//    Input, int CR_NUM, the total number of CR values.
//    1 <= CR_NUM.
//
//    Input/output, int CR_UPS[CR_NUM], the number of updates 
//    for each CR.
//
//    Input, int GEN_INDEX, the index of the generation.
//    0 <= GEN_INDEX < GEN_NUM.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int i;
  int i1;
  int i2;
  double *std;

  //  Compute the standard deviations.
  std = std_compute ( chain_num, gen_index, gen_num, par_num, z );

  //  Increment the update count.
  cr_ups[cr_index] = cr_ups[cr_index] + 1;

  //  Update the CR distance.
  for(i = 0; i < par_num; i++){
    i1 = i + chain_index * par_num +   gen_index       * par_num * chain_num;
    i2 = i + chain_index * par_num + ( gen_index - 1 ) * par_num * chain_num;
    // DES - Added for situations where the ranges are both zero (k_bw)
    if(std[i]>1.0e-8){
      cr_dis[cr_index] = cr_dis[cr_index] + pow ( ( z[i1] - z[i2] ) / std[i], 2 );  
    }
  }

  // free
  delete [] std;

  return;
}


//****************************************************************************80

void acActionDREAMseq::dream_algm_seq(int chain_num, int cr_num, double fit[], int gen_num, 
                                      double gr[], int &gr_conv, int &gr_count, int gr_num, double gr_threshold,
                                      double jumprate_table[], int jumpstep, double limits[], int pair_num, 
                                      int par_num, int printstep, double z[], 
                                      int prior_num, int* prPtr, const stdVec& prAv, const stdVec& prSd)
    
//****************************************************************************80
//
//  Purpose:
//
//    DREAM_ALGM gets a candidate parameter sample.
//
// Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    25 May 2013
//
//  Author:
//
//    Original FORTRAN90 version by Guannan Zhang.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Jasper Vrugt, CJF ter Braak, CGH Diks, Bruce Robinson, James Hyman, 
//    Dave Higdon,
//    Accelerating Markov Chain Monte Carlo Simulation by Differential 
//    Evolution with Self-Adaptive Randomized Subspace Sampling,
//    International Journal of Nonlinear Sciences and Numerical Simulation,
//    Volume 10, Number 3, March 2009, pages 271-288.
//
//  Parameters:
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Input, int CR_NUM, the total number of CR values.
//    1 <= CR_NUM.
//
//    Input, double FIT[CHAIN_NUM*GEN_NUM], the likelihood of
//    each sample.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, double GR[PAR_NUM*GR_NUM], 
//    the Gelman-Rubin R statistic.
//
//    Input/output, int &GR_CONV, the Gelman-Rubin convergence flag.
//
//    Input/output, int &GR_COUNT, counts the number of generations
//    at which the Gelman-Rubin statistic has been computed.
//
//    Input, int GR_NUM, the number of times the Gelman-Rubin
//    statistic may be computed.
//
//    Input, double GR_THRESHOLD, the convergence tolerance for
//    the Gelman-Rubin statistic.
//
//    Input, double JUMPRATE_TABLE[PAR_NUM], the jumprate table.
//
//    Input, int JUMPSTEP, forces a "long jump" every
//    JUMPSTEP generations.
//
//    Input, double LIMITS[2*PAR_NUM], lower and upper bounds
//    for each parameter.
//
//    Input, int PAIR_NUM, the number of pairs of 
//    crossover chains.
//    0 <= PAIR_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, int PRINTSTEP, the interval between generations on 
//    which the Gelman-Rubin statistic will be computed and written to a file.
//
//    Output, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
//  Local parameters:
//
//    Local, int CHAIN_INDEX, the index of the current chain.
//    1 <= CHAIN_INDEX <= CHAIN_NUM.
//
//    Local, double CR[CR_NUM], the CR values.
//
//    Local, double CR_DIS[CR_NUM], the CR distances.
//
//    Local, int CR_INDEX, the index of the selected CR value.
//    1 <= CR_INDEX <= CR_NUM.
//
//    Local, double CR_PROB[CR_NUM], the probability of each CR.
//
//    Local, double CR_UPS[CR_NUM], the number of updates for each CR.
//
//    Local, int GEN_INDEX, the index of the current generation.
//    1 <= GEN_INDEX <= GEN_NUM.
//
//    Local, double ZP[PAR_NUM], a candidate sample.
//
//    Local, int ZP_ACCEPT, the number of candidates accepted.
//
//    Local, double ZP_ACCEPT_RATE, the rate at which generated
//    candidates were accepted.
//
//    Local, int ZP_COUNT, the number of candidates generated.
//
//    Local, double ZP_RATIO, the Metropolis ratio for a candidate.
//
{
  int chain_index;
  double *cr;
  double *cr_dis;
  int cr_index;
  double *cr_prob;
  int *cr_ups;
  int gen_index;
  int i;
  int ind1;
  int ind2;
  double pd1;
  double pd2;
  double r;
  double *zp;
  int zp_accept;
  double zp_accept_rate;
  int zp_count;
  double zp_fit;
  double *zp_old;
  double zp_old_fit;
  double zp_ratio;
  stdVec outputs;

  zp_old = new double[par_num];
  zp_count = 0;
  zp_accept = 0;
//
//  Initialize the CR values.
//
  cr      = new double[cr_num];
  cr_dis  = new double[cr_num];
  cr_prob = new double[cr_num];
  cr_ups  = new int[cr_num];

  cr_init ( cr, cr_dis, cr_num, cr_prob, cr_ups );  

  bool doneProgress[10];
  for(int loopA=0;loopA<10;loopA++){
    doneProgress[loopA] = false;
  }

  for(gen_index=1;gen_index<gen_num;gen_index++){

    // Print Progress 
    int currProgress = 0;
    if(printLevel == 0){
      if(gen_index == 1){
        printf("Sampling...");
      }else{
        currProgress = int(((gen_index-1)/double(gen_num-1))*100);
        if((currProgress % 10 == 0)&&(!doneProgress[currProgress/10])){
          doneProgress[currProgress/10] = true;
          printf("%d.",currProgress);
        }
      }
      if(gen_index == gen_num-1){
      printf("100.OK.\n");
      }
    }

    for(chain_index = 0; chain_index < chain_num; chain_index++){
      
      //  Choose CR_INDEX, the index of a CR.
      cr_index = cr_index_choose(cr_num, cr_prob);

      //  Generate a sample candidate ZP.
      zp = sample_candidate ( chain_index, chain_num, cr, cr_index, cr_num, 
        gen_index, gen_num, jumprate_table, jumpstep, limits, pair_num, 
        par_num, z );

      zp_count = zp_count + 1;

      //  Compute the log likelihood function for ZP.
      zp_fit = sample_likelihood ( par_num, zp, outputs );

      for ( i = 0; i < par_num; i++ ){
        zp_old[i] = z[i+chain_index*par_num+(gen_index-1)*par_num*chain_num];
      }
      zp_old_fit = fit[chain_index+(gen_index-1)*chain_num];

      //  Compute the Metropolis ratio for ZP.
      pd1 = prior_density ( par_num, zp , prior_num, prPtr, prAv, prSd );      

      pd2 = prior_density ( par_num, 
        z+0+chain_index*par_num+(gen_index-1)*par_num*chain_num ,
        prior_num, prPtr, prAv, prSd );

      zp_ratio = exp((zp_fit+log(pd1))-(zp_old_fit+log(pd2)));

      zp_ratio = min(zp_ratio, 1.0);

      //  Accept the candidate, or copy the value from the previous generation.
      r = uSampler->sample(0.0,1.0);

      if(r <= zp_ratio){
        for(i = 0; i < par_num; i++){
          z[i+chain_index*par_num+gen_index*par_num*chain_num] = zp[i];
        }
        zp_accept = zp_accept + 1;
        fit[chain_index+gen_index*chain_num] = zp_fit;
      }else{
        for(i = 0; i < par_num; i++){
          z[i+chain_index*par_num+gen_index*par_num*chain_num] = zp_old[i];
        }
        fit[chain_index+gen_index*chain_num] = zp_old_fit; 
      }
//
//  Update the CR distance.
//
      if(!gr_conv){
        if(cr_num > 1){
          cr_dis_update ( chain_index, chain_num, cr_dis, cr_index, 
                          cr_num, cr_ups, gen_index, gen_num, par_num, z );
        }
      }

      delete [] zp;
    }

    //  Update the multinomial distribution of CR.
    if ( ! gr_conv )
    {
      if ( 1 < cr_num )
      {
        if ( ( gen_index + 1 ) % 10 == 0 )
        {
          cr_prob_update ( cr_dis, cr_num, cr_prob, cr_ups );
        }
      }
    }
//
//  Every PRINTSTEP interval,
//  * compute the Gelman Rubin R statistic for this generation,
//    and determine if convergence has occurred.
//
    if ( ( gen_index + 1 ) % printstep == 0 )
    {
      gr_compute ( chain_num, gen_index, gen_num, gr, gr_conv, gr_count, 
        gr_num, gr_threshold, par_num, z );
    }
//
//  Check for outlier chains.
//
    if ( ! gr_conv )
    {
      if ( ( gen_index + 1 ) % 10 == 0 )
      {
        chain_outliers ( chain_num, gen_index, gen_num, par_num, fit, z );
      }
    }
  }    
//
//  Compute the acceptance rate.
//
  zp_accept_rate = ( double ) ( zp_accept ) / ( double ) ( zp_count );

  cout << "MCMC acceptance rate: " << zp_accept_rate << "\n";

  delete [] cr;
  delete [] cr_dis;
  delete [] cr_prob;
  delete [] cr_ups;
  delete [] zp_old;

  return;
}

void acActionDREAMseq::input_print (string chain_filename, int chain_num, int cr_num, 
                                    string gr_filename, double gr_threshold, int jumpstep, double limits[], 
                                    int gen_num, int pair_num, int par_num, int printstep, 
                                    string restart_read_filename, string restart_write_filename)

//  Parameters:
//
//    Input, string CHAIN_FILENAME, the "base" filename
//    to be used for the chain files.  If this is ""
//    then the chain files will not be written.  This name should 
//    include a string of 0's which will be replaced by the chain 
//    indices.  For example, "chain000.txt" would work as long as the
//    number of chains was 1000 or less.
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Input, int CR_NUM, the total number of CR values.
//    1 <= CR_NUM.
//
//    Input, string GR_FILENAME, the name of the file
//    in which values of the Gelman-Rubin statistic will be recorded,
//    or the empty string "" if this file is not to be written.
//
//    Input, double GR_THRESHOLD, the convergence tolerance for the
//    Gelman-Rubin statistic.
//
//    Input, int JUMPSTEP, forces a "long jump" every
//    JUMPSTEP generations.
//
//    Input, double LIMITS[2*PAR_NUM], lower and upper limits
//    for each parameter.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAIR_NUM, the number of pairs of 
//    crossover chains.
//    0 <= PAIR_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, int PRINTSTEP, the interval between generations on 
//    which the Gelman-Rubin statistic will be computed and written to a file.
//
//    Local, string RESTART_READ_FILENAME, the name of the file
//    containing restart information.  If the calculation is not a restart,
//    then this should be set to "".
//
//    Local, string RESTART_WRITE_FILENAME, the name of the file
//    to be written, containing restart information.  If a restart file is
//    not to be written, this should be set to "".
//
{
  int j;

  cout << "\n";
  cout << "INPUT_PRINT:\n";
  cout << "\n";
  cout << "  Number of parameters\n";
  cout << "  PAR_NUM = " << par_num << "\n";
  cout << "\n";
  cout << "  LIMITS: Lower and upper limits for each parameter:\n";
  cout << "\n";
  cout << "  Index           Lower           Upper\n";
  cout << "\n";
  for(j = 0; j < par_num; j++){
    cout << "  " << setw(5) << j
         << "  " << setw(14) << limits[0+j*2]
         << "  " << setw(14) << limits[1+j*2] << "\n";
  }
  cout << "\n";
  cout << "  Number of generations:\n";
  cout << "  GEN_NUM = " << gen_num << "\n";
  cout << "\n";
  cout << "  Number of simultaneous chains:\n";
  cout << "  CHAIN_NUM = " << chain_num << "\n";
  cout << "\n";
  cout << "  Chain filename (base):\n";
  if (chain_filename.empty()){
    cout << "  CHAIN_FILENAME = \"(Null)\".\n";
  }else{
    cout << "  CHAIN_FILENAME = \"" << chain_filename << "\".\n";
  }
  cout << "\n";
  cout << "  Number of pairs of chains for crossover:\n";
  cout << "  PAIR_NUM = " << pair_num << "\n";
  cout << "\n";
  cout << "  Number of crossover values:\n";
  cout << "  CR_NUM = " << cr_num << "\n";
  cout << "\n";
  cout << "  Number of steps til a long jump:\n";
  cout << "  JUMPSTEP = " << jumpstep << "\n";
  cout << "\n";
  cout << "  Interval between Gelman-Rubin computations:\n";
  cout << "  PRINTSTEP = " << printstep << "\n";
  cout << "\n";
  cout << "  Gelman-Rubin data filename:\n";
  if(gr_filename.empty()){
    cout << "  GR_FILENAME = \"(Null)\".\n";
  }else{
    cout << "  GR_FILENAME = \"" << gr_filename << "\".\n";
  }
  cout << "  GR_THRESHOLD = " << gr_threshold << "\n";
  cout << "\n";
  cout << "  Restart read filename:\n";
  if(restart_read_filename.empty()){
    cout << "  RESTART_READ_FILENAME = \"(Null)\".\n";
  }else{
    cout << "  RESTART_READ_FILENAME = \"" << restart_read_filename << "\".\n";
  }
  cout << "\n";
  cout << "  Restart write filename:\n";
  if(restart_write_filename.empty()){
    cout << "  RESTART_WRITE_FILENAME = \"(Null)\".\n";
  }else{
    cout << "  RESTART_WRITE_FILENAME = \"" << restart_write_filename << "\".\n";
  }

  return;
}


void acActionDREAMseq::jumprate_table_print ( double jumprate_table[], int pair_num, int par_num )
//  Parameters:
//
//    Input, double JUMPRATE_TABLE[PAR_NUM], the jumprate table.
//
//    Input, int PAIR_NUM, the number of pairs of 
//    crossover chains.
//    0 <= PAIR_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
{
  int i;
 
  cout << "\n";
  cout << "JUMPRATE_TABLE_PRINT\n";
  cout << "\n";
  cout << "   I        Jumprate\n";
  cout << "\n";
  for ( i = 0; i < par_num; i++ )
  {
    cout << "  " << setw(2) << i
         << "  " << setw(14) << jumprate_table[i] << "\n";
  }
  return;
}

void acActionDREAMseq::restart_read ( int chain_num, double fit[], int gen_num, int par_num, 
  string restart_read_filename, double z[] )
//
//  Parameters:
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Output, double FIT[CHAIN_NUM*GEN_NUM], the likelihood of
//    each sample.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, string RESTART_READ_FILENAME, the name of 
//    the restart file.
//
//    Output, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int chain_index;
  int gen_index = 0;
  int index;
  string line;
  size_t n;
  int par_index;
  ifstream restart;

  restart.open ( restart_read_filename.c_str ( ) );

  if ( ! restart )
  {
    cout << "\n";
    cout << "RESTART_READ - Fatal error!\n";
    cout << "  Could not open the file \"" 
         << restart_read_filename << "\".\n";
    exit ( 1 );
  }
//
//  Read and ignore line 1.
//
  getline ( restart, line );
//
//  Read the final fitness and parameter values for each chain.
//
  for ( chain_index = 0; chain_index < chain_num; chain_index++ )
  {
    index = chain_index 
          + chain_num * gen_index;
    restart >> fit[index];
    for ( par_index = 0; par_index < par_num; par_index++ )
    {
      index = par_index 
            + par_num * chain_index 
            + par_num * chain_num * gen_index;
      restart >> z[index];
    }
  }

  restart.close ( );

  return;
}

void acActionDREAMseq::restart_write ( int chain_num, double fit[], int gen_num, int par_num, 
  string restart_write_filename, double z[] )
//
//  Parameters:
//
//    Input, int CHAIN_NUM, the total number of chains.
//    3 <= CHAIN_NUM.
//
//    Input, double FIT[CHAIN_NUM*GEN_NUM], the likelihood of
//    each sample.
//
//    Input, int GEN_NUM, the total number of generations.
//    2 <= GEN_NUM.
//
//    Input, int PAR_NUM, the total number of parameters.
//    1 <= PAR_NUM.
//
//    Input, string RESTART_WRITE_FILENAME, the name of the 
//    restart file.
//
//    Input, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int c;
  int p;
  ofstream restart;

  restart.open ( restart_write_filename.c_str ( ) );   

  if(!restart ){
    cout << "\n";
    cout << "RESTART_WRITE - Fatal error!\n";
    cout << "  Could not open \"" << restart_write_filename << "\".\n";
    exit ( 1 );
  }

  restart << "DREAM.C:Parameter_values_for_restart.\n";

  for(c = 0; c < chain_num; c++){
    restart << "  " << c
            << "  "<< fit[c+(gen_num-1)*chain_num];
    for(p = 0; p < par_num; p++){
      restart << "  " << z[p+c*par_num+(gen_num-1)*par_num*chain_num];
    }
    restart << "\n"; 
  }

  restart.close ( );

  if(printLevel > 0){
    cout << "\n";
    cout << "RESTART_WRITE:\n";
    cout << "  Created restart file \"" << restart_write_filename << "\".\n";  
  }

  return;
}


int acActionDREAMseq::go(){

  string chain_filename = "";
  int chain_num;
  int cr_num;
  double *fit;
  int gen_num;
  double *gr;
  int gr_conv;
  int gr_count;
  string gr_filename = "";
  int gr_num;
  double gr_threshold;
  double *jumprate_table;
  int jumpstep;
  double *limits;
  int pair_num;
  int par_num;
  int printstep;
  string restart_read_filename = "";
  string restart_write_filename = "";
  double *z;
  // Prior Info
  int prior_num = 0;
  int* prPtr = NULL;
  stdVec prAv;
  stdVec prSd;

  if(printLevel > 0){
    uqUtils::printTimestamp();  
  }

  // Allocate Samplers
  uSampler = new uqUniformPDF();
  nSampler = new uqGaussianPDF();
  catSampler = new uqCategoricalPMF();

  if(printLevel > 0){
    cout << "\n";
    cout << "DREAM\n";
    cout << "  C++ version\n";
    cout << "  MCMC acceleration by Differential Evolution.\n";
    cout << "  Sequential Algorithm.\n";
    cout << "\n";
    // Print sampler seeds
    cout << "Uniform Sampler Seed: " << uSampler->getSeed() << endl;
    cout << "Normal Sampler Seed: " << nSampler->getSeed() << endl;
    cout << "Categorical Sampler Seed: " << catSampler->getSeed() << endl;    
  }

  // Get the problem sizes.
  problem_size ( chain_num, cr_num, gen_num, pair_num, par_num );

  // Decide if the problem sizes are acceptable.
  if(chain_num < 3){
    cout << "\n";
    cout << "DREAM - Fatal error!\n";
    cout << "  CHAIN_NUM < 3.\n";
    exit(1);
  }
  if(cr_num < 1){
    cout << "\n";
    cout << "DREAM - Fatal error!\n";
    cout << "  CR_NUM < 1.\n";
    exit(1);
  }
  if(gen_num < 2){
    cout << "\n";
    cout << "DREAM - Fatal error!\n";
    cout << "  GEN_NUM < 2.\n";
    exit(1);
  }
  if(pair_num < 0){
    cout << "\n";
    cout << "DREAM - Fatal error!\n";
    cout << "  PAIR_NUM < 0.\n";
    exit (1);
  }
  if(par_num < 1){
    cout << "\n";
    cout << "DREAM - Fatal error!\n";
    cout << "  PAR_NUM < 1.\n";
    exit ( 1 );
  }

  //  Read Prior Information From File if Available
  if(usePriorFromFile){    
    if(printLevel > 0){
      printf("READING PRIOR INFORMATION FROM FILE.\n");
    }
    // Read Statistics from file
    int error = cmUtils::readPriorFromFile(priorFileName,prior_num,prAv,prSd);    
    if(error != 0){  
      printf("ERROR: CANNOT READ FILE WITH PRIOR.\n");
      exit(1);
    }
    
    // Get Prior Mapping
    prPtr = new int[prior_num];
    model->getPriorMapping(priorModelType,prPtr);
      
    // PRINT PRIOR INFORMATION READ
    if(printLevel > 0){
      printf("\n");
      printf("PRIOR INFORMATION.\n");
      printf("%10s %10s %10s %10s\n","NUM","PTR","AV VALUE","SD VALUE");
      for(int loopA=0;loopA<prior_num;loopA++){
        printf("%10d %10d %10f %10f\n",loopA,prPtr[loopA],prAv[loopA],prSd[loopA]);
      }
      printf("\n");
    }
  }else{
    if(printLevel > 0){
      printf("NO PRIOR FILE PROVIDED.\n");
    }
  }

  //  Get the problem data values;
  limits = r8mat_zero_new ( 2, par_num );

  problem_value ( &chain_filename, &gr_filename, gr_threshold, jumpstep, 
                  limits, par_num, printstep, &restart_read_filename, 
                  &restart_write_filename);
//
//  Print the data as a job record.
//
  if(printLevel > 0){
    input_print(chain_filename, chain_num, cr_num, gr_filename, gr_threshold, 
                jumpstep, limits, gen_num, pair_num, par_num, 
                printstep, restart_read_filename, restart_write_filename);
  }
//
//  Allocate and zero out memory.
//
  gr_num = gen_num / printstep;

  fit = r8mat_zero_new ( chain_num, gen_num );
  gr = r8mat_zero_new ( par_num, gr_num );
  z = r8block_zero_new ( par_num, chain_num, gen_num );
//
//  Set the jump rate table.
//
  jumprate_table = jumprate_table_init ( pair_num, par_num );
  
  if(printLevel > 0){
    jumprate_table_print(jumprate_table, pair_num, par_num);
  }
  
//
//  Initialize the Gelman-Rubin data.
//
  gr_init(gr,gr_conv,gr_count,gr_num,par_num);

  if(printLevel > 0){
    cout << "\n";
    cout << "GR_PRINT:\n";
    cout << "  GR_CONV  = " << gr_conv << "\n";
    cout << "  GR_COUNT = " << gr_count << "\n";
    cout << "  GR_NUM   = " << gr_num << "\n";  
  }

//
//  Set the first generation of the chains from restart data, or by sampling.
//
  if(restart_read_filename.empty()){
    chain_init ( chain_num, fit, gen_num, par_num, z,
                 prior_num, prPtr, prAv, prSd);
  }else{
    restart_read ( chain_num, fit, gen_num, par_num, restart_read_filename, z);
  }
  chain_init_print ( chain_num, fit, gen_num, par_num, restart_read_filename, z);
//
//  Carry out the DREAM algorithm.
//
  dream_algm_seq(chain_num, cr_num, fit, gen_num, gr, gr_conv, gr_count, 
                 gr_num, gr_threshold, jumprate_table, jumpstep, limits, pair_num, 
                 par_num, printstep, z ,prior_num, prPtr, prAv, prSd);
//
//  Save Gelman-Rubin statistic values to a file.
//
  if ( ! gr_filename.empty ( ) )
  {
    gr_write ( gr, gr_filename, gr_num, par_num, printstep );
  }
//
//  Save parameter values for all chains at last generation.
//
  if ( ! restart_write_filename.empty ( ) )
  {
    restart_write ( chain_num, fit, gen_num, par_num, restart_write_filename,
      z );
  }
//
//  Write each chain to a separate file.
//
  if ( ! chain_filename.empty ( ) )
  {
    chain_write ( chain_filename, chain_num, fit, gen_num, par_num, z );
  }
//
//  Free memory.
//
  delete [] fit;
  delete [] gr;
  delete [] jumprate_table;
  delete [] limits;
  delete [] z;
  // Free Prior Params
  delete [] prPtr;
  // Free random samplers
  delete uSampler;
  delete nSampler;
  delete catSampler;
//
//  Terminate.
//
  if(printLevel > 0){
    cout << "\n";
    cout << "DREAM\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    uqUtils::printTimestamp();  
  }

  return 0;
}
