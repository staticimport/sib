//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace sib
{
  template <typename K, typename T, typename H, typename E>
  inline array_hash_map<K,T,H,E>::array_hash_map(size_type min_capacity, 
                                                 double load_factor)
  : _table(min_capacity, load_factor)
  {
  }

  template <typename K, typename T, typename H, typename E>
  inline std::pair<typename array_hash_map<K,T,H,E>::iterator, bool>
  array_hash_map<K,T,H,E>::insert(value_type const& x)
  {
    _table.insert(x.first, x);
  }

}

