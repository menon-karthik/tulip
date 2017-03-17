# include "uqException.h"

uqException::uqException(const char* m):msg(m){
}
    
uqException::~uqException() throw(){
}

const char* uqException::what() const throw(){
  return msg.c_str();
}
