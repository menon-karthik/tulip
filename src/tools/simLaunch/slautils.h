#ifndef SLAUTILS_H
#define SLAUTILS_H

#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>

// MRI UTILITIES
namespace SLAUtils{

// =========================
// CONVERT INTEGER TO STRING
// =========================
inline std::string IntToStr(int number){
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

// ========================
// CONVERT DOUBLE TO STRING
// ========================
inline std::string FloatToStr(double number){
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

// =============
// WRITE MESSAGE
// =============
inline void WriteMessage(std::string Msg){
    printf("%s",Msg.c_str());
}

// =============
// WRITE MESSAGE
// =============
inline bool isInStringList(std::string Msg, std::vector<std::string> List){
  bool found = false;
  for(int loopA=0;loopA<List.size();loopA++){
    if (Msg.compare(List[loopA]) == 0){
      found = true;
    }
  }
  // RETURN
  return found;
}


}

#endif // SLAUTILS_H
