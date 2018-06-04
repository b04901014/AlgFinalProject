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
BitString::set(string& str)
{
  size_t l = str.length();
  const size_t sizeun = sizeof(unsigned char) * 8;
  size_t r = l % sizeun;
  if (r) 
    r = sizeun - r;
//  cout << l << ' ' << sizeun << ' ' << r << endl;
  for (int i=0; i<r; i++)
    str += "0";
  l = str.length();
  _q = l / sizeun;
  _bytes = new unsigned char[_q];
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
    printf ("%u\n", _bytes[i]);
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
  _t[_c]._i = BitString(in);
  _t[_c]._o = BitString(out);
  return true;
}
