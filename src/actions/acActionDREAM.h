#ifndef ACACTIONDREAM_H
#define ACACTIONDREAM_H

# include <cstdlib>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <cmath>
# include <ctime>
# include <cstring>
# include <vector>

#include <sys/types.h>
#include <dirent.h>
#include <limits>
#include <math.h> 
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

# include <string>
# include <mpi.h>

# include "acAction.h"
# include "pdflib.h"
# include "rnglib.h"
# include "auxlib.h"
# include "cmUtils.h"

using namespace std;

/*! 
This class implements the Differential Evolution adaptive Metropolis algorithm 
to sample from the joint parameter posterior for arbitrary models.
*/
class acActionDREAM: public acAction{
  protected:
  	int totChains;
    // Total Samples Generate in every chain
    int totGenerations;
    // Total number of CR
    int totalCR;
    // Total number of crossover pairs
    int totCrossoverPairs;
    // Threshold for Gelman Rubin convergence criterion
  	double dreamGRThreshold;
  	// Forces a "long jump" every "dreamJumpStep" generations.
  	int dreamJumpStep;
  	// Print interval for Gelman-Rubin statistc
  	int dreamGRPrintStep;
  	// Name of the file storing chain statistics
  	string dreamChainFileName;
  	// Name of the file storing Gelman-Rubin convergence criterion
  	string dreamGRFileName;
    // Restart file to read
  	string dreamRestartReadFileName;
  	// Restart file to write
  	string dreamRestartWriteFileName;
    // Prior Information
    bool usePriorFromFile;
    string priorFileName;
    int priorModelType;
    
    // Private Functions
  	void problem_size(int &chain_num, int &cr_num, int &gen_num, int &pair_num, int &par_num);
  	void problem_value ( string *chain_filename, string *gr_filename, 
    double &gr_threshold, int &jumpstep, double limits[], int par_num, 
    int &printstep, string *restart_read_filename, 
    string *restart_write_filename );
    double* prior_sample (int par_num, int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd);
    double sample_likelihood ( int par_num, double zp[] );
    void chain_init ( int chain_num, double fit[], int gen_num, int par_num, 
                      double z[], int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd, int globRank,
                      MPI_Comm comm_sm, MPI_Comm mainProcessComm, int size_sm, int totGroups, int rank_sm);
    double* sample_candidate ( int chain_index, int chain_num, double cr[], 
    int cr_index, int cr_num, int gen_index, int gen_num, 
    double jumprate_table[], int jumpstep, double limits[], int pair_num, 
    int par_num, double z[] );
    int cr_index_choose ( int cr_num, double cr_prob[] );
    void cr_init ( double cr[], double cr_dis[], int cr_num, double cr_prob[], 
    int cr_ups[] );
    void dream_algm ( int rank_sm, int id, int num_procs, int size_sm, int* localGroupNumbers, 
                      MPI_Comm comm_sm, MPI_Status info, int totGroups, MPI_Comm mainProcessComm,
                      int chain_num, int cr_num, double fit[], int gen_num, 
                      double gr[], int &gr_conv, int &gr_count, int gr_num, double gr_threshold,
                      double jumprate_table[], int jumpstep, double limits[], int pair_num, 
                      int par_num, int printstep, double z[], 
                      int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd,
                      double iniMean[], double iniStd[]);
    void chain_init_print ( int chain_num, double fit[], int gen_num, int par_num, 
    string restart_read_filename, double z[] );
    void restart_read ( int chain_num, double fit[], int gen_num, int par_num, 
    string restart_read_filename, double z[] );
    void gr_init ( double gr[], int &gr_conv, int &gr_count, int gr_num, 
    int par_num );
    void jumprate_table_print ( double jumprate_table[], int pair_num, int par_num );
    double* jumprate_table_init ( int pair_num, int par_num );
    double* r8block_zero_new ( int l, int m, int n );
    double* r8mat_zero_new ( int m, int n );
    void input_print ( string chain_filename, int chain_num, int cr_num, 
    string gr_filename, double gr_threshold, int jumpstep, double limits[], 
    int gen_num, int pair_num, int par_num, int printstep, 
    string restart_read_filename, string restart_write_filename );
    double prior_density ( int par_num, double zp[], int prior_num, int* prPtr, 
    vector<double> prAv, vector<double> prSd );
    void chain_outliers ( int chain_num, int gen_index, int gen_num, int par_num,
    double fit[], double z[] );
    void chain_write ( string chain_filename, int chain_num, double fit[], 
    int gen_num, int par_num, double z[] );
    void cr_dis_update ( int chain_index, int chain_num, double cr_dis[], 
    int cr_index, int cr_num, int cr_ups[], int gen_index, int gen_num, 
    int par_num, double z[], int id, double storedMean[], double storedStd[],
    double currentMean[], double currentStd[]);
    void cr_prob_update ( double cr_dis[], int cr_num, double cr_prob[], 
    int cr_ups[] );
    double* diff_compute ( int chain_num, int gen_index, int gen_num, 
    int jump_dim[], int jump_num, int pair_num, int par_num, int r[], 
    double z[] );
    void filename_inc ( string *filename );
    void gr_compute ( int chain_num, int gen_index, int gen_num, double gr[], 
    int &gr_conv, int &gr_count, int gr_num, double gr_threshold, int par_num, 
    double z[] );
    void gr_write ( double gr[], string gr_filename, int gr_num, int par_num, 
    int printstep );
    void jumprate_choose ( double cr[], int cr_index, int cr_num, int gen_index,
    int jump_dim[], int &jump_num, double &jumprate, double jumprate_table[],
    int jumpstep, int par_num );
    void sample_limits ( double limits[], int par_num, double zp[] );
    // POST
    void writeStatisticsToLatexTable(int par_num,vector<vector<double> > &modelStats);

  public:
  	/*! 
    \verbatim embed:rst
    **Purpose**
    Constructor
    
    **Discussion**
    None
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] totChains total number of parallel Markov chains.
    \param[in] locTotGenerations maximum number of model evaluation for each chain.
    \param[in] locTotalCR total number of crossover probabilities.
    \param[in] loctTotCrossoverPairs number of pairs in crossover chains.
    \param[in] locDreamChainFileName file name prefix for chain outputs.
    \param[in] locDreamGRFileName file name for Gelman-Rubin convergence diagnostics.
    \param[in] locDreamGRThreshold admissible threshold for Gelman-Rubin diagnostics (usually 1.2)
    \param[in] locDreamJumpStep,
    \param[in] locDreamGRPrintStep Maximum number of iteration between Gelman-Rubin diagnostic printout.
    \param[in] locDreamRestartReadFileName restart file in input.
    \param[in] locDreamRestartWriteFileName name of the file where to write the restart.
    \param[in] usePriorFromFile Read prior parameters from file.
    \param[in] priorFileName Name of the file with prior statistical moments. 
    \param[in] priorModelType Type of prior information.
    */
  	acActionDREAM(int totChains,
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
                  int priorModelType);

    /*! 
    \verbatim embed:rst
    **Purpose**
    Post process DREAM chain results
    
    **Discussion**
    Once the chain files are available, this procedure reads all the samples, discards those in the
    burn in (defined through the burnInPercent parameter) and compute the associated statistics (typically first two statistical moments)
    
    **Modified**
    10 January 2016 - DES
    
    **Reference**
    No Reference
    \endverbatim

    \param[in] debugMode write debug information to stdout
    \param[in] burnInPercent percent of initial samples in each chain output to be rejected.
    */
  	int postProcess(bool debugMode, double burnInPercent);

  	// Virtual Function from acAction
  	virtual int go();
};

#endif //ACACTIONDREAM_H
