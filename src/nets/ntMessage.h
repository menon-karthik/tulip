#ifndef NTMESSAGE_H
#define NTMESSAGE_H

# include "uqTypes.h"
# include "ntUtils.h"

enum messageTypes{mtFactorToNode,mtNodeToFactor};

class ntMessage{
  public:
    
    // Type of message
  	messageTypes messageType;
  	// Source Entity that sent the message
    int sourceID;
    // Target Entity that received the message
    int targetID;
    // Labels of the variables
    stdStringVec varLabels;
    // Message variable Standard Deviations
    stdVec varSTDs;
    // Message variable limits
    stdVec varLimits;
    // Samples forming the message
    stdMat msg;

    //! Default Constructor
    ntMessage();
    ntMessage(ntMessage* newMsg);
  	ntMessage(messageTypes messageType,
              int sourceID,int targetID,
              const stdStringVec& labels,const stdVec& stds,
              const stdVec& limits,const stdMat& msg);
    //! Virtual Destructor
    virtual ~ntMessage();

    void aggregate(ntMessage* msg);

    // Same samples only the variables are different- Aggregation on the dimensionality
    void aggregateFromList(vector<ntMessage*> msgs);

    // Same variables in every message: aggregate on the sampling
    void aggregateMarginals(vector<ntMessage*> msgs);

    // Add Evidence to message
    void addEvidence(int varID,const stdVec& vals);

    // Compute Standard deviations directly from message samples
    stdVec getSTDFromSamples();


    string getMsgTypeString();

    void writeToFile(string fileName);

    void show();
  
};

#endif // NTMESSAGE_H
