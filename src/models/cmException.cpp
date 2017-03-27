# include "cmException.h"

cmException::cmException(const char* m):msg(m){
}
    
cmException::~cmException() throw(){
}

const char* cmException::what() const throw(){
  return msg.c_str();
}

