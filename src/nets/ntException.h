#ifndef NTEXCEPTION_H
#define NTEXCEPTION_H

#include <string>

using namespace std;

class ntException: public exception{
  public:
    // Constructor and Destructor
    ntException(const char* m);
    virtual ~ntException() throw();
    // Member Functions
	virtual const char* what() const throw();
  protected:
    std::string msg;
};

#endif // NTEXCEPTION_H
