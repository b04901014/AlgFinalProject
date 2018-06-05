#include "utils.h"

BitString::BitString(string& str)
{
  set(str);
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

unsigned long
BitString::tolong() 
{
  return _bytes.to_ulong();
}

State::State()
{
  _t = 0;
}

State::~State()
{
  if (_t) {
    for (int i=0; i<_c; i++) {
      delete _t[i]._s;
      delete _t[i]._i;
      delete _t[i]._o;
      _t[i]._s = 0;
      _t[i]._i = 0;
      _t[i]._o = 0;
    }
    delete[] _t;
    _t = 0;
  }
}

void
State::set(size_t ni)
{
  if (ni >= 64) {
    cerr << "Number of transition is over 64 bits" << endl;
    exit(0);
  }
  _ni = pow(2, ni);
  _t = new Transition[_ni];
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
    BitString* tmpb = new BitString(tmpstr);
    bool flag = true;
    for (int j=0; j<_c; j++)
      if (*(_t[j]._i) == *tmpb) {
        flag = false;
        break;
      }
    if (flag) {
      _t[_c]._s = s;
      _t[_c]._i = new BitString(tmpstr);
      _t[_c]._o = new BitString(out);
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
  for (int i=1; i<n; i++) {
    for (int j=pow(2, i); j<pow(2, i+1); j++) {
      str[j] = str[size_t(j - pow(2, i))];
      str[j][i] = '1';
    }
  }
}

