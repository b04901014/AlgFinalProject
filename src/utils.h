#include <iostream>
#include <cmath>

using namespace std;

#ifndef UTILS
#define UTILS

struct Transition;

class BitString
{
public:
  BitString();
  BitString(string&);
  ~BitString();

  void set(string&);
  void print();

private:
  unsigned char* _bytes;
  size_t         _q; //size of bytes
};

class State
{
public:
  State();
  State(size_t);
  ~State();

  void set(size_t);
  bool addtrans(string&, string&, State*); //return false if transition is full

private:
  Transition*    _t;
  size_t         _ni;  //max input transitions
  size_t         _c;   //current occupied states

};

struct Transition
{
  State*         _s; //next state
  BitString      _i; //input weight
  BitString      _o; //output weight
};

#endif
