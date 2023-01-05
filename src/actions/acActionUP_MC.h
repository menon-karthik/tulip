# ifndef ACACTIONUP_MC_H
# define ACACTIONUP_MC_H

# include "acActionUP.h"
# include "acException.h"
# include "uqSamples.h"

using namespace std;

// OPTIONS FOR MC ACTION
struct mcOptions{
  // Number of Initial Samples In Domain
  int numberOfRepeats;
  // Sample groups
  stdIntVec sampleGroups;
  // Store Samples 
  bool storeSamples;
  // Generate Samples within the MC loop
  // This means you can use only one group of samples
  bool useExistingSamples;
};

// GENERIC CLASS FOR UNCERTAINTY PROPAGATION ACTION
class acActionUP_MC: public acActionUP{
  public:
    // Options for MC Sampler
    mcOptions opts;

    // Store First and Second Statistic 
    // by Sample number, repeat and output quantity
    // avgStats[i][j][k] is the average value for 
    // repeat i, sample group j and output k.
    vector<stdMat> avgStats;
    vector<stdMat> stdStats;

    // Storage for simulation inputs and outputs
    stdMat all_inputs;
    stdMat all_outputs;

  	// Constructor
  	acActionUP_MC(uqSamples* locInputs);
    virtual ~acActionUP_MC();

    // Print Stored Statistics to File
    void printStatsToFile(string fileName);

    // Construct stochastic approximation or evaluate samples
    virtual int go();
    // Get Approximant
    virtual uqApproximant* getApproximant();
    // Get Statistics
    virtual stdMat getStatistics();
};

# endif //ACACTIONUP_H
