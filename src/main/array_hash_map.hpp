//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_ARRAY_HASH_MAP_HPP
#define SIB_ARRAY_HASH_MAP_HPP

#include "internal/array_hash_table.hpp"
#include "internal/internal_common.hpp"

#include <boost/functional/hash.hpp>

#include <functional>

namespace sib
{
  template <typename K, typename T,
            typename Hash=boost::hash<K>,
            typename Equal=std::equal_to<K> >
  class array_hash_map
  {
  public:
    typedef std::pair<K const, T>           value_type;
  private:
    typedef array_hash_table<K,value_type,Hash,
      pair_first_equal<K,value_type,Equal>,
      std::allocator<value_type> > table_type;
  public:
    typedef typename table_type::const_iterator   const_iterator;
    typedef typename table_type::iterator         iterator;
    typedef typename table_type::size_type        size_type; 

    // 'structors
    array_hash_map(size_type min_capacity=16, double load_factor=0.7);
    //~array_hash_map();

    // iterators
    iterator begin()              { return _table.begin(); }
    iterator end()                { return _table.end(); }
    const_iterator begin() const  { return _table.cbegin(); }
    const_iterator cbegin() const { return _table.cbegin(); }
    const_iterator end() const    { return _table.cend(); }
    const_iterator cend() const   { return _table.cend(); }

    // capacity
    bool empty() const            { return _table.empty(); }
    size_type size() const        { return _table.size(); }

    // modify
    void clear()                        { _table.clear(); }
    void erase(iterator pos)            { _table.erase(pos); }
    iterator erase(const_iterator pos)  { return _table.erase(pos); }
    size_type erase(K const& key)       { return _table.erase(key); }
    std::pair<iterator,bool> insert(value_type const& x);
    void swap(array_hash_map& map)  { _table.swap(map._table); }
    array_hash_map& operator=(array_hash_map const& map);

    // lookup
    T& at(K const& key)                     { return _table.at(key).second; }
    T const& at(K const& key) const         { return _table.at(key).second; }
    size_type count(K const& key) const     { return _table.count(key); }
    const_iterator find(K const& key) const { return _table.find(key); }
    iterator find(K const& key)             { return _table.find(key); }
    T& operator[](K const& key)             { return _table[key].second; }
  private:
    table_type _table;
  };
}

#include "internal/array_hash_map.inl"

#endif /* SIB_ARRAY_HASH_MAP_HPP */

