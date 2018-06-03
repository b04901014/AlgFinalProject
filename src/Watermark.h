#include <iostream>
#include <bitset>
#include <stdlib.h>
#include <fstream>
#include "utils.h"

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
  void split(string&, int&, int&, int&, int&);

  BitString* inputs;

};

#endif

