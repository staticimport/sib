//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_CLUSTER_MAP
#define SIB_CLUSTER_MAP

#include <cstddef>
#include <vector>

namespace sib
{
  template <typename K,
            typename T,
            typename Compare=std::less<K>,
            typename Equals=std::is_equal<K> >
  cluster_map
  {
  public:
    cluster_map(typename param<K>::type max_cluster_gap);
    ~cluster_map();

    T& operator[](typename param<K>::type key);
  private:
    struct entry
    {
      entry() : _key(K()), _value(T()) {}

      K _key;
      T _value;
    };

    struct cluster
    {
      cluster();

      std::size_t distance(typename param<K>::type key);
      T& initialize(typename param<K>::type first_key);
      T& operator[](typename param<K>::type key);

      std::vector<entry>* _entries;
    };

    K const _max_cluster_gap;
    std::size_t _size;
    std::vector<cluster> _clusters;
    Compare _compare;
    Equals _equals;
  };
}

#include "cluster_map.inl"

#endif /* SIB_CLUSTER_MAP */

