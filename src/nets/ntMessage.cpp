# include "ntMessage.h"

ntMessage::ntMessage(){
}

ntMessage::ntMessage(messageTypes messageType,int sourceID,int targetID,
                     const stdStringVec& labels,const stdVec& stds,const stdVec& limits,
                     const stdMat& msg){
  this->messageType = messageType;
  this->sourceID = sourceID;
  this->targetID = targetID;
  this->varLabels = labels;
  this->varSTDs = stds;
  this->varLimits = limits;
  this->msg = ntUtils::copySamples(msg);
}

// Copy Contructor
ntMessage::ntMessage(ntMessage* newMsg){
  this->messageType = newMsg->messageType;
  this->sourceID = newMsg->sourceID;
  this->targetID = newMsg->targetID;
  this->varLabels = newMsg->varLabels;
  this->varSTDs = newMsg->varSTDs;
  this->varLimits = newMsg->varLimits;
  this->msg = ntUtils::copySamples(newMsg->msg);
}

ntMessage::~ntMessage(){
}

void ntMessage::aggregate(ntMessage* newMsg){

  // If the current message is empty then copy the new one
  if(msg.size() == 0){

  	this->messageType = newMsg->messageType;
  	// Source Entity that sent the message
    this->sourceID = newMsg->sourceID;
    // Target Entity that received the message
    this->targetID = newMsg->targetID;
    // Label variables in the message
    this->varLabels = newMsg->varLabels;
    // Variable standard deviations
    this->varSTDs = newMsg->varSTDs;
    // Variable limits
    this->varLimits = newMsg->varLimits;
    // Samples forming the message
   this->msg = ntUtils::copySamples(newMsg->msg);

  }else{

    // Check the number of samples to be the same 
    // otherwise merging is not possible
    if(this->msg.size() != newMsg->msg.size()){
      printf("Msg1 Size: %d, Msg2 Size: %d\n",int(this->msg.size()),int(newMsg->msg.size()));
      throw ntException("ERROR: Incompatible messages in ntMessage::aggregate.");
    }else{

      // Add variable labels, standard deviations and limits
      for(int loopA=0;loopA<newMsg->varLabels.size();loopA++){
        this->varLabels.push_back(newMsg->varLabels[loopA]);
        this->varSTDs.push_back(newMsg->varSTDs[loopA]);
        this->varLimits.push_back(newMsg->varLimits[loopA*2]);
        this->varLimits.push_back(newMsg->varLimits[loopA*2+1]);
      }

      // Since these are different variables add to the dimensionality
      for(int loopA=0;loopA<this->msg.size();loopA++){
        for(int loopB=0;loopB<newMsg->msg[loopA].size();loopB++){
          this->msg[loopA].push_back(newMsg->msg[loopA][loopB]);
        }
      }
    }
  }
}

void ntMessage::aggregateFromList(vector<ntMessage*> msgs){

  // Check that all messages have the same sourceID and targetID
  bool allEq = true;
  if(msgs.size() > 1){
    int sID = msgs[0]->sourceID;
    int tID = msgs[0]->targetID;
    for(int loopA=1;loopA<msgs.size();loopA++){  
      allEq = allEq && ((sID == msgs[loopA]->sourceID)&&(tID == msgs[loopA]->targetID));
    }
  }
  if(!allEq){
    throw ntException("ERROR: Incompatible messages in ntMessage::aggregateFromList.");
  }else{
    // Aggregate
    for(int loopA=0;loopA<msgs.size();loopA++){
      // printf("aggregateFromList iteration %d\n",loopA);
      this->aggregate(msgs[loopA]);
    }
  }
}

void ntMessage::addEvidence(int varID,const stdVec& vals){
  double noise = 0.0;
  // Check compatibility
  if(vals.size() != msg.size()){
    throw ntException("ERROR: Incompatible messages in ntMessage::addEvidence.");
  }
  for(int loopB=0;loopB<vals.size();loopB++){
    msg[loopB][varID] = vals[loopB];
  }
}

string ntMessage::getMsgTypeString(){
  if(messageType == mtFactorToNode){
    return string("Factor_To_Node");
  }else if(messageType == mtNodeToFactor){
    return string("Node_To_Factor");
  }else{
    throw ntException("ERROR: Invalid message type in getMsgTypeString."); 
  }
}

bool areCompatibleMsgs(vector<ntMessage*> msgs){
  bool check = true;
  for(int loopA=1;loopA<msgs.size();loopA++){
    check = check && ntUtils::sameStringVec(msgs[0]->varLabels,msgs[loopA]->varLabels);
    check = check && ntUtils::sameVec(msgs[0]->varSTDs,msgs[loopA]->varSTDs);
    check = check && ntUtils::sameVec(msgs[0]->varLimits,msgs[loopA]->varLimits);
  }
  return check;
}

void ntMessage::aggregateMarginals_fake(vector<ntMessage*> msgs){
  // Only one message
  this->messageType = msgs[0]->messageType;
  this->sourceID    = msgs[0]->sourceID;
  this->targetID    = msgs[0]->targetID;
  this->varLabels   = msgs[0]->varLabels;
  this->varSTDs     = msgs[0]->varSTDs;
  this->varLimits   = msgs[0]->varLimits;
  this->msg = ntUtils::copySamples(msgs[0]->msg);
}

void ntMessage::aggregateMarginals(vector<ntMessage*> msgs){

  int totBins = 30;
  int numSamples = 0;
  uqPDF* uSampler = new uqUniformPDF();

  if(msgs.size() == 1){

    // Only one message
    this->messageType = msgs[0]->messageType;
    this->sourceID    = msgs[0]->sourceID;
    this->targetID    = msgs[0]->targetID;
    this->varLabels   = msgs[0]->varLabels;
    this->varSTDs     = msgs[0]->varSTDs;
    this->varLimits   = msgs[0]->varLimits;
    this->msg = ntUtils::copySamples(msgs[0]->msg);

  }else{
    
    // Multiple Messages
    if(areCompatibleMsgs(msgs)){

      // Assign common properties
      this->messageType = msgs[0]->messageType;
      this->sourceID    = msgs[0]->sourceID;
      this->targetID    = msgs[0]->targetID;
      this->varLabels   = msgs[0]->varLabels;
      this->varSTDs     = msgs[0]->varSTDs;
      this->varLimits   = msgs[0]->varLimits;

      // Get total number of samples
      numSamples = msgs[0]->msg.size();

      // Allocate message sample storage      
      stdVec temp;
      for(int loopA=0;loopA<numSamples;loopA++){
        temp.clear();
        for(int loopB=0;loopB<varLabels.size();loopB++){
          temp.push_back(0.0);
        }
        this->msg.push_back(temp);
      }
    
      // Determine maximum and minimum for each variable
      stdVec sampleLimits(2*varLabels.size(),0.0);
      double currMax = 0.0;
      double currMin = 0.0;
      double groupMax = 0.0;
      double groupMin = 0.0;
      for(int loopA=0;loopA<varLabels.size();loopA++){                
        // printf("Variable %s\n",varLabels[loopA].c_str());
        groupMax = numeric_limits<double>::max();
        groupMin = -numeric_limits<double>::max();        
        for(int loopB=0;loopB<msgs.size();loopB++){
          // Set the min and max for the current message
          currMax = -numeric_limits<double>::max();
          currMin = numeric_limits<double>::max();
          for(int loopC=0;loopC<numSamples;loopC++){
            
            if(msgs[loopB]->msg[loopC][loopA] > currMax){
              currMax = msgs[loopB]->msg[loopC][loopA];
            }

            if(msgs[loopB]->msg[loopC][loopA] < currMin){
              currMin = msgs[loopB]->msg[loopC][loopA];
            }
          }

          // printf("Message %d\n",loopB);
          // printf("min: %f, max: %f\n",currMin,currMax);

          if(currMin > groupMin){
            groupMin = currMin;
          }

          if(currMax < groupMax){
            groupMax = currMax;
          }
        }
        // printf("group min: %f, group max: %f\n",groupMin,groupMax);        
        sampleLimits[2*loopA+0] = groupMin;
        sampleLimits[2*loopA+1] = groupMax;
      }

      // for(int loopA=0;loopA<varLabels.size();loopA++){
      //   printf("%s min: %f max: %f\n",varLabels[loopA].c_str(),sampleLimits[2*loopA+0],sampleLimits[2*loopA+1]);
      // }

      // Determine common discretization using limits      
      stdMat discrX;
      double deltaX = 0.0;
      double currBinCenter = 0.0;
      stdVec tmp;
      stdVec deltas;
      stdVec prod(totBins,0.0);
      for(int loopA=0;loopA<varLabels.size();loopA++){        
        deltaX = (sampleLimits[2*loopA+1]-sampleLimits[2*loopA+0])/double(totBins);
        deltas.push_back(deltaX);
        currBinCenter = sampleLimits[2*loopA+0] + 0.5*deltaX;
        tmp.clear();
        tmp.push_back(currBinCenter);
        for(int loopB=0;loopB<totBins-1;loopB++){
          currBinCenter += deltaX;
          tmp.push_back(currBinCenter);
        }
        discrX.push_back(tmp);
      }

      // for(int loopA=0;loopA<varLabels.size();loopA++){
      //   printf("%s\n",varLabels[loopA].c_str());
      //   for(int loopB=0;loopB<totBins;loopB++){
      //     printf("%f ",discrX[loopA][loopB]);
      //   }
      //   printf("\n");
      // }

      // Allocate PMF
      stdMat pmfs;
      stdVec pmfTemp;
      for(int loopA=0;loopA<msgs.size();loopA++){
        pmfTemp.clear();
        for(int loopB=0;loopB<totBins;loopB++){
          pmfTemp.push_back(0.0);
        }
        pmfs.push_back(pmfTemp);
      }

      // Construct PMF
      double currSample = 0.0;
      int currBinID = 0;
      double normConst = 0.0;
      stdVec newSamples;

      // Loop on the variables
      for(int loopA=0;loopA<varLabels.size();loopA++){        

        // Loop on messages to be aggregated
        for(int loopB=0;loopB<msgs.size();loopB++){

          // Loop on the samples
          for(int loopC=0;loopC<numSamples;loopC++){
            // Get Sample
            currSample = msgs[loopB]->msg[loopC][loopA];
            // Get corresponding bin
            currBinID = int((currSample-sampleLimits[2*loopA+0])/deltas[loopA]);
            // Some of the samples might be outside the limits so be careful!!!
            if((currBinID >= 0)&&(currBinID < numSamples)){
              pmfs[loopB][currBinID] += 1.0;
            }
          }

          // Make it a valid PMF
          normConst = 0.0;
          for(int loopC=0;loopC<totBins;loopC++){
            normConst += pmfs[loopB][loopC];
          }
          for(int loopC=0;loopC<totBins;loopC++){
            pmfs[loopB][loopC] /= normConst;
          }
          // for(int loopC=0;loopC<pmfs[loopB].size();loopC++){
          //   printf("%f ",pmfs[loopB][loopC]);
          // }
          // printf("\n");


        }

        // Perform product
        for(int loopB=0;loopB<totBins;loopB++){
          prod[loopB] = 1.0;
          for(int loopD=0;loopD<msgs.size();loopD++){
            prod[loopB] *= pmfs[loopD][loopB];
          }
        }

        // Normalize resulting PMFs
        normConst = 0.0;
        for(int loopB=0;loopB<totBins;loopB++){
          normConst += prod[loopB];
        }
        for(int loopB=0;loopB<totBins;loopB++){
          prod[loopB] /= normConst;
        }

        // cmUtils::writeVectorToFile(varLabels[loopA],prod);
        
        newSamples = ntUtils::genSampleFromPMF(discrX[loopA],prod,numSamples,uSampler);

        // for(int loopC=0;loopC<newSamples.size();loopC++){
        //   printf("%f ",newSamples[loopC]);
        // }
        // printf("\n");


        // printf("Eccolo 7\n");
        // fflush(stdout);        


        // Add samples to message
        for(int loopB=0;loopB<numSamples;loopB++){
          this->msg[loopB][loopA] = newSamples[loopB];
        }

      }
    }else{
      throw ntException("ERROR: Incoming messages are not compatible in ntMessage::aggregateMarginals."); 
    }
  }
  delete uSampler;
  uSampler = NULL;

}

void ntMessage::writeToFile(string fileName){
  FILE* f;
  f = fopen(fileName.c_str(),"w");
  // Message type, sourceID and targetID
  fprintf(f,"%s,%d,%d\n",getMsgTypeString().c_str(),sourceID,targetID);
  
  // Variable labels
  for(int loopA=0;loopA<varLabels.size();loopA++){
    if(loopA<varLabels.size()-1){
      fprintf(f,"%s,",varLabels[loopA].c_str());  
    }else{
      fprintf(f,"%s\n",varLabels[loopA].c_str());  
    } 
  }
  
  // Variable STDs
  for(int loopA=0;loopA<varSTDs.size();loopA++){
    if(loopA<varSTDs.size()-1){
      fprintf(f,"%.4e,",varSTDs[loopA]);
    }else{
      fprintf(f,"%.4e\n",varSTDs[loopA]);
    } 
  }

  // Variable Limits
  for(int loopA=0;loopA<varLimits.size();loopA++){
    if(loopA<varLimits.size()-1){
      fprintf(f,"%.4e,",varLimits[loopA]);
    }else{
      fprintf(f,"%.4e\n",varLimits[loopA]);
    }
  }

  // Print Samples
  for(int loopA=0;loopA<msg.size();loopA++){
    for(int loopB=0;loopB<msg[loopA].size();loopB++){
      if(loopB<msg[loopA].size()-1){
        fprintf(f,"%.4e,",msg[loopA][loopB]);  
      }else{
        fprintf(f,"%.4e\n",msg[loopA][loopB]);  
      }      
    }
  }
  // Close State File
  fclose(f);
}

void ntMessage::show(){
  // Message type, sourceID and targetID
  printf("%s,%d,%d\n",getMsgTypeString().c_str(),sourceID,targetID);
  // Variable labels
  for(int loopA=0;loopA<varLabels.size();loopA++){
    printf("%s,",varLabels[loopA].c_str());
  }
  printf("\n");
  // Variable STDs
  for(int loopA=0;loopA<varSTDs.size();loopA++){
    printf("%.4e,",varSTDs[loopA]);
  }
  printf("\n");
  // Variable Limits
  for(int loopA=0;loopA<varLimits.size();loopA++){
    printf("%.4e,",varLimits[loopA]);
  }
  printf("\n");
  // Print Samples
  for(int loopA=0;loopA<msg.size();loopA++){
    for(int loopB=0;loopB<msg[loopA].size();loopB++){
      printf("%.4e,",msg[loopA][loopB]);
    }
    printf("\n");
  }
}

stdVec ntMessage::getSTDFromSamples(){
  stdVec res;
  double currAVG = 0.0;
  double currSTD = 0.0;
  // Cannot do this with only one sample
  if(msg.size() < 2){
    throw ntException("ERROR: You need al least two samples to computed the sample statistics for messages.");
  }
  for(int loopA=0;loopA<varLabels.size();loopA++){
    // Compute Mean 
    currAVG = 0.0;
    for(int loopB=0;loopB<msg.size();loopB++){
      currAVG += msg[loopB][loopA];
    }
    currAVG /= msg.size();
    // Compute Standard deviation with unbiased formula
    currSTD = 0.0;
    for(int loopB=0;loopB<msg.size();loopB++){
      currSTD += (msg[loopB][loopA] - currAVG)*(msg[loopB][loopA] - currAVG);
    } 
    currSTD = sqrt(currSTD/(msg.size()-1));
    // Add Standard deviation
    res.push_back(currSTD);
  }
  return res;
}




