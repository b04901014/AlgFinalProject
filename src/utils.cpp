#include "utils.h"

BitString::BitString(string& str)
{
  set(str);
}
void
BitString::set(string& str)
{
  size_t l = str.length();
  const size_t sizeun = sizeof(unsigned char);
  size_t r = l % sizeun;
  if (r) 
    r = sizeun - r;
  q = l / sizeun;
  bits = new unsigned char[q];
  for (int i=0; i<q; i++)
    bits[i] &= ~bits[i];
  for (int i=0; i<r; i++)
    str += "0";
  size_t j = 0;
  for (int i=0; i<str.length(); i++) {
    if (str[i] == '1'):
      bits[j] |= 0x01;
    bits[j] <<= 1;
    if ((i + 1) % sizeun == 0)
      j++;
  }
}

