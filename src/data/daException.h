#ifndef DAEXCEPTION_H
#define DAEXCEPTION_H

#include <string>

using namespace std;

class daException: public exception{
  public:
    // Constructor and Destructor
    daException(const char* m):msg(m){};
    virtual ~daException() throw(){};
    // Member Functions
	virtual const char* what() const throw() {return msg.c_str();}
  protected:
    std::string msg;
};

#endif // DAEXCEPTION_H
