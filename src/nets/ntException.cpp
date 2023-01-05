# include "ntException.h"

ntException::ntException(const char* m):msg(m){
}
    
ntException::~ntException() throw(){
}

const char* ntException::what() const throw(){
  return msg.c_str();
}

