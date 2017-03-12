#ifndef UQEXCEPTION_H
#define UQEXCEPTION_H

# include <string>
# include <exception>

using namespace std;

class uqException: public exception{
  public:
    // Constructor and Destructor
    uqException(const char* m):msg(m){};
    virtual ~uqException() throw(){};
    // Member Functions
		virtual const char* what() const throw() {return msg.c_str();}
  protected:
    std::string msg;
};

class uqQuadratureException: public uqException{
public:
  uqQuadratureException(const char* m):uqException(m){};
  virtual ~uqQuadratureException() throw(){};
    // Member Functions
	  virtual const char* what() const throw() {return msg.c_str();}
};

// Solver Exception for UQ
class uqSolverException: public uqException{
public:
  uqSolverException(const char* m):uqException(m){};
  virtual ~uqSolverException() throw(){};
    // Member Functions
	  virtual const char* what() const throw() {return msg.c_str();}
};

// Solver Exception for UQ
class uqInterpolationException: public uqException
{
public:
  uqInterpolationException(const char* m):uqException(m){};
  virtual ~uqInterpolationException() throw(){};
    // Member Functions
	  virtual const char* what() const throw() {return msg.c_str();}
};




#endif // UQEXCEPTION_H
