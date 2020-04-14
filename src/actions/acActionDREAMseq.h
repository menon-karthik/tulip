#ifndef ACACTIONDREAMSEQ_H
#define ACACTIONDREAMSEQ_H

# include "acActionDREAM.h"

using namespace std;

// Sequential MCMC based on the DREAM algorithm
class acActionDREAMseq: public acActionDREAM{
  
  protected:
  
    void chain_init (int chain_num, double fit[], int gen_num, int par_num, 
                     double z[], int prior_num, int* prPtr, 
                     stdVec prAv, stdVec prSd);
    void chain_init_print (int chain_num, double fit[], int gen_num, 
                           int par_num, string restart_read_filename, double z[]);
    void dream_algm_seq(int chain_num, int cr_num, double fit[], int gen_num, 
                        double gr[], int &gr_conv, int &gr_count, int gr_num, double gr_threshold,
                        double jumprate_table[], int jumpstep, double limits[], int pair_num, 
                        int par_num, int printstep, double z[], 
                        int prior_num, int* prPtr, const stdVec& prAv, const stdVec& prSd);
    void input_print(string chain_filename, int chain_num, int cr_num, 
                     string gr_filename, double gr_threshold, int jumpstep, double limits[], 
                     int gen_num, int pair_num, int par_num, int printstep, 
                     string restart_read_filename, string restart_write_filename);
    void jumprate_table_print(double jumprate_table[], int pair_num, int par_num);
    void restart_read(int chain_num, double fit[], int gen_num, int par_num, string restart_read_filename, double z[]);
    void restart_write(int chain_num, double fit[], int gen_num, int par_num, string restart_write_filename, double z[]);
    void cr_dis_update (int chain_index, int chain_num, double cr_dis[], 
                        int cr_index, int cr_num, int cr_ups[], int gen_index, int gen_num, 
                        int par_num, double z[]);
    double* std_compute(int chain_num, int gen_index, int gen_num, int par_num, double z[]);


  public:

  	acActionDREAMseq(int totChains,
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
    virtual ~acActionDREAMseq();

  	// Virtual Function from acAction
  	virtual int go();
};

#endif //ACACTIONDREAMSEQ_H
