#ifndef SIB_BITS_HPP
#define SIB_BITS_HPP

namespace sib
{
  template <typename Integer>
  bool is_power2(Integer x);

  template <typename Integer>
  Integer power2_above(Integer x, bool inclusive=true);
}

#include "internal/bits.inl"

#endif /* SIB_BITS_HPP */

