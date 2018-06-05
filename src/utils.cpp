#include "utils.h"
#include <stdio.h>
#include <cassert>

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
  for (int i=127; i>=0; i--)
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
  for (int i=0; i<128; i++) {
    if (!_bytes[i]) {
      _bytes[i] = 1;
      break;
    }
    _bytes[i] = 0;
  }
  return *this;
}

void
BitString::set(string& str)
{
  size_t l = str.length();
  const size_t sizeun = 128;
  size_t r = l % sizeun;
  if (r) 
    r = sizeun - r;
  for (int i=0; i<r; i++)
    str = "0" + str;
  _bytes = bitset<128>(str);
/*  size_t l = str.length();
  const size_t sizeun = sizeof(long long) * 8;
  size_t r = l % sizeun;
  if (r) 
    r = sizeun - r;
  for (int i=0; i<r; i++)
    str += "0";
  l = str.length();
  _q = l / sizeun;
  _bytes = new long long[_q];
  for (int i=0; i<_q; i++)
    _bytes[i] &= ~_bytes[i];
  size_t j = 0;
  for (int i=0; i<l; i++) {
    _bytes[j] <<= 1;
    if (str[i] == '1') {
      _bytes[j] |= 0x01;
    }
    if ((i + 1) % sizeun == 0)
      j++;
  }*/
}

void
BitString::print()
{
  cout << _bytes.to_string() << endl;
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
  _ni = pow(2, ni);
  _t = new Transition[_ni];
  _c = 0;
}

bool
State::addtrans(string& in, string& out, State* s)
{
  if (_c == _ni)
    return false;
  size_t tmp[128];
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
    _t[_c]._s = 0;
    _t[_c]._i = new BitString(tmpstr);
    _t[_c]._o = new BitString(out);
    ++*(_t[_c]._i);
    _t[_c]._i->print();
    _c++;
/*    _t[_c]._i->print();
    string x = "111111";
    BitString tmp(x);
    cout << bool(*(_t[_c]._i) > tmp) << endl;
    */
//    _t[_c]._i++;
//    _t[_c]._i->print();
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
