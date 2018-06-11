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
  _bin = 0;
  _bout = 0;
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
  size_t n_states =  atoi(line.substr(pos, line.size() - pos + 1).c_str());
  getline(f, line);
  pos = line.find(' ') + 1;
  _res =  atoi(line.substr(pos + 1, line.size() - pos + 1).c_str());
  for (int i=0; i<n_states; i++) {
    State* tmp = new State(_n_ib, i);
    _states.push_back(tmp);
  }
  set<size_t>* graph = new set<size_t>[n_states];
  for (int i=0; i<np; i++) {
    getline(f, line);
    size_t s1, s2;
    string in, out;
    split(line, in, s1, s2, out);
    _states[s1]->addtrans(in, out, _states[s2]);
    graph[s1].insert(s2);
  }
  UnreachState(graph);
  for (size_t i=0; i<graph->size(); i++)
    graph[i].clear();
  delete[] graph;
//  for (int i=0; i<_states.size(); i++)
//    _states[i]->print(_n_ib, _n_ob);
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

  if (a[_res].size() < _states.size()) {
    for (size_t i=0; i<_states.size(); i++)
      if (a[_res].find(i) == a[_res].end()) {
        cout << 'S' << i << ' ';
        DeleteState(i);
      }
    cout << "is disconnected to the reset state" << endl;
  }
}

void
Watermark::Parsemd5(char* md5fn)
{
  ifstream f(md5fn, ifstream::in);  
  string line, md5;
  md5 = "";
  if (!f.is_open()) {
    cerr << "Failed to open file " << md5fn << '!' << endl;
    exit(0);
  }
  getline(f, line);
  for (int i=0; i<line.length(); i++) {
    char c = line[i];
    int x = c;
    if (c >= 'a')
      x = c - 'a' + 10;
    bitset<4> tmp(x);
    md5 += tmp.to_string();
  }
  size_t totalsize = _n_ib + _n_ob;
  size_t r = md5.length() % totalsize;
  if (r)
    r = totalsize - r;
  for (int i=0; i<r; i++)
    md5 += '0';
  _lenb = md5.length() / totalsize;
  _bin = new BitString[_lenb];
  _bout = new BitString[_lenb];
  for (int i=0; i<_lenb; i++) {
    string tmp = md5.substr(i * totalsize, _n_ib);
    _bin[i].set(tmp);
    tmp = md5.substr(i * totalsize + _n_ib, _n_ob);
    _bout[i].set(tmp);
  }
}

void
Watermark::Run(char* md5fn)
{
  Parsemd5(md5fn);
  for (vector<State*>::iterator i=_states.begin(); i!=_states.end(); ++i) {
    if (*i) {
      //run FindMaxLength
    }
  }
  delete[] _bin;
  delete[] _bout;
  _lenb = 0;
}

void
Watermark::NewState()
{
  size_t i = _states.size();
  State* tmp = new State(_n_ib, i);
  _states.push_back(tmp);
}

void
Watermark::DeleteState(size_t i)
{
  delete _states[i];
  _states[i] = 0;
}
