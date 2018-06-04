#include <iostream>
#include "Watermark.h"

using namespace std;

void help_message() {
  cout << "usage: " << endl;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    help_message();
    return 0;
  }   
  Watermark wk(argv[1]);
  wk.write(argv[2]);
  return 0;
}

