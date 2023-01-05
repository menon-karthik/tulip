#ifndef NTNODE_H
#define NTNODE_H

# include <boost/algorithm/string.hpp>

# include "uqTypes.h"
# include "uqPDF.h"

# include "ntNetIO.h"
# include "ntFactor.h"
# include "ntMessage.h"
# include "ntUtils.h"
# include "ntConstants.h"
# include "ntException.h"

# include "cmApproximant.h"

# include "acActionUP.h"
# include "acActionUP_MC.h"
# include "acActionDREAMseq.h"

# include "daData_multiple_Table.h"

# include "cm_darpaSimple_code1.h"
# include "cm_darpaSimple_code2.h"
# include "cm_darpaSimple_code3.h"

# include "cmCombineInputOutput.h"

using namespace std;

class ntFactor;

/*! 
Abstract parent class for probabilistic networks in UQ
*/
class ntNode{
  public:
    // Nodes ID
    int nodeID;
    // Number of variables
    int numVariables;
    // Type of dependence encoded in the node
    nodeDependenceTypes nodeType;
    // Number of variables
    int numSamples;
    // Variables Name strings
    stdStringVec varNames;
    // Variable standard deviations
    stdVec varSTD;
    // Variable limits
    stdVec limits;
    // Variable realizations
    stdMat varSamples;
    // Models for deterministic nodes
    modelTypes detVarType;
    string detModelName;

    // Variables for probabilistic nodes
    // Subdivisions of the variables stored in the node
    stdIntVec varSubdiv;
    // Total inputs to the node, the size of the CPT clearly depends on the number of inputs
    int totalInputs;
    // Total subdivisions for each input variable
    stdIntVec inputSubdiv;
    // Name of the input variables
    stdStringVec inputNames;
    // Conditional probability table (CPT)
    // The CPT stores a single line for each possible input variable combination with the value of the input variables
    // plus as many PMF as variables stored in the probabilistic node
    stdMat CPT;

    // Evidence: variable ID and evidence value
    stdIntVec evidenceVarID;
    stdVec evidenceVarAvg;
    stdVec evidenceVarStd;
    // List of pointers to the factors
    vector<ntFactor*> nodeFactors;
    stdBoolVec isDownstreamFactor;
    // List of incoming messages for factor
    vector<ntMessage*> inMsgs;

    // Gaussian and uniform samplers
    uqPDF* nSampler;
    uqPDF* uSampler;

    // Approximant 
    cmModel* nodeModel;

    // Approximant 
    bool hasMarginal;
    ntMessage* marginal;

  	//! Default Constructor
  	ntNode(int nodeID,ntNetIO* netIO);
    //! Virtual Destructor
    virtual ~ntNode();

    // Find incoming message for a given factor ID
    bool findInMsg(int factorID);
    // Check that all messages are present to send a message to factor factorID
    bool messagesAreReadyFor(int factorID);
    bool messagesAreReadyFor_2(int factorID); // DEBUG
    // Check that all incoming messages are present
    bool hasProcessedAllMsgs();

    // Get message from node
    ntMessage* copyInMsg(int targetFactorID);

    // Check if variable name belongs to node and get its idx
    bool isNodeVariableName(string name);
    int getNodeInputVariableNameIdx(string name);

    // Check if the node has evidence
    bool hasEvidence();

    // Forward and inverse Uncertainty Propagation
    ntMessage* forwardUQ(ntMessage* currMsg);
    ntMessage* inverseUQ(const stdStringVec& upNames,
                         const stdVec& upSTD,
                         const stdVec& upLimits,
                         ntMessage* currMsg);

    // Forward and inverse propagation through CPT
    ntMessage* forwardCPT(ntMessage* currMsg);
    ntMessage* InverseCPT(const stdStringVec& upNames,
                          const stdVec& upSTD,
                          const stdVec& upLimits,
                          ntMessage* currMsg);

    // Update message 
    void updateMsg(int nodeID,ntMessage* currMsg);

    // Check if variable is unobserved
    bool varIsUnobserved(int varID);

    // Random sample the unobserved variables from the limits
    stdMat uniformSampleUnobserved(const stdMat& msg);

    // Utilities
    void appendToNodeFactors(const vector<ntFactor*>& factors,const stdBoolVec& isDownFactor);

    // Propagate from the node to the factors that are connected
    // Return false if 
    void sendMsgToFactors();

    void printMessages();

    void computeMarginal();

    void getPMFSfromCPT(const stdVec& sample,stdMat& pmfs);

    void prepareIO(ntMessage* currMsg, stdBoolVec& isOutCol, stdIntVec& inColIdx);
    int findOutputIDX(string output);
    int getoutputColumn(string output, double sample);

    void extractCPTColumnPMFFromSample(const stdVec& sample, 
                                       const stdStringVec& msgVarNames, 
                                       const stdBoolVec& isOutCol, 
                                       const stdIntVec& inColIdx, 
                                       stdVec& rowIDX, 
                                       stdVec& pmf);
};

#endif // NTNODE_H
