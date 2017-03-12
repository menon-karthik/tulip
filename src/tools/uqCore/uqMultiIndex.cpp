# include "uqMultiIndex.h"

// ==================================================
// GENERATE THE MULTI-INDEX FOR A MAXIMUM TOTAL ORDER
// ==================================================
void uqMultiIndex::generateMultiIndex_Partial(int dim, int order){
  // Allocate
  vector<int> vecStore;
  int x[dim];
  for(int loopA=0;loopA<dim;loopA++){
    x[loopA] = 0;
  }
  vecStore.clear();
  for(int loopB=0;loopB<dim;loopB++){
    vecStore.push_back(x[loopB]);
  }
  multiIndex.push_back(vecStore);

  // Get the number of Monomials
  basis_num = mono_upto_enum(dim,order);

  // Loop to generate the multi-index
  for(int loopA=0;loopA<(basis_num-1);loopA++){
    mono_next_grlex(dim,x);
    vecStore.clear();
    for(int loopB=0;loopB<dim;loopB++){
      vecStore.push_back(x[loopB]);
    }
    multiIndex.push_back(vecStore);
  }
}

// =================================================================
// GENERATE THE MULTI-INDEX FOR A MAXIMUM ORDER OF EACH 1-D VARIABLE
// =================================================================
void uqMultiIndex::generateMultiIndex_Full(int dim, int order){
  
  // Allocate
  vector<int> vecStore;
  vecStore.clear();

  // Get the number of Monomials
  basis_num = 1;
  for(int loopA=0;loopA<dim;loopA++){
    basis_num *= (order+1);
  }

  // Allocate the space for the multi-index Matrix
  multiIndex.resize(basis_num);
  for(int loopA=0;loopA<basis_num;loopA++){
    multiIndex[loopA].resize(dim);
  }

  // Build with Full Tensor Product
  int currentRow = 0;
  int stepIdx = ((int)basis_num/(int)(order+1));
  for(int loopA=0;loopA<dim;loopA++){
    for(int loopB=0;loopB<basis_num;loopB++){
      currentRow = ((int)(loopB)/(int)stepIdx);
      //printf("CURRENTROW 1: %d\n",currentRow);
      currentRow = ((int)currentRow % (int)(order+1));
      multiIndex[loopB][loopA] = currentRow;
    }
    stepIdx = ((int)stepIdx/(int)(order+1));
  }
}

// ====================
// PRINT MATRIX TO FILE
// ====================
void uqMultiIndex::printToFile(string outMatFile,bool printTitle){
  FILE *f;
  f = fopen(outMatFile.c_str(),"w");    
  int totDims = multiIndex[0].size();
  // Print Basis Matrix
  if(printTitle){
    fprintf(f,"MultiIndex, Rows: %d; Columns: %d\n",basis_num,int(multiIndex[0].size()));  
  }
  for(int loopA=0;loopA<basis_num;loopA++){
    for(int loopB=0;loopB<totDims;loopB++){
      fprintf(f,"%d ",multiIndex[loopA][loopB]);  
    }
    fprintf(f,"\n");
  }
  fclose(f);
}


// ===========
// CONSTRUCTOR
// ===========
uqMultiIndex::uqMultiIndex(int dim, int order, int miType){
  if(miType == kMIPartialOrder){
    generateMultiIndex_Partial(dim,order);
  }else{
    generateMultiIndex_Full(dim,order);
  }
}

// ================
// GET BASIS NUMBER
// ================
int uqMultiIndex::getTotalBasis(){
  return multiIndex.size();
}

// =================
// ACCESS TO MEMBERS
// =================
double uqMultiIndex::operator()(const int nRow, const int nCol){
  return multiIndex[nRow][nCol];
}

// =========================================
// CHECK IF A CERTAIN COMPONENT IS CONTAINED
// =========================================
bool uqMultiIndex::contains(int variable, int component){
  return multiIndex[component][variable] > 0;
}
// ================================================
// CHECK IF A CERTAIN VARIABLE IS THE ONLY NON ZERO
// ================================================
bool uqMultiIndex::containsOnly(int variable, int component){
  bool result = true;
  for(int loopA=0;loopA<multiIndex[component].size();loopA++){
    if(loopA != variable){
      result = (result && (multiIndex[component][loopA] == 0));
    }
  }
  result = (result && (multiIndex[component][variable] > 0));
  return result;
}
