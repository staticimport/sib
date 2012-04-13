//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_INTERNAL_COMMON_HPP
#define SIB_INTERNAL_COMMON_HPP

namespace sib
{
  template <typename K, typename Pair, typename Equal>
  struct pair_first_equal
  {
    bool operator()(K const& k, Pair const& p) const { return _equal(k, p.first); }

    Equal _equal;
  };
};

#endif /* SIB_INTERNAL_COMMON_HPP */

