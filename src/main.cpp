#include <iostream>
#include "Watermark.h"

using namespace std;

void help_message() {
  cout << "Usage: Program <input> <md5file_1> <md5file_2> ... <md5file_n> <output>" << endl;
  cout << "Input : kiss format file" << endl;
  cout << "md5file : A file with a single line string, which represents 128 bits." << endl;
}

int main(int argc, char* argv[])
{
  if (argc < 4) {
    help_message();
    return 0;
  }
  Watermark wk(argv[1]);
  for (int i=2; i<argc-1; i++)
    wk.run(argv[2]);
  wk.write(argv[argc-1]);
  return 0;
}

