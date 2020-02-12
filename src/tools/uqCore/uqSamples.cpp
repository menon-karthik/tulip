# include "uqSamples.h"

using namespace std;

// CONSTRUCTOR
uqSamples::uqSamples(){
  totSamples = 0;
  totDims = 0;
  areIndependent = true;
}

// CONSTRUCTOR
uqSamples::uqSamples(int dimNum){
  totSamples = 0;
  totDims = dimNum;
  areIndependent = true;
}

uqSamples::uqSamples(stdVec onePoint){
  totSamples = 0;
  totDims = onePoint.size();
  addOneSample(onePoint);
  areIndependent = true;
}

// COPY CONSTRUCTOR
uqSamples::uqSamples(uqSamples* sample){

  // Copy samples and dimensionality
  this->totSamples = sample->totSamples;
  this->totDims = sample->totDims;

  stdMat otherValues;
  sample->getValues(otherValues);
  stdMat otherWeights = sample->getWeights();  
   
  // Copy Values
  stdVec temp;
  for(int loopA=0;loopA<otherValues.size();loopA++){
    temp.clear();
    for(int loopB=0;loopB<otherValues[loopA].size();loopB++){
      temp.push_back(otherValues[loopA][loopB]);
    }
    this->values.push_back(temp);
  }

  // Copy Weights
  for(int loopA=0;loopA<otherWeights.size();loopA++){
    temp.clear();
    for(int loopB=0;loopB<otherWeights[loopA].size();loopB++){
      temp.push_back(otherWeights[loopA][loopB]);
    }
    this->weights.push_back(temp);
  }

  // Copy RVS
  this->areIndependent = sample->areIndependent;  
  this->rvs.resize(sample->rvs.size());
  for(int loopA=0;loopA<sample->rvs.size();loopA++){
    this->rvs[loopA].name = sample->rvs[loopA].name;
    this->rvs[loopA].type = sample->rvs[loopA].type;
    this->rvs[loopA].parameter1 = sample->rvs[loopA].parameter1;
    this->rvs[loopA].parameter2 = sample->rvs[loopA].parameter2;
  }

  // Copy Cholesky factor of Covariance Matrix
  if(!sample->areIndependent){
    for(size_t loopA=0;loopA<sample->corrCholFactor.size();loopA++){
      temp.clear();
      for(size_t loopB=0;loopB<sample->corrCholFactor[loopA].size();loopA++){
        temp.push_back(corrCholFactor[loopA][loopB]);
      }
      this->corrCholFactor.push_back(temp);
    }
  }
}

// DESTRUCTOR
uqSamples::~uqSamples(){
  values.clear();
  corrValues.clear();
  weights.clear();
  rvs.clear();
  covCholFactor.clear();
  corrCholFactor.clear();
}

// COPY VALUE MATRIX
void uqSamples::getValues(stdMat& copyVals){
  stdVec tmp;
  copyVals.clear();
  for(int loopA=0;loopA<values.size();loopA++){
    tmp.clear();
    for(int loopB=0;loopB<values[loopA].size();loopB++){
      tmp.push_back(values[loopA][loopB]);
    }
    copyVals.push_back(tmp);
  }
}

// SET DEPENDENCE
void uqSamples::setSampleIndependence(bool flag){
  areIndependent = flag;
}

// EASY ACCESS TO VALUES
double uqSamples::operator()(const int nRow, const int nCol){
  double res = 0.0;
  if(areIndependent){
    res = values[nRow][nCol];
  }else{
    res = corrValues[nRow][nCol];
  }
  return res;
}

// ADD SINGLE SAMPLE
void uqSamples::addOneSample(stdVec sample){
  totSamples++;
  values.push_back(sample);
}

// ================================================
// SUBSAMPLE INTEGER ARRAY USING VARIOUS APPROACHES
// ================================================
void generateIndexes(int totSamples,int size,int extractionType,int bufferSize,stdIntVec &indexes){
  
  // Create Initial Array
  for(int loopA=0;loopA<totSamples;loopA++){
    indexes.push_back(loopA);
  }
  
  // Remove ends if needed
  switch(extractionType){
    case kExtractRandomWithInitialBuffer:
      indexes.erase(indexes.begin(),indexes.begin()+bufferSize);
      break;
    case kExtractRandomWithFinalBuffer:
      indexes.erase(indexes.end()-bufferSize,indexes.end());
      break;
    case kExtractRandomWithBothEndsBuffer:
      indexes.erase(indexes.begin(),indexes.begin()+floor(bufferSize/2));
      indexes.erase(indexes.end()-floor(bufferSize/2),indexes.end());
      break;
  }
  
  // Extract Sequential or Random
  if(extractionType == kExtractSequential){
    stdIntVec newIndexes;
    int currIndex = 0;
    for(int loopA=0;loopA<size;loopA++){
      currIndex = loopA*(totSamples-1)/(size-1);
      newIndexes.push_back(indexes[currIndex]);
    }
    indexes.resize(size);
    for(int loopA=0;loopA<size;loopA++){
      indexes[loopA] = newIndexes[loopA];
    }

  }else{
    printf("RANDOM SHUFFLE\n");
    // Use built-in random generator
    random_shuffle(indexes.begin(),indexes.end());
    // Reduce to size
    indexes.erase(indexes.end()-(totSamples-size),indexes.end());
  }
}

// =====================
// EXTRACT SAMPLE SUBSET
// =====================
uqSamples uqSamples::extractSamples(int size,int extractionType,int extractionTypeAux,stdIntVec &indexes){
  // Crate New object with a subset of samples
  uqSamples result(totDims);
  stdVec sample;
  // Generate a Vector with the indexes
  indexes.clear();
  int currIndex = 0;
  generateIndexes(totSamples,size,extractionType,extractionTypeAux,indexes);
  // Fill values for result uqSamples
  for(int loopA=0;loopA<size;loopA++){
    currIndex = indexes[loopA];
    sample.clear();
    for(int loopB=0;loopB<totDims;loopB++){
      sample.push_back(values[currIndex][loopB]);  
    }
    result.addOneSample(sample);
  }
  return result;
}

// =========================
// EXTRACT SAMPLE WITH INDEX
// =========================
uqSamples uqSamples::extractSamplesWithIndex(stdIntVec indexes){
  // Crate New object with a subset of samples
  uqSamples result(totDims);
  stdVec sample;
  // Generate a Vector with the indexes
  int currIndex = 0;
  for(int loopA=0;loopA<indexes.size();loopA++){
    currIndex = indexes[loopA];
    sample.clear();
    for(int loopB=0;loopB<totDims;loopB++){
      sample.push_back(values[currIndex][loopB]);  
    }
    result.addOneSample(sample);
  }
  return result;
}


// ==============
// READ FROM FILE
// ==============
void uqSamples::readFromFile(string outMatFile,bool skipFirstLine){
  
  // Declare input File
  ifstream infile;
  infile.open(outMatFile.c_str());

  // Check if file has problems
  if (infile.fail()){
    printf("ERROR: Opening sample file: %s\n",outMatFile.c_str());
    exit(1);
  }

  // Read Data From File
  std::string buffer;
  std::vector<string> tokenizedString;
  stdVec storeX;
  
  // Initialize
  int lineCount = 0;
  
  // Skip First Row If Required
  if(skipFirstLine){
    std::getline(infile,buffer);
  }

  // Loop through the file
  while (std::getline(infile,buffer)){

    // Clear Row Storage
    storeX.clear();
    
    // Increment line count
    lineCount++;
    
    // Trim String
    trim(buffer);
    if((buffer != "")&&(buffer.at(0) != '#')){
      
      // Tokenize String
      schSplit(buffer,tokenizedString);
      
      // Assign Total Number of dimensions for first line
      if(lineCount == 1){
        totDims = tokenizedString.size();
      }
      
      // Read Strings In
      for(int loopA=0;loopA<tokenizedString.size();loopA++){
        storeX.push_back(atof(tokenizedString[loopA].c_str()));
      }

    }
    addOneSample(storeX);
  }
  // Close File
  infile.close();
}

// =====================
// PRINT WEIGHTS TO FILE
// =====================
void uqSamples::printWeightsToFile(string outMatFile,bool printTitle){
  FILE *f;
  f = fopen(outMatFile.c_str(),"w");    
  // Print Basis Matrix
  if(printTitle){
    fprintf(f,"Total Weights: %d;Dims: %d\n",(int)weights.size(),(int)weights[0].size());  
  }
  for(int loopA=0;loopA<weights.size();loopA++){
    for(int loopB=0;loopB<weights[0].size();loopB++){
      fprintf(f,"%15.6e ",weights[loopA][loopB]);  
    }
    fprintf(f,"\n");
  }
  fclose(f);
}
// =============
// PRINT TO FILE
// =============
void uqSamples::printToFile(string outMatFile,bool printTitle){
  FILE *f;
  f = fopen(outMatFile.c_str(),"w");    
  // Print Basis Matrix
  if(printTitle){
    fprintf(f,"Total Samples: %d;Dims: %d\n",totSamples,totDims);  
  }
  // Print Random Samples
  for(int loopA=0;loopA<getTotSamples();loopA++){
    for(int loopB=0;loopB<getTotDims();loopB++){
      if(areIndependent){
        fprintf(f,"%15.6e ",values[loopA][loopB]);    
      }else{
        fprintf(f,"%15.6e ",corrValues[loopA][loopB]);
      }
    }
    fprintf(f,"\n");
  }
  fclose(f);
}

// =================
// NORMALIZE COLUMNS
// =================
stdMat uqSamples::normalizeColumns(bool symmetric){
  // Allocate Result
  stdMat result;
  double minVal = 0.0;
  double maxVal = 0.0;
  result.resize(2);
  for(int loopA=0;loopA<2;loopA++){
    result[loopA].resize(totDims);
  }
  // printf("totDims: %d, totSamples: %d\n",totDims,totSamples);
  // Get and Store Maximum and Minimum Value for each Columns
  for(int loopA=0;loopA<totDims;loopA++){
    result[0][loopA] = values[0][loopA];
    result[1][loopA] = values[0][loopA];
    for(int loopB=1;loopB<totSamples;loopB++){
      if(values[loopB][loopA] > result[1][loopA]){
        result[1][loopA] = values[loopB][loopA];
      }
      if(values[loopB][loopA] < result[0][loopA]){
        result[0][loopA] = values[loopB][loopA];
      }
    }
  }
  // printf("Max values for each column found!\n");

  // Rescale Values
  for(int loopA=0;loopA<totDims;loopA++){
    // Scale Symmetrically
    if(symmetric){
      if(fabs(result[1][loopA])>fabs(result[0][loopA])){
        minVal = -fabs(result[1][loopA]);
        maxVal = fabs(result[1][loopA]);
        result[0][loopA] = -fabs(result[1][loopA]);
      }else{
        minVal = -fabs(result[0][loopA]);
        maxVal = fabs(result[0][loopA]);
        result[1][loopA] = fabs(result[0][loopA]);
      }
    }else{
      minVal = result[0][loopA];
      maxVal = result[1][loopA];     
    }
    // SCALE VALUE
    for(int loopB=0;loopB<totSamples;loopB++){            
      values[loopB][loopA] = 2.0*((values[loopB][loopA]-minVal)/(maxVal-minVal))-1.0;
    }
  }
  // Return
  return result;
}

// =======================
// ADD VARIABLE DEFINITION
// =======================
void uqSamples::addVariable(string varName,int varType,double par1,double par2){
  // Increase the Dimensionality
  totDims += 1;
  // Perform Checks
  if(varType == kSAMPLEUniform){
    if(par1 > par2){
      printf("Warning: Parameter swap for variable %s\n",varName.c_str());
      int temp = 0.0;
      temp = par1;
      par1 = par2;
      par2 = temp;
    }
  }
  // Create New Record
  variableRecord myVar;
  // Assign Values
  myVar.name = varName;
  myVar.type = varType;
  myVar.parameter1 = par1;
  myVar.parameter2 = par2;
  // Add to list
  rvs.push_back(myVar);
}

// ==========================================
// GENERATE SAMPLES FROM VARIABLE INFORMATION
// ==========================================
void uqSamples::generateRandomSamples(int numSamples, int seed){
  // Clear available samples
  values.clear();
  stdVec storeVec;
  double currValue = 0.0;
  double m2Val = 0.0;
  double vVal = 0.0;
  double lnAV = 0.0;
  double lnSD = 0.0;

  // If required Init Library
  if(seed > 0){
    set_seed(seed,seed + 100);
  }

  // Loop through the new samples
  for(int loopA=0;loopA<numSamples;loopA++){
    storeVec.clear();
    // Loop through the variable types
    for(int loopB=0;loopB<rvs.size();loopB++){
      // Generate new value
      switch(rvs[loopB].type){
        case kSAMPLEUniform:
          currValue = r8_uniform_sample(rvs[loopB].parameter1,rvs[loopB].parameter2);
          break;
        case kSAMPLEGaussian:
          currValue = r8_normal_sample(rvs[loopB].parameter1,rvs[loopB].parameter2);
          break;
        case kSAMPLELognormal:
          // Store parameters
          m2Val = rvs[loopB].parameter1*rvs[loopB].parameter1;
          vVal = rvs[loopB].parameter2*rvs[loopB].parameter2;
          // Get lognormal parameters
          lnAV = log(m2Val/(sqrt(vVal + m2Val)));
          lnSD = sqrt(log(1.0 + (vVal/m2Val)));
          currValue = exp(r8_normal_sample(lnAV,lnSD));
          break;
        case kSAMPLEConstant:
          currValue = rvs[loopB].parameter1;
          break;
      }
      // Add value to vector
      storeVec.push_back(currValue);
    }
    // Add to existing samples
    values.push_back(storeVec);
  }
  // Assign Samples and number of dimensions
  totSamples = numSamples;
  totDims = rvs.size();
  // Reset Integration Weights: They are only for grids
  weights.clear();
}

// Prime Number Generator from table
int getPrimeInteger(int n){

  int result = 0;

  const int PRIME_MAX = 1600;

  int npvec[PRIME_MAX] = {
        2,    3,    5,    7,   11,   13,   17,   19,   23,   29,
       31,   37,   41,   43,   47,   53,   59,   61,   67,   71,
       73,   79,   83,   89,   97,  101,  103,  107,  109,  113,
      127,  131,  137,  139,  149,  151,  157,  163,  167,  173,
      179,  181,  191,  193,  197,  199,  211,  223,  227,  229,
      233,  239,  241,  251,  257,  263,  269,  271,  277,  281,
      283,  293,  307,  311,  313,  317,  331,  337,  347,  349,
      353,  359,  367,  373,  379,  383,  389,  397,  401,  409,
      419,  421,  431,  433,  439,  443,  449,  457,  461,  463,
      467,  479,  487,  491,  499,  503,  509,  521,  523,  541,
      547,  557,  563,  569,  571,  577,  587,  593,  599,  601,
      607,  613,  617,  619,  631,  641,  643,  647,  653,  659,
      661,  673,  677,  683,  691,  701,  709,  719,  727,  733,
      739,  743,  751,  757,  761,  769,  773,  787,  797,  809,
      811,  821,  823,  827,  829,  839,  853,  857,  859,  863,
      877,  881,  883,  887,  907,  911,  919,  929,  937,  941,
      947,  953,  967,  971,  977,  983,  991,  997, 1009, 1013,
     1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069,
     1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151,
     1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,
     1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291,
     1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373,
     1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451,
     1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,
     1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583,
     1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
     1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733,
     1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,
     1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
     1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987,
     1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053,
     2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,
     2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213,
     2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
     2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357,
     2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,
     2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531,
     2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617,
     2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687,
     2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,
     2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819,
     2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887, 2897, 2903,
     2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999,
     3001, 3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079,
     3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167, 3169, 3181,
     3187, 3191, 3203, 3209, 3217, 3221, 3229, 3251, 3253, 3257,
     3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331,
     3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413,
     3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491, 3499, 3511,
     3517, 3527, 3529, 3533, 3539, 3541, 3547, 3557, 3559, 3571,
     3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637, 3643,
     3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727,
     3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821,
     3823, 3833, 3847, 3851, 3853, 3863, 3877, 3881, 3889, 3907,
     3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947, 3967, 3989,
     4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057,
     4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139,
     4153, 4157, 4159, 4177, 4201, 4211, 4217, 4219, 4229, 4231,
     4241, 4243, 4253, 4259, 4261, 4271, 4273, 4283, 4289, 4297,
     4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,
     4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493,
     4507, 4513, 4517, 4519, 4523, 4547, 4549, 4561, 4567, 4583,
     4591, 4597, 4603, 4621, 4637, 4639, 4643, 4649, 4651, 4657,
     4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751,
     4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831,
     4861, 4871, 4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937,
     4943, 4951, 4957, 4967, 4969, 4973, 4987, 4993, 4999, 5003,
     5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087,
     5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179,
     5189, 5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279,
     5281, 5297, 5303, 5309, 5323, 5333, 5347, 5351, 5381, 5387,
     5393, 5399, 5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443,
     5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521,
     5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639,
     5641, 5647, 5651, 5653, 5657, 5659, 5669, 5683, 5689, 5693,
     5701, 5711, 5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791,
     5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851, 5857,
     5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939,
     5953, 5981, 5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053,
     6067, 6073, 6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133,
     6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211, 6217, 6221,
     6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301,
     6311, 6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367,
     6373, 6379, 6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473,
     6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563, 6569, 6571,
     6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673,
     6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761,
     6763, 6779, 6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833,
     6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907, 6911, 6917,
     6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991, 6997,
     7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103,
     7109, 7121, 7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207,
     7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253, 7283, 7297,
     7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369, 7393, 7411,
     7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499,
     7507, 7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561,
     7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621, 7639, 7643,
     7649, 7669, 7673, 7681, 7687, 7691, 7699, 7703, 7717, 7723,
     7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829,
     7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919,
     7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009, 8011, 8017,
     8039, 8053, 8059, 8069, 8081, 8087, 8089, 8093, 8101, 8111,
     8117, 8123, 8147, 8161, 8167, 8171, 8179, 8191, 8209, 8219,
     8221, 8231, 8233, 8237, 8243, 8263, 8269, 8273, 8287, 8291,
     8293, 8297, 8311, 8317, 8329, 8353, 8363, 8369, 8377, 8387,
     8389, 8419, 8423, 8429, 8431, 8443, 8447, 8461, 8467, 8501,
     8513, 8521, 8527, 8537, 8539, 8543, 8563, 8573, 8581, 8597,
     8599, 8609, 8623, 8627, 8629, 8641, 8647, 8663, 8669, 8677,
     8681, 8689, 8693, 8699, 8707, 8713, 8719, 8731, 8737, 8741,
     8747, 8753, 8761, 8779, 8783, 8803, 8807, 8819, 8821, 8831,
     8837, 8839, 8849, 8861, 8863, 8867, 8887, 8893, 8923, 8929,
     8933, 8941, 8951, 8963, 8969, 8971, 8999, 9001, 9007, 9011,
     9013, 9029, 9041, 9043, 9049, 9059, 9067, 9091, 9103, 9109,
     9127, 9133, 9137, 9151, 9157, 9161, 9173, 9181, 9187, 9199,
     9203, 9209, 9221, 9227, 9239, 9241, 9257, 9277, 9281, 9283,
     9293, 9311, 9319, 9323, 9337, 9341, 9343, 9349, 9371, 9377,
     9391, 9397, 9403, 9413, 9419, 9421, 9431, 9433, 9437, 9439,
     9461, 9463, 9467, 9473, 9479, 9491, 9497, 9511, 9521, 9533,
     9539, 9547, 9551, 9587, 9601, 9613, 9619, 9623, 9629, 9631,
     9643, 9649, 9661, 9677, 9679, 9689, 9697, 9719, 9721, 9733,
     9739, 9743, 9749, 9767, 9769, 9781, 9787, 9791, 9803, 9811,
     9817, 9829, 9833, 9839, 9851, 9857, 9859, 9871, 9883, 9887,
     9901, 9907, 9923, 9929, 9931, 9941, 9949, 9967, 9973,10007,
    10009,10037,10039,10061,10067,10069,10079,10091,10093,10099,
    10103,10111,10133,10139,10141,10151,10159,10163,10169,10177,
    10181,10193,10211,10223,10243,10247,10253,10259,10267,10271,
    10273,10289,10301,10303,10313,10321,10331,10333,10337,10343,
    10357,10369,10391,10399,10427,10429,10433,10453,10457,10459,
    10463,10477,10487,10499,10501,10513,10529,10531,10559,10567,
    10589,10597,10601,10607,10613,10627,10631,10639,10651,10657,
    10663,10667,10687,10691,10709,10711,10723,10729,10733,10739,
    10753,10771,10781,10789,10799,10831,10837,10847,10853,10859,
    10861,10867,10883,10889,10891,10903,10909,10937,10939,10949,
    10957,10973,10979,10987,10993,11003,11027,11047,11057,11059,
    11069,11071,11083,11087,11093,11113,11117,11119,11131,11149,
    11159,11161,11171,11173,11177,11197,11213,11239,11243,11251,
    11257,11261,11273,11279,11287,11299,11311,11317,11321,11329,
    11351,11353,11369,11383,11393,11399,11411,11423,11437,11443,
    11447,11467,11471,11483,11489,11491,11497,11503,11519,11527,
    11549,11551,11579,11587,11593,11597,11617,11621,11633,11657,
    11677,11681,11689,11699,11701,11717,11719,11731,11743,11777,
    11779,11783,11789,11801,11807,11813,11821,11827,11831,11833,
    11839,11863,11867,11887,11897,11903,11909,11923,11927,11933,
    11939,11941,11953,11959,11969,11971,11981,11987,12007,12011,
    12037,12041,12043,12049,12071,12073,12097,12101,12107,12109,
    12113,12119,12143,12149,12157,12161,12163,12197,12203,12211,
    12227,12239,12241,12251,12253,12263,12269,12277,12281,12289,
    12301,12323,12329,12343,12347,12373,12377,12379,12391,12401,
    12409,12413,12421,12433,12437,12451,12457,12473,12479,12487,
    12491,12497,12503,12511,12517,12527,12539,12541,12547,12553,
    12569,12577,12583,12589,12601,12611,12613,12619,12637,12641,
    12647,12653,12659,12671,12689,12697,12703,12713,12721,12739,
    12743,12757,12763,12781,12791,12799,12809,12821,12823,12829,
    12841,12853,12889,12893,12899,12907,12911,12917,12919,12923,
    12941,12953,12959,12967,12973,12979,12983,13001,13003,13007,
    13009,13033,13037,13043,13049,13063,13093,13099,13103,13109,
    13121,13127,13147,13151,13159,13163,13171,13177,13183,13187,
    13217,13219,13229,13241,13249,13259,13267,13291,13297,13309,
    13313,13327,13331,13337,13339,13367,13381,13397,13399,13411,
    13417,13421,13441,13451,13457,13463,13469,13477,13487,13499};

  if((n >= 0)&&(n <= PRIME_MAX)){
    result = npvec[n];
  }else{
    uqException("Error: Required prime number exceeds storage...\n");
  }
  return result;
}

// Generate Halton Sequence: Be Careful in High (14+) dimensions
double evalOneDHalton(int index,int base){
  double h = 0.0;
  double fac = 1./base;
  int digit = 0;
  while(index > 0){
    digit = index % base;
    h = h + digit*fac;
    index = (index-digit)/base;
    fac = fac/base;
    //printf("digit: %d, h: %f, index: %d, fac: %f\n",digit,h,index,fac);
    //getchar();
  }
  return h;
}

// Scale the limits such that rootLimits is an hypercube
void scaleLimitsForQuasiRandomSequence(stdVec rootLimits,stdVec& limits){
  for(int loopA=0;loopA<(int)limits.size()/2;loopA++){
    limits[2*loopA + 0] = (limits[2*loopA + 0]-rootLimits[2*loopA + 0])/(double)(rootLimits[2*loopA + 1] - rootLimits[2*loopA + 0]);
    limits[2*loopA + 1] = (limits[2*loopA + 1]-rootLimits[2*loopA + 0])/(double)(rootLimits[2*loopA + 1] - rootLimits[2*loopA + 0]);
  }
}

// Scale the point back
void rescalePointForQuasiRandomSequence(stdVec rootLimits,stdVec& point){
  for(int loopA=0;loopA<(int)rootLimits.size()/2;loopA++){
    point[loopA] = rootLimits[2*loopA + 0] + point[loopA]*(rootLimits[2*loopA + 1] - rootLimits[2*loopA + 0]);
  }
}

// ==========================================
// MAP SPARSE GRID LOCATIONS TO RANDOM DOMAIN
// ==========================================
double uqSamples::denormalizeRV(double value, int dimId){
  double result = 0.0;
  switch(rvs[dimId].type){
    // ASSUME that value is in [-1,1]
    case kSAMPLEUniform:
      // Return a value between the limits
      result = rvs[dimId].parameter1 + (rvs[dimId].parameter2 - rvs[dimId].parameter1) * (0.5*value+0.5);
      break;
    case kSAMPLEGaussian:
      // Return a value between the limits
      result = value * 3.0 * rvs[dimId].parameter2 + rvs[dimId].parameter1;
      break;
    case kSAMPLELognormal:
      // CHECK !!!
      throw new uqException("LogNormal Probability Not Implemented!");
      break;
    case kSAMPLEConstant:
      // Always return the fixed value
      result = rvs[dimId].parameter1;
      break;
  }
  return result;
}

// ====================================================
// ADJUST WEIGHTS FOR THE VARIOUS PROBABILITY DENSITIES
// ====================================================
void uqSamples::adjustWeights(int maxOrder){
  // Initialize
  double currCCVal = 0.0;
  double mappedValue = 0.0;
  double stdGaussVal = 0.0;

  // Loop on the order
  for(int loop0=0;loop0<maxOrder+1;loop0++){

    // Multiply by the probability density
    for(int loopA=0;loopA<rvs.size();loopA++){
      if(rvs[loopA].type == kSAMPLEUniform){ 
        // Divide by 2
        for(int loopB=0;loopB<weights.size();loopB++){
          weights[loopB][loop0] = 0.5 * weights[loopB][loop0];  
        }
      }else if(rvs[loopA].type == kSAMPLEGaussian){
        // Loop on the nodes
        for(int loopB=0;loopB<weights.size();loopB++){
          // Get CC Nodes in [-1,1]
          currCCVal = values[loopB][loopA];
          // Map to Gaussian in [-inf,inf] ~ [-4.0*sigma,4.0*sigma]
          mappedValue = currCCVal * 3.0 * 1.0 + 0.0;
          // Eval standard Gaussian on this location
          stdGaussVal = (1.0/sqrt(2.0*M_PI))*exp(-0.5*mappedValue*mappedValue);
          //printf("cc %f, mapped %f, stdGauss %f\n",currCCVal,mappedValue,stdGaussVal);
          // Modify Weight
          weights[loopB][loop0] = stdGaussVal * 0.5 * 6.0 * weights[loopB][loop0] * (1.0/0.997300204);
        }
      }else if(rvs[loopA].type == kSAMPLELognormal){
        throw new uqException("Not Implemented!\n");
      }
    }
  }

}

// DEBUG: Print Weight Indexes
void printWeightIndexes(stdIntMat indices){
  printf("\n");
  printf("Weight Indices\n");
  for(int loopA=0;loopA<indices.size();loopA++){
    printf("Weight Indices, order: %d\n",loopA);
    for(int loopB=0;loopB<indices[loopA].size();loopB++){
      printf("%d\n",indices[loopA][loopB]);
    }
  }
  printf("\n");
}

// ================================================
// CHECK IF NODE DISTANCE IS SMALLER THAN TOLERANCE
// ================================================
bool isSameNode(double tolerance,stdVec currCoords,stdVec totCoords){
  
  // If the dimension is different they are different
  if(currCoords.size() != totCoords.size()){
    return false;
  }
  
  // Otherwise compute the distance
  double distance = 0.0;
  for(int loopA=0;loopA<currCoords.size();loopA++){
    distance += (currCoords[loopA] - totCoords[loopA]) * (currCoords[loopA] - totCoords[loopA]);
  }
  distance = sqrt(distance);

  // Return Comparison
  return (distance <= tolerance);
}

// =====================================================
// GENERATE NODE VECTOR AND WEIGHT MATRIX FOR ALL ORDERS
// =====================================================
int findNodeMap(int nodeIdx, int dims, int totalNodesRed, double* nodesRed,
                                       int totalNodes, double* nodes){
  
  // Store Current Node Coordinates
  stdVec currCoords;
  for(int loopA=0;loopA<dims;loopA++){
    currCoords.push_back(nodesRed[nodeIdx*dims + loopA]);
  }
  
  // While Loop to Find component
  int count = 0;
  bool found = false;
  stdVec totCoords;
  double tolerance = 1.0e-5;
  while((!found)&&(count<totalNodes)){
    
    // Get Current Node From Global Node Array
    totCoords.clear();
    for(int loopA=0;loopA<dims;loopA++){
      totCoords.push_back(nodes[count*dims + loopA]);
    }
    
    // Check if the Nodes are the same
    found = isSameNode(tolerance,currCoords,totCoords);
    if(!found){
      count++;
    }
  }
  // Return
  if(!found){
    return -1;
  }else{
    return count;
  }
}

// =====================================================
// GENERATE NODE VECTOR AND WEIGHT MATRIX FOR ALL ORDERS
// =====================================================
void generateNodesWeightMat(int dims, int maxOrder, double* resNodes,stdMat &resWeights){
  
  // Generate the nodes at the maximum Order
  int totalNodes = sparse_grid_cfn_size (dims,maxOrder);
  double* maxNodes = new double[dims*totalNodes];
  double* maxWeights = new double[totalNodes];
  sparse_grid_cc (dims,maxOrder,totalNodes,maxWeights,maxNodes);

  // Loop on the lower orders
  stdIntMat weightIndexMatrix;
  stdMat weightValueMatrix;
  stdIntVec tempIdx;
  stdVec tempValue;
  int currNodeIdx = 0;
  for(int loopA=0;loopA<maxOrder;loopA++){
    
    // Get Nodes and weight at the current Order
    int currTotNodes = sparse_grid_cfn_size (dims,loopA);
    double* currNodes = new double[dims*currTotNodes];
    double* currWeights = new double[totalNodes];
    sparse_grid_cc(dims,loopA,currTotNodes,currWeights,currNodes);

    // Loop on the lower order grid
    tempIdx.clear();
    tempValue.clear();
    for(int loopB=0;loopB<currTotNodes;loopB++){
      tempIdx.push_back(loopB);
      tempValue.push_back(currWeights[loopB]);
    }
    weightIndexMatrix.push_back(tempIdx);
    weightValueMatrix.push_back(tempValue);
    
    // Free lower order Nodes and Weights
    delete[] currNodes;
    delete[] currWeights;
  }

  // DEBUG: Print Weight Indexes
  // printWeightIndexes(weightIndexMatrix);

  // Create Weight Matrix
  resWeights.resize(totalNodes);
  for(int loopA=0;loopA<totalNodes;loopA++){
    resWeights[loopA].resize(maxOrder+1);
    for(int loopB=0;loopB<maxOrder+1;loopB++){
      resWeights[loopA][loopB] = 0.0;
    }
  }

  // Fill Weight Matrix
  // Lower Order
  int currWeightIndex = 0;
  double currWeightValue = 0.0;
  for(int loopA=0;loopA<maxOrder;loopA++){
    for(int loopB=0;loopB<weightIndexMatrix[loopA].size();loopB++){
      currWeightIndex = weightIndexMatrix[loopA][loopB];
      currWeightValue = weightValueMatrix[loopA][loopB];
      resWeights[currWeightIndex][loopA] = currWeightValue;
    }
  }

  // Max Order
  for(int loopA=0;loopA<totalNodes;loopA++){
    resWeights[loopA][maxOrder] = maxWeights[loopA];
  }

  // Copy Max Nodes
  for(int loopA=0;loopA<dims*totalNodes;loopA++){
    resNodes[loopA] = maxNodes[loopA];
  }
   
  // Free Memory
  delete[] maxNodes;
  delete[] maxWeights;

}


// =================================================
// GENERATE SAMPLES ON A CLENSHAW-CURTIS SPARSE GRID
// =================================================
void uqSamples::generateSparseGrid(int gridOrder){

  // Check the effective dimensionality of the sparse Grid (remove constant variables)
  // Mark the Variables that are constant
  int effDims = 0;
  bool isConstant[(int)rvs.size()];
  for(int loopA=0;loopA<rvs.size();loopA++){
    isConstant[loopA] = false;
  }
  for(int loopA=0;loopA<rvs.size();loopA++){
    if(rvs[loopA].type != kSAMPLEConstant){
      effDims++;
    }else{
      isConstant[loopA] = true;
    }
  }

  // Allocate Nodes and weight matrix for all orders
  int totalNodes = sparse_grid_cfn_size (effDims,gridOrder);
  double* currNodes = new double[effDims*totalNodes];
  stdMat currWeights;

  // Generate Node Vector and Weight Matrix
  generateNodesWeightMat(effDims,gridOrder,currNodes,currWeights);
  
  // Copy Nodes and weight to a local Matrix and Vector
  stdVec temp;
  stdVec tempWeight;
  stdMat newValues;
  weights.clear();
  values.clear();
  int currentCount = 0;
  for(int loopA=0;loopA<totalNodes;loopA++){
    temp.clear();
    currentCount = 0;
    for(int loopB=0;loopB<rvs.size();loopB++){
      if(isConstant[loopB]){
        temp.push_back(1.0);
      }else{
        temp.push_back(currNodes[loopA * effDims + currentCount]);
        currentCount++;
      }
    }
    newValues.push_back(temp);
    tempWeight.clear();
    for(int loopB=0;loopB<gridOrder+1;loopB++){
      tempWeight.push_back(currWeights[loopA][loopB]);
    }
    weights.push_back(tempWeight);
  }

  // Clear existing realizations if required
  totDims = rvs.size();
  totSamples = totalNodes;

  // Denormalizing Sparse Grid Values
  double denorm = 0.0;
  for(int loopA=0;loopA<totalNodes;loopA++){
    temp.clear();
    for(int loopB=0;loopB<rvs.size();loopB++){
      denorm = denormalizeRV(newValues[loopA][loopB],loopB);
      temp.push_back(denorm);
    }
    values.push_back(temp);
  }

  // Correlate Variables
  // TO FINISH !!!


  // Adjust Weights To account for the probabilities
  adjustWeights(gridOrder);

  // Free Memory
  delete[] currNodes;
}

// GET SPARSE GRID WEIGHTS
double uqSamples::getWeightAt(int nodeId, int order){
  return weights[nodeId][order];
}

// MAPPING BETWEEN THE NATURALS AND GRID COORDINATES
void getIntegerCoords(int dims,int numPoints1D,int currNumber,stdIntVec& intCoords){
  intCoords.clear();
  for(int loopA=0;loopA<dims;loopA++){    
    intCoords.push_back((currNumber % numPoints1D));
    currNumber = int(currNumber/numPoints1D);
  }
}

// ===============================
// GENERATE CARTESIAN PRODUCT GRID
// ===============================
void uqSamples::generateCartesianGrid(int gridOrder, int gridType, int rangeType){
  // Clear Quantities
  weights.clear();
  values.clear();  

  // Get the 1D integration rule
  uq1DQuadratureRule* quadRule;
  switch(gridType){
    case kRegular:
      quadRule = new uq1DQuadratureRule_Regular(gridOrder,rangeType);
      quadRule->generatePointsAndWeights();        
      break;
    case kCC:
      quadRule = new uq1DQuadratureRule_CC(gridOrder,rangeType);
      quadRule->generatePointsAndWeights();    
      break;
    case kDoubleCC:
      quadRule = new uq1DQuadratureRule_CCDouble(gridOrder,rangeType);
      quadRule->generatePointsAndWeights();
      break;
  }    

  // Perform the Cartesian Product
  // Get Information on the grid
  int num1DPoints = quadRule->getTotalPoints();
  stdVec currPoints = quadRule->getPoints();
  stdVec currWeights = quadRule->getWeights();

  //printf("1D Integration Rule Weights\n");
  //for(int loopA=0;loopA<num1DPoints;loopA++){
  //  printf("Point: %d, X %e, Weight: %f\n",loopA,currPoints[loopA],currWeights[loopA]);
  //}  

  int totPoints = 1;
  for(int loopA=0;loopA<getTotDims();loopA++){
    totPoints *= num1DPoints;
  }
  // Store the total number of points
  totSamples = totPoints;  

  // Loop Through the Points
  stdVec tempNode;
  stdVec tempWeightVec;
  double tempWeight = 0.0;
  stdIntVec intCoords;

  for(int loopA=0;loopA<totPoints;loopA++){    
    tempNode.clear();
    tempWeight = 1.0;    

    // Find the Coordinates of the node
    getIntegerCoords(getTotDims(),num1DPoints,loopA,intCoords);

    // Loop through the dimensions
    for(int loopB=0;loopB<getTotDims();loopB++){
      tempNode.push_back(currPoints[intCoords[loopB]]);
      tempWeight *= currWeights[intCoords[loopB]];
    }
    // Add To Values
    values.push_back(tempNode);    
    // Add to the Weights
    tempWeightVec.clear();
    tempWeightVec.push_back(tempWeight);
    weights.push_back(tempWeightVec);
  }  
}

// ==============================
// EXTRACT SAMPLES ON A PARTITION
// ==============================
void uqSamples::extractPartitionSamples(uqSamples* samples,stdVec limits,stdIntVec& ind){
  
  // Check dimension match
  totDims = samples->totDims;
  ind.clear();

  // Get Input Samples
  stdMat allSamples;
  samples->getValues(allSamples);
  // Get Input Nodes and weights
  stdMat allWeights = samples->getWeights();

  // Loop on existing Samples
  bool useCurrSample = false;
  stdVec newSample;
  double currValue = 0.0;
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){
    // Init Current Sample
    useCurrSample = true;
    // Loop through the Samples
    newSample.clear();
    for(int loopB=0;loopB<samples->getTotDims();loopB++){
      currValue = allSamples[loopA][loopB];
      newSample.push_back(currValue);
      // CAREFULL: EVAL REPEATED MODELS ON THE BORDER !!!
      useCurrSample = (useCurrSample && ((currValue >= limits[2*loopB + 0])&&(currValue <= limits[2*loopB + 1])));
      //useCurrSample = (useCurrSample && ((currValue > limits[2*loopB + 0])&&(currValue < limits[2*loopB + 1])));
    }

    // Add samples if it belongs to the domain
    if(useCurrSample){
      ind.push_back(loopA);
      addOneSample(newSample);
    }
  }

  // Transfer Weights if any
  double weightSum = 0.0;
  double currWeight = 0.0;
  stdVec tempWeight;
  int currMaxOrder = 0;
  if(allWeights.size() > 0){
    currMaxOrder = allWeights[0].size();
    weights.clear();
    for(int loopA=0;loopA<ind.size();loopA++){
    
      // Loop on the integration orders
      tempWeight.clear();
      for(int loopB=0;loopB<currMaxOrder;loopB++){
        // Get Weights at the maximum order
        currWeight = allWeights[ind[loopA]][loopB];
        tempWeight.push_back(currWeight);
      }
      // Take the Weight Sum at the highest integration order
      weightSum += allWeights[ind[loopA]][currMaxOrder];
      weights.push_back(tempWeight);
    }
  }
}

// EXTRACT BASED ON ONE DIMENSIONAL COORDINATES
void uqSamples::extractSamplesWithOneOverlappingDimension(uqSamples* samples,stdVec limits,int dim,stdIntVec& ind){
  
  // Check dimension match
  totDims = samples->totDims;
  ind.clear();

  // Get Input Samples
  stdMat allSamples;
  samples->getValues(allSamples);
  // Get Input Nodes and weights
  stdMat allWeights = samples->getWeights();

  stdVec newSample;
  double currValue = 0.0;
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){    
    
    // Check the dimension of interest
    currValue = allSamples[loopA][dim];
    if((currValue >= limits[2*dim + 0])&&(currValue <= limits[2*dim + 1])){

      // Insert this sample in the index set
      ind.push_back(loopA);

      // Add this sample physically
      newSample.clear();
      for(int loopB=0;loopB<samples->getTotDims();loopB++){
        currValue = allSamples[loopA][loopB];
        newSample.push_back(currValue);
      }
      addOneSample(newSample);
    }
  }

  // Transfer Weights if any
  double weightSum = 0.0;
  double currWeight = 0.0;
  stdVec tempWeight;
  int currMaxOrder = 0;
  if(allWeights.size() > 0){
    currMaxOrder = allWeights[0].size();
    weights.clear();
    for(int loopA=0;loopA<ind.size();loopA++){
    
      // Loop on the integration orders
      tempWeight.clear();
      for(int loopB=0;loopB<currMaxOrder;loopB++){
        // Get Weights at the maximum order
        currWeight = allWeights[ind[loopA]][loopB];
        tempWeight.push_back(currWeight);
      }
      // Take the Weight Sum at the highest integration order
      weightSum += allWeights[ind[loopA]][currMaxOrder];
      weights.push_back(tempWeight);
    }
  }
}


// ==============================
// EXTRACT SAMPLES ON A PARTITION
// ==============================
void uqSamples::evalDifference(int refColumn){
  double refValue = 0.0;
  for(int loopA=0;loopA<totSamples;loopA++){
    refValue = values[loopA][refColumn];
    for(int loopB=0;loopB<totDims;loopB++){
      values[loopA][loopB] = refValue - values[loopA][loopB];
    }
  }
  // Remove Variable from the list
  totDims--;
  for(int loopA=0;loopA<totSamples;loopA++){
    values[loopA].erase(std::next(values[loopA].begin(), refColumn));  
  }
}

// ==============
// REMOVE SAMPLES
// ==============
void uqSamples::removeFirstSamples(int samplesToRemove){
  totSamples -= samplesToRemove;
  values.erase (values.begin(),values.begin()+samplesToRemove);
}

// =========================
// ADD VARIABLES FROM SAMPLE
// =========================
void uqSamples::addVariablesFromSample(uqSamples qVals){
  // Increase Dimensionality
  totDims += qVals.totDims;
  stdMat qMat;
  qVals.getValues(qMat);
  // Append Dimensions to each sample
  for(int loopA=0;loopA<totSamples;loopA++){
    for(int loopB=0;loopB<qVals.totDims;loopB++){
      values[loopA].push_back(qMat[loopA][loopB]);
    }
  }
}

// ================
// REMOVE VARIABLES
// ================
uqSamples uqSamples::removeVariable(int idx){
  uqSamples result;
  // Get total samples and dimensionality
  result.totSamples = totSamples;
  result.totDims = totDims-1;
  // Get Sample Values
  stdVec temp;
  for(int loopA=0;loopA<totSamples;loopA++){
    temp.clear();
    for(int loopB=0;loopB<totDims;loopB++){
      if(loopB != idx){
        temp.push_back(values[loopA][loopB]);  
      }      
    }
    result.values.push_back(temp);
  }
  return result;
}

// ============================================
// READ ADDITIONAL VARIABLE FROM COLUMN IN FILE
// ============================================
void uqSamples::addVariableFromFile(string fileName,int numColumn){
  // Read Table From File
  stdMat table;
  int error = readTableFromFile(fileName,table);
  if(error != 0){
    throw uqException("ERROR: Invalid Outlet Mapping File.\n");
  }

  // Check if the dimensions are compatible otherwise exit
  if(table.size() != totSamples){
    printf("ERROR: Invalid Size of Additional Variable File. Desired %d, got %d.\n",totSamples,(int)table.size());
    throw uqException("ERROR: Invalid Size of Additional Variable File.\n"); 
  }

  // Append Column at the end
  for(int loopA=0;loopA<values.size();loopA++){
    values[loopA].push_back(table[loopA][numColumn]);
  }

  // Increment the number of dimensions
  totDims++;
}

// =========================================================
// RESCALE SAMPLE FROM CARTESIAN PARTITION TO FULL HYPERCUBE
// =========================================================
void uqSamples::rescaleOnHypercube(stdVec currLimits){
  // Sanity Check
  double currValue1 = 0.0;
  double currValue2 = 0.0;
  double minLim = 0.0;
  double maxLim = 0.0;
  if(totDims != currLimits.size()/2){
    throw uqException("ERROR in rescaleOnHypercube: Incompatible dimension.\n");
  }
  for(int loopA=0;loopA<totSamples;loopA++){
    for(int loopB=0;loopB<totDims;loopB++){
      // Get Values and Limits
      currValue1 = values[loopA][loopB];
      if(!areIndependent){
        currValue2 = corrValues[loopA][loopB];  
      }      
      minLim = currLimits[2 * loopB + 0];
      maxLim = currLimits[2 * loopB + 1];
      // Scale Value around the center
      values[loopA][loopB] = 0.5 + (currValue1 - 0.5*(minLim + maxLim))*(1.0/(maxLim-minLim));
      if(!areIndependent){
        corrValues[loopA][loopB] = 0.5 + (currValue2 - 0.5*(minLim + maxLim))*(1.0/(maxLim-minLim));
      }
    }
  }
}

// ====================================================
// RESCALE SAMPLE FROM HYPERCUBE TO CARTESIAN PARTITION
// ====================================================
void uqSamples::rescaleFromHypercubeToPartition(stdVec currLimits){
  // Sanity Check
  double currValue1 = 0.0;
  double currValue2 = 0.0;
  double minLim = 0.0;
  double maxLim = 0.0;
  if(totDims != currLimits.size()/2){
    throw uqException("ERROR in rescaleOnHypercube: Incompatible dimension.\n");
  }
  for(int loopA=0;loopA<totSamples;loopA++){
    for(int loopB=0;loopB<totDims;loopB++){
      // Get Values and Limits
      currValue1 = values[loopA][loopB];
      if(!areIndependent){
        currValue2 = corrValues[loopA][loopB];
      }
      minLim = currLimits[2 * loopB + 0];
      maxLim = currLimits[2 * loopB + 1];
      // Scale Value around the center
      values[loopA][loopB] = 0.5*(minLim + maxLim) + (currValue1 - 0.5)*(maxLim-minLim);
      if(!areIndependent){
        corrValues[loopA][loopB] = 0.5*(minLim + maxLim) + (currValue2 - 0.5)*(maxLim-minLim);
      }      
    }
  }
}

// ========================================
// EXTRACT SAMPLES GIVEN COLUMN AND INDEXES
// ========================================
void uqSamples::extractSampleColumnWithIndex(stdIntVec Indexes,int outCol,stdVec& rhs){
  rhs.clear();
  if(Indexes.size() > values.size()){
    throw uqException("ERROR in extractSampleColumnWithIndex: Input/output mismatch.\n");
  }
  for(int loopA=0;loopA<Indexes.size();loopA++){
    rhs.push_back(values[Indexes[loopA]][outCol]);
  }
}

// ===========================================
// ADAPTIVE SAMPLING FOR BINARY PARTITION TREE
// ===========================================
void uqSamples::addSamplesAdaptive(uqSamples* inputs,uqPartitionBinaryTree* tree,double metricThreshold,int samplesInPartition,randomAdaptiveSamplingTypes type){
  
  int startSampleIdx = 0;
  // Loop on the partitions
  stdIntVec leafIndex = tree->getLeafIndex();
  // Get the limits of the root node
  stdVec rootLimits = tree->getAllNodes(0)->getLimits();
  
  // Loop on all leaves
  int currLeafIndex = 0;
  uqPartitionNode* currPartition;
  stdVec currLimits;
  int partSampleCount = 0;
  int samplesToAdd = 0;
  bool onPartition = true; 
  for(int loopA=0;loopA<leafIndex.size();loopA++){      

    // Get Leaf Index
    currLeafIndex = leafIndex[loopA];

    // Get Current Partition
    currPartition = tree->getAllNodes(currLeafIndex);

    // Check if the metric is above the partition
    if(true){

      // Get Current Limits
      currLimits = currPartition->getLimits();
      
      // Count how many samples are missing
      partSampleCount = currPartition->countSamplesOnPartition(inputs);
      //printf("Current Samples on partition %d\n",partSampleCount);
      //printf("Desired Samples on partition %d\n",samplesInPartition);

      // Count how many samples are missing
      samplesToAdd = samplesInPartition - partSampleCount;

      //printf("Partition %d, Samples To Add: %d\n",loopA,samplesToAdd);
      //getchar();

      // Add the samples
      if(samplesToAdd > 0){
        if(type == asRandom){
          addUniformSamplesFromPartition(samplesToAdd,currLimits);    
        }else if(type == asHalton){
          // Get Maximum Sample index for current partition
          startSampleIdx = currPartition->getQuasiRandomSampleId();
          //printf("Current Sample Index: %d\n",startSampleIdx);
          // Add Samples
          //printf("startSampleIdx PRIMA: %d\n",startSampleIdx);          
          addQuasiRandomSamplesFromPartition(samplesToAdd,rsHalton,onPartition,currLimits,rootLimits,startSampleIdx);
          //getchar();
          // Store the maximum ID
          currPartition->setQuasiRandomSampleId(startSampleIdx);
        }
      }      
    }
  }
}

// ===========================================
// ADD UNIFORM RANDOM SAMPLES FROM PARTITION
// ===========================================
void uqSamples::addUniformSamplesFromPartition(int samplesToAdd, stdVec limits){  
  stdVec storeVec;
  double currValue = 0.0;
  // Loop through the new samples
  for(int loopA=0;loopA<samplesToAdd;loopA++){
    storeVec.clear();
    // Loop through the variable types
    for(int loopB=0;loopB<getTotDims();loopB++){
      currValue = r8_uniform_sample(limits[2*loopB + 0],limits[2*loopB + 1]);
      // Add value to vector
      storeVec.push_back(currValue);
    }
    // Add to existing samples
    addOneSample(storeVec);
  }
}

// =========================================
// ADD UNIFORM RANDOM SAMPLES FROM PARTITION
// =========================================
void uqSamples::addQuasiRandomSamplesFromPartition(int samplesToAdd, quasiRandomTypes type,bool onPartition, stdVec limits, stdVec rootLimits, int& startSampleIdx){

  // Clear available samples
  stdVec storeVec;
  double currValue = 0.0;
  bool addPoint = false;

  //printf("Tot Dims: %d\n",getTotDims());

  //printf("Root Limits\n");
  //for(int loopA=0;loopA<rootLimits.size()/2;loopA++){
  //  printf("%15.6f %15.6f\n",rootLimits[2*loopA + 0],rootLimits[2*loopA + 1]);
  //}

  // printf("Part Limits BEFORE\n");
  // for(int loopA=0;loopA<limits.size()/2;loopA++){
  //   printf("%15.6f %15.6f\n",limits[2*loopA + 0],limits[2*loopA + 1]);
  // }


  // Scale the limits such that rootLimits is an hypercube
  if(onPartition){
    scaleLimitsForQuasiRandomSequence(rootLimits,limits);  
  }

  //printf("Part Limits AFTER\n");
  //for(int loopA=0;loopA<limits.size()/2;loopA++){
  //  printf("%15.6f %15.6f\n",limits[2*loopA + 0],limits[2*loopA + 1]);
  //}
  //getchar();
  
  // Loop through the new samples
  int storedSamplesIdx = startSampleIdx;
  int loopA = startSampleIdx;
  int addedPoints = 0;
  bool finished = false;
  while(!finished){
    storeVec.clear();
    // Loop through the variable types
    for(int loopB=0;loopB<totDims;loopB++){
      if(type == rsHalton){            
        //printf("loopA: %d\n",loopA);
        currValue = evalOneDHalton(loopA,getPrimeInteger(loopB));
        //printf("Current Value: %f\n",currValue);
        //printf("Prime: %d\n",getPrimeInteger(loopB));
      }else{
        throw uqException("Invalid Type of Quasi-Random Sampling...\n");
      }
      // Add value to vector
      storeVec.push_back(currValue);
    }   

    // Add to existing samples if is inside the limits
    addPoint = true;
    if(onPartition){
      for(int loopB=0;loopB<totDims;loopB++){
        //addPoint = (addPoint && ((storeVec[loopB] >= limits[2*loopB + 0])&&(storeVec[loopB] <= limits[2*loopB + 1])));
        addPoint = (addPoint && ((storeVec[loopB] > limits[2*loopB + 0])&&(storeVec[loopB] < limits[2*loopB + 1])));
      }
    }
    if(addPoint){      
      addedPoints++;
      if(onPartition){
        // Scale the point back
        rescalePointForQuasiRandomSequence(rootLimits,storeVec);
        //printf("Added Adaptive Current Point\n");
        //for(int loopC=0;loopC<storeVec.size();loopC++){
        //  printf("%15.6f ",storeVec[loopC]);          
        //}
        //printf("\n");
      }
      // Add the point
      addOneSample(storeVec);
    } 
    // Increment the counter
    loopA++;
    // Check if all the points have been added
    finished = (addedPoints >= samplesToAdd);
  }
  // Assign end index
  startSampleIdx = storedSamplesIdx + loopA;
  // Reset Integration Weights: They are only for grids
  weights.clear();
}

// ====================================
// RETURN THE MAXIMUM INTEGRATION ORDER
// ====================================
int uqSamples::getMaxWeightOrder(){
  if(weights.size() > 0){
    return weights[0].size()-1;
  }else{
    return 0;
  }
}

// =======================================================
// EVALUATE STANDARD PROBABILITY DISTRIBUTION ON HYPERCUBE
// =======================================================
double uqSamples::evalNormalizedProbabilityOnHypercube(int dim, double location){
  double res = 0.0;
  if(rvs.size() > 0){
    switch(rvs[dim].type){
      case kSAMPLEUniform:
      {  res = 1.0;
        break;
      }
      case kSAMPLEGaussian:
      {
        double av = rvs[dim].parameter1;
        double sigma = rvs[dim].parameter2;
        res = 1.0/(sqrt(2.0*M_PI)*sigma)*exp(-0.5*((location - av) * (location - av))/(sigma * sigma));     
        break;
      }
      case kSAMPLELognormal:
      {
        uqException("ERROR: evalStandardProbabilityOnHypercube not implemented for lognormal distribution.\n");
        break;
      }
      case kSAMPLEConstant:
      {
        res = 0.0;
        break;
      }
    }
  }else{
    // No Random Variable Defined: e.g.: samples from Table
    res = 1.0;
  }
  // Return
  return res;
}

// ==================================
// CHECK IF ALL VARIABLES ARE UNIFORM
// ==================================
bool uqSamples::areAllUniformVariables(){
  bool res = true;
  for(int loopA=0;loopA<rvs.size();loopA++){
    res = (res && (rvs[loopA].type == kSAMPLEUniform));
  }
  return res;
}

// =====================
// ADD COVARIANCE MATRIX
// =====================
void uqSamples::addCovariance(stdMat covariance){
  if(rvs.size() != covariance.size()){
    throw uqException("Incompatible Covariance size in AddCovariance.\n");
  }

  // Assign Variances to Variables
  for(int loopA=0;loopA<covariance.size();loopA++){
    rvs[loopA].parameter2 = sqrt(covariance[loopA][loopA]);
  }
  // Normalize and copy covariance to correlation
  double den = 0.0;
  arma::mat corrMat(covariance.size(),covariance.size());
  arma::mat covMat(covariance.size(),covariance.size());
  for(int loopA=0;loopA<covariance.size();loopA++){
    for(int loopB=0;loopB<covariance[loopA].size();loopB++){
      den = sqrt(covariance[loopA][loopA]) * sqrt(covariance[loopB][loopB]);
      corrMat(loopA,loopB) = covariance[loopA][loopB]/den;
      covMat(loopA,loopB) = covariance[loopA][loopB];
    }
  }

  // Compute the Cholesky decomposition of the correlation Matrix
  // Use Armadillo
  arma::mat corrFac = arma::chol(corrMat,"lower");
  arma::mat covFac = arma::chol(covMat,"lower");

  // Store the correlation matrix factor
  stdVec temp;
  stdVec tempCov;
  for(int loopA=0;loopA<covariance.size();loopA++){
    temp.clear();
    tempCov.clear();
    for(int loopB=0;loopB<covariance[loopA].size();loopB++){
      temp.push_back(corrFac(loopA,loopB));
      tempCov.push_back(covFac(loopA,loopB));
    }
    corrCholFactor.push_back(temp);
    covCholFactor.push_back(tempCov);
  }

  // Now they are not independent and values and corrValues
  // are different in general
  areIndependent = false;
}

// ===============================================
// CREATE CORRELATED SAMPLES FROM STANDARD SAMPLES
// ===============================================
void uqSamples::correlateVariables(){
  // Allocate Correlated Variables
  corrValues.resize(values.size());
  for(int loopA=0;loopA<values.size();loopA++){
    corrValues[loopA].resize(values[loopA].size());
  }
  // If no covariance is specified use the same samples
  if(areIndependent){
    for(int loopA=0;loopA<values.size();loopA++){
      for(int loopB=0;loopB<values[loopA].size();loopB++){
        corrValues[loopA][loopB] = values[loopA][loopB];
      }
    }
  }else{
    // Multiply by the Cholesky factor
    double currSD = 0.0;
    double currAV = 0.0;
    // Loop on the samples
    for(int loopA=0;loopA<values.size();loopA++){
      // Perform Matrix Multiplication
      for(int loopB=0;loopB<values[loopA].size();loopB++){
        corrValues[loopA][loopB] = rvs[loopB].parameter1;
        for(int loopC=0;loopC<values[loopA].size();loopC++){
          currAV = rvs[loopC].parameter1;
          currSD = rvs[loopC].parameter2;
          corrValues[loopA][loopB] += corrCholFactor[loopB][loopC] * ((values[loopA][loopC] - currAV)/currSD);
        }
      }
    }
  }
}

// ==================================================================
// CREATE STANDARD SAMPLES FROM CORRELATED AND NON-NORMALIZED SAMPLES
// ==================================================================
void uqSamples::decorrelateVariables(){
  // Allocate Correlated Variables
  values.resize(corrValues.size());
  for(int loopA=0;loopA<corrValues.size();loopA++){
    values[loopA].resize(corrValues[loopA].size());
  }
  // If no covariance is specified use the same samples
  if(areIndependent){
    for(int loopA=0;loopA<corrValues.size();loopA++){
      for(int loopB=0;loopB<corrValues[loopA].size();loopB++){
        values[loopA][loopB] = corrValues[loopA][loopB];
      }
    }
  }else{
    // Solve the Triangular System for every Sample
    // Form Matrix and RHS
    arma::mat lhs(corrCholFactor.size(),corrCholFactor.size());
    arma::vec rhs(corrCholFactor.size());
    arma::vec sol(corrCholFactor.size());
    for(int loopB=0;loopB<corrCholFactor.size();loopB++){
      for(int loopC=0;loopC<corrCholFactor.size();loopC++){
        lhs(loopB,loopC) = corrCholFactor[loopB][loopC];
      }
    }
    // Decorrelate each Sample
    for(int loopA=0;loopA<corrValues.size();loopA++){
      // Fill RHS
      for(int loopB=0;loopB<corrCholFactor.size();loopB++){
        // rhs(loopB) = corrValues[loopA][loopB] - rvs[loopB].parameter1;
        rhs(loopB) = corrValues[loopA][loopB];
      }
      // Solve
      sol = arma::solve(lhs,rhs);
      // Assign the solution to values
      for(int loopB=0;loopB<corrCholFactor.size();loopB++){
        values[loopA][loopB] = sol(loopB);
      }
    }
  }
}

// ==================================
// READ AND TRANSFORM EXISTING INPUTS
// ==================================
void uqSamples::readAndTransform(uqSamples* samples){
  // Read Samples as Correlated
  stdVec temp;
  this->totDims = samples->getTotDims();
  this->totSamples = samples->getTotSamples();
  corrValues.clear();
  for(int loopA=0;loopA<samples->values.size();loopA++){
    temp.clear();
    for(int loopB=0;loopB<samples->values[loopA].size();loopB++){
      temp.push_back(samples->values[loopA][loopB]);
    }
    corrValues.push_back(temp);
  }
  // Create Normalized and Decorrelated Samples
  decorrelateVariables();
}

// =================================================
// EVAL MULTIVARIATE GAUSSIAN AT THE CURRENT SAMPLES
// =================================================
void uqSamples::evalMultiVariateGaussian(stdVec& mvg){
  double ll = 0.0;
  double currMVG = 0.0;
  mvg.clear();
  // Get Average Vector
  stdVec avgVector;
  for(int loopA=0;loopA<getTotDims();loopA++){
    avgVector.push_back(rvs[loopA].parameter1);
  }

  // Translate Covariance Factor in Armadillo format
  double det = 1.0;
  arma::mat covFac(covCholFactor.size(),covCholFactor.size());
  for(int loopA=0;loopA<covCholFactor.size();loopA++){
    for(int loopB=0;loopB<covCholFactor[loopA].size();loopB++){
      covFac(loopA,loopB) = covCholFactor[loopA][loopB];
      if(loopA == loopB){
        det *= covFac(loopA,loopB) * covFac(loopA,loopB);
      }
    }
  }

  // Compute Product for all Samples
  arma::vec currSample(getTotDims());
  for(int loopA=0;loopA<getTotSamples();loopA++){

    // Translate Samples minus Average in Armadillo format  
    for(int loopB=0;loopB<getTotDims();loopB++){
      if(areIndependent){
        //printf("Are Independent\n");
        currSample(loopB) = values[loopA][loopB];
      }else{
        //printf("Are Dependent\n");
        currSample(loopB) = corrValues[loopA][loopB];
      }
    }

    // Perform 
    arma::vec firstVec = arma::solve(covFac.t(), currSample);
    arma::vec secondVec = arma::solve(covFac, firstVec);
    ll = arma::dot(secondVec,currSample);
    currMVG = (1.0/(sqrt(pow(2.0 * M_PI,getTotDims()) * det))) * exp(-0.5 * ll);

    // Add Result for this sample
    mvg.push_back(currMVG);
  }

}

// =======================
// GET LIMITS FROM SAMPLES
// =======================
void uqSamples::getSampleLimits(stdVec& currLimits, int limitType){
  // Check for inconsistencies
  if((!areIndependent)&&(corrValues.size() == 0)){
    throw uqException("Error: need to explicitly correlate variables.\n");
  }

  double currValue = 0.0;  
  currLimits.clear();
  currLimits.resize(2 * getTotDims());    

  if(limitType == kBoundingBox){


    // Loop on the Dimensions
    for(int loopA=0;loopA<getTotDims();loopA++){
      // Init to minimum and maximum value
      currLimits[2 * loopA] = std::numeric_limits<double>::max();
      currLimits[2 * loopA + 1] = -std::numeric_limits<double>::max();    

      // Loop on the samples
      for(int loopB=0;loopB<getTotSamples();loopB++){
        if(areIndependent){
          currValue = values[loopB][loopA];
        }else{
          currValue = corrValues[loopB][loopA];
        }

        // Assign Minimum and Maximum
        if(currValue < currLimits[2 * loopA]){
          currLimits[2 * loopA] = currValue;
        }      
        if(currValue > currLimits[2 * loopA + 1]){
          currLimits[2 * loopA + 1] = currValue;
        }
      }
    }
  }else if(limitType == kFourSTD){

    double currMean = 0.0;
    double currSTD = 0.0;
    
    // Evaluate Sample Mean
    for(int loopA=0;loopA<getTotDims();loopA++){
      // Loop on the samples
      currMean = 0.0;
      for(int loopB=0;loopB<getTotSamples();loopB++){
        if(areIndependent){
          currValue = values[loopB][loopA];
        }else{
          currValue = corrValues[loopB][loopA];
        }
        currMean += currValue;
      }
      currMean = currMean / (double)getTotSamples();
      // Evaluate Sample STD
      currSTD = 0.0;
      for(int loopB=0;loopB<getTotSamples();loopB++){
        if(areIndependent){
          currValue = values[loopB][loopA];
        }else{
          currValue = corrValues[loopB][loopA];
        }
        currSTD += (currValue - currMean) * (currValue - currMean);
      }
      currSTD = sqrt(currSTD / (double)(getTotSamples() - 1));
      printf("Mean %f, STD %f\n",currMean,currSTD);
      // Assign Limits Based on MEAN and STD
      currLimits[2 * loopA] = currMean - 4.0 * currSTD;
      currLimits[2 * loopA + 1] = currMean + 4.0 * currSTD;
    }
  }
}

// ===========================
// ADD SAMPLES AT THE BOUNDARY
// ===========================
void uqSamples::addPointsOnBoundary(int boundPointOrder){
  stdVec currLimits;
  double lb, lbO;
  double ub, ubO;
  stdVec sample;
  int otherDim = 0;
  sample.resize(2);

  // kBoundingBox, kFourSTD
  // Get Limits
  getSampleLimits(currLimits,kFourSTD);
  // Write code for two dimensions only: TO BE COMPLETED!
  if(getTotDims() == 2){
    for(int loopA=0;loopA<2;loopA++){
      if(loopA == 0){
        otherDim = 1;
      }else{
        otherDim = 0;
      }
      // Get Lower and Upper Bound
      lb  = currLimits[2*loopA];
      ub  = currLimits[2*loopA + 1];
      lbO = currLimits[2*otherDim];
      ubO = currLimits[2*otherDim + 1];
      // Add lb
      for(int loopB=0;loopB<boundPointOrder;loopB++){
        sample[loopA] = lb;
        sample[otherDim] = lbO + (loopB/(double)(boundPointOrder-1))*(ubO - lbO);
        addOneSample(sample);
      }
      // Add ub
      for(int loopB=0;loopB<boundPointOrder;loopB++){
        sample[loopA] = ub;
        sample[otherDim] = lbO + (loopB/(double)(boundPointOrder-1))*(ubO - lbO);
        addOneSample(sample);
      }
    }
  }else{
    throw uqException("Error: addBoundaryPoints must be implemented in arbitrary dimensions.\n");
  }
}

// Read Input Parameters from sub-sampled MCMC realizations
void uqSamples::readMCMCSubSamples(string mcmcTraceFile,int totSubSamples,int startColumn,int endColumn,stdIntVec& sampleIndexes){
  
  stdMat subSampleTable;
  stdVec oneSample;

  // Read Table From File using Util
  subSampleTableData(mcmcTraceFile,totSubSamples,startColumn,endColumn,subSampleTable,sampleIndexes);

  // Assign the dimensionality
  totDims = (int)subSampleTable[0].size();

  // Add Samples to This
  for(int loopA=0;loopA<subSampleTable.size();loopA++){
    oneSample.clear();
    for(int loopB=0;loopB<subSampleTable[loopA].size();loopB++){
      oneSample.push_back(subSampleTable[loopA][loopB]);
    }
    addOneSample(oneSample);
  }
}

void uqSamples::readMCMCSubSamplesWithIndex(string mcmcTraceFile,int totSubSamples,stdIntVec sampleIndexes,int startColumn,int endColumn){
  
  stdMat subSampleTable;
  stdVec oneSample;

  // Read Table From File using Util
  subSampleTableDataWithIndex(mcmcTraceFile,sampleIndexes,startColumn,endColumn,subSampleTable);

  // Assign the dimensionality
  totDims = (int)subSampleTable[0].size();

  // Add Samples to This
  for(int loopA=0;loopA<subSampleTable.size();loopA++){
    oneSample.clear();
    for(int loopB=0;loopB<subSampleTable[loopA].size();loopB++){
      oneSample.push_back(subSampleTable[loopA][loopB]);
    }
    addOneSample(oneSample);
  }  
}

// Add Samples From Matrix
void uqSamples::buildFromMatrix(stdMat params){
  stdVec temp;
  if(params.size() > 0){
    // Set number of samples and total dimensionality
    totDims = params[0].size();
    // Loop over the Samples
    for(int loopA=0;loopA<params.size();loopA++){
      temp.clear();
      for(int loopB=0;loopB<params[loopA].size();loopB++){
        temp.push_back(params[loopA][loopB]);
      }
      addOneSample(temp);      
    }
  }else{
    throw uqException("ERROR: Cannot build samples from empty table.");
  }
}

// Extract samples along a single dimension
void uqSamples::extractOneDimensionalSamples(uqSamples* samples,int dim){
  stdVec tmp;
  // Argument checking
  if((dim>samples->getTotDims())||(dim<0)){
    throw uqException("ERROR: Invalid dimension in extractOneDimensionalSamples.");
  }
  // Make it one-dimensional
  totDims = 1;  
  // Add the samples
  for(int loopA=0;loopA<samples->getTotSamples();loopA++){
    tmp.clear();
    tmp.push_back(samples->getValuesAt(loopA,dim));
    addOneSample(tmp);
  }
}

// Applies a log transform
void uqSamples::expTransform(){
  for(int loopA=0;loopA<getTotSamples();loopA++){
    for(int loopB=0;loopB<getTotDims();loopB++){
      values[loopA][loopB] = exp(values[loopA][loopB]);
    }
  }
}

// Normalize all columns with max at one
void uqSamples::normalizeToOne(){
  double maxColVal = 0.0;
  for(int loopA=0;loopA<getTotDims();loopA++){
    maxColVal = 0.0;
    for(int loopB=0;loopB<getTotSamples();loopB++){
      // Get the maximum abs value
      if(fabs(values[loopB][loopA]) > maxColVal){
        maxColVal = fabs(values[loopB][loopA]);
      }
    }
    // Normalize
    for(int loopB=0;loopB<getTotSamples();loopB++){
      values[loopB][loopA] = values[loopB][loopA]/maxColVal;
    }
  }  
}

// Eval the permutation which orders the dim-dimensional inputs in increasing order
void uqSamples::evalSamplePermutation(int dim,stdIntVec& samplePermutation){

  std::vector<inputSort> oneDLoc;
  oneDLoc.resize(getTotSamples());
  samplePermutation.clear();

  // Fill Aux Vector
  for(int loopA=0;loopA<getTotSamples();loopA++){
    oneDLoc[loopA].coord = values[loopA][dim];
    oneDLoc[loopA].idx = loopA;
  }

  // Sort Array
  std::sort(oneDLoc.begin(), oneDLoc.end(), by_idx());

  // Fill return vector
  for(int loopA=0;loopA<getTotSamples();loopA++){
    samplePermutation.push_back(oneDLoc[loopA].idx);
  }
}

// Multiply all samples by a scalar
void uqSamples::multiplyByScalar(stdVec scalars){
  for(int loopA=0;loopA<totSamples;loopA++){
    for(int loopB=0;loopB<totDims;loopB++){
      values[loopA][loopB] *= scalars[loopA];
    }
  }
}