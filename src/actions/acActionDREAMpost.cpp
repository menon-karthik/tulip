// =============================================
// POST-PROCESSING RESULTS FROM MCMC SIMULATIONS
// Daniele Schiavazzi, 2014
// =============================================

#include "acActionDREAM.h"

using namespace std;

// =============================
// WRITE BEST PARAMETERS TO FILE
// =============================
void writeBestParamsToFile(string fileName,double bestFit,int par_num, double* currBestParams){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  for(int loopA=0;loopA<(par_num-2);loopA++){
    fprintf(f,"%e\n",currBestParams[loopA]);
  }
  fclose(f);
}

// ==================================
// WRITE PARAMETER STATISTICS TO FILE
// ==================================
void writeStatisticsToFile(string fileName,int par_num,vector<vector<double> > &modelStats){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  for(int loopA=0;loopA<(par_num-2);loopA++){
    fprintf(f,"%e %e\n",modelStats[loopA][0],modelStats[loopA][1]);
  }
  fclose(f);
}

// ========================
// GET FILES IN CURRENT DIR
// ========================
int getFilesInCurrentDir(std::vector<std::string> &allFiles){
  DIR *dp;
  struct dirent *ep;     
  dp = opendir ("./");

  if (dp != NULL){
    while ((ep = readdir (dp))){
      allFiles.push_back(ep->d_name);
    }      
    (void) closedir (dp);
  }
  else{
    perror ("Couldn't open the directory");
  }
  // Return
  return 0;
}

// ==================
// Print Sample Table
// ==================
void printSampleTable(string fileName,vector<vector<double> > mcmcSamples){
  FILE *f;
  f = fopen(fileName.c_str(), "w");
  // Get Number of Samples and Runs
  int sample_num = mcmcSamples.size();
  int par_num = mcmcSamples[0].size();
  // Print info
  fprintf(f,"%d %d\n",sample_num,par_num);
  for(int loopA=0;loopA<sample_num;loopA++){
    for(int loopB=0;loopB<par_num;loopB++){
      fprintf(f,"%f ",mcmcSamples[loopA][loopB]);
    }
    fprintf(f,"\n");
  }
  fclose(f);
}

// ====================
// GET MCMC CHAIN FILES
// ====================
void getMCMCChainFileList(std::string fileNameToken, std::vector<std::string> &mcmcFileList){
  
  // Get List of files in current Dir
  std::vector<std::string> allFiles;
  getFilesInCurrentDir(allFiles);
  
  // Filter Based on Name
  int found = 0;
  for(int loopA=0;loopA<allFiles.size();loopA++){
    found = allFiles[loopA].find(fileNameToken);
    if (found!=std::string::npos){
      mcmcFileList.push_back(allFiles[loopA]);
    }
  }
}

// ======================================
// READ CHAIN FILE AND EXTRACT PARAMETERS
// ======================================
int readMCMCSamplesFromFile(double burnInPercent,std::string fileName,std::vector< std::vector<double> > &samples){
  std::vector< std::vector<double> > samplesTemp;
  // Open File
  ifstream myReadFile;
  string buffer;
  int lineCount = 0;
  int paramCount = 0;
  std::vector<std::string> tokens;
  std::vector<double> currParams;
  myReadFile.open(fileName.c_str());
  if (myReadFile.is_open()) {
    // Skip first comment line
    std::getline(myReadFile,buffer);
    lineCount++;
    // Loop through the File
    while (!myReadFile.eof()){
      // Read One Line of Input File
      std::getline(myReadFile,buffer);
      if(!buffer.empty()){
        lineCount++;
        //printf("Buffer: %s \n",buffer.c_str());
        cmUtils::schSplit(buffer,tokens," ");      
        currParams.clear();
        for(int loopA=0;loopA<tokens.size();loopA++){
          try{
            currParams.push_back(atof(tokens[loopA].c_str()));  
            //printf("token %d: %s\n",loopA,tokens[loopA].c_str());
          }catch(...){
            printf("Invalid Chain File. Exiting.\n");
            return 1;
          }        
        }
        samplesTemp.push_back(currParams);
      }
    }
  }else{
    printf("Cannot Open File. Exiting.\n");
    return 1;
  }
  myReadFile.close();

  // Delete The Burn-In
  int num_cleanSamples = floor(samplesTemp.size()*burnInPercent/100.0);

  //Copy to Samples
  for(int loopA=num_cleanSamples+1;loopA<samplesTemp.size();loopA++){
    samples.push_back(samplesTemp[loopA]);
  }

  // Return
  return 0;
}

// =========================================
// WRITE PARAMETER STATISTICS TO LATEX TABLE
// =========================================
void acActionDREAM::writeStatisticsToLatexTable(int par_num,stdMat& modelStats){
  FILE *f;
  string paramName;
  f = fopen("priorTable.txt", "w");
  fprintf(f,"\\begin{table}[h!]\n");
  fprintf(f,"\\centering\n");
  fprintf(f,"\\begin{tabular}{l c c}\n");
  fprintf(f,"\\toprule\n");
  fprintf(f,"Parameter & $\\mu$ & $\\sigma$ \\\\\n");
  fprintf(f,"\\midrule\n");
    for(int loopA=0;loopA<(par_num-2);loopA++){
    paramName = model->getParamName(loopA);
    fprintf(f,"%s & %f & %f \\\\\n",paramName.c_str(),modelStats[loopA][0],modelStats[loopA][1]);
  }
  fprintf(f,"\\bottomrule\n");
  fprintf(f,"\\end{tabular}\n");
  fprintf(f,"\\caption{Prior heart model parameters for patient \\patient}\n");
  fprintf(f,"\\end{table}\n");  
  fclose(f);  
}

// =============
// MAIN FUNCTION
// =============
int acActionDREAM::postProcess(bool debugMode, double burnInPercent){

  // Initialize File Token
  string fileNameToken = "chain_GR";
  //dreamChainFileName;

  // Print token
  if(printLevel > 0){
    printf("TOKEN: %s\n",fileNameToken.c_str());  
  }

  // Get List of Files with MCMC Chain
  std::vector<std::string> mcmcFileList;
  getMCMCChainFileList(fileNameToken,mcmcFileList);

  // PRINT FILES
  if(printLevel > 0){
    printf("\n");
    printf("LIST OF MCMC CHAIN FILES\n");
    for(int loopA=0;loopA<mcmcFileList.size();loopA++){
      printf("%s\n",mcmcFileList[loopA].c_str());
    }
  }

  // Fill Matrix with samples and NO BURN-IN
  std::vector< std::vector<double> > mcmcSamples;
  for(int loopA=0;loopA<mcmcFileList.size();loopA++){
    readMCMCSamplesFromFile(burnInPercent,mcmcFileList[loopA],mcmcSamples);
  }

  // Print Sample Table
  printSampleTable("paramTraces.txt",mcmcSamples);

  // Set Size
  int sample_num = mcmcSamples.size();
  int par_num = mcmcSamples[0].size();

  if(printLevel > 0){
    printf("\n");
    printf("TOTALS\n");
    printf("Number of Samples: %d\n",sample_num);
    printf("Number of Parameters: %d\n",par_num);
  }

  // Get Best Result and Write File
  double bestFit = -numeric_limits<double>::max();
  double currBestParams[par_num-2];
  double currFit = 0.0;
  for(int loopA=0;loopA<sample_num;loopA++){    
    currFit = mcmcSamples[loopA][1];
    if(currFit>bestFit){
      // Store Best Fit
      bestFit = currFit;
      // Store Parameter Values
      for(int loopB=2;loopB<par_num;loopB++){
        currBestParams[loopB-2] = mcmcSamples[loopA][loopB];
      }
    }
  }

  // Write Best Parameters to File
  writeBestParamsToFile("bestParams.txt",bestFit,par_num,currBestParams);
  if(printLevel > 0){
    printf("\n");
    printf("BEST PARAMETERS\n");
    printf("Best Fit: %f\n",bestFit);
    for(int loopA=0;loopA<(par_num-2);loopA++){
      printf("Param %d: %f\n",(loopA+1),currBestParams[loopA]);
    }    
  }
  
  // Get First and Second Moments of LPN Parameters
  vector<vector<double> > modelStats;
  // Allocate
  modelStats.resize(par_num-2);
  for(int loopA=0;loopA<(par_num-2);loopA++){
    modelStats[loopA].resize(2);
  } 

  // Compute Average
  for(int loopA=2;loopA<par_num;loopA++){
    modelStats[loopA-2][0] = 0.0;
    for(int loopB=0;loopB<sample_num;loopB++){
      modelStats[loopA-2][0] += mcmcSamples[loopB][loopA];       
    }
    modelStats[loopA-2][0] = modelStats[loopA-2][0]/(double)sample_num;
  }

  // Compute Standard Deviation
  for(int loopA=2;loopA<par_num;loopA++){
    modelStats[loopA-2][1] = 0.0;
    for(int loopB=0;loopB<sample_num;loopB++){
      modelStats[loopA-2][1] += (mcmcSamples[loopB][loopA] - modelStats[loopA-2][0])*(mcmcSamples[loopB][loopA] - modelStats[loopA-2][0]);
    }
    modelStats[loopA-2][1] = sqrt(modelStats[loopA-2][1]/(double)(sample_num - 1));
  }

  // Write Model Stats To File
  writeStatisticsToFile("paramStats.txt",par_num,modelStats);
  //writeStatisticsToLatexTable(par_num,modelStats);
  if(printLevel > 0){
    printf("\n");
    printf("PARAMETER STATISTICS\n");
    for(int loopA=0;loopA<(par_num-2);loopA++){
      printf("Param %d: %f %f\n",(loopA+1),modelStats[loopA][0],modelStats[loopA][1]);
    }
  }

  // Completed
  if(printLevel > 0){
    printf("\n");
    printf("Post-Processing Completed.\n");  
  }

  // RETURN
  return 0;
}


