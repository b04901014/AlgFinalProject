#include "utils.h"
#include <stdio.h>
#include <bitset>

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

void
BitString::set(string str)
{
  size_t l = str.length();
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
    if (str[i] == '1')
      _bytes[j] |= 0x01;
    if ((i + 1) % sizeun == 0)
      j++;
  }
}

void
BitString::print()
{
  for (int i=0; i<_q; i++)
    cout << _bytes[i] << endl;
}

State::State()
{

}

State::~State()
{

}

void
State::set(size_t ni)
{
  _ni = pow(2, ni);
  _t = new Transition[ni];
  _c = 0;
}

bool
State::addtrans(string& in, string& out, State* s)
{
  if (_c == _ni)
    return false;
  _c++;
  _t[_c]._s = s;
  _t[_c]._ni = 1;
  size_t tmp[128];
  size_t count = 0;
  for (int i=0; i<in.length(); i++)
    if (in[i] == '-') {
      _t[_c]._ni *= 2;
      tmp[i] = i;
      count++;
    }
  _t[_c]._i = new BitString[_t[_c]._ni];
  string* bc;
  bitcomb(count, bc);
  for (int i=0; i<count; i++) {
    string tmpstr = in;
    for (int j=0; j<bc[i].length(); j++)
      tmpstr[tmp[j]] = bc[i][j];
    _t[_c]._i[j] = BitString(in);
  }
  _t[_c]._o = BitString(out);
  return true;
}

void
State::bitcomb(int n, string* str)
{
  
}
