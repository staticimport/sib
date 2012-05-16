//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_CUCKOO_HASH_MAP_HPP
#define SIB_CUCKOO_HASH_MAP_HPP

#include <functional>

#include <boost/functional/hash.hpp>

#include "hash.hpp"
#include "internal/internal_common.hpp"
#include "internal/cuckoo_hash_table.hpp"

namespace sib
{
  template <typename K,
            typename T,
            typename Alloc=std::allocator<T>,
            typename Equal=std::equal_to<K>,
            typename Hash1=boost::hash<K>,
            typename Hash2=sib::hash<K> >
  class cuckoo_hash_map
  {
  public:
    typedef std::pair<K const,T> value_type;
  private:
    typedef cuckoo_hash_table<K, value_type, Hash1, Hash2,
      pair_first_equal<K,value_type,Equal>, Alloc> table_type;
  public:
    typedef typename table_type::const_iterator         const_iterator;
    typedef value_type const&                           const_reference;
    typedef std::ptrdiff_t                              difference_type;
    typedef typename table_type::iterator               iterator;
    typedef K                                           key_type;
    typedef T                                           mapped_type;
    typedef value_type&                                 reference;
    typedef typename table_type::size_type              size_type;

    explicit cuckoo_hash_map(size_type min_init_capacity = 16) : _table(min_init_capacity) {}
    ~cuckoo_hash_map() {}

    // iterator
    const_iterator begin() const              { return _table.cbegin(); }
    const_iterator cbegin() const             { return _table.cbegin(); }
    const_iterator end() const                { return _table.cend(); }
    const_iterator cend() const               { return _table.cend(); }
    iterator begin()                          { return _table.begin(); }
    iterator end()                            { return _table.end(); }

    // element access
    T& at(K const& key);
    T const& at(K const& key) const;
    T& operator[](K const& key);

    // capacity
    bool empty() const                        { return _table.empty(); }
    void reserve(size_type size)              { _table.reserve(size); }
    size_type size() const                    { return _table.size(); }

    // modify
    void clear()                                          { _table.clear(); }
    std::pair<iterator,bool> insert(value_type const& x)  { return _table.insert(x.first,x); }
    void swap(cuckoo_hash_map& other)                     { } //_table.swap(other._table); }
    cuckoo_hash_map& operator=(cuckoo_hash_map const& map);

    // lookup
    size_type count(key_type const& key) const      { return _table.count(key); }
    iterator find(key_type const& key)              { return _table.find(key); }
    const_iterator find(key_type const& key) const  { return _table.find(key); }
  private:
    table_type _table;
  };
};

#endif /* SIB_CUCKOO_HASH_MAP_HPP */

