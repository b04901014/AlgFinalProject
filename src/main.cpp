#include <iostream>
#include "Watermark.h"

using namespace std;

void help_message() {
  cout << "usage: " << endl;
}

int main(int argc, char* argv[])
{
  if (argc != 3) {
    help_message();
    return 0;
  }   
  Watermark wk(argv[1]);
  wk.run(argv[2]);
  wk.write(argv[3]);
  return 0;
}

