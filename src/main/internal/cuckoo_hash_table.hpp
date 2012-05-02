//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_CUCKOO_HASH_TABLE_HPP
#define SIB_CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdint.h>
#include <utility>

#include "common.hpp"
#include "hash.hpp"
#include "vector.hpp"

namespace sib
{
  template <typename K, typename V, 
            typename Hash1, typename Hash2, 
            typename Equal, typename Allocator>
  class cuckoo_hash_table
  {
  private:
    struct entry
    {
      entry() : _data(NULL) {}

      inline bool is_empty() const { return NULL == _data; }

      V* _data;
      uint32_t _hash1;
      uint32_t _hash2;
    };

    template <typename D>
    class cuckoo_iterator
    {
    public:
      cuckoo_iterator(D* data) : _data(data) {}
      D& operator*() { return *_data; }
      D* operator->() { return _data; }
      bool operator==(cuckoo_iterator const& o) const { return _data == o._data; }
    private:
      /*void back();
      void forward();

      cuckoo_hash_table& _table;
      bucket* _buckets_begin;
      bucket* _buckets_end;
      bucket* _current_bucket;
      unsigned _entry_index;*/
      D* _data;
    };
  public:
    typedef cuckoo_iterator<V const>  const_iterator;
    typedef cuckoo_iterator<V>        iterator;
    typedef std::size_t size_type;

    // 'structors
    explicit cuckoo_hash_table(size_type min_init_capacity = 16);
    ~cuckoo_hash_table();

    // capacity
    bool empty() const                                { return 0 == _size; }
    void reserve(size_type size);
    size_type size() const                            { return _size; }

    // modifiers
    void clear();
    std::pair<iterator,bool> insert(K const& key, V const& value);
    //size_type erase(K const& key);
    //void swap(cuckoo_hash_table& other);

    // lookup
    size_type count(K const& key) const;
    iterator find(K const& key);
    const_iterator find(K const& key) const;
  private:
    static unsigned const ENTRIES_PER_BUCKET = 4;
    static unsigned const SIZE_SHIFT = 6;


    struct bucket
    {
      entry _entries[ENTRIES_PER_BUCKET];
    };

    template <typename Hash>
    struct bucket_set
    {
      bucket_set(size_type capacity);
      ~bucket_set() { delete [] _begin; }

      void expand(size_type const min_size);
      //V* find(K const& key);
      //V const* find(K const& key) const;

      size_type _mask;
      bucket* _begin;
      bucket* _end;
      Hash _hasher;
    };

    void expand(size_type const min_size);

    template <typename H>
    bucket* lookup(bucket_set<H>& set, uint32_t hash);
    template <typename H>
    bucket const* lookup(bucket_set<H>& set, uint32_t hash) const;

    void move_with_hash1(entry& e, unsigned moves_left);
    void move_with_hash2(entry& e, unsigned moves_left);

    bucket_set<Hash1> _bucket_set1;
    bucket_set<Hash2> _bucket_set2;
    sib::vector<V*> _stash;
    typename Allocator::template rebind<V>::other _alloc;
    Equal _equal;
    double const _load_factor;
    std::size_t _size;
    std::size_t _resize;
    bool _expand_set1_next;
  };
}

#include "internal/cuckoo_hash_table.inl"

#endif /* SIB_CUCKOO_HASH_TABLE_HPP */

