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
  BitString(unsigned long);
  ~BitString();

  bool operator < (const BitString&);
  bool operator > (const BitString&);
  bool operator == (const BitString&);
  BitString& operator ++ ();
  void set(string&);
  void print();
  unsigned long tolong();
  string tostring();

private:
  bitset<BITSIZE>    _bytes;
};

class State
{
public:
  State();
  State(unsigned long, unsigned long);
  ~State();

  void set(unsigned long, unsigned long);
  bool addtrans(string&, string&, State*); //return false if transition is full
  void print(size_t = 0, size_t = 0);
  State* MaxLengthRun(BitString*, BitString*, size_t, size_t&, size_t);

private:
  void bitcomb(int, string*);
  bool IsTransitionOccupied(unsigned long);

  Transition*    _t;
  unsigned long  _ni;    //max input transitions
  unsigned long  _c;     //current occupied states
  unsigned long  _index; //state index
};

struct Transition
{
  State*         _s;   //next state
  BitString*     _o;   //output weight
  
  unsigned long getoutputlong() {
    return _o->tolong();
  }

};

#endif
