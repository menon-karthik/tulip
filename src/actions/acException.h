#ifndef ACEXCEPTION_H
#define ACEXCEPTION_H

# include <string>

using namespace std;

class acException: public exception{
  public:
    // Constructor and Destructor
    acException(const char* m):msg(m){};
    virtual ~acException() throw(){};
    // Member Functions
		virtual const char* what() const throw() {return msg.c_str();}
  protected:
    std::string msg;
};

#endif // ACEXCEPTION_H
