#include <iostream>

using namespace std;

#ifndef UTILS
#define UTILS

class BitString
{
public:
  BitString();
  BitString(string&);
  ~BitString();

  void set(string&);

private:
  unsigned char* _bytes;
  size_t         _q; //size of bytes
};

class State
{
public:
  State();
  State(size_t&, size_t&, size_t&);
  ~State();

  void set(size_t&, size_t&, size_t&);

private:
  State*         _s; //next state
  BitString*     _i; //input transition
  BitString*     _o; //output transition

};
#endif
