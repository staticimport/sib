//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_ARRAY_HASH_TABLE_HPP
#define SIB_ARRAY_HASH_TABLE_HPP

#include <cstddef>
#include <stdint.h>

namespace sib
{
  namespace internal_aht
  {
    template <typename V>
    struct bucket
    {
      uint32_t _capacity;
      uint32_t _used;
      //V _data[0];
      V* _data;
    };

    template <typename B, typename V>
    struct iterator
    {
    public:
      iterator(B* begin, B* end);
      iterator(B* end);
      iterator(B* b, B* b_end, V* v, V* v_end);

      V& operator*()              { return *_data; }
      V* operator->()             { return _data; }
      iterator operator++()       { next(); return *this; }
      iterator operator++(int)    { iterator i(*this); next(); return i; }
      bool operator==(iterator const& other) const;
      bool operator!=(iterator const& other) const;
    private:
      void next();

      B* _bucket;
      B* const _bucket_end;
      V* _data;
      V* _data_end;
    };
  }

  template <typename K, typename V, 
            typename Hash, typename Equal,
            typename Allocator>
  class array_hash_table
  {
  private:
    typedef internal_aht::bucket<V> bucket;
  public:
    typedef internal_aht::iterator<bucket const, V const>   const_iterator;
    typedef internal_aht::iterator<bucket, V>               iterator;
    typedef std::size_t                                     size_type;

    // 'structors
    array_hash_table(size_type min_capacity=16, double load_factor=0.7);
    ~array_hash_table();

    // iterators
    const_iterator begin() const    { return cbegin(); }
    const_iterator cbegin() const   { return const_iterator(_buckets, _buckets_end); }
    const_iterator end() const      { return cend(); }
    const_iterator cend() const     { return const_iterator(_buckets_end); }
    iterator begin()                { return iterator(_buckets, _buckets_end); }
    iterator end()                  { return iterator(_buckets_end); }

    // capacity
    bool empty() const              { return 0 == _size; }
    size_type size() const          { return _size; }

    // modify
    void clear();
    void erase(iterator pos)        { erase(pos->first); }
    iterator erase(const_iterator pos);
    size_type erase(K const& key);
    std::pair<iterator,bool> insert(K const& key, V const& value);
    void swap(array_hash_table& table);
    array_hash_table& operator=(array_hash_table const& table);

    // lookup
    V& at(K const& key);
    V const& at(K const& key) const;
    size_type count(K const& key) const;
    const_iterator find(K const& key) const;
    iterator find(K const& key);
    V& operator[](K const& key);
  private:
    size_type hash(K const& key) const;
    void expand(bucket& b);
    void expand();

    double const  _load_factor;
    std::size_t   _capacity;
    std::size_t   _mask;
    std::size_t   _size;
    std::size_t   _resize;
    bucket*       _buckets;
    bucket*       _buckets_end;
    Hash          _hasher;
    Equal         _equator;
    Allocator     _allocator;
  };
}

#include "array_hash_table.inl"

#endif /* SIB_ARRAY_HASH_TABLE_HPP */

