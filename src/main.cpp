#include <iostream>
#include "Watermark.h"

using namespace std;

void help_message(char* p) {
  cout << "usage   : "<< p <<" -i <input> -o <output> -m <md5file_1> <md5file_2> ... <md5file_n>" << endl;
  cout << "input   : kiss format file" << endl;
  cout << "md5file : A file with a single line string, which represents 128 bits." << endl;
}

int main(int argc, char* argv[])
{
  if (argc <= 6) {
    help_message(argv[0]);
    return 0;
  }
  if (string(argv[1]) != "-i" || string(argv[3]) != "-o" || string(argv[5]) != "-m") {
    help_message(argv[0]);
    return 0;
  }
  Watermark wk(argv[2]);
  for (int i=6; i<argc; i++)
    wk.run(argv[i]);
  wk.write(argv[4]);
  return 0;
}

