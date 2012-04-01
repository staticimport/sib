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

