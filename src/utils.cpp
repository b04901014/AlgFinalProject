#include "utils.h"

BitString::BitString(string& str)
{
  set(str);
}

BitString::BitString(unsigned long x)
{
  _bytes = bitset<BITSIZE>(x);
}

BitString::BitString()
{

}

BitString::~BitString()
{

}

bool
BitString::operator < (const BitString& y)
{
  for (int i=BITSIZE-1; i>=0; i--)
    if (_bytes[i] ^ y._bytes[i])
      return y._bytes[i];
  return false;
}

bool
BitString::operator > (const BitString& y)
{
  return !(*this < y);
}

BitString&
BitString::operator ++ ()
{
  for (int i=0; i<BITSIZE; i++) {
    if (!_bytes[i]) {
      _bytes[i] = 1;
      break;
    }
    _bytes[i] = 0;
  }
  return *this;
}

bool
BitString::operator == (const BitString& y)
{
  return bool(_bytes == y._bytes);
}

void
BitString::set(string& str)
{
  size_t l = str.length();
  const size_t sizeun = BITSIZE;
  size_t r = l % sizeun;
  if (r) 
    r = sizeun - r;
  for (int i=0; i<r; i++)
    str = "0" + str;
  _bytes = bitset<BITSIZE>(str);
}

void
BitString::print()
{
  cout << _bytes.to_string() << endl;
}

string
BitString::tostring()
{
  return _bytes.to_string();
}

unsigned long
BitString::tolong() 
{
  return _bytes.to_ulong();
}

State::State()
{
  _t = 0;
}

State::State(unsigned long ni, unsigned long idx)
{
  _t = 0;
  set(ni, idx);
}

State::~State()
{
  if (_t) {
    for (int i=0; i<_c; i++) {
      delete _t[i]._s;
      delete _t[i]._o;
      _t[i]._s = 0;
      _t[i]._o = 0;
    }
    delete[] _t;
    _t = 0;
  }
}

void
State::set(unsigned long ni, unsigned long idx)
{
  if (ni >= 64) {
    cerr << "Number of transition is over 64 bits" << endl;
    exit(0);
  }
  _index = idx;
  _ni = pow(2, ni);
  _t = new Transition[_ni];
  for (int i=0; i<_ni; i++) {
    _t[i]._s = 0;
    _t[i]._o = 0;
  }
  _c = 0;
}

bool
State::addtrans(string& in, string& out, State* s)
{
  if (_c == _ni)
    return false;
  size_t tmp[BITSIZE];
  size_t count = 0;
  size_t nw = 1;
  for (int i=0; i<in.length(); i++)
    if (in[i] == '-') {
      nw *= 2;
      tmp[count] = i;
      count++;
    }
  string bc[nw];
  for (int i=0; i<nw; i++)
    bc[i] = "";
  bitcomb(count, bc);
  for (int i=0; i<nw; i++) {
    string tmpstr = in;
    for (int j=0; j<bc[i].length(); j++)
      tmpstr[tmp[j]] = bc[i][j];
    assert (_c < _ni);
    BitString tmpb(tmpstr);
    unsigned long idx = tmpb.tolong();
    if (!IsTransitionOccupied(idx)) {
      _t[idx]._s = s;
      _t[idx]._o = new BitString(out);
      _c++;
    }
  }
  return true;
}

void
State::bitcomb(int n, string* str)
{
  if (n == 0)
    return;
  size_t np = pow(2, n);
  for (int i=0; i<np; i++)
    str[i] = string(n, '0');
  str[1][0] = '1';
  for (int i=1; i<n; i++)
    for (int j=pow(2, i); j<pow(2, i+1); j++) {
      str[j] = str[size_t(j - pow(2, i))];
      str[j][i] = '1';
    }
}

bool
State::IsTransitionOccupied(unsigned long iw)
{
  return bool(_t[iw]._s);
}

void
State::print(size_t trunc1, size_t trunc2)
{
  if (!_t)
    return;
  if (trunc1)
    trunc1 = 8 * sizeof(unsigned long) - trunc1;
  if (trunc2)
    trunc1 = 8 * sizeof(unsigned long) - trunc2;
  BitString* tmp;
  for (unsigned long i=0; i<_ni; i++)
    if (IsTransitionOccupied(i)) {
      tmp = new BitString(i);
      cout << 'S' << _index << ' ';
      cout << (tmp->tostring()).substr(trunc1) << ' ';
      cout << (_t[i]._o->tostring()).substr(trunc2) << ' ';
      cout << 'S' << _t[i]._s->_index << endl;
      delete tmp;
    }
}

State*
State::MaxLengthRun(BitString* bin, BitString* bout, size_t j, size_t& maxlen, size_t n)
{
  maxlen = 0;
  State* s = this;
  unsigned long idx = bin[j].tolong();
  if (!s->IsTransitionOccupied(idx))
    return 0;
  while (s->IsTransitionOccupied(idx)) {
    State* ns = (s->_t)[idx]._s;
    BitString* o = (s->_t)[idx]._o;
    if (*o == bout[j]) {
      s = ns;
      maxlen++;
      if (j + maxlen == n) {
        break;
      }
      idx = bin[j + maxlen].tolong();
    }
    else
      break;
  }
  if (j + maxlen < n - 1)
    if (s->IsTransitionOccupied(bin[j + maxlen + 1].tolong()) || s->IsFull()) {
      maxlen = 0;
      return 0;
    }
  return s;
}

unsigned long
State::getfreetrans()
{
  assert (_c < _ni);
  for (unsigned long i=0; i<_ni; i++)
    if (!IsTransitionOccupied(i))
      return i;
  return 0;
}

void
State::pushtrans(State* s)
{
  unsigned long i = getfreetrans();
  _t[i]._s = s;
  _t[i]._o = new BitString(0);
}

void
State::printmergetrans(stringstream& ofs, size_t trunc1, size_t trunc2, size_t& nump)
{
  mymap::iterator it;
  trunc1 = 8 * sizeof(unsigned long) - trunc1;
  trunc2 = 8 * sizeof(unsigned long) - trunc2;
  for (unsigned long i=0; i<_ni; i++) {
    if (!IsTransitionOccupied(i))
      continue;
    BitString tmpb(i);
    string tmpstring = tmpb.tostring().substr(trunc1);
    it = tmpmap.find(_t[i]);
    bool p = (_t[i]._s->_index == 0);
    if (it != tmpmap.end()) {
      vector<mypair>* tmpv = &(it->second);
      bool rc = false;
      vector<mypair>::iterator it2 = tmpv->end();
      while (!strash(tmpv, tmpstring, it2, p))
        rc = true;
      if (!rc && !tmpstring.empty())
        tmpmap[_t[i]].push_back(mypair(tmpstring, true));
      it2 = tmpv->end();
    }
    else {
      tmpmap[_t[i]] = vector<mypair>();
      tmpmap[_t[i]].push_back(mypair(tmpstring, true));
    }
  }
  for (it=tmpmap.begin(); it!=tmpmap.end(); ++it) {
    Transition t = it->first;
    string ostring = t._o->tostring().substr(trunc2);
    unsigned long stateidx = t._s->_index;
    vector<mypair>* tmpv = &(it->second);
    for (vector<mypair>::iterator j=tmpv->begin(); j!=tmpv->end(); ++j) {
      if (!(*j).second) {
        continue;
      }
      ofs << (*j).first << " S" << _index;
      ofs << " S" << stateidx << ' ' << ostring;
      ofs << endl;
      nump++;
    }
  }
}

bool
State::strash(vector<mypair>* tmpv, string& tmpstring, vector<mypair>::iterator& it, bool p)
{
  bool addpush = true;
  for (vector<mypair>::iterator j=tmpv->begin(); j!=tmpv->end(); ++j) {
    if (it == j)
      continue;
    size_t count = 0;
    size_t pos = 0;
    bool merge = true;
    for (size_t k=0; k<tmpstring.length(); k++) {
      if (tmpstring[k] != ((*j).first)[k]) {
        pos = k;
        count++;
        if (tmpstring[k] != '-' and ((*j).first)[k] != '-')
          merge = false;
      }
    }
    if (count == 0) {
      addpush = false;
      it = j;
      return false;
    }
    if (merge) {
      string tmps = "";
      for (size_t k=0; k<tmpstring.length(); k++) {
        if (tmpstring[k] == '-' or ((*j).first)[k] == '-')
          tmps += '-';
        else
          tmps += tmpstring[k];
      }
      if (tmps == (*j).first) { //terminate
        if (it != tmpv->end())
          (*it).second = false;
        tmpstring.clear();
        it = tmpv->end();
        return true;
      }
      if (it != tmpv->end())
        (*it).second = false;
      it = j;
      tmpstring = tmps;
      (*j).first = tmps;
      return false;
    }
    if (count == 1) {
      string gg = (*j).first;
      ((*j).first)[pos] = '-';
      tmpstring = (*j).first;
      it = j;
      return false;
    }
  }
  return addpush;
}

void
State::setmap(string& in, string& out, State* ns)
{
  Transition tmpt;
  tmpt._s = ns;
  tmpt._o = new BitString(out);
  mymap::iterator it = tmpmap.find(tmpt);
  if (it != tmpmap.end())
    it->second.push_back(mypair(in, true));
  else {
    tmpmap[tmpt] = vector<mypair>();
    tmpmap[tmpt].push_back(mypair(in, true));
  }
}
