#include <iostream>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <cassert>
#include <stdlib.h>
#include <cassert>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <sstream>

using namespace std;

#ifndef UTILS
#define UTILS

#define BITSIZE 64
struct Transition;
typedef pair< string, bool > mypair;
typedef map< Transition, vector<mypair> > mymap;

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
  unsigned long getidx() { return _index; }
  bool IsFull() { return _c == _ni; }
  bool IsOne() { return _c == _ni - 1; }
  void pushtrans(State*);
  bool IsTransitionOccupied(unsigned long);
  unsigned long getfreetrans();
  void printmergetrans(stringstream&, size_t, size_t, size_t&);
  void setmap(string&, string&, State*);

private:
  void bitcomb(int, string*);
  bool strash(vector<mypair>*, string&, vector<mypair>::iterator&, bool);

  Transition*    _t;
  unsigned long  _ni;      //max input transitions
  unsigned long  _c;       //current occupied number
  unsigned long  _index;   //state index
  mymap          tmpmap;
};

struct Transition
{
  State*         _s;   //next state
  BitString*     _o;   //output weight
  
  unsigned long getoutputlong() {
    return _o->tolong();
  }
  bool operator == (const Transition& x) { 
    return (_o->tolong() == x._o->tolong() &&
            _s->getidx() == x._s->getidx());
  }
  bool operator < (const Transition& x) const {
    if (_o->tolong() < x._o->tolong())
      return true;
    if (_o->tolong() == x._o->tolong())
      return _s->getidx() < x._s->getidx();
    return false;
  }
};

#endif
