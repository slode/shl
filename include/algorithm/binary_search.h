/*
MIT License

Copyright (c) 2017 Stian Lode,
                   stian.lode@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/
#include <stdint.h>
#include <vector>

#ifdef _MSC_VER
  #include <intrin.h>
  #define FORCEINLINE __forceinline
  FORCEINLINE uint32_t bsr(uint32_t x) {
    unsigned long res;
    _BitScanReverse(&res, x);
    return res;
  }
#else
  #define FORCEINLINE __attribute__((always_inline)) inline
  FORCEINLINE uint32_t bsr(uint32_t x) {
    return 31 - __builtin_clz(x);
  }
#endif

namespace shl { namespace algorithm {

template<class T, class S>
int binary_search(const std::vector<T> &arr, const S &key) {
  const unsigned n = arr.size();
  if (n < 1) return 0;
  if (arr[n-1] < key) return n;

  unsigned step = 1 << bsr(n-1);
  int pos = arr[step - 1] < key ? n - step - 1 : -1;

  while ((step >>= 1) > 0) {
    pos = (arr[pos + step] < key ? pos + step : pos);
  }
  return pos + 1;
}

}}
