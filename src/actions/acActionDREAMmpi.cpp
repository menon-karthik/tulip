# include "acActionDREAMmpi.h"

using namespace std;

// Constructor
acActionDREAMmpi::acActionDREAMmpi(int totChains,
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

acActionDREAMmpi::~acActionDREAMmpi(){
}

/*
#### Purpose
Build the network topology using 
local process and Group number

#### Discussion
None

#### Modified
18 September 2015 - Daniele Schiavazzi

#### Reference
No Reference

\param[out] localProcessNumbers local process number for every process
\param[out] localGroupNumbers local Group number for every group
*/
void buildNetworkTopology(int totProc,int globRank,int locRank,int* localProcessNumbers,int* localGroupNumbers, int& totGroups){
  // Print Message
  if(globRank == 0){
    printf("\n");
    printf("Building Network Topology...\n");
  }
  
  // Get Information from all processes
  MPI_Allgather(&locRank,1,MPI_INT,localProcessNumbers,1,MPI_INT,MPI_COMM_WORLD);

  // Find Max 
  int maxLoc = 0;
  for(int loopA=0;loopA<totProc;loopA++){
    if(localProcessNumbers[loopA] > maxLoc){
      maxLoc = localProcessNumbers[loopA];
    }
  }
  maxLoc++;
  int localProcCount[maxLoc+1];
  for(int loopA=0;loopA<maxLoc;loopA++){
    localProcCount[loopA] = 0;
  }
  for(int loopA=0;loopA<totProc;loopA++){
    localGroupNumbers[loopA] = localProcCount[localProcessNumbers[loopA]];
    localProcCount[localProcessNumbers[loopA]]++;
  }

  // Find the total number of groups
  totGroups = 0;
  for(int loopA=0;loopA<totProc;loopA++){
    if(localGroupNumbers[loopA] > totGroups){
      totGroups = localGroupNumbers[loopA];
    }
  }
  totGroups++;
  
  // Print information on local id and group
  if(globRank == 0){
    for(int loopA=0;loopA<totProc;loopA++){
      printf("Global Proc %d has local id: %d and group %d\n",loopA,localProcessNumbers[loopA],localGroupNumbers[loopA]);
    }
  }

  // Finished
  if(globRank == 0){
    printf("\n");
    printf("Network Topology OK...\n");
  }

}

//****************************************************************************80

void restart_write ( int chain_num, double fit[], int gen_num, int par_num, 
  string restart_write_filename, double z[] )

//****************************************************************************80
//
//  Purpose:
//
//    RESTART_WRITE writes a restart file.
//
//  Discussion:
//
//    Only data for the final generation is written.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    23 April 2013
//
//  Author:
//
//    Original FORTRAN90 version by Guannan Zhang.
//    C++ version by John Burkardt.
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
  int globRank;
  MPI_Comm_rank(MPI_COMM_WORLD,&globRank);

  if(globRank == 0){
    
    restart.open ( restart_write_filename.c_str ( ) );   

    if (!restart){    
      cerr << "\n";
      cerr << "RESTART_WRITE - Fatal error!\n";
      cerr << "  Could not open \"" << restart_write_filename << "\".\n";
      exit ( 1 );
    }

    restart << "DREAM.C:Parameter_values_for_restart.\n";

    for ( c = 0; c < chain_num; c++ ){
      restart << "  " << c
            << "  "<< fit[c+(gen_num-1)*chain_num];
      for ( p = 0; p < par_num; p++ ){
        restart << "  " << z[p+c*par_num+(gen_num-1)*par_num*chain_num];
      }
      restart << "\n"; 
    }

    restart.close ( );

    cout << "\n";
    cout << "RESTART_WRITE:\n";
    cout << "  Created restart file \"" << restart_write_filename << "\".\n";
  }
  // Return
  return;
}

void acActionDREAMmpi::cr_dis_update (int chain_index, int chain_num, double cr_dis[], 
                                      int cr_index, int cr_num, int cr_ups[], int gen_index, int gen_num, 
                                      int par_num, double z[], double currentStd[])
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
//
//  Increment the update count.
//
  cr_ups[cr_index] = cr_ups[cr_index] + 1;
//
//  Update the CR distance.
//
  for(i = 0; i < par_num; i++){
    i1 = i + chain_index * par_num +   gen_index       * par_num * chain_num;
    i2 = i + chain_index * par_num + ( gen_index - 1 ) * par_num * chain_num;
    // DES - Added for situations where the ranges are both zero (k_bw)
    if(currentStd[i]>1.0e-8){
      cr_dis[cr_index] = cr_dis[cr_index] + pow ( ( z[i1] - z[i2] ) / currentStd[i], 2 );  
    }
  }
  return;
}

void acActionDREAMmpi::dream_algm_mpi (int rank_sm, int id, int num_procs, int size_sm, int* localGroupNumbers, 
                                       MPI_Comm comm_sm, MPI_Status info, int totGroups, MPI_Comm mainProcessComm,
                                       int chain_num, int cr_num, double fit[], int gen_num, 
                                       double gr[], int &gr_conv, int &gr_count, int gr_num, double gr_threshold,
                                       double jumprate_table[], int jumpstep, double limits[], int pair_num, 
                                       int par_num, int printstep, double z[], 
                                       int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd,
                                       double iniMean[], double iniStd[])
    
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

  int currIndex = 0;
  // "Manager" processor - One for each node
  //  Handles all the bookkeeping in getting new parameter sets and sending them out, also receives them and processes them
  if(rank_sm == 0){
	
    printf("Main Processor %d is active!\n",id);
    fflush(stdout);
    int proc_index;
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
    double zp_ratio;
    double received_LH;

/**************************************************************************************************************************/

	zp_count = 0;
	zp_accept = 0;
	cr = new double[cr_num];
	cr_dis = new double[cr_num];
	cr_prob = new double[cr_num];
	cr_ups = new int[cr_num];
	double AllLHs[num_procs];
	double AllLHs_tmp[num_procs];
	// Stored
    double storedMean[par_num];
    double storedStd[par_num];
    // Current 
    double currentMean[par_num];
    double currentStd[par_num];
    // Test
    double testMean[par_num];
    double testStd[par_num];

    for(int loopA=0;loopA<par_num;loopA++){
      storedMean[loopA] = iniMean[loopA];
      storedStd[loopA] = iniStd[loopA];
    }
//
//  Initialize the CR values
//
	cr_init(cr, cr_dis, cr_num, cr_prob, cr_ups);

    //  Initialize temporary storage for parameter candidates
	double z_temp[num_procs*par_num];
	double z_temp_aux[num_procs*par_num];
	double zp_old[par_num];
	double fit_old;

    // Start main loop over all generations
	for( gen_index = 1; gen_index < gen_num; gen_index++) {
    	  
    // This is the "Prepare, Sample, and Send" loop that generates samples and sends them to Worker processes
	  // Send Samples to the sub-processed in this Node
	  //for( proc_index = 1; proc_index < num_procs; proc_index++) {
	  // Loop on all the chain numbers for this sub group
	  for(proc_index=1;proc_index<size_sm;proc_index++){

	  	// Careful: The proc_index should depend on the node!!!
	  	currIndex = localGroupNumbers[id]*size_sm + proc_index;

        cr_index = cr_index_choose(cr_num, cr_prob);
        // Sample candidate for all processors with id > 0
        zp = sample_candidate (currIndex, num_procs, cr, cr_index, cr_num, 
                               gen_index, gen_num, jumprate_table, jumpstep, limits, pair_num, 
                               par_num, z );
        zp_count++;
        for ( i = 0; i < par_num; i++) {
          z_temp_aux[proc_index*par_num + i] = zp[i];
        }

        // Send Sample info to all other processes in the same shared memory Group
        //  MPI_Send(zp, par_num, MPI_DOUBLE, proc_index, 5, MPI_COMM_WORLD);
        MPI_Send(zp,par_num,MPI_DOUBLE,proc_index,5,comm_sm);
        delete [] zp;
        zp = NULL;
      }

      // Receive from sub-Processes
      for( proc_index = 1; proc_index < size_sm; proc_index++) {
        MPI_Recv(&received_LH,1,MPI_DOUBLE,proc_index,6,comm_sm,&info);
        AllLHs_tmp[proc_index] = received_LH;
      }

      // Continue to process Sample for primary node processor
      cr_index = cr_index_choose(cr_num, cr_prob);

      zp = sample_candidate(id,num_procs,cr,cr_index,cr_num, 
                            gen_index, gen_num, jumprate_table, jumpstep, limits, pair_num, 
                            par_num, z );
      zp_count++;

      //  Copy candidates into z_temp
      for(i=0;i<par_num;i++){
        z_temp_aux[i] = zp[i];
      }

      //  Compute the LH Function for the parameter set on this node
      zp_fit = sample_likelihood(par_num, zp);
      AllLHs_tmp[0] = zp_fit;
      
      // Gather From Other Nodes if multiple nodes are present
      if(totGroups > 1){
      	// Sync Likelihood Values
        MPI_Allgather(AllLHs_tmp,size_sm,MPI_DOUBLE,AllLHs,size_sm,MPI_DOUBLE,mainProcessComm);
        // Sync z_temp
        MPI_Allgather(z_temp_aux,size_sm*par_num,MPI_DOUBLE,z_temp,size_sm*par_num,MPI_DOUBLE,mainProcessComm);
      }else{
      	for(int loopA=0;loopA<size_sm;loopA++){
          AllLHs[loopA] = AllLHs_tmp[loopA];
      	}
      	for(int loopA=0;loopA<size_sm*par_num;loopA++){
          z_temp[loopA] = z_temp_aux[loopA];
      	}      	
      }
      
      //  Process results from all chains
      for( proc_index = 0; proc_index < num_procs; proc_index++) {
        
      	// Get Computed LH
      	received_LH = AllLHs[proc_index];

        //  Retrieve the old parameter sets and old fit value
        for(int j = 0; j < par_num; j++) {
          zp_old[j] = z[j+proc_index*par_num+(gen_index-1)*par_num*num_procs];
          zp[j] = z_temp[proc_index*par_num+j];
        }
        fit_old = fit[proc_index+(gen_index-1)*num_procs];

        //  Compute Metropolis Ratio for this Worker trial
        pd1 = prior_density(par_num, zp, prior_num, prPtr, prAv, prSd);
        pd2 = prior_density(par_num, zp_old, prior_num, prPtr, prAv, prSd);

        zp_ratio = exp( (received_LH + log (pd1) ) - ( fit_old + log (pd2) ) );
        zp_ratio = r8_min (zp_ratio, 1.0);

        //  Accept this candidate, or copy value from previous generation
        r = r8_uniform_01_sample ( );

        if(r<=zp_ratio){
          for(i=0;i<par_num;i++){
            z[i+proc_index*par_num+gen_index*par_num*num_procs] = zp[i];
          }
          zp_accept++;
          fit[proc_index+gen_index*num_procs] = received_LH;
        }else{
          for(i=0;i<par_num;i++){
            z[i+proc_index*par_num+gen_index*par_num*num_procs] = zp_old[i];
          }
          fit[proc_index+gen_index*num_procs] = fit_old;
        }
      }      
      delete [] zp;

      //  Compute the standard deviations once for all chains
      std_compute (chain_num,gen_index,gen_num,par_num,z,storedMean,storedStd, currentMean, currentStd);

      // Update stored statistics for this parameter
      for(i=0;i<par_num;i++){
        storedMean[i] = currentMean[i];
        storedStd[i] = currentStd[i];
      }

      //  Update the CR distance
      //  Process results from all chains
      for( proc_index = 0; proc_index < num_procs; proc_index++) {
        if(!gr_conv){
          if(cr_num > 1){
            cr_dis_update ( proc_index, num_procs, cr_dis, cr_index, cr_num, cr_ups, 
                            gen_index, gen_num, par_num, z, currentStd);
          }
        }
      }

      //  Update the multinomial distribution of CR
      if(!gr_conv){
        if(1<cr_num){
          if((gen_index+1)%10 == 0){
            cr_prob_update ( cr_dis, cr_num, cr_prob, cr_ups );
          }
        }
      }

      //  Every PRINTSTEP interval, compute the Gelman Rubin R statistic for this generation, and check for convergence
      if((gen_index + 1) % printstep == 0){
        gr_compute ( num_procs, gen_index, gen_num, gr, gr_conv, gr_count, gr_num, gr_threshold, par_num, z );
      }

      //  Check for outlier chains.
      if(!gr_conv){
        if((gen_index + 1) % 10 == 0){
          chain_outliers ( num_procs, gen_index, gen_num, par_num, fit, z );
        }
      }

      if((gen_index + 1) % 100 == 0){
        restart_write ( num_procs, fit, gen_index, par_num, dreamRestartWriteFileName,z);
      }       
      // Print Generation Progress
      if(id == 0){
        printf("PROGRESS: (%d / %d)\n", gen_index, gen_num);
      }
    }

    //  Compute the acceptance rate.
    zp_accept_rate = ( double ) ( zp_accept ) / ( double ) ( zp_count );

    if(id == 0){
      cout << "\n";
      cout << "  The acceptance rate is " << zp_accept_rate << "\n";
    }

    delete [] cr;
    delete [] cr_dis;
    delete [] cr_prob;
    delete [] cr_ups;
	
    string chain_filename = "";
    string gr_filename = "";
    string restart_read_filename = "";
    string restart_write_filename = "";

    problem_value (&chain_filename, &gr_filename, gr_threshold, jumpstep,
                   limits, par_num, printstep, &restart_read_filename, 
                   &restart_write_filename);

    //  Save Gelman-Rubin statistic values to a file.
    if(id == 0){
      if(!gr_filename.empty()){
        gr_write ( gr, gr_filename, gr_num, par_num, printstep);
      }
    }

    //  Save parameter values for all chains at last generation.
    if(id == 0){
      if (!restart_write_filename.empty()){
        restart_write ( chain_num, fit, gen_num, par_num, restart_write_filename,z);
      }
    }

    //  Write each chain to a separate file.
    if(id == 0){
      if(!chain_filename.empty()){
        chain_write ( chain_filename, chain_num, fit, gen_num, par_num, z );
      }	  
    }

    //  Sub-processors in node. 
    //  All this does is receive parameter sets from the "Manager" and evaluates the model using those parameters. 
    //  Then sends back the LH function
    }else{
      printf("Sub-Processor %d is active!\n",id);
      fflush(stdout);
      double local_params[par_num];
      double local_LH;
      for(int i = 1; i < gen_num; i++) {
        MPI_Recv(local_params,par_num,MPI_DOUBLE,0,5,comm_sm,&info);
        local_LH = sample_likelihood(par_num, local_params);
        MPI_Send(&local_LH,1,MPI_DOUBLE,0,6,comm_sm);
      }
    }
  
  // Return
  return;
}

void acActionDREAMmpi::chain_init_print ( int chain_num, double fit[], int gen_num, int par_num, 
  string restart_read_filename, double z[] )
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
//    Input, string RESTART_READ_FILENAME, the name of the 
//    restart file.
//
//    Input, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int i;
  int j;
  int globRank;
  MPI_Comm_rank(MPI_COMM_WORLD,&globRank);

  if(globRank == 0){

    cout << "\n";
    cout << "CHAIN_INIT_PRINT\n";
    cout << "  Display initial values of Markov chains.\n";
  
    if(! restart_read_filename.empty ( ) ){
      cout << "  Initialization from restart file \"" 
           << restart_read_filename << "\"\n";
    }else{
      cout << "  Initialization by sampling prior density.\n";
    }
    for ( j = 0; j < chain_num; j++ ){
      cout << "\n";
      cout << "  Chain " << j << "\n";
      cout << "  Fitness " << fit[j+0*chain_num] << "\n";
      for ( i = 0; i < par_num; i++ ){
        cout << "  " << setw(14) << z[i+j*par_num+0*par_num*chain_num];
        if ( ( i + 1 ) % 5 == 0 || i == par_num - 1 ){
          cout << "\n";
        }
      }
    }
  }
  // Return
  return;
}

void acActionDREAMmpi::jumprate_table_print ( double jumprate_table[], int pair_num, int par_num )
//
//  Purpose:
//
//    JUMPRATE_TABLE_PRINT prints the jump rate table.
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
//    John Burkardt
//
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
  int globRank;
  MPI_Comm_rank(MPI_COMM_WORLD,&globRank);

  if(globRank == 0){
    cout << "\n";
    cout << "JUMPRATE_TABLE_PRINT\n";
    cout << "\n";
    cout << "   I        Jumprate\n";
    cout << "\n";
    for ( i = 0; i < par_num; i++ ){
      cout << "  " << setw(2) << i
           << "  " << setw(14) << jumprate_table[i] << "\n";
    }
  }
  // Return
  return;
}
//****************************************************************************80

void acActionDREAMmpi::input_print ( string chain_filename, int chain_num, int cr_num, 
  string gr_filename, double gr_threshold, int jumpstep, double limits[], 
  int gen_num, int pair_num, int par_num, int printstep, 
  string restart_read_filename, string restart_write_filename )

//****************************************************************************80
//
//  Purpose:
//
//    INPUT_PRINT prints the data from the input file.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    26 May 2013
//
//  Author:
//
//    John Burkardt
//
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
  int globRank;
  MPI_Comm_rank(MPI_COMM_WORLD,&globRank);

  if(globRank == 0){
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
    for ( j = 0; j < par_num; j++ ){
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
    if ( chain_filename.empty ( ) ){
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
    if ( gr_filename.empty ( ) ){
      cout << "  GR_FILENAME = \"(Null)\".\n";
    }else{
      cout << "  GR_FILENAME = \"" << gr_filename << "\".\n";
    }
    cout << "  GR_THRESHOLD = " << gr_threshold << "\n";
    cout << "\n";
    cout << "  Restart read filename:\n";
    if ( restart_read_filename.empty ( ) ){
      cout << "  RESTART_READ_FILENAME = \"(Null)\".\n";
    }else{
      cout << "  RESTART_READ_FILENAME = \"" << restart_read_filename << "\".\n";
    }
    cout << "\n";
    cout << "  Restart write filename:\n";
    if ( restart_write_filename.empty ( ) ){
      cout << "  RESTART_WRITE_FILENAME = \"(Null)\".\n";
    }else{
      cout << "  RESTART_WRITE_FILENAME = \"" << restart_write_filename << "\".\n";
    }
  }
  // Return
  return;
}

void acActionDREAMmpi::chain_init ( int chain_num, double fit[], int gen_num, int par_num, 
  double z[], int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd, int globRank,
  MPI_Comm comm_sm, MPI_Comm mainProcessComm, int size_sm, int totGroups, int rank_sm)
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
//    Output, double Z[PAR_NUM*CHAIN_NUM*GEN_NUM], the Markov chain 
//    sample data.
//
{
  int p;
  double tempFit[1];
  double* zp;
  // Temp Quantities on Nodes
  double* z_node = NULL;
  double* fit_node = NULL;
  if(rank_sm == 0){
    z_node = new double[size_sm*par_num];
    fit_node = new double[size_sm];
  }

  zp = prior_sample ( par_num ,prior_num, prPtr, prAv, prSd );
  tempFit[0] = sample_likelihood ( par_num, zp );

  // Sync Vector z and fit
  // Remember: z[p+c*par_num+0*par_num*chain_num] = zp[p];
  // Pass to Main Node Processor
  MPI_Gather(zp,par_num,MPI_DOUBLE,z_node,par_num,MPI_DOUBLE,0,comm_sm);
  MPI_Gather(tempFit,1,MPI_DOUBLE,fit_node,1,MPI_DOUBLE,0,comm_sm);

  // Communicate between nodes only if there are more than one group
  if(totGroups > 1){
  	if(rank_sm == 0){
      MPI_Allgather(z_node,size_sm*par_num,MPI_DOUBLE,z,size_sm*par_num,MPI_DOUBLE,mainProcessComm);
      MPI_Allgather(fit_node,size_sm,MPI_DOUBLE,fit,size_sm,MPI_DOUBLE,mainProcessComm);
  	}    
  }else{
    if(rank_sm == 0){
  	  for(int loopA=0;loopA<size_sm*par_num;loopA++){
        z[loopA] = z_node[loopA];
  	  } 
  	  for(int loopA=0;loopA<size_sm;loopA++){
        fit[loopA] = fit_node[loopA];
  	  } 
  	}
  }
  // Deallocate
  free(zp);
  if(rank_sm == 0){
    delete [] z_node;
    delete [] fit_node;
  }  
  return;
}

int acActionDREAMmpi::go()
{
  string chain_filename = "";
  int chain_num;
  int cr_num;
  double *fit = NULL;
  int gen_num;
  double *gr = NULL;
  int gr_conv;
  int gr_count;
  string gr_filename = "";
  int gr_num;
  double gr_threshold;
  double *jumprate_table = NULL;
  int jumpstep;
  double *limits = NULL;
  int pair_num;
  int par_num;
  int printstep;
  string restart_read_filename = "";
  string restart_write_filename = "";
  double* z = NULL;
  // Prior Info
  int prior_num = 0;
  int* prPtr = NULL;
  vector<double> prAv;
  vector<double> prSd;

  int globRank;
  MPI_Comm_rank(MPI_COMM_WORLD,&globRank);
  const clock_t begin_time = clock();

  // Write Time stamp and Header
  if(globRank == 0){
  	timestamp ( );
    cout << "\n";
    cout << "DREAM\n";
    cout << "  C++ version\n";
    cout << "  MCMC acceleration by Differential Evolution.\n";
  }
//
//  Set the random generator seed different for each processor
//
  if(globRank == 0){
    printf("   SETTING RANDOM GENERATOR SEED FOR ALL PROCESSES.\n");
    fflush(stdout);
  }
  initialize();
  set_initial_seed(globRank+1,5*globRank+1);
//
// Get Info from global and local numbering
//
  int rank_sm = 0;
  int size_sm = 0;
  int totGroups = 0;  
  int num_procs;
  int id;
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD,&id);
  MPI_Status info;
  MPI_Comm comm_sm;
  MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &comm_sm);
  MPI_Comm_rank(comm_sm, &rank_sm);
  MPI_Comm_size(comm_sm, &size_sm);

  // Build Network Topology
  int localProcessNumbers[num_procs];
  int localGroupNumbers[num_procs];
  buildNetworkTopology(num_procs,id,rank_sm,localProcessNumbers,localGroupNumbers,totGroups);

  int mainProcessRanks[totGroups];
  int count = 0;
  for(int loopA=0;loopA<num_procs;loopA++){
    if(localProcessNumbers[loopA] == 0){
      mainProcessRanks[count] = loopA;
      count++;
    }
  }

  // Print Ranks
  if(id == 0){
    printf("MAIN PROCESS RANKS ON EACH NODE\n");
    for(int loopA=0;loopA<totGroups;loopA++){
      printf("%d\n",mainProcessRanks[loopA]);
    }
  }

  // Create Communicator Groups
  MPI_Group MPI_GROUP_WORLD, mainProcessGroup;
  MPI_Comm mainProcessComm;
  MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD); 
  MPI_Group_incl(MPI_GROUP_WORLD, totGroups, mainProcessRanks, &mainProcessGroup);
  MPI_Comm_create(MPI_COMM_WORLD, mainProcessGroup, &mainProcessComm); 
//
//  Get the problem sizes.
//
  problem_size ( chain_num, cr_num, gen_num, pair_num, par_num );
//
//  Decide if the problem sizes are acceptable.
//
  if(chain_num < 3){
    if(globRank == 0){
      cerr << "\n";
      cerr << "DREAM - Fatal error!\n";
      cerr << "  CHAIN_NUM < 3. Please use the serial version of DREAM instead.\n";
    }
    exit(1);
  }
  if (cr_num < 1){
    if(globRank == 0){
      cerr << "\n";
      cerr << "DREAM - Fatal error!\n";
      cerr << "  CR_NUM < 1.\n";
    }
    exit(1);
  }
  if(gen_num < 2){
    if(globRank == 0){
      cerr << "\n";
      cerr << "DREAM - Fatal error!\n";
      cerr << "  GEN_NUM < 2.\n";
    }
    exit(1);
  }
  if(pair_num < 0){
    if(globRank == 0){
      cerr << "\n";
      cerr << "DREAM - Fatal error!\n";
      cerr << "  PAIR_NUM < 0.\n";
    }
    exit(1);
  }
  if(par_num < 1){
    if(globRank == 0){
      cerr << "\n";
      cerr << "DREAM - Fatal error!\n";
      cerr << "  PAR_NUM < 1.\n";
    }
    exit(1);
  }
//
//  Read Prior Information From File if Available
//
  if(usePriorFromFile){    
    if(globRank == 0){
      printf("READING PRIOR INFORMATION FROM FILE.\n");
    }
    // Read Statistics from file
    int error = cmUtils::readPriorFromFile(priorFileName,prior_num,prAv,prSd);    
    if(error != 0){
      if(globRank == 0){
        printf("ERROR: CANNOT READ FILE WITH PRIOR.\n");
        exit(1);
      }
    }
    
    // Get Prior Mapping
    prPtr = new int[prior_num];
    model->getPriorMapping(priorModelType,prPtr);
      
    // PRINT PRIOR INFORMATION READ
    if(globRank == 0){
      printf("\n");
      printf("PRIOR INFORMATION.\n");
      printf("%10s %10s %10s %10s\n","NUM","PTR","AV VALUE","SD VALUE");
      for(int loopA=0;loopA<prior_num;loopA++){
        printf("%10d %10d %10f %10f\n",loopA,prPtr[loopA],prAv[loopA],prSd[loopA]);
      }
      printf("\n");
    }
  }else{
    if(globRank == 0){
      printf("NO PRIOR FILE PROVIDED.\n");
    }
  }
//
//  Get the problem data values;
//
  limits = r8mat_zero_new ( 2, par_num );

  problem_value ( &chain_filename, &gr_filename, gr_threshold, jumpstep, 
    limits, par_num, printstep, &restart_read_filename, 
    &restart_write_filename );
//
//  Print the data as a job record.
//
  input_print ( chain_filename, chain_num, cr_num, gr_filename, gr_threshold, 
    jumpstep, limits, gen_num, pair_num, par_num, 
    printstep, restart_read_filename, restart_write_filename );
//
//  Allocate and zero out memory.
//
  gr_num = gen_num / printstep;

  // Only the main processes (1 for node) stores the iteration quantities
  if(rank_sm == 0){
  	printf("Allocated on %d\n",rank_sm);
  	fflush(stdout);
    fit = r8mat_zero_new   ( chain_num, gen_num );
    gr  = r8mat_zero_new   ( par_num, gr_num );
    z   = r8block_zero_new ( par_num, chain_num, gen_num );
  }
//
//  Set the jump rate table.
//
  jumprate_table = jumprate_table_init ( pair_num, par_num );

  jumprate_table_print ( jumprate_table, pair_num, par_num );

//
//  Initialize the Gelman-Rubin data.
//
  if(rank_sm == 0){
    gr_init ( gr, gr_conv, gr_count, gr_num, par_num );
  }

  if(globRank == 0){
    cout << "\n";
    cout << "GR_PRINT:\n";
    cout << "  GR_CONV  = " << gr_conv << "\n";
    cout << "  GR_COUNT = " << gr_count << "\n";
    cout << "  GR_NUM   = " << gr_num << "\n";
  }  
//
//  Set the first generation of the chains from restart data, or by sampling.
//
  if (restart_read_filename.empty()){
  	// Only Proc 0 Does the Init
    if(globRank == 0){
      cout << "INITIALIZING CHAINS\n";
    }
    chain_init ( chain_num, fit, gen_num, par_num, z,
                 prior_num, prPtr, prAv, prSd, globRank, 
                 comm_sm, mainProcessComm, size_sm, totGroups, rank_sm );
  }else{
    if(globRank == 0){
      cout << "READING RESTARTS?\n";
    }
    restart_read(chain_num,fit,gen_num,par_num,restart_read_filename,z);
  }
  if(globRank == 0){
    cout << "PRINTING CHAINS\n";
  }
  chain_init_print(chain_num,fit,gen_num,par_num,restart_read_filename,z);
//
//  Computing the Statistics of the initial chains for incremental update
//
  if(globRank == 0){
    cout << "COMPUTING INITIAL STATISTICS\n";
  }
  double iniMean[par_num];
  double iniStd[par_num];
  if(rank_sm == 0){
    std_compute_ini (chain_num,1,gen_num,par_num,z,iniMean,iniStd);
  }
//
//  Carry out the DREAM algorithm.
//
  if(globRank == 0){
    cout << "STARTING DREAM ALGORITHM\n";
  }

  dream_algm_mpi (rank_sm, id, num_procs, size_sm, localGroupNumbers, comm_sm, info, totGroups, mainProcessComm,
                  chain_num, cr_num, fit, gen_num, gr, gr_conv, gr_count, 
                  gr_num, gr_threshold, jumprate_table, jumpstep, limits, pair_num, 
                  par_num, printstep, z ,prior_num, prPtr, prAv, prSd, iniMean, iniStd);
//
// Free Groups and Communicators
//
  if(rank_sm == 0){
    MPI_Comm_free(&mainProcessComm); 
    MPI_Group_free(&mainProcessGroup); 
    MPI_Group_free(&MPI_GROUP_WORLD); 
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
//
//  Terminate.
//
  if(globRank == 0){
    cout << "\n";
    cout << "DREAM\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    timestamp ( );
    // Print Final time
    cout << "Total Time: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " s" << endl;
  }

  // Return
  return 0;
}
