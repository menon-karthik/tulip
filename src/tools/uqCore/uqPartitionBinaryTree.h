#ifndef UQPARTITIONBINARYTREE_H
#define UQPARTITIONBINARYTREE_H

# include <math.h>

# include "uqPartition.h"
# include "uqTypes.h"
# include "uqSamples.h"

// Need Forward Declaration to Resolve Circular Reference
class uqSamples;

// GENERIC PARTITION NODE
class uqPartitionNode{
  protected:
  	// Vectors with limits
  	stdVec limits;
    // Volume: accounts for the probability
    // measure defined on the partition
    double volume;
  	// Pointer to the left subdivision
    uqPartitionNode* left;
    // Pointer to the right subdivision
    uqPartitionNode* right;
    // Need to Refine partition
    bool isConverged;
    // Set Scalar Value
    double scalar;
    // Set Integer
    int intValue;
    // Stats for partition
    double stats[2];
    // Vectors to store error metrics for this partition
    stdVec partErrorMetrics;
    stdIntVec partErrorIdx;
    // Total Size of the multivariate Basis
    int msTotBasis;
    // Quadrature Order for Multiwavelet Basis
    int mwQuadOrder;
    // Vector of Expansion Coefficients
    stdVec chaosCoeffs;
    // Store the marginals
    stdMat measure;    
    // Store the multiindex of the approximation Matrix
    stdIntMat multiIndex;
    // Residual Norm for the current partition
    double residualNorm;
    // Has Zero Samples
    bool hasZeroSamples;
    // Max Partition Sample ID for Quasi Random Sampling
    int maxSampleIdx;
  public:

  	// Constructor
  	uqPartitionNode(uqSamples* sample);
    // Other Constructor
    uqPartitionNode();

    // Get and Set The Scalar
    double getScalar(){return scalar;};
    void   setScalar(double value){scalar = value;};

    // Get and Set the Integer
    double getInteger(){return intValue;};
    void   setInteger(int value){intValue = value;};

    // Refinement Needed
    bool hasConverged(){return isConverged;};
    void setConvergence(bool conv){isConverged = conv;};

    // Statistics
    double getStats(int idx){return stats[idx];};
    void   setStats(int idx,double val){stats[idx] = val;};

    // Get and Set Partition Error Metrics
    void setPartitionErrorMetrics(stdVec values,stdIntVec indexes);
    void getPartitionErrorMetrics(stdVec& values,stdIntVec& indexes);

    // Get and Set Expansion Coefficients
    void setChaosCoefficients(stdVec coeffs);
    void getChaosCoefficients(stdVec& coeffs);

    // Get and Set Expansion Coefficients
    void getMeasure(stdMat& currMeasure);
    void setMeasure(stdMat currMeasure);

    // Get and Set Expansion Coefficients
    void getMultiIndex(stdIntMat& mi);
    void setMultiIndex(stdIntMat mi);

    // Get and Set Expansion Coefficients
    void getMSTotBasis(int& msBasis);
    void setMSTotBasis(int msBasis);

    // Get and Set Quadrature Order
    void getMWQuadOrder(int& mwQuadOrder);
    void setMWQuadOrder(int mwQuadOrder);
 
    // Get and set residual norm
    double getResidualNorm();
    void   setResidualNorm(double resNorm);

    // Get and Set the fact that the partition was constructed with zero samples
    bool getHasZeroSamples();
    void setHasZeroSamples(bool flag);

    // Get and set the maximum integer number for Quasi Random Sampling
    int getQuasiRandomSampleId(){return maxSampleIdx;};
    void setQuasiRandomSampleId(int maxId){maxSampleIdx = maxId;};

    // Get Limits
  	stdVec getLimits();

    // Count Samples On partition
    // Includes samples are the border
    int countSamplesOnPartition(uqSamples* samples);

    // Get min size
    double getMinSize();
    void   getMaxSizeAndDimension(double& maxPartSize,int& dimMaxSize);
    
    // Get volume
    double getVolume();
    void setVolume(double vol);

    // Get Left and Right Node
  	uqPartitionNode* getLeft(){return left;}
  	uqPartitionNode* getRight(){return right;}

    // Refine partition
  	void refine(int dimToRefine);      
};


// BINARY TREE PARTITION
class uqPartitionBinaryTree: public uqPartition{
  protected:
    uqPartitionNode* root;
    vector<uqPartitionNode*> allNodes;
  public:

  	// Constructor
  	uqPartitionBinaryTree(uqSamples* samples);

    // Access to the elements of allNodes
  	uqPartitionNode* getAllNodes(int index);
  	void AddToAllNodes(uqPartitionNode* node){allNodes.push_back(node);}

    // Get number of leaves
  	int getLeafCount();

    // Get index of leaves in allNodes vector
  	stdIntVec getLeafIndex();

    // Refine partition
    void printToScreen();
    void printLeavesToFile(string outFileName);

};

#endif // UQPARTITIONBINARYTREE_H
