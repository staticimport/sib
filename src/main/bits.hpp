//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

