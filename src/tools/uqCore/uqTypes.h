#ifndef UQTYPES_H
#define UQTYPES_H

# include <vector>
# include <map>
# include <stdio.h>
# include <stdlib.h>
# include <string>

using namespace std;

//! Std vector of doubles
typedef vector<double> stdVec;
//! Std matrix of doubles
typedef vector<vector<double> > stdMat;

//! Std vector of booleans
typedef vector<bool> stdBoolVec;
//! Std matrix of booleans
typedef vector<vector<bool> > stdBoolMat;

//! Std vector of integers
typedef vector<int> stdIntVec;
//! Std matrix of integers
typedef vector<vector<int> > stdIntMat;

//! Redefinition of string
typedef string stdString;
//! Std vector of strings
typedef vector<string> stdStringVec;
//! Std matrix of strings
typedef vector<vector<string> > stdStringMat;

//! Patient data map
typedef map< string, stdStringVec > dataMap;

#endif // UQTYPES