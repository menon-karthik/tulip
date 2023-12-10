#ifndef ACACTIONDREAMMPI_H
#define ACACTIONDREAMMPI_H

# include "acActionDREAM.h"

using namespace std;

/*! 
This class implements the Differential Evolution adaptive Metropolis algorithm 
to sample from the joint parameter posterior for arbitrary models.
*/
class acActionDREAMmpi: public acActionDREAM{
  protected:
    void dream_algm_mpi (int rank_sm, int id, int num_procs, int size_sm, int* localGroupNumbers, 
                         MPI_Comm comm_sm, MPI_Status info, int totGroups, MPI_Comm mainProcessComm,
                         int chain_num, int cr_num, double fit[], int gen_num, 
                         double gr[], int &gr_conv, int &gr_count, int gr_num, double gr_threshold,
                         double jumprate_table[], int jumpstep, double limits[], int pair_num, 
                         int par_num, int printstep, double z[], 
                         int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd,
                         double iniMean[], double iniStd[], int res_num, double outputs[]);
    void chain_init_print(int chain_num, double fit[], int gen_num, int par_num, string restart_read_filename, double z[]);
    void jumprate_table_print(double jumprate_table[], int pair_num, int par_num);
    void input_print(string chain_filename, int chain_num, int cr_num, 
                     string gr_filename, double gr_threshold, int jumpstep, double limits[], 
                     int gen_num, int pair_num, int par_num, int printstep, 
                     string restart_read_filename, string restart_write_filename);
    void chain_init(int chain_num, double fit[], int gen_num, int par_num, 
                    double z[], int prior_num, int* prPtr, vector<double> prAv, vector<double> prSd, int globRank,
                    MPI_Comm comm_sm, MPI_Comm mainProcessComm, int size_sm, int totGroups, int rank_sm, int res_num, 
                    double outputs[]);
    void cr_dis_update (int chain_index, int chain_num, double cr_dis[], 
                        int cr_index, int cr_num, int cr_ups[], int gen_index, int gen_num, 
                        int par_num, double z[], double currentStd[]);

  
  public:
  	acActionDREAMmpi(int totChains,
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
    virtual ~acActionDREAMmpi();

  	// Virtual Function from acAction
  	virtual int go();
};

#endif //ACACTIONDREAMMPI_H
