#include "daException.h"

daException::daException(const char* m):msg(m){
}
    
daException::~daException() throw(){
}

const char* daException::what() const throw(){
  return msg.c_str();
}

