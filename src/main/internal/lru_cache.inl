//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>

#include "bits.hpp"

namespace sib
{
  template <typename K, typename T, typename E>
  lru_cache<K,T,E>::lru_cache(size_type min_size, K const& empty_key)
  : _capacity(power2_above(min_size, true))
  , _mask(_capacity - 1)
  , _entries(static_cast<value_type*>(malloc(_capacity * sizeof(value_type))))
  , _entries_end(_entries + _capacity)
  , _front_index(0)
  , _empty_key(empty_key)
  {
    for(size_type ii = 0; ii != _capacity; ++ii)
      new(_entries + ii) value_type(_empty_key, T());
  }

  template <typename K, typename T, typename E>
  inline void
  lru_cache<K,T,E>::add(value_type const& x)
  {
    _entries[(--_front_index) & _mask] = x;
  }

  template <typename K, typename T, typename E>
  lru_cache<K,T,E>::~lru_cache()
  {
    for(size_type ii = 0; ii != _capacity; ++ii)
      _entries[ii].~value_type();
    free(_entries);
  }
  
  template <typename K, typename T, typename E>
  inline T*
  lru_cache<K,T,E>::lookup(K const& key)
  {
    value_type* const first = _entries + (_front_index & _mask);
    for(value_type* e = first; e != _entries_end; ++e) {
      if (_equator(e->first, key)) {
        value_type& new_first = _entries[(--_front_index) & _mask];
        std::swap(new_first, *e);
        return &(new_first.second);
      }
    }
    for(value_type* e = _entries; e != first; ++e) {
      if (_equator(e->first, key)) {
        value_type& new_first = _entries[(--_front_index) & _mask];
        std::swap(new_first, *e);
        return &(new_first.second);
      }
    }
    return NULL;
  }
  
  /*template <typename K, typename T, typename E>
  inline T const*
  lru_cache<K,T,E>::find(K const& key) const
  {
    value_type const* const first = _entries[_front_index & _mask];
    for(value_type const* e = first; e != _entries_end; ++e) {
      if (_equator(e->first, x.first))
        return &(e->second);
    }
    for(value_type const* e = _entries; e != first; ++e) {
      if (_equator(e->first, x.first))
        reutrn &(e->second);
    }
    return NULL;
  }*/
  
  /*template <typename K, typename T, typename E>
  inline void
  lru_cache<K,T,E>::make_new_head()
  {
    if (NULL == _free) {
      entry* const e = _sentinal_head->previous;
      e->_next = _sentinal_head->_next;
      e->_next->_previous = e;
      e->_previous->_next = _sentinal_head;
      _sentinal_head->previous = e->_previous;
      e->_previous = _sentinal_head;
    } else {
      entry* const e = _free;
      _free = e->_next;
      e->_next = _sentinal_head->_next;
      e->_next->_previous = e;
      e->_previous = _sentinal_head;
      _sentinal_head->_next = e;
    }
  }
  template <typename K, typename T, typename E>
  template <typename K, typename T, typename E>
  template <typename K, typename T, typename E>
  template <typename K, typename T, typename E>
  template <typename K, typename T, typename E>
  template <typename K, typename T, typename E>
  template <typename K, typename T, typename E>*/
}

