#ifndef UQEXCEPTION_H
#define UQEXCEPTION_H

# include <string>
# include <exception>

using namespace std;

class uqException: public exception{
  public:
    // Constructor and Destructor
    uqException(const char* m);
    virtual ~uqException() throw();
    // Member Functions
    virtual const char* what() const throw();
  protected:
    string msg;
};

#endif // UQEXCEPTION_H
