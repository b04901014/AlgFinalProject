#include <iostream>
#include <cmath>
#include <bitset>

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

  bool operator < (const BitString&);
  bool operator > (const BitString&);
  BitString& operator ++ ();
  void set(string&);
  void print();

private:
  bitset<128>    _bytes;
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
  void bitcomb(int, string*);

  Transition*    _t;
  size_t         _ni;  //max input transitions
  size_t         _c;   //current occupied states

};

struct Transition
{
  State*         _s;   //next state
  BitString*     _i;   //input weights
  BitString*     _o;   //output weight
//  size_t         _nw;  //number of input weights
};

#endif
