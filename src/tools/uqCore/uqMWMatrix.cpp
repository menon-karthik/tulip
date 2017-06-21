# include "uqMWMatrix.h"

// ====================================
// BUILD A BINARY ARRAY FROM AN INTEGER
// ====================================
void convertTo01Array(int intValue,
                      int totDims,
                      stdIntVec &resultVector,
                      bool useBinaryPartitions){
  // Allocation
  resultVector.resize(totDims);

  // Initialize to zero
  for(int loopA=0;loopA<totDims;loopA++){
    resultVector[loopA] = 0;
  }

  if((useBinaryPartitions)&&(intValue > 0)){
    resultVector[intValue-1] = 1;
  }else{
    // Fill Array
    for(int loopA=0;loopA<totDims;loopA++){
      resultVector[totDims - loopA - 1] = (intValue % 2);
      intValue = int(intValue / 2);
    }
  }
}

// =============================
// CHECK AVAILABILITY OF SAMPLES
// =============================
bool checkSamplesAvailability(int totDims,int totSamples,
                              uqSamples* xValues,
                              int mwScaleIDX, int mwShiftIDX,
                              stdIntVec currentIDX){
  // Check whether there are samples available
  if(totSamples < 1){
    return false;
  }
  
  // Loop On Samples
  bool result = true;
  double sampleValue = 0.0;
  int totIntervals = 0;
  int singleDimensionShiftIDX = 0;
  double minIntevalLimit = 0.0;
  double maxIntevalLimit = 0.0;
  for(int loopA=0;loopA<totSamples;loopA++)
  {
    // Initialize Result
    result = true;
    for(int loopB=0;loopB<totDims;loopB++)
    {
      sampleValue = (*xValues)(loopA,loopB);
      
      // Find Current Interval Limits for the Current Dimension
      totIntervals = floor(pow(2,mwScaleIDX));
      
      // Find Shift Index For Current Dimension
      singleDimensionShiftIDX = currentIDX[loopB];
      minIntevalLimit = (singleDimensionShiftIDX/totIntervals);
      maxIntevalLimit = ((singleDimensionShiftIDX+1.0)/totIntervals);
      
      // Check If is In
      result = (result)&&((sampleValue > minIntevalLimit)&&(sampleValue <= maxIntevalLimit));
    }
    if(result){
      return result;
    } 
  }
  // Return if not already returned
  return result;
}

// ========================================
// GET LOCATIONS FROM SCALE AND SHIFT INDEX
// ========================================
void getMWShiftIndexes(int totDims,
                       int mwScaleIDX, int mwShiftIDX,
                       stdIntVec &currentIDX){
  
  // Check If The Index is Too Big
  if((mwShiftIDX < 0)&&(mwShiftIDX > floor(pow(2,totDims * mwScaleIDX)) - 1)){
    throw uqException("ERROR: Invalid mwShiftIDX.\n");
  }

  // Declare
  int currentInt = 0;

  // Allocate
  currentIDX.clear();
  currentIDX.resize(totDims);

  // Check If Shift Index is Equal to Zero
  if(mwShiftIDX == 0){
    // All Zeros
    for(int loopA=0;loopA<totDims;loopA++){
      currentIDX[loopA] = 0; 
    }
  }else{
    // Initialize
    currentInt = mwShiftIDX;

    // Find the indexes for all fathers
    // Shift Coordinates account for father-child relationship
    stdIntVec fatherIDs;
    fatherIDs.resize(mwScaleIDX);
    for(int loopA=0;loopA<mwScaleIDX;loopA++){
      fatherIDs[loopA] = (currentInt % int(pow(2,totDims)));
      currentInt = int(((currentInt)/(floor(pow(2,totDims)))));
    }

    // Initialize
    for(int loopA=0;loopA<totDims;loopA++){
      currentIDX[loopA] = 0;
    } 
    
    // Get Coordinates
    int value = 0;
    for(int loopA=0;loopA<mwScaleIDX;loopA++)
    {
      value = fatherIDs[loopA];
      for(int loopB=0;loopB<totDims;loopB++)
      {
        currentIDX[loopB] += (value % 2)*(floor(pow(2,loopA)));
        value = (value / 2);
      }
    }
  }
}

bool measureNotUniform(const stdVec& measure){
  bool res = true;
  for(int loopA=0;loopA<measure.size();loopA++){
    res = res && (fabs(measure[loopA]-1.0)<1.0e-8)
  }
  return res;
}

// =====================
// MW MATRIX CONSTRUCTOR
// =====================
uqMWMatrix::uqMWMatrix(int maxOrder,uqSamples* XValues,
                       bool addLegendrePoly, bool addMW, bool useBinaryPartitions,
                       int mwMatType, bool mwMatIncludeNullColumns,
                       bool useExactMW, int mwQuadOrder, stdMat measure,
                       int &maxColumns, int &maxDetailLevel){

  
  // Reset Arrays
  scaleIdx.clear();
  multiIdx.clear();
  shiftIdx.clear();
  orderIdx.clear();
  isMW.clear();
  multiIndex.clear();

  // Declarations
  int totSamples = XValues->getTotSamples();
  int totDims = XValues->getTotDims();
  stdVec storeRow;
  double coeff = 0.0;
  double currXValue = 0.0;
  int currPolyOrder = 0;
  double currBaseValue = 0.0;
  stdIntVec tmpMI;

  // Build the multiwavelet basis
  vector<uqMWBasis*> myInterps;
  // Build one MW basis for every dimension
  uqMWBasis* temp;
  for(int loopA=0;loopA<totDims;loopA++){
    // printf("Building Basis for dim: %d\n",loopA);
    if(measureNotUniform(measure[loopA])){
      temp = new uqMWBasis(maxOrder+1,mwQuadOrder,measure[loopA]);
    }else{
      temp = new uqMWBasis(maxOrder+1);
    }
    myInterps.push_back(temp);
  }  
  
  // Generate the multi-index
  //uqMultiIndex mi(totDims,maxOrder,kMIFullOrder);
  uqMultiIndex mi(totDims,maxOrder,kMIPartialOrder);
  num_basis = mi.getTotalBasis();

  //string miFile("miFileOut.txt");
  //mi.printToFile(miFile,true);  

  if(addLegendrePoly){
    // Assemble the Lagrange polynomial Terms
    for(int loopA=0;loopA<totSamples;loopA++){
      storeRow.clear();
      for(int loopB=0;loopB<num_basis;loopB++){
        coeff = 1.0;
        if(loopA == 0){
          // Store Indexes
          scaleIdx.push_back(-1);
          multiIdx.push_back(-1);
          shiftIdx.push_back(0);
          orderIdx.push_back(loopB);
          isMW.push_back(false);
          // Assign MultiIndex
          tmpMI.clear();
          for(int loopC=0;loopC<totDims;loopC++){
            tmpMI.push_back(mi(loopB,loopC));
          }
          multiIndex.push_back(tmpMI);
        }
        // Loop on the number of dimensions
        for(int loopC=0;loopC<totDims;loopC++){
          // Copy X Value
          currXValue = XValues->getValuesAt(loopA,loopC);
          // Eval Base Function
          currPolyOrder = mi(loopB,loopC);
          // Eval Multiscaling Function
          currBaseValue = myInterps[loopC]->EvalMS(currXValue,0,0,currPolyOrder);          
          // Fill Constraint Mat
          coeff = coeff * currBaseValue;
        }
        storeRow.push_back(coeff);
      }
      values.push_back(storeRow);
    }
  }

  // Declarations for MW Matrix
  int currentColumn = 0;
  bool finished = false;

  // Assembla i Termini di Dettaglio Successivi
  if(addLegendrePoly){
    currentColumn = num_basis;
  }else{
    currentColumn = 0;  
  }
  
  if(mwMatType == kMWFixedColumns){
    finished = (currentColumn == maxColumns);
  }else{
    finished = false;
  }
  if (!addMW){
    finished = true;
  }   

  int currentMWScaleIndex = -1;
  int maxShiftIndex = 0;
  int currentMWShiftIndex = -1;
  bool areSamplesAvailable = false;
  stdIntVec mwShiftIndex;
  int numberOfCombinations = 0;
  int currentOrder = 0;
  stdIntVec permArray;
  int currentPolyOrder = 0;
  int currentShifIDX = 0;
  double currentBaseValue = 0.0;
  stdMat mwMatTranspose;
  stdVec storeMWCols;
  double currValues = 1.0;
  while (!finished){
    currentMWScaleIndex++;

    //printf("CURRENT SCALE: %d\n",currentMWScaleIndex);

    if(mwMatType == kMWFixedMaxDetailLevel){
      finished = (currentMWScaleIndex > maxDetailLevel);
    } 

    maxShiftIndex = floor(pow(2,currentMWScaleIndex * totDims))-1;
    
    currentMWShiftIndex = -1;
    
    while((currentMWShiftIndex < maxShiftIndex)&&(!finished)){
      currentMWShiftIndex++;

      //printf("CURRENT SHIFT INDEX: %d\n",currentMWShiftIndex);
      
      // Get Shift Coordinates For Interval
      getMWShiftIndexes(totDims,currentMWScaleIndex,currentMWShiftIndex,mwShiftIndex);      

      //printf("CURRENT SHIFT COORDS\n");
      //for(int loopA=0;loopA<totDims;loopA++){
      //  printf("%d ",mwShiftIndex[loopA]);
      //}
      //printf("\n");
      
      // Check If There is At Least One Sample Falling Within the Interval
      if(mwMatIncludeNullColumns){
        areSamplesAvailable = true;
      }else{
        areSamplesAvailable = checkSamplesAvailability(totDims,totSamples,XValues,
                              currentMWScaleIndex,currentMWShiftIndex,mwShiftIndex);        
      } 

      
      // Build Basis
      if(areSamplesAvailable){

        //printf("SAMPLES ARE AVAILABLE\n");
        if(useBinaryPartitions){
          // Subdivision along only one dimension
          numberOfCombinations = totDims + 1;
        }else{
          // Subdivisions in all dimensions at the same time
          numberOfCombinations = floor(pow(2,totDims));  
        }

        // Init Order
        currentOrder = 0;
        for(int loopF=1;loopF<numberOfCombinations;loopF++){
          // Build a Binary Array From an Integer
          convertTo01Array(loopF,totDims,permArray,useBinaryPartitions);          

          //printf("BINARY ARRAY\n");
          //for(int loopA=0;loopA<totDims;loopA++){
            //printf("%d ",permArray[loopA]);
          //}
          //printf("\n");
          //getchar();

          // Loop on the number of Basis
          for(int loopC=0;loopC<num_basis;loopC++){
            
            // Update Column Number
            currentColumn++;

            //printf("ADDING COLUMN: %d\n",currentColumn);
            
            // Check When Finished
            if(mwMatType == kMWFixedColumns){
              finished = (currentColumn > maxColumns);
            }else{
              finished = false;  
            }
            
            // Add Columns
            if(!finished){              
              
              // Allocate Storage Vectors
              // Store Indexes
              scaleIdx.push_back(currentMWScaleIndex);
              shiftIdx.push_back(currentMWShiftIndex);
              multiIdx.push_back(loopF);
              orderIdx.push_back(loopC);
              isMW.push_back(true);
              // Assign MultiIndex
              tmpMI.clear();
              for(int loopE=0;loopE<totDims;loopE++){
                tmpMI.push_back(mi(loopC,loopE));
              }

              //hasMWOrderZero = checkMWOrderZero(permArray,tmpMI);

              multiIndex.push_back(tmpMI);

              //printf("SCALE %d, SHIFT %d, MULTI %d, BASIS %d\n",currentMWScaleIndex,currentMWShiftIndex,loopF,loopC);
              
              storeMWCols.clear();

              // Fill Rows and Columns
              for(int loopD=0;loopD<totSamples;loopD++)
              {

                //printf("SAMPLE %d\n",loopD);
                
                // Initialize Matrix
                currValues = 1.0;
                for(int loopE=0;loopE<totDims;loopE++){                  

                  //printf("DIM %d\n",loopE);

                  // Copy X Value
                  currXValue = (*XValues)(loopD,loopE);

                  //printf("CURRENT XVALUE: %e\n",currXValue);
                  // Eval Base Function
                  currentPolyOrder = mi(loopC,loopE);
                  currentShifIDX = mwShiftIndex[loopE];

                  //printf("CURRENT ORDER: %d\n",currentPolyOrder);
                  //printf("CURRENT SHIFT: %d\n",currentShifIDX);                  

                  if(permArray[loopE] == 0){
                    // Scaled Legendre Basis                   
                    currentBaseValue = myInterps[loopE]->EvalMS(currXValue,currentMWScaleIndex,currentShifIDX,currentPolyOrder);                      
                  }else{
                    // MultiWavelet Basis
                    //if(useExactMW){
                    //  if(notUniformMeasure){
                    //    throw uqException("Error: Exact MW are not available for arbitrary measures.\n");
                    //  }else{
                    //    currentBaseValue = myInterp->EvalSingleExactMW(currXValue,currentMWScaleIndex,currentShifIDX,currentPolyOrder,maxOrder);  
                    //  }                      
                    //}else{
                    currentBaseValue = myInterps[loopE]->EvalMW(currXValue,currentMWScaleIndex,currentShifIDX,currentPolyOrder);  
                    //}
                  }
                  // Write Matrix Entries
                  currValues *= currentBaseValue;
                }
                // Add Compoment to column vector
                storeMWCols.push_back(currValues);
              }
              // Add Column to transpose Matrix
              mwMatTranspose.push_back(storeMWCols);
            }
          }
        }
      }
    }
  }    

  // Copy the temporary matrix
  int lastEntry = values[0].size();
  for(int loopA=0;loopA<totSamples;loopA++){
    values[loopA].resize(currentColumn);
    for(int loopB=lastEntry;loopB<currentColumn;loopB++){
      values[loopA][loopB] = mwMatTranspose[loopB - lastEntry][loopA];
    }
  }

  // Set Matrix Size
  rowCount = totSamples;
  colCount = currentColumn;
  
  // If Fixed Detail
  if(mwMatType == kMWFixedMaxDetailLevel){
    maxColumns = currentColumn;
  } 

  // Delete the basis
  for(int loopA=0;loopA<totDims;loopA++){
    delete myInterps[loopA];  
  }  
}

// DESTRUCTOR
uqMWMatrix::~uqMWMatrix(){

}