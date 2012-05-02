//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>

#include "bits.hpp"

namespace
{
  static uint8_t const SIB_BLOOM_MASKS[8] =
  {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80
  };
}

template <typename T, unsigned C, typename H>
sib::bloom_filter<T,C,H>::bloom_filter(size_type min_byte_count)
: _byte_count(power2_above(min_byte_count, true)),
  _byte_mask(_byte_count - 1),
  _data(static_cast<uint8_t*>(calloc(_byte_count, 1)))
{
}

template <typename T, unsigned C, typename H>
sib::bloom_filter<T,C,H>::~bloom_filter()
{
  free(_data);
}

template <typename T, unsigned C, typename H>
inline void sib::bloom_filter<T,C,H>::insert(T const& x)
{
  uint32_t const base_hash = static_cast<uint32_t>(_hasher(x));
  for(unsigned i = 0; i != C; ++i) {
    uint32_t const hash = base_hash + i*((base_hash << i)+1);
    _data[(hash >> 3) & _byte_mask] |= SIB_BLOOM_MASKS[hash & 7];
  }
}

template <typename T, unsigned C, typename H>
inline bool sib::bloom_filter<T,C,H>::operator[](T const& x) const
{
  uint32_t const base_hash = static_cast<uint32_t>(_hasher(x));
  for(unsigned i = 0; i != C; ++i) {
    uint32_t const hash = base_hash + i*((base_hash << i)+1);
    if (!(_data[(hash >> 3) & _byte_mask] & SIB_BLOOM_MASKS[hash & 7]))
      return false;
  }
  return true;
}

