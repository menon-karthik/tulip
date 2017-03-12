#ifndef SLAEXCEPTION_H
#define SLAEXCEPTION_H

#include <exception>
#include <string>

class SLAException: public std::exception{
  public:
    SLAException(const char* m):msg(m){}
    virtual ~SLAException() throw(){}
    // Member Functions
    virtual const char* what() const throw() {return msg.c_str();}
  protected:
    std::string msg;
};

#endif // SLAEXCEPTION_H
