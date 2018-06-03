#include "Watermark.h"

Watermark::~Watermark()
{

}
Watermark::Watermark(char* fn)
{
  ifstream f(fn, ifstream::in);
  string line;
  if (f.is_open()) {
    getline(f, line);
    size_t pos = line.find(' ') + 1;
    int i =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
    getline(f, line);
    pos = line.find(' ') + 1;
    int o =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
    getline(f, line);
    pos = line.find(' ') + 1;
    int s =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
    getline(f, line);
    pos = line.find(' ') + 1;
    int p =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
    getline(f, line);
    pos = line.find(' ') + 1;
    int r =  atoi(line.substr(pos + 1, line.size() - pos + 1).c_str());
  }

}

void
Watermark::split(string& s, int& x, int& y, int& z, int& w)
{
    size_t pos = s.find(' ');
    size_t start = 0;
    x = atoi(s.substr(start, pos - start).c_str());
    start = pos + 1;
    pos = s.find(' ', start);
    y = atoi(s.substr(start + 1, pos - start).c_str());
    start = pos + 1;
    pos = s.find(' ', start);
    z = atoi(s.substr(start + 1, pos - start).c_str());
    start = pos + 1;
    w = atoi(s.substr(start, s.size() - start + 1).c_str());
}

void
Watermark::write(char* fn)
{

}
