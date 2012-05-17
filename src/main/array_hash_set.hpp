//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_ARRAY_HASH_SET_HPP
#define SIB_ARRAY_HASH_SET_HPP

#include "internal/array_hash_table.hpp"

#include <boost/functional/hash.hpp>

#include <functional>

namespace sib
{
  template <typename T,
            typename Hash=boost::hash<T>,
            typename Equal=std::equal_to<T> >
  class array_hash_set
  {
  private:
    typedef array_hash_table<T,T,Hash,Equal,std::allocator<T> > table_type;
  public:
    typedef typename table_type::const_iterator   const_iterator;
    typedef typename table_type::iterator         iterator;
    typedef typename table_type::size_type        size_type;

    // 'structors
    array_hash_set(size_type min_capacity=16, double load_factor=0.7);
    
    // iterators
    const_iterator begin() const        { return _table.begin(); }
    const_iterator cbegin() const       { return _table.cbegin(); }
    const_iterator end() const          { return _table.end(); }
    const_iterator cend() const         { return _table.cend(); }
    iterator begin()                    { return _table.begin(); }
    iterator end()                      { return _table.end(); }

    // capacity
    bool empty() const                  { return _table.empty(); }
    size_type size() const              { return _table.size(); }

    // modify
    void clear()                                { _table.clear(); }
    void erase(iterator pos)                    { _table.erase(pos); }
    iterator erase(const_iterator pos)          { return _table.erase(pos); }
    size_type erase(T const& x)                 { return _table.erase(x); }
    std::pair<iterator,bool> insert(T const& x) { return _table.insert(x,x); }
    void swap(array_hash_set& set)              { _table.swap(set._table); }

    // lookup
    size_type count(T const& x) const           { return _table.count(x); }
    const_iterator find(T const& x) const       { return _table.find(x); }
    iterator find(T const& x)                   { return _table.find(x); }
  private:
    table_type _table;
};

#include "internal/array_hash_table.hpp"

#endif /* SIB_ARRAY_HASH_SET_HPP */

