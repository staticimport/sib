//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_INDEX_TABLE_HPP
#define SIB_INDEX_TABLE_HPP

#include <cstddef>
#include <map>
#include <utility>

namespace sib
{
  template <typename K, typename T, K MaxWidth,
            typename Overflow=std::map<K,T> >
  class index_map
  {
  public:
    typedef std::size_t size_type;
    typedef std::pair<K, T> value_type;

    // 'structors
    index_map(K const empty_key, size_type min_init_capacity=16);
    ~index_map();

    // capacity
    bool empty() const      { return 0 == _indexed_size && _overflow.empty(); }
    size_type size() const  { return _indexed_size + _overflow.size(); }

    // modifiers
    void clear();
    T& operator[](K const key);

    // lookup
    size_type count(K const key) const  { return find(key) ? 1 : 0; }
    T* find(K const key);
    T const* find(K const key) const;
  private:
    void reserve_begin(K const key);
    void reserve_end(K const key);

    value_type* _begin;
    value_type* _end;
    K const _empty_key;
    K _begin_key;
    K _end_key;
    size_type _indexed_size;
    Overflow _overflow;
  };
}

#include "index_map.inl"

#endif /* SIB_INDEX_TABLE_HPP */

