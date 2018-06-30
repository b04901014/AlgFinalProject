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
    _states[s1]->setmap(in, out, _states[s2]);
    graph[s1].insert(s2);
  }
  UnreachState(graph);
  for (size_t i=0; i<graph->size(); i++)
    graph[i].clear();
  delete[] graph;
  State* tmp;
  CheckValid(tmp);
  if (!tmp) {
    cout << "Is's a CSFSM!" << endl;
    exit(0);
  }
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
  ofstream ofs(fn, ofstream::out);
  stringstream ss;
  size_t nump = 0;
  for (vector<State*>::iterator s=_states.begin(); s!=_states.end(); ++s)
    if (*s)
      (*s)->printmergetrans(ss, _n_ib, _n_ob, nump);
  ofs << ".i " << _n_ib << endl;
  ofs << ".o " << _n_ob << endl;
  ofs << ".p " << nump << endl;
  ofs << ".s " << _states.size() << endl;
  ofs << ".r S" << _res << endl;
  ofs << ss.str();
  ofs << ".e";
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
  bool pr = false;
  if (a[_res].size() < _states.size()) {
    for (size_t i=0; i<_states.size(); i++)
      if (a[_res].find(i) == a[_res].end() && i != _res) {
        cout << 'S' << i << ' ';
        DeleteState(i);
        pr = true;
      }
    if (pr)
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
  #ifdef DEBUG
  int cnt = 0;
  cout << cnt << " ";
  for (int i=0; i<md5.length(); i++) {
    cout << md5[i];
    if ((i + 1) % (_n_ib + _n_ob) == 0) {
      cout << endl;
      cnt++;
      if (i != md5.length() - 1)
        cout << cnt << " ";
    }
    else if ((i + 1 + _n_ob) % (_n_ib + _n_ob) == 0)
      cout << " ";
  }
  #endif
}

void
Watermark::run(char* md5fn)
{
  Parsemd5(md5fn);
  bool gate = true;
  while (gate)
    gate = runcore();
  delete[] _bin;
  delete[] _bout;
  _lenb = 0;
}

bool
Watermark::runcore()
{
  State* start = _states[_res];
  size_t maxlen = 0;
  State* dest = start;
  State* nst = 0;
  for (vector<State*>::iterator s=_states.begin(); s!=_states.end(); ++s) {
    if (*s) {
      size_t m;
      State* d = (*s)->MaxLengthRun(_bin, _bout, 0, m, _lenb);
      if (d and m >= maxlen) {
        maxlen = m;
        dest = *s;
        nst = d;
      }
    }
  }
  if (maxlen == _lenb)
    return false;
  if (nst)
    start = nst;
  else { 
    start = FreeTransition(0); // have free transition of specific output or not
    if (!start) {
      CheckValid(start);
      NewStateTrans(start);
      return true;
    }
  }
  int pos = maxlen;
  #ifdef DEBUG
  cout << "Start State : s" << start->getidx() << endl;
  cout << "Maxlen : " << maxlen;
  if (dest)
    cout << " Destination : s" << dest->getidx(); 
  else
    cout << " Destination None";
  if (nst)
    cout << " Next State : s" << nst->getidx(); 
  else
    cout << " Next State None";
  cout << endl;
  #endif
  while (pos != _lenb - 1) {
    maxlen = 0;
    dest = start;
    nst = 0;
    string a = _bin[pos].tostring();
    string b = _bout[pos].tostring();
    for (vector<State*>::iterator s=_states.begin(); s!=_states.end(); ++s) {
      if (*s) {
        size_t m;
        start->addtrans(a, b, *s);
        State* d = (*s)->MaxLengthRun(_bin, _bout, pos + 1, m, _lenb);
        start->deltrans(a);
        if (d and m >= maxlen) {
          maxlen = m;
          dest = *s;
          nst = d;
        }
      }
    }
    #ifdef DEBUG
    cout << "Maxlen : " << maxlen;
    if (dest)
      cout << " Destination : s" << dest->getidx(); 
    else
      cout << " Destination None";
    if (nst)
      cout << " Next State : s" << nst->getidx(); 
    else
      cout << " Next State None";
    cout << endl;
    #endif
    if (nst) {
      #ifdef DEBUG
      cout << "At Position : " << pos;
      cout << " Add Transition from s" << start->getidx();
      cout << " to s" << dest->getidx();
      cout << " with " << a.substr(BITSIZE - _n_ib);
      cout << " " << b.substr(BITSIZE - _n_ob) << endl;
      #endif
      start->addtrans(a, b, dest);
    }
    else {
      NewState();
      #ifdef DEBUG
      cout << "At Position : " << pos;
      cout << " Add Transition from s" << start->getidx();
      cout << " to s" << _states.back()->getidx();
      cout << " with " << a.substr(BITSIZE - _n_ib);
      cout << " " << b.substr(BITSIZE - _n_ob) << endl;
      #endif
      start->addtrans(a, b, _states.back());
      nst = _states.back();
    }
    pos = pos + maxlen + 1;
    start = nst;
    if (pos == _lenb)
      return false;
  }
  if (pos == _lenb - 1) {
    string a = _bin[pos].tostring();
    string b = _bout[pos].tostring();
    start->addtrans(a, b, start);
    #ifdef DEBUG
    cout << "At Position : " << pos;
    cout << " Add Transition from s" << start->getidx();
    cout << " to s" << start->getidx();
    cout << " with " << a.substr(BITSIZE - _n_ib);
    cout << " " << b.substr(BITSIZE - _n_ob) << endl;
    #endif
    return false;
  }
  return (pos != _lenb);
}

void
Watermark::NewState()
{
  size_t i = _states.size();
  State* tmp = new State(_n_ib, i);
  _states.push_back(tmp);
  #ifdef DEBUG
  cout << "Adding state s" << i << endl;
  #endif
}

void
Watermark::NewStateTrans(State* s)
{
  NewState();
  s->pushtrans(_states.back());
  #ifdef DEBUG
  cout << "Add Transition from s" << s->getidx();
  cout << " to s" << _states.back()->getidx();
  cout << " with zero" << endl;
  #endif
}

void
Watermark::DeleteState(size_t i)
{
  delete _states[i];
  _states[i] = 0;
}

State*
Watermark::FreeTransition(size_t j)
{
  State* d = 0;
  for (vector<State*>::iterator s=_states.begin(); s!=_states.end(); ++s) {
    if (*s)
      if (!(*s)->IsTransitionOccupied(_bin[j].tolong())) {
        d = *s;
        break;
      }
  }
  return d;
}

bool
Watermark::CheckValid(State*& p)
{
  // CSFSM -> return false + p = 0
  // only one transition -> return false + p = that state
  // multiple transitions -> return true + p = last state
  bool full = true;
  p = 0;
  for (vector<State*>::iterator s=_states.begin(); s!=_states.end(); ++s) {
    if (*s) {
      if (full) {
        full = (*s)->IsFull();
        if (!full) {
          p = *s;
        }
      }
      else if (!(*s)->IsFull()) { //second empty
        p = *s;
        return true;
      }
    }
  }
  if (!full) //only one transition left
    return !p->IsOne();
//  if (p)
//    cout << "state " << p->getidx() << " is the only one transition" << endl;
  return false;

}
