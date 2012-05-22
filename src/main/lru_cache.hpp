//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_ARRAY_HASH_MAP_HPP
#define SIB_ARRAY_HASH_MAP_HPP

#include <cstddef>
#include <functional>
#include <utility>

namespace sib
{
  namespace internal_lru
  {
    /*template <typename E, typename V>
    class iterator
    {
    public:
      iterator(E* entry, std::size_t ) : _entry(entry) {}

      V& operator*() const      { return _entry->_data; }
      V* operator->() const     { return &(_entry->_data); }
      iterator operator++()     { _entry = _entry->_next; return *this; }
      iterator operator++(int)  { iterator i(_entry); _entry = _entry->next; return i; }

      bool operator==(iterator const& i) const  { return _entry == i._entry; }
      bool operator!=(iterator const& i) const  { return _entry != i._entry; }
    private:
      E* _entry;
    }; */
  };

  template <typename K, typename T, typename Equal=std::equal_to<K> >
  class lru_cache
  {
  public:
    typedef std::pair<K, T>     value_type;

    //typedef internal_lru::iterator<entry const, value_type const> const_iterator;
    //typedef internal_lru::iterator<entry, value_type>             iterator;
    typedef std::size_t                                           size_type;

    lru_cache(size_type min_size, K const& empty_key);
    ~lru_cache();

    // iterators
    /*iterator begin()                              { return iterator(_head); }
    iterator end()                                { return iterator(_tail); }
    const_iterator begin() const                  { return cbegin(); }
    const_iterator cbegin() const                 { return const_iterator(_head); }
    const_iterator end() const                    { return cend(); }
    const_iterator cend() const                   { return const_iterator(_tail); }*/

    // modify
    //void clear();
    //void erase(K const& key);
    //std::pair<iterator,bool> insert(value_type const& x);
    //T& operator[](K const& key);
    void add(value_type const& x);

    // lookup
    //size_type count(K const& key) const;
    T* lookup(K const& key);
    //T& operator[](K const& key);
  //private:
    struct entry
    {
      value_type _data;
      entry* _previous;
      entry* _next;
    };

    void move_to_head(entry* const entry);
    void move_to_tail(entry* const entry);

    size_type const _capacity;
    size_type const _mask;
    value_type* const _entries;
    value_type* const _entries_end;
    size_type _front_index;
    Equal _equator;
    K const _empty_key;
  };
}

#include "internal/lru_cache.inl"

#endif /* SIB_ARRAY_HASH_MAP_HPP */

