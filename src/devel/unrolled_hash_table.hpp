//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_UNROLLED_HASH_TABLE_HPP
#define SIB_UNROLLED_HASH_TABLE_HPP

#include <cstddef>
#include <stdint.h>

namespace sib
{
  namespace unrolled_hash_table_internal
  {
    typedef uint32_t hash_type;

    template <typename K, typename V, bool SaveHash>
    struct entry
    {
      template <typename H>
      hash_type get_hash() const;

      template <typename E>
      bool equals(boost::call_traits<K>::param_type key, E const& equator);
    };

    template <typename E, unsigned Depth>
    struct bucket
    {
      E _entries[Depth];
      bucket<E,Depth>* _next;
    };
  };

  template <typename K, 
            typename V,
            unsigned Depth,
            typename Hash,
            typename Eqaul,
            typename Alloc>
  class unrolled_hash_table
  {
  private:
    typedef unrolled_hash_table_internal::entry<V> entry_type;
    typedef unrolled_hash_table_internal::bucket<V,Depth> bucket_type;
    typedef boost::call_traits<K>::param_type key_param_type;
    typedef boost::call_traits<V>::param_type value_param_type;
    typedef unrolled_hash_table_internal::hash_type hash_type;
  public:
    typedef std::size_t size_type;

    // 'structors
    unrolled_hash_table(size_type min_init_capacity=16, double load_factor=0.5);
    ~unrolled_hash_table();

    // capacity
    bool empty() const            { return 0 == _size; }
    size_type size() const        { return _size; }

    // modify
    void clear();
    //void erase(iterator pos)        { erase(pos->first); }
    //iterator erase(const_iterator pos);
    //size_type erase(K const& key);
    //std::pair<iterator,bool> insert(K const& key, V const& value);
    void insert(key_param_type key, value_param_type value);
    //void swap(array_hash_table& table);
    //array_hash_table& operator=(array_hash_table const& table);

    // lookup
    //V& at(K const& key);
    //V const& at(K const& key) const;
    //size_type count(K const& key) const;
    //const_iterator find(K const& key) const;
    //iterator find(K const& key);
    V& operator[](key_param_type key);
  private:
    hash_type get_hash(key_param_type key) const;

    double const  _load_factor;
    size_type     _capacity;
    size_type     _mask;
    size_type     _size;
    size_type     _resize; // ~= _load_factor * _capacity * Depth
    bucket_type*  _buckets;
    bucket_type*  _buckets_end;
    Hash          _hasher;
    Equal         _equator;
    Allocator     _allocator;
  };
}

#include "unrolled_hash_table.inl"

#endif /* SIB_UNROLLED_HASH_TABLE_HPP */

