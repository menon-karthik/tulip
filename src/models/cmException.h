#ifndef CMEXCEPTION_H
#define CMEXCEPTION_H

#include <string>

using namespace std;

class cmException: public exception{
  public:
    // Constructor and Destructor
    cmException(const char* m):msg(m){};
    virtual ~cmException() throw(){};
    // Member Functions
	virtual const char* what() const throw() {return msg.c_str();}
  protected:
    std::string msg;
};

#endif // CMEXCEPTION_H
