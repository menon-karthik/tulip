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

  	// Constructor
  	acActionUP_MC(uqSamples* locInputs);

  	// Get Statistics
  	double getStats(int index){return stats[index];}

  	// Perform Action
  	virtual int go();

    // Print Stored Statistics to File
    void printStatsToFile(string fileName);
};

# endif //ACACTIONUP_H
