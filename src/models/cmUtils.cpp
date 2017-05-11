# include "cmUtils.h"

using namespace std;


double getMax(int start, int stop, double* vector){
  double maxVal = -std::numeric_limits<double>::max();
  double currValue = 0.0;
  for(int loopA=start;loopA<stop;loopA++){
     currValue = vector[loopA];
     if(currValue>maxVal){
       maxVal = currValue;
     }
  }
  return maxVal;
}


double getMin(int start, int stop, double* vector){
  double minVal = std::numeric_limits<double>::max();
  double currValue = 0.0;
  for(int loopA=start;loopA<stop;loopA++){
     currValue = vector[loopA];
     if(currValue<minVal){
       minVal = currValue;
     }
  }
  return minVal;
}

double getMean(int start, int stop, double* vector){
  double result = 0.0;
  for(int loopA=start;loopA<stop;loopA++){
     result += vector[loopA];
  }
  return result/(double)(stop - start);
}

double getSum(int start, int stop, double* vector){
  double result = 0.0;
  for(int loopA=start;loopA<stop;loopA++){
     result += vector[loopA];
  }
  return result;
}

int getMaxLoc(int start, int stop, double* vector){
  double maxVal = -std::numeric_limits<double>::max();
  int maxLoc = 0;
  double currValue = 0.0;
  for(int loopA=start;loopA<stop;loopA++){
     currValue = vector[loopA];
     if(currValue>maxVal){
       maxVal = currValue;
       maxLoc = loopA;
     }
  }
  return maxLoc;
}

double trapz(int start, int stop, double* xVals, double* yVals){
  double result = 0.0;
  for(int loopA=start;loopA<(stop-1);loopA++){
    result += 0.5*(yVals[loopA+1] + yVals[loopA])*(xVals[loopA+1] - xVals[loopA]);
  }
  return result;
}

double trapz(int start, int stop, const stdVec& xVals, const stdVec& yVals){
  double result = 0.0;
  for(int loopA=start;loopA<(stop-1);loopA++){
    result += 0.5*(yVals[loopA+1] + yVals[loopA])*(xVals[loopA+1] - xVals[loopA]);
  }
  return result;
}


string &ltrim(string &s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
        return s;
}

string &rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), not1(std::ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
}

string &trim(string &s) {
        return ltrim(rtrim(s));
}

void schSplit(string sentence,vector<string> &tokens,const char* separator){
  char* pch;
  tokens.clear();
  sentence = ltrim(rtrim(sentence));
  pch = strtok ((char*)sentence.c_str(),separator);
  while (pch != NULL){
    tokens.push_back(string(pch));
    pch = strtok (NULL,separator);
  }
}

int ReadParamsFromFile(std:: string inputFileName,double* params){
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  myReadFile.open(inputFileName.c_str());
  if (myReadFile.is_open()) {
    while (!myReadFile.eof()) {
      lineCount++;
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      // CAREFULL: SKIP FIRST FILE: FITNESS PARAMETER
      if ((buffer!="")&&(buffer[0]!='/')){
        try{
          params[paramCount] = atof(buffer.c_str());
          // Update Counter
          paramCount++;
        }catch (exception& e){
          printf("ERROR: Invalid Parameter File.\n");
          myReadFile.close();
          return 1;
        }
      }
    }
  }else{
    printf("ERROR: Cannot Open Parameter File.\n");
    myReadFile.close();
    return 1;
  }
  myReadFile.close();
  return 0;
}

int readPriorFromFile(string inputFileName,int &prior_num,vector<double> &prAv,vector<double> &prSd){
  // Open File
  ifstream myReadFile;
  string buffer;
  vector<string> tokens;
  int lineCount = 0;
  prior_num = 0;
  myReadFile.open(inputFileName.c_str());
  if (myReadFile.is_open()) {
    while (!myReadFile.eof()) {
      lineCount++;
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      // CAREFULL: SKIP FIRST FILE: FITNESS PARAMETER
      if ((buffer!="")&&(buffer[0]!='/')){
        try{
          schSplit(buffer,tokens);
          // Update Counter
          prior_num++;
          // Assign
          prAv.push_back(atof(tokens[0].c_str()));
          prSd.push_back(atof(tokens[1].c_str()));          
        }catch (exception& e){
          printf("ERROR: Invalid Parameter File.\n");
          myReadFile.close();
          return 1;
        }
      }
    }
  }else{
    printf("ERROR: Cannot Open Parameter File.\n");
    myReadFile.close();
    return 1;
  }
  myReadFile.close();
  return 0;
}

void WriteResultsToFile(std::string debugFileName, int totalSteps, int totalStates, double* time, double** outVals){
  FILE* stateFile;
  stateFile = fopen(debugFileName.c_str(),"w");
  for(int loopA=0;loopA<totalSteps;loopA++){
    fprintf(stateFile,"%f ",time[loopA]);
    for(int loopB=0;loopB<totalStates;loopB++){
      fprintf(stateFile,"%f ",outVals[loopA][loopB]);
    }
    fprintf(stateFile,"\n");
  }
  // Close State File
  fclose(stateFile);
}

void printCurvesAndPeaks(string fileName,int size,double* t,double* Q,
                         double Q_S,double t_S,double Q_M,double t_M,double Q_D,double t_D,double Q_A,double t_A){
  FILE* outFile;
  outFile = fopen(fileName.c_str(),"w");
  fprintf(outFile,"%f %f %f %f\n",t[0],Q[0],t_S,Q_S);
  fprintf(outFile,"%f %f %f %f\n",t[1],Q[1],t_M,Q_M);
  fprintf(outFile,"%f %f %f %f\n",t[2],Q[2],t_D,Q_D);
  fprintf(outFile,"%f %f %f %f\n",t[3],Q[3],t_A,Q_A);
  for(int loopA=4;loopA<size;loopA++){
    fprintf(outFile,"%f %f\n",t[loopA],Q[loopA]);
  }
  // CLOSE THE FILE
  fclose(outFile);
}

void getHeartStage12BlocksPriorMapping(int* prPtr){
  prPtr[0] = 0;
  prPtr[1] = 1;
  prPtr[2] = 2;
  prPtr[3] = 3;
  prPtr[4] = 4;
  prPtr[5] = 5;
  prPtr[6] = 6;
  prPtr[7] = 7;
  prPtr[8] = 8;
  prPtr[9] = 9;
  prPtr[10] = 10;
  prPtr[11] = 11;
  prPtr[12] = 12;
  prPtr[13] = 13;
  prPtr[14] = 14;
  prPtr[15] = 15;
  prPtr[16] = 16;
  prPtr[17] = -1;
  prPtr[18] = -1;
  prPtr[19] = 17;
  prPtr[20] = 32;
  prPtr[21] = 33;
  prPtr[22] = 34;
  prPtr[23] = 35;
  prPtr[24] = 36;
}

void getHeartStage13BlocksPriorMapping(int* prPtr){
  prPtr[0] = 0;
  prPtr[1] = 1;
  prPtr[2] = 2;
  prPtr[3] = 3;
  prPtr[4] = 4;
  prPtr[5] = 5;
  prPtr[6] = 6;
  prPtr[7] = 7;
  prPtr[8] = 8;
  prPtr[9] = 9;
  prPtr[10] = 10;
  prPtr[11] = 11;
  prPtr[12] = 12;
  prPtr[13] = 13;
  prPtr[14] = 14;
  prPtr[15] = 15;
  prPtr[16] = 16;
  prPtr[17] = -1;
  prPtr[18] = -1;
  prPtr[19] = 17;
  prPtr[20] = 38;
  prPtr[21] = 39;
  prPtr[22] = 40;
  prPtr[23] = 41;
  prPtr[24] = 42;
}

void getHeartStage2PriorMapping(int* prPtr){
  prPtr[0] = 0;
  prPtr[1] = 1;
  prPtr[2] = 2;
  prPtr[3] = 3;
  prPtr[4] = 4;
  prPtr[5] = 5;
  prPtr[6] = 6;
  prPtr[7] = 7;
  prPtr[8] = 8;
  prPtr[9] = 9;
  prPtr[10] = 10;
  prPtr[11] = 11;
  prPtr[12] = 12;
  prPtr[13] = 13;
  prPtr[14] = 14;
  prPtr[15] = 15;
  prPtr[16] = 16;
  prPtr[17] = -1;
  prPtr[18] = -1;
  prPtr[19] = 17;
  prPtr[20] = 38;
  prPtr[21] = 39;
  prPtr[22] = 40;
  prPtr[23] = 41;
  prPtr[24] = 42;
}

void getHeartINDStage12BlocksINDPriorMapping(int* prPtr){
  prPtr[0] = 0;
  prPtr[1] = 1;
  prPtr[2] = 2;
  prPtr[3] = 3;
  prPtr[4] = 4;
  prPtr[5] = 5;
  prPtr[6] = 6;
  prPtr[7] = 7;
  prPtr[8] = 8;
  prPtr[9] = 9;
  prPtr[10] = 10;
  prPtr[11] = 11;
  prPtr[12] = 12;
  prPtr[13] = 13;
  prPtr[14] = 14;
  prPtr[15] = 15;
  prPtr[16] = 16;
  prPtr[17] = -1;
  prPtr[18] = -1;
  prPtr[19] = 17;
  prPtr[20] = 32;
  prPtr[21] = 33;
  prPtr[22] = 34;
  prPtr[23] = 35;
  prPtr[24] = 36;
  prPtr[25] = 38;
  prPtr[26] = 46;
}

void applyCircShift(int shift,int size,double* Qvp){
  double temp[size];
  int shiftedComp = 0;
  // Copy to temporary vector
  for(int loopA=0;loopA<size;loopA++){
    shiftedComp = ((loopA + shift) % (size-1));
    temp[shiftedComp] = Qvp[loopA];
  }
  // Periodicity: first and last components should be exactly the same
  temp[size-1] = temp[0];
  // Copy Back
  for(int loopA=0;loopA<size;loopA++){
    Qvp[loopA] = temp[loopA];
  }
}

/*
### Function sortPeaks

#### Purpose
Sort peak indexes according to their value in array.

#### Discussion
For example, if the sort is of type ipSortDescending, the first index will refer to the 
maximum function value

#### Modified
2 May 2015 - Daniele Schiavazzi

#### Reference
No Reference

\param[in] data_size The size of the data array
\param[in] data Array with data_size values
\param[in] peak_size Total number of detected peaks
\param[in/out] peaks Peak location indexes
\param[in] sortType Type of sort, either ipSortDescending, or ipSortAscending
*/
void sortPeaks(int data_size, double* data, int peak_size, int* peaks, int sortType){
  double firstVal = 0.0;
  double secondVal = 0.0;
  bool doSort = true;
  double temp = 0.0;
  for(int loopA=0;loopA<(peak_size-1);loopA++){
    for(int loopB=loopA+1;loopB<peak_size;loopB++){
      firstVal = data[peaks[loopA]];
      secondVal = data[peaks[loopB]];
      if(sortType == ipSortAscending){
        doSort = (firstVal>secondVal);
      }else{
        doSort = (firstVal<secondVal);
      }
      if(doSort){
        temp = peaks[loopA];
        peaks[loopA] = peaks[loopB];
        peaks[loopB] = temp;
      }
    }
  }
}

/*
### Function storeFirstNonZeroPeaks

#### Purpose
Store the first two peaks to array ignoring peaks associated with zero index

#### Discussion
None

#### Modified
2 May 2015 - Daniele Schiavazzi

#### Reference
No Reference

\param[out] firstPeaks The array with the stored peaks
\param[in] num_peaks Size of the peak array
\param[in] peaks Array with peak values
*/
bool storeFirstNonZeroPeaks(int num_peaks,int* peaks,int* firstPeaks, int& peakCount){
  bool complete = false;
  peakCount = 0;
  int currentIDX = 0;
  for(int loopA=0;loopA<num_peaks;loopA++){
    currentIDX = peaks[loopA];
    if(currentIDX>0){
      if(!complete){
        firstPeaks[peakCount] = currentIDX;
        peakCount++;
        if(peakCount>1){
          complete = true;
        }
      }
    }
  }
  if(complete){
    return true;
  }else{
    return false;
  }
}

int detect_peak(int data_count, double* t, double* data,
                double &S_time, double &S_peak, bool &S_found,
                double &M_time, double &M_peak, bool &M_found,
                double &D_time, double &D_peak, bool &D_found,
                double &A_time, double &A_peak, bool &A_found,
                int& min_Found_Peaks, int& max_Found_Peaks){

    int    max_emi_peaks = 20;            /* maximum number of emission peaks */ 
    int    emi_peaks[max_emi_peaks];      /* emission peaks will be put here */ 
    int    num_emi_peaks;                 /* number of emission peaks found */
    int    max_absop_peaks = 20;          /* maximum number of absorption peaks */ 
    int    absop_peaks[max_absop_peaks];  /* absorption peaks will be put here */ 
    int    num_absop_peaks;               /* number of absorption peaks found */    
    double delta = 1.0e-3;                /* delta used for distinguishing peaks */
    int    emi_first = 0;                 /* should we search emission peak first of absorption peak first? */

    // Initialize all peaks
    S_found = true;
    M_found = true;
    D_found = true;
    A_found = true;

    // Vars
    int     i;
    double  mx;
    double  mn;
    int     mx_pos = 0;
    int     mn_pos = 0;
    int     is_detecting_emi = emi_first;
    bool    finished;
    int     count;
    int     peakCount;
    int     retValue = 0;    

    mx = data[0];
    mn = data[0];

    num_emi_peaks = 0;
    num_absop_peaks = 0;

    for(i = 1; i < data_count; ++i){
        if(data[i] > mx){
            mx_pos = i;
            mx = data[i];
        }
        if(data[i] < mn){
            mn_pos = i;
            mn = data[i];
        }

        if((is_detecting_emi)&&(data[i] < mx - delta)){
            if(num_emi_peaks >= max_emi_peaks){
              /* not enough spaces */
              printf("NOT SPACE - ");
              return 1;
            }                 
            emi_peaks[num_emi_peaks] = mx_pos;
            ++ (num_emi_peaks);
            is_detecting_emi = 0;
            i = mx_pos - 1;
            mn = data[mx_pos];
            mn_pos = mx_pos;
        }
        else if((!is_detecting_emi)&&(data[i] > mn + delta)){
            if(num_absop_peaks >= max_absop_peaks){
              return 2;
            }              
            absop_peaks[num_absop_peaks] = mn_pos;
            ++ (num_absop_peaks);
            is_detecting_emi = 1;            
            i = mn_pos - 1;
            mx = data[mn_pos];
            mx_pos = mn_pos;
        }
    }

    // IF NO PEAKS FOUND THAN THERE IS A PROBLEM
    if((num_emi_peaks<1)&&((num_absop_peaks<1))){
      S_found = false;
      M_found = false;
      D_found = false;
      A_found = false;
      retValue = -1;
      return retValue;
    }

    // SORT FINAL PEAKS
    sortPeaks(data_count,(double*)data,num_emi_peaks,emi_peaks,ipSortDescending);
    sortPeaks(data_count,(double*)data,num_absop_peaks,absop_peaks,ipSortAscending);

    // Get the first if they are not zero - MAX
    int max_Peak_IDX[2];
    max_Found_Peaks = 0;
    int min_Peak_IDX[2];
    min_Found_Peaks = 0;

    // Store First two peaks
    bool completeEmi = storeFirstNonZeroPeaks(num_emi_peaks,emi_peaks,max_Peak_IDX,max_Found_Peaks);
    bool completeAbs = storeFirstNonZeroPeaks(num_absop_peaks,absop_peaks,min_Peak_IDX,min_Found_Peaks);    
    
    if(completeAbs){
      // Get M and A
      if(t[min_Peak_IDX[0]]<t[min_Peak_IDX[1]]){
        M_time = t[min_Peak_IDX[0]];
        M_peak = data[min_Peak_IDX[0]];
        A_time = t[min_Peak_IDX[1]];
        A_peak = data[min_Peak_IDX[1]];
      }else{
        M_time = t[min_Peak_IDX[1]];
        M_peak = data[min_Peak_IDX[1]];
        A_time = t[min_Peak_IDX[0]];
        A_peak = data[min_Peak_IDX[0]];
      }
    }else{
      // Only one peak for absorption
      if(min_Found_Peaks == 1){
        M_time = t[min_Peak_IDX[0]];
        M_peak = data[min_Peak_IDX[0]];
      }else{
        M_time = 0.0;
        M_peak = 0.0;
        M_found = false;        
      }
      A_time = 0.0;
      A_peak = 0.0;
      A_found = false;
    }
    
    if(completeEmi){
      if(completeAbs){
        // The point in between is D
        if((t[max_Peak_IDX[0]]>M_time)&&(t[max_Peak_IDX[0]]<A_time)){
          D_time = t[max_Peak_IDX[0]];
          D_peak = data[max_Peak_IDX[0]];
          if((t[max_Peak_IDX[1]]<M_time)||(t[max_Peak_IDX[1]]>A_time)){
            S_time = t[max_Peak_IDX[1]];
            S_peak = data[max_Peak_IDX[1]];
          }else{
            S_time = 0.0;
            S_peak = 0.0;
            S_found = false;
          }
        }else if((t[max_Peak_IDX[1]]>M_time)&&(t[max_Peak_IDX[1]]<A_time)){
          D_time = t[max_Peak_IDX[1]];
          D_peak = data[max_Peak_IDX[1]];
          if((t[max_Peak_IDX[0]]<M_time)||(t[max_Peak_IDX[0]]>A_time)){
            S_time = t[max_Peak_IDX[0]];
            S_peak = data[max_Peak_IDX[0]];
          }else{
            S_time = 0.0;
            S_peak = 0.0;
            S_found = false;          
          }
        }else{
          D_time = 0.0;
          D_peak = 0.0;
          D_found = false;
          retValue = 1;
          // See if S can be assigned
          if((t[max_Peak_IDX[0]]<M_time)||(t[max_Peak_IDX[0]]>A_time)){
            S_time = t[max_Peak_IDX[0]];
            S_peak = data[max_Peak_IDX[0]];
          }else if((t[max_Peak_IDX[1]]<M_time)||(t[max_Peak_IDX[1]]>A_time)){
            S_time = t[max_Peak_IDX[0]];
            S_peak = data[max_Peak_IDX[0]];
          }else{          
            S_time = 0.0;
            S_peak = 0.0;
            S_found = false;          
            retValue = 1;
          }        
        }
      }else{
        // We Only Have M
        if((t[max_Peak_IDX[0]]>M_time)){
          D_time = t[max_Peak_IDX[0]];
          D_peak = data[max_Peak_IDX[0]];
          if((t[max_Peak_IDX[1]]<M_time)){
            S_time = t[max_Peak_IDX[1]];
            S_peak = data[max_Peak_IDX[1]];
          }else{
            S_time = 0.0;
            S_peak = 0.0;
            S_found = false;
          }
        }else if((t[max_Peak_IDX[1]]>M_time)){
          D_time = t[max_Peak_IDX[1]];
          D_peak = data[max_Peak_IDX[1]];
          if((t[max_Peak_IDX[0]]<M_time)){
            S_time = t[max_Peak_IDX[0]];
            S_peak = data[max_Peak_IDX[0]];
          }else{
            S_time = 0.0;
            S_peak = 0.0;
            S_found = false;          
          }
        }else{
          D_time = 0.0;
          D_peak = 0.0;
          D_found = false;
          retValue = 1;
          // See if S can be assigned
          if((t[max_Peak_IDX[0]]<M_time)){
            S_time = t[max_Peak_IDX[0]];
            S_peak = data[max_Peak_IDX[0]];
          }else if((t[max_Peak_IDX[1]]<M_time)){
            S_time = t[max_Peak_IDX[0]];
            S_peak = data[max_Peak_IDX[0]];
          }else{          
            S_time = 0.0;
            S_peak = 0.0;
            S_found = false;          
            retValue = 1;
          }        
        }

      }
    }else{
      // Only one peak for emission
      if(max_Found_Peaks == 1){
        S_time = t[max_Peak_IDX[0]];
        S_peak = data[max_Peak_IDX[0]];
      }else{
        S_time = 0.0;
        S_peak = 0.0;
        S_found = false;        
      }
      D_time = 0.0;
      D_peak = 0.0;
      D_found = false;
      retValue = 1;
    }

    // Final Return
    return retValue;
}

int readTableFromFile(std::string fileName,vector<vector<double> > &samples){
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  std::vector<std::string> tokens;
  std::vector<double> currParams;
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()) {
    // Loop through the File
    while (!myReadFile.eof()){
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      if(!buffer.empty()){
        lineCount++;
        schSplit(buffer,tokens);      
        currParams.clear();
        for(int loopA=0;loopA<tokens.size();loopA++){
          try{
            currParams.push_back(atof(tokens[loopA].c_str()));  
          }catch(...){
            printf("Invalid Table File. Exiting.\n");
            return 1;
          }        
        }
        samples.push_back(currParams);
      }
    }
  }else{
    printf("Cannot Open File. Exiting.\n");
    return 1;
  }
  myReadFile.close();

  // Return
  return 0;
}

int readIntTableFromCSVFile(std::string fileName,stdIntMat &samples){
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  std::vector<std::string> tokens;
  std::vector<int> currParams;
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()) {
    // Loop through the File
    while (!myReadFile.eof()){
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      if(!buffer.empty()){
        lineCount++;
        schSplit(buffer,tokens,",");      
        currParams.clear();
        for(int loopA=0;loopA<tokens.size();loopA++){
          try{
            currParams.push_back(atoi(tokens[loopA].c_str()));
          }catch(...){
            printf("Invalid Table File. Exiting.\n");
            return 1;
          }        
        }
        samples.push_back(currParams);
      }
    }
  }else{
    printf("Cannot Open File. Exiting.\n");
    return 1;
  }
  myReadFile.close();

  // Return
  return 0;
}

void writeTableToFile(std::string fileName,vector<vector<double> > table){
  FILE* stateFile;
  stateFile = fopen(fileName.c_str(),"w");
  for(int loopA=0;loopA<table.size();loopA++){
    for(int loopB=0;loopB<table[loopA].size();loopB++){
      fprintf(stateFile,"%25.10e ",table[loopA][loopB]);
    }
    fprintf(stateFile,"\n");
  }
  // Close State File
  fclose(stateFile);
}

// Write Vector to Text File
void writeVectorToFile(std::string fileName,stdVec vec){
  FILE* stateFile;
  stateFile = fopen(fileName.c_str(),"w");
  for(int loopA=0;loopA<vec.size();loopA++){
    fprintf(stateFile,"%25.10e\n",vec[loopA]);
  }
  // Close State File
  fclose(stateFile);
}

// Write Vector from text file
int readVectorFromFile(std::string fileName,int column,stdVec& vec){
  // Read the full Matrix
  stdMat fileTable;
  int error = readTableFromFile(fileName,fileTable);
  // Check size consistency
  if(column > fileTable[0].size()){
    cmException("ERROR: The column number is too big for the file.\n");
  }
  // Store the specified column in the vector
  vec.clear();
  for(int loopA=0;loopA<fileTable.size();loopA++){
    vec.push_back(fileTable[loopA][column]);
  }
  return error;
}

double linInterp(vector<vector<double> > table, int xColumn,int yColumn,double currX){
  double result = 0.0;
  data temp;
  vector<data> items;
  
  // Copy to Data Structure
  for(int loopA=0;loopA<table.size();loopA++){
    temp.xVal = table[loopA][xColumn];
    temp.yVal = table[loopA][yColumn];
    items.push_back(temp);
  }

  // Sort 
  sort(items.begin(), items.end(), by_number());
  // Check for extrapolation
  if(currX<items[0].xVal){    
    result = items[0].yVal - ((items[1].yVal - items[0].yVal)/(items[1].xVal - items[0].xVal))*(items[0].xVal - currX);
    return result;
  }else if(currX>items[items.size()-1].xVal){
    result = items[items.size()-1].yVal + ((items[items.size()-1].yVal - items[items.size()-2].yVal)/(items[items.size()-1].xVal - items[items.size()-2].xVal))*(currX - items[items.size()-1].xVal);
    return result;
  }

  // Find X Values
  int count = 0;
  bool found = false;
  double thisVal = 0.0;
  double nextVal = 0.0;
  double thisX = 0.0;
  double nextX = 0.0;
  double thisY = 0.0;
  double nextY = 0.0;
  while((!found)&&(count<items.size())){
    thisX = items[count].xVal;
    nextX = items[count + 1].xVal;
    thisY = items[count].yVal;
    nextY = items[count + 1].yVal;
    if(count != (items.size()-1)){
      found = (currX >= thisX)&&(currX < nextX);  
    }else{
      found = (currX >= thisX)&&(currX <= nextX);  
    }
    // Get Result
    if(found){
      result = thisY + ((nextY - thisY)/(nextX - thisX))*(currX - thisX);
      return result;
    }else{
      count++;
    }    
  }
  // Return 0.0 if not found
  throw cmException("Value not found. Returning zero\n");
  return 0.0;
}

void read3DModelSurrogateFromFile(string fileName,model3DRecord &model,bool printReport){
  // Declare input File
  ifstream infile;
  infile.open(fileName.c_str());
  
  if (infile.fail()){    
    printf("Problems Opening File...\n");
    throw cmException("ERROR: Cannot Find 3D Model Surrogate File.\n");
  }

  // Read Data From File
  std::string buffer;
  stdVec temp;
  std::vector<string> tokenizedString;
  while (std::getline(infile,buffer)){
    // Trim String
    boost::trim(buffer);
    // Tokenize String
    boost::split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
    // Try to read file
    try{
      if(boost::iequals(tokenizedString[0],"PARAMS:")){
        // Read polynomial degree, dimensions, poly type      
        // Read POLY TYPE
        if(boost::iequals(tokenizedString[1],"LEGENDRE")){
          model.polyType = kPolyLegendre;
        }else if(boost::iequals(tokenizedString[1],"HERMITE")){
          model.polyType = kPolyHermite;
        }else{
          throw cmException("ERROR: Invalid Polynomial Type in read3DModelSurrogateFile.\n");    
        }
        // Read polynomial degree, dimensions
        model.polyOrder = atoi(tokenizedString[2].c_str());
        model.totDims = atoi(tokenizedString[3].c_str());
      }else if(tokenizedString[0].at(0) == '#'){
        // Comment: Do Nothing
      }else if(boost::iequals(tokenizedString[0],"NORMALIZED:")){
        if(boost::iequals(tokenizedString[1],"TRUE")){
          model.normalizeData = true;
        }else{
          model.normalizeData = false;
        }  
        for(int loopA=0;loopA<model.totDims;loopA++){
          temp.clear();
          // Read Line in buffer
          std::getline(infile,buffer);
          // Trim String
          boost::trim(buffer);
          // Tokenize String
          boost::split(tokenizedString, buffer, boost::is_any_of(" ,"), boost::token_compress_on);
          // Add to Temp
          temp.push_back(atof(tokenizedString[0].c_str()));
          temp.push_back(atof(tokenizedString[1].c_str()));
          // Add to Limits
          model.limits.push_back(temp);
        }      
      }else{
        // READ COEFFICIENTS
        temp.clear();
        for(int loopA=0;loopA<tokenizedString.size();loopA++){
          temp.push_back(atof(tokenizedString[loopA].c_str()));
        }        
        model.expCoeffs.push_back(temp);
      }
    }catch(...){
      throw cmException("ERROR: Invalid Input File Format in read3DModelSurrogateFile.\n");
    }
  }
  // Model ECHO
  if(printReport){
    printf("\n");
    printf("--- SURROGATE MODEL ECHO:\n");
    printf("Surrogate Model File: %s\n",fileName.c_str());
    printf("Polynomial Type: %s\n",getPolyTypeString(model.polyType).c_str());
    printf("Polynomial Order: %d\n",model.polyOrder);
    printf("Input Data Dimensionality: %d\n",model.totDims);
    printf("Normalized Data: %s\n", model.normalizeData ? "True" : "False");
    printf("Limit Matrix Size: %d, %d\n",(int)model.limits.size(),(int)model.limits[0].size());
    printf("Coefficient Matrix Size: %d, %d\n",(int)model.expCoeffs.size(),(int)model.expCoeffs[0].size());
  }
  // Close File
  infile.close();
}

string getPolyTypeString(int polyType){
  string result;
  if(polyType == kPolyLegendre){
    result = "LEGENDRE";
  }else if(polyType == kPolyHermite){
    result = "HERMITE";
  }
  return result;
}

int readIntVectorFromFile(std::string fileName,vector<int> &vec){
  // Open File
  ifstream myReadFile;
  string buffer;
  int paramCount = 0;
  std::vector<std::string> tokens;
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()) {
    // Loop through the File
    while (!myReadFile.eof()){
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      if(!buffer.empty()){
        vec.push_back(atoi(buffer.c_str()));
      }
    }
  }else{
    printf("Cannot Open File. Exiting.\n");
    return 1;
  }
  myReadFile.close();

  // Return
  return 0;
}

bool isParamPositive(int curr_par, stdVec limits){
  double limitMin = limits[curr_par*2 + 0];
  double limitMax = limits[curr_par*2 + 1];
  if((limitMin>=0.0)&&(limitMax>=0.0)){
    return true;
  }else{
    return false;
  }
}

bool isParamNegative(int curr_par, stdVec limits){
  double limitMin = limits[curr_par*2 + 0];
  double limitMax = limits[curr_par*2 + 1];
  if((limitMin<0.0)&&(limitMax<0.0)){
    return true;
  }else{
    return false;
  }
}

int readCSStringTableFromFile(std::string fileName,vector<vector<string> > &table){
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  std::vector<std::string> tokens;
  table.clear();
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()) {
    // Loop through the File
    while (!myReadFile.eof()){
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      if(!buffer.empty()){
        lineCount++;
        schSplit(buffer,tokens,",");      
        table.push_back(tokens);
      }
    }
  }else{
    printf("Cannot Open File. Exiting.\n");
    return 1;
  }
  myReadFile.close();

  // Return
  return 0;
}

int getFrozenParametersFromCSVFile(string fileName,stdIntVec& frozParIDX,stdVec& frozParVAL){
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  frozParIDX.clear();
  frozParVAL.clear();
  std::vector<std::string> tokens;
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()) {
    // Loop through the File
    while (!myReadFile.eof()){
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      if(!buffer.empty()){
        lineCount++;
        schSplit(buffer,tokens,",");
        try{
          frozParIDX.push_back(atoi(tokens[0].c_str()));
          frozParVAL.push_back(atof(tokens[1].c_str()));
        }catch(...){
          printf("Invalid Frozen Parameter File Format. Exiting.\n");
          return 1;
        }
      }
    }
  }else{
    printf("Cannot Open File. Exiting.\n");
    return 1;
  }
  myReadFile.close();

  // Return
  return 0;
}

bool getAccelerationTime(int start, int stop, double timeStep, double* vector, double& at){
  
  // Declare Results
  double S_time = 0.0; 
  double S_peak = 0.0; 
  double M_time = 0.0; 
  double M_peak = 0.0; 
  double D_time = 0.0; 
  double D_peak = 0.0; 
  double A_time = 0.0;  
  double A_peak = 0.0; 
  bool S_found;
  bool M_found;
  bool D_found;
  bool A_found;
  int  minTotPeaks,maxTotPeaks;
  bool retVal;

  // Build Auxiliary Arrays
  int data_count = stop-start;
  double* t = new double[data_count];
  double* data = new double[data_count];
  for(int loopA=start;loopA<stop;loopA++){
    t[loopA-start] = (loopA-start) * timeStep;
    data[loopA-start] = vector[loopA];
  }

  // Find Peaks
  int error = detect_peak(data_count,t,data,
                          S_time,S_peak,S_found,
                          M_time,M_peak,M_found,
                          D_time,D_peak,D_found,
                          A_time,A_peak,A_found,
                          minTotPeaks,maxTotPeaks);
  
  if(S_found){
    at = (S_time) * 1000.0;
    retVal = true;
  }else{
    at = 0.0;
    retVal = false;
  }

  // Delete Auxiliary Vectors
  delete [] t;
  delete [] data;

  // Return
  return retVal;
}

bool getDecelerationTime(int start, int stop, double timeStep, double* vector, double& dt){
  
  // Declare Results
  double S_time = 0.0; 
  double S_peak = 0.0; 
  double M_time = 0.0; 
  double M_peak = 0.0; 
  double D_time = 0.0; 
  double D_peak = 0.0; 
  double A_time = 0.0;  
  double A_peak = 0.0; 
  bool S_found;
  bool M_found;
  bool D_found;
  bool A_found;
  int  minTotPeaks,maxTotPeaks;
  bool retVal;

  // Build Auxiliary Arrays
  int data_count = stop-start;
  double* t = new double[data_count];
  double* data = new double[data_count];
  for(int loopA=start;loopA<stop;loopA++){
    t[loopA-start] = (loopA-start) * timeStep;
    data[loopA-start] = vector[loopA];
  }

  // Find Peaks
  int error = detect_peak(data_count,t,data,
                          S_time,S_peak,S_found,
                          M_time,M_peak,M_found,
                          D_time,D_peak,D_found,
                          A_time,A_peak,A_found,
                          minTotPeaks,maxTotPeaks);
  
  if((M_found)&&(S_found)){
    dt = (M_time - S_time) * 1000.0;
    retVal = true;
  }else{
    dt = 0.0;
    retVal = false;
  }

  // Delete Auxiliary Vectors
  delete [] t;
  delete [] data;

  // Return
  return retVal;
}

bool getEARatio(int start, int stop, double* vector,double& EARatio){
  
  // Declare Results
  double S_time = 0.0;
  double S_peak = 0.0; 
  double M_time = 0.0; 
  double M_peak = 0.0; 
  double D_time = 0.0; 
  double D_peak = 0.0; 
  double A_time = 0.0;  
  double A_peak = 0.0; 
  bool S_found;
  bool M_found;
  bool D_found;
  bool A_found;
  int  minTotPeaks,maxTotPeaks;
  bool retVal;

  // Build Auxiliary Arrays
  int data_count = stop-start;
  double* t = new double[data_count];
  double* data = new double[data_count];
  for(int loopA=start;loopA<stop;loopA++){
    t[loopA-start] = loopA-start;
    data[loopA-start] = vector[loopA];
  }

  // Find Peaks
  int error = detect_peak(data_count,t,data,
                          S_time,S_peak,S_found,
                          M_time,M_peak,M_found,
                          D_time,D_peak,D_found,
                          A_time,A_peak,A_found,
                          minTotPeaks,maxTotPeaks);

  if((S_found)&&(D_found)){
    EARatio = (S_peak/D_peak);
    retVal = true;
  }else{
    EARatio = 0.0;
    retVal = false;
  }

  // Delete Auxiliary Vectors
  delete [] t;
  delete [] data;

  // Return
  return retVal;
}

void zeroAtValveOpening(int start, int stop, double* curve, double* valveIsOpen){
  // Determine the valve opening time
  bool foundOpeningTime = false;
  int valveOpeningIDX = 0;
  int count = start;
  while((!foundOpeningTime)&&(count<stop-1)){
    if(valveIsOpen[count+1] > valveIsOpen[count]){
      foundOpeningTime = true;
      valveOpeningIDX = count;
    }
    if(!foundOpeningTime){
      count++;
    }
  }
  if(!foundOpeningTime){
    //printf("Error: Valve Not Opening at zeroAtValveOpening...\n");
    //fflush(stdout);
    throw cmException("Error: Valve is not opening in heart cycle.\n");
  }
  // Perform Circular Shift
  double* aux = new double[stop-start];
  for(int loopA=valveOpeningIDX;loopA<stop;loopA++){
    aux[loopA-valveOpeningIDX] = curve[loopA];
  }
  for(int loopA=start;loopA<valveOpeningIDX;loopA++){
    aux[(stop - valveOpeningIDX) + loopA - start] = curve[loopA];
  }
  // Copy Back
  for(int loopA=start;loopA<stop;loopA++){
    curve[loopA] = aux[loopA-start];
  }
  // Deallocate
  delete [] aux;
}

void subSampleTableData(string currTraceFile,int& totSubSamples,int startColumn,int endColumn,stdMat& subSampleTable,stdIntVec& sampleIndexes){
  // Read Parameter Table
  subSampleTable.clear();
  stdMat AllSamples;
  stdVec tmpVec;
  int error = readTableFromFile(currTraceFile,AllSamples);
  if(error != 0){
    throw cmException("Error: Cannot Open table with readTableFromFile.\n");
  }

  // Get Total Samples
  int totSamples = AllSamples.size();

  // PLOT ALL DATA IF NUMBER OF SUBSAMPLE IS BIGGER THAN TOTAL
  sampleIndexes.clear();
  if(totSubSamples > totSamples){
    totSubSamples = totSamples;
    for(int loopA=0;loopA<totSamples;loopA++){
      // Add the sample index
      sampleIndexes.push_back(loopA);
      // Add Row to subSampleTable
      tmpVec.clear();
      for(int loopB=startColumn;loopB<=endColumn;loopB++){
        tmpVec.push_back(AllSamples[loopA][loopB]);
      }
      subSampleTable.push_back(tmpVec);
    }
  }else{
    // SUB-SAMPLE
    // INIT IS USED VECTOR
    bool* isUsed = new bool[totSamples];
    for(int loopA=0;loopA<totSamples;loopA++){
      isUsed[loopA] = false;
    }

    // INIT RANDOM GENERATOR
    //srand(time(NULL));
    srand(1384.0);

    // SUBSAMPLE
    int currIDX = 0;    
    int count = 0;
    while(count<totSubSamples){
      // Select Random index from 0 to totSamples-1
      currIDX = rand()%totSamples;
      if(!isUsed[currIDX]){
        sampleIndexes.push_back(currIDX);
        // Add Row to subSampleTable
        tmpVec.clear();
        for(int loopB=startColumn;loopB<=endColumn;loopB++){
          tmpVec.push_back(AllSamples[currIDX][loopB]);
        }
        subSampleTable.push_back(tmpVec);
        // Fill isUsed
        isUsed[currIDX] = true;
        count++;
      }
    }
    // Deallocate
    delete [] isUsed;
  }
}

// Subsample from Trace file with known indexes
void subSampleTableDataWithIndex(string currTraceFile,stdIntVec sampleIndexes,int startColumn,int endColumn,stdMat& subSampleTable){
  // Read Parameter Table
  subSampleTable.clear();
  stdMat AllSamples;
  stdVec tmpVec;
  int error = readTableFromFile(currTraceFile,AllSamples);
  if(error != 0){
    throw cmException("Error: Cannot Open table with readTableFromFile.\n");
  }

  // Get Total Samples
  int totSamples = sampleIndexes.size();

  // Add Table entries at specified indexes
  int currIDX = 0;
  for(int loopA=0;loopA<totSamples;loopA++){
    currIDX = sampleIndexes[loopA];
    tmpVec.clear();
    for(int loopB=startColumn;loopB<endColumn+1;loopB++){
      tmpVec.push_back(AllSamples[currIDX][loopB]);
    }
    subSampleTable.push_back(tmpVec);
  }
}

// Create Frquency Plot from series of doubles
void generateFrequencyPlot(double minVal,double maxVal,stdVec values,int totBins,bool normalize,stdVec& resX,stdVec& resY){

  // Check is Min and Max are the same
  bool isConstant = (fabs(maxVal-minVal) < kMathZero);

  // step
  double step = (maxVal-minVal)/(double)totBins;

  if(isConstant){
    step = 1.0;
  }else{
    step = (maxVal-minVal)/(double)totBins;
  }
    
  // Form X Vector
  resX.clear();
  resY.clear();
  resX.resize(totBins);
  resY.resize(totBins);

  // Form 
  for(int loopA=0;loopA<totBins;loopA++){
    resX[loopA] = minVal + 0.5*step + loopA*(maxVal-minVal-step)/(double)(totBins-1);  
    resY[loopA] = 0.0;
  }

  if(isConstant){
    return;
  }

  // Loop through values and assign to bins
  double currValue = 0.0;
  int currBin = 0;
  for(int loopA=0;loopA<values.size();loopA++){
    currValue = values[loopA];
    if(isConstant){
      currBin = 0;
    }else{
      currBin = int((currValue - minVal)/step);  
    }
    resY[currBin] += 1.0; 
  }

  // Normalize if required
  if(normalize){
    double intVal = 0.0;
    for(int loopA=0;loopA<totBins;loopA++){
      intVal += step * resY[loopA];
    }
    for(int loopA=0;loopA<totBins;loopA++){
      resY[loopA] /= intVal;
    }
  }
}