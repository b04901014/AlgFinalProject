#include "Watermark.h"

Watermark::~Watermark()
{

}
Watermark::Watermark(char* fn)
{
  ifstream f(fn, ifstream::in);
  string line;
  if (!f.is_open()) {
    cerr << "Failed to open file!" << endl;
    exit(0);
  }
  while (line[0] != '.')
    getline(f, line);
  size_t pos = line.find(' ') + 1;
  _n_ib =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
  getline(f, line);
  pos = line.find(' ') + 1;
  _n_ob =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
  getline(f, line);
  pos = line.find(' ') + 1;
  int np =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
  getline(f, line);
  pos = line.find(' ') + 1;
  _n_states =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
  getline(f, line);
  pos = line.find(' ') + 1;
  _res =  atoi(line.substr(pos + 1, line.size() - pos + 1).c_str());
  _states = new State[_n_states];
  for (int i=0; i<_n_states; i++)
    _states[i].set(_n_ib, i);
  set<size_t>* tmpgraph = new set<size_t>[_n_states];
  for (int i=0; i<np; i++) {
    getline(f, line);
    size_t s1, s2;
    string in, out;
    split(line, in, s1, s2, out);
    _states[s1].addtrans(in, out, &_states[s2]);
    tmpgraph[s1].insert(s2);
  }
//  for (int i=0; i<_n_states; i++)
//    _states[i].print(_n_ib, _n_ob);
  UnreachState(tmpgraph);
  for (set<size_t>::iterator i=tmpgraph[_res].begin(); i!=tmpgraph[_res].end(); ++i) {
    //run FindMaxLength
  }
  for (size_t i=0; i<_n_states; i++)
    tmpgraph[i].clear();
  delete[] tmpgraph;
}

void
Watermark::split(string& s, string& x, size_t& y, size_t& z, string& w)
{
    size_t pos = s.find(' ');
    size_t start = 0;
    x = s.substr(start, pos - start).c_str();
    start = pos + 1;
    pos = s.find(' ', start);
    y = atoi(s.substr(start + 1, pos - start).c_str());
    start = pos + 1;
    pos = s.find(' ', start);
    z = atoi(s.substr(start + 1, pos - start).c_str());
    start = pos + 1;
    w = s.substr(start, s.size() - start + 1).c_str();
}

void
Watermark::write(char* fn)
{

}

void
Watermark::UnreachState(set<size_t>* a)
{
  bool control;
  do {
    control = false;
    for (set<size_t>::iterator i=a[_res].begin(); i!=a[_res].end(); ++i)
      if (*i != _res)
        for (set<size_t>::iterator j=a[*i].begin(); j!=a[*i].end(); ++j)
          if (a[_res].insert(*j).second)
            control = true;
  } while (control);

  if (a[_res].size() < _n_states) {
    for (size_t i=0; i<_n_states; i++)
      if (a[_res].find(i) == a[_res].end())
        cout << 'S' << i << ' ';
    cout << "is disconnected to the reset state" << endl;
  }
}
