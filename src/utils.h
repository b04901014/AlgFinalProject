#include <iostream>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <cassert>
#include <stdlib.h>

using namespace std;

#ifndef UTILS
#define UTILS

#define BITSIZE 64

struct Transition;

class BitString
{
public:
  BitString();
  BitString(string&);
  ~BitString();

  bool operator < (const BitString&);
  bool operator > (const BitString&);
  bool operator == (const BitString&);
  BitString& operator ++ ();
  void set(string&);
  void print();
  unsigned long tolong();

private:
  bitset<BITSIZE>    _bytes;
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
  unsigned long  _ni;  //max input transitions
  unsigned long  _c;   //current occupied states

};

struct Transition
{
  State*         _s;   //next state
  BitString*     _i;   //input weights
  BitString*     _o;   //output weight
};

#endif
