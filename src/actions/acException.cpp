#include "acException.h"

acException::acException(const char* m):msg(m){
}
    
acException::~acException() throw(){
}

const char* acException::what() const throw(){
  return msg.c_str();
}

