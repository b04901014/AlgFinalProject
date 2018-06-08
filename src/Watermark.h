#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <fstream>
#include "utils.h"
#include <set>

using namespace std;

#ifndef WATERMARK
#define WATERMARK

class Watermark
{
public:
  Watermark(char*);
  ~Watermark();
  void write(char*);

private:
  void split(string&, string&, size_t&, size_t&, string&);
  void UnreachState(set<size_t>*);

  State*      _states;
  size_t      _n_states;
  size_t      _n_ib;
  size_t      _n_ob;
  size_t      _res;
};

#endif

