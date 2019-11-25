# include <math.h>
# include "acActionUP_MC.h"

// CONSTRUCTOR
acActionUP_MC::acActionUP_MC(uqSamples* locInputs): acActionUP(locInputs){
  inputs = locInputs;
  // Initialize options
  opts.numberOfRepeats = 1;
  opts.sampleGroups.push_back(10);
  opts.storeSamples = false;
  opts.useExistingSamples = true;
}

// PERFORM MCMC
int acActionUP_MC::go(){

  // Write Warning if using existing samples but more than one group
  if((opts.sampleGroups.size()>1)&&(opts.useExistingSamples)){
    printf("Warning: using existing samples with multiple sampling groups. All groups will be equal.\n");
  }

  // Inputs for the numerical model
  stdVec modelInputs;
  stdVec modelOutputs;
  stdIntVec modelErrors;
  
  // Get number of dimensions
  int totDims = inputs->getTotDims();
  
  // Get total number of model outputs
  int totOutputs = model->getResultTotal();

  // Allocate Statistics Storage
  avgStats.resize(opts.numberOfRepeats);
  stdStats.resize(opts.numberOfRepeats);
  for(int loopA=0;loopA<opts.numberOfRepeats;loopA++){
    avgStats[loopA].resize(opts.sampleGroups.size());
    stdStats[loopA].resize(opts.sampleGroups.size());
  }

  // Results for the current sample Group
  stdMat currGroupOutputs;
  double ll = 0.0;
  double currAVG = 0.0;
  double currSTD = 0.0;
  int currProgress = 0;
  int prevProgress = 1;

  // Loop on the number of Repeats
  for(int loopA=0;loopA<opts.numberOfRepeats;loopA++){

    // Print number of repeat
    printf("Repeat %d/%d\n",loopA+1,opts.numberOfRepeats);

    // Loop on the number of Sample Groups
    int currNumSamples = 0;
    for(int loopB=0;loopB<opts.sampleGroups.size();loopB++){

      // Print number of repeat
      printf("Group %d/%d, Samples %d\n",loopB+1,(int)opts.sampleGroups.size(),opts.sampleGroups[loopB]);

      // Get number of Samples for this group
      currNumSamples = opts.sampleGroups[loopB];

      // Generate Random Samples
      // printf("Generating Samples...\n");
      if(!opts.useExistingSamples){
        inputs->generateRandomSamples(currNumSamples);
      }
      // inputs->printToFile("sampleFile.txt",true);
      // getchar();

      // Loop on the number of Samples
      currGroupOutputs.clear();
      for(int loopC=0;loopC<currNumSamples;loopC++){

        // Print Progress
        currProgress = int(100.0 * loopC/(double)currNumSamples);
        if((currProgress % 10 == 0)&&(currProgress != prevProgress)){
          printf("%d..",currProgress);
          fflush(stdout);
          prevProgress = currProgress;
        }

      	// Copy Samples as model inputs
        modelInputs.clear();
      	for(int loopD=0;loopD<totDims;loopD++){
      	  // Add Samples
          modelInputs.push_back(inputs->getValuesAt(loopC,loopD));
        }

        // Solve Models
        // printf("Evaluating Sample %d...\n",loopC+1);
        ll = model->evalModelError(modelInputs,modelOutputs,modelErrors);

        // Store inputs and outputs in tables if required
        if(opts.storeSamples){
          // store inputs in table
          all_inputs.push_back(modelInputs);
          // store outputs in table
          all_outputs.push_back(modelOutputs);
        }

        // Store Results
        currGroupOutputs.push_back(modelOutputs);
      }
      printf("100..OK.\n");

      // Compute and Store Statistics (First and Second)
      // printf("Computing Statistics...\n");
      // For Every Result you have two Statistics      
      for(int loopC=0;loopC<totOutputs;loopC++){
        
        // Compute Avergage Value	
        currAVG = 0.0;
      	for(int loopD=0;loopD<currNumSamples;loopD++){
          // printf("Output: %f\n",currGroupOutputs[loopD][loopC]);
          currAVG += currGroupOutputs[loopD][loopC];
      	}
      	currAVG = currAVG/(double)currNumSamples;
        // printf("Average: %f\n",currAVG);
        // getchar();
      	
      	// Compute Standard Deviation
      	currSTD = 0.0;
      	for(int loopD=0;loopD<currNumSamples;loopD++){
          // printf("Output: %f, Average: %f\n",currGroupOutputs[loopD][loopC],currAVG);
          currSTD += (currGroupOutputs[loopD][loopC] - currAVG) * (currGroupOutputs[loopD][loopC] - currAVG);
      	}
        // printf("Squared Sum: %f\n",currSTD);
      	currSTD = sqrt(currSTD/(double)(currNumSamples - 1));
        // printf("Current deviation: %f\n",currSTD);
        // getchar();

      	// Store Computed Statistics
        avgStats[loopA][loopB].push_back(currAVG);
        stdStats[loopA][loopB].push_back(currSTD);
      }
    }
  }
  // Return 
  return 0;
}

// PRINT MC STATS TO FILE
void acActionUP_MC::printStatsToFile(string fileName){
  
  // Get Counters
  int totRepeats = avgStats.size();
  int totSamples = avgStats[0].size();
  int totOutputs = avgStats[0][0].size();

  // Save to files
  FILE* avgF;
  FILE* stdF;
  // Loop on the model outputs
  for(int loopA=0;loopA<totOutputs;loopA++){
    // Open File 
    avgF = fopen(string(fileName + "_AVG_out" + to_string(loopA+1) + ".txt").c_str(),"w");
    stdF = fopen(string(fileName + "_STD_out" + to_string(loopA+1) + ".txt").c_str(),"w");
    // Print Header
    fprintf(avgF,"00 ");
    fprintf(stdF,"00 ");
    for(int loopC=0;loopC<totSamples;loopC++){
      fprintf(avgF,"%d ",opts.sampleGroups[loopC]);
      fprintf(stdF,"%d ",opts.sampleGroups[loopC]);
    }
    fprintf(avgF,"\n");
    fprintf(stdF,"\n");
    // Write Matrix to File
    for(int loopB=0;loopB<totRepeats;loopB++){
      fprintf(avgF,"%d ",loopB+1);
      fprintf(stdF,"%d ",loopB+1);
      for(int loopC=0;loopC<totSamples;loopC++){
        fprintf(avgF,"%e ",avgStats[loopB][loopC][loopA]);
        fprintf(stdF,"%e ",stdStats[loopB][loopC][loopA]);
      }      
      fprintf(avgF,"\n");
      fprintf(stdF,"\n");
    }
    // Close State File
    fclose(avgF);
    fclose(stdF);
  }
}

