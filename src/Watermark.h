#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <fstream>
#include "utils.h"
#include <vector>
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
  void run(char*);

private:
  void split(string&, string&, size_t&, size_t&, string&);
  void UnreachState(set<size_t>*);
  void NewState();
  void DeleteState(size_t);
  void Parsemd5(char*);
  void runcore();
  bool CheckValid(State*&);
  State* FreeTransition(size_t);
  void NewStateTrans(State*);

  vector<State*>    _states;
  size_t            _n_ib;
  size_t            _n_ob;
  size_t            _res;
  BitString*        _bin;
  BitString*        _bout;
  size_t            _lenb;
};

#endif

