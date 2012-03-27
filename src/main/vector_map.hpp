#ifndef SIB_VECTOR_MAP_HPP
#define SIB_VECTOR_MAP_HPP

#include <functional>

#include "internal/internal_common.hpp"
#include "internal/vector_table.hpp"

namespace sib
{
  template <typename K,
            typename T,
            typename Equal=std::equal_to<K> >
  class vector_map
  {
  public:
    typedef std::pair<K const,T>  value_type;
  private:
    typedef vector_table<K, value_type, 
      pair_first_equal<K,value_type,Equal> > table_type;
  public:
    typedef typename table_type::const_iterator         const_iterator;
    typedef value_type const&                           const_reference;
    typedef typename table_type::const_reverse_iterator const_reverse_iterator;
    typedef std::ptrdiff_t                              difference_type;
    typedef K                                           key_type;
    typedef typename table_type::iterator               iterator;
    typedef T                                           mapped_type;
    typedef value_type&                                 reference;
    typedef typename table_type::reverse_iterator       reverse_iterator;
    typedef std::size_t                                 size_type;

    vector_map() {}

    // element access
    T& at(K const& key);
    T const& at(K const& key) const;
    T& operator[](K const& key)               { return find(key)->second; }

    // iterators
    iterator begin()                          { return _table.begin(); }
    const_iterator begin() const              { return _table.begin(); }
    const_iterator cbegin() const             { return _table.begin(); }
    iterator end()                            { return _table.end(); }
    const_iterator end() const                { return _table.end(); }
    const_iterator cend() const               { return _table.end(); }
    reverse_iterator rbegin()                 { return _table.rbegin(); }
    const_reverse_iterator rbegin() const     { return _table.rbegin(); }
    const_reverse_iterator crbegin() const    { return _table.rbegin(); }
    reverse_iterator rend()                   { return _table.rend(); }
    const_reverse_iterator rend() const       { return _table.rend(); }
    const_reverse_iterator crend() const      { return _table.rend(); }

    // capacity
    size_type capacity() const                { return _table.capacity(); }
    bool empty() const                        { return _table.empty(); }
    size_type max_size() const                { return _table.max_size(); }
    void reserve(size_type size)              { _table.reserve(size); }
    size_type size() const                    { return _table.size(); }

    // modifiers
    void clear()                              { _table.clear(); }
    std::pair<iterator,bool> insert(value_type const& x);
    void swap(vector_map& other)              { _table.swap(other._table); }
    vector_map& operator=(vector_map const& map);

    // lookup
    size_type count(key_type const& key) const      { return _table.count(key); }
    iterator find(key_type const& key)              { return _table.find(key); }
    const_iterator find(key_type const& key) const  { return _table.find(key); }
  private:
    table_type _table;
  };
}

#include "internal/vector_map.inl"

#endif /* SIB_VECTOR_MAP_HPP */

