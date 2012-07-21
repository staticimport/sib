//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_SPECIAL_ARRAY_HASH_TABLE_HPP
#define SIB_SPECIAL_ARRAY_HASH_TABLE_HPP

#include <cstddef>
#include <stdint.h>

namespace sib
{
  namespace internal_saht
  {
    typedef uint32_t hash_t;
    
    template <typename K, typename V>
    struct helper
    {
      static K const& get_key(V const& v)
      {
        return v.first;
      }
    };

    template <typename K>
    struct helper<K,K>
    {
      static K const& get_key(K const& k)
      {
        return k;
      }
    };

    template <typename K, typename V, bool SaveHash>
    struct entry
    {
      typedef V value_type;

      //entry(K const& key, hash_t hash) 
      entry(V const& value, hash_t hash) : _value(value), _hash(hash) {}

      template <typename H>
      hash_t hash(H const& hasher) 
      { 
        return _hash; 
      }

      template <typename E>
      bool is_equal(K const& key, hash_t const hash, E const& equator)
      {
        return hash == _hash && equator(key, _value);
      }

      V _value;
      hash_t const _hash;
    };

    template <typename K, typename V>
    struct entry<K,V,false>
    {
      typedef V type;

      //entry(K const& key, hash_t unused);
      entry(V const& value, hash_t unused) : _value(value) {}

      template <typename H>
      hash_t hash(H const& hasher)
      {
        return hasher(helper<K,V>::get_key(_value));
      }

      template <typename E>
      bool is_equal(K const& key, hash_t const hash, E const& equator)
      {
        return equator(key, _value);
      }

      V _value;
    };

    template <typename E, unsigned D>
    struct bucket
    {
      static unsigned const Depth = D;

      uint32_t _used;
      uint32_t _overflow_capacity;
      E _local[D];
      E* _overflow;
    };

    template <typename B, typename E>
    class iterator
    {
    private;
      typedef typename E::value_type V;
    public:
      iterator(B* begin, B* end);
      iterator(B* end);
      iterator(B* b, B* b_end, E* e, E* e_end, bool overflow);

      V& operator*()                                { return _entry->_value; }
      V* operator->()                               { return &(_entry->_value); }
      iterator operator++()                         { next(); return *this; }
      iterator operator++(int)                      { iterator i(*this); next(); return i; }
      bool operator==(iterator const& other) const  { return _entry == other._entry; }
      bool operator!=(iterator const& other) const  { return _entry != other._entry; }
    private:
      void next();

      B* _bucket;
      B* const _bucket_end;
      E* _entry;
      E* _entry_end;
      bool _overflow_entry;
    };
  };

  template <typename K, typename V, unsigned Depth,
            typename Hash, typename Equal>
  class array_hash_table
  {
  private:
    typedef internal_saht::entry<K,V,
      boost::is_fundamental<K>::value || boost::is_enum<K>::value || boost::is_pointer<K>::value>
      entry_type;
    typedef internal_saht::bucket<entry_type,Depth> bucket_type;
  public:
    typedef internal_saht::iterator<bucket_type const, V const>   const_iterator;
    typedef internal_saht::iterator<bucket_type, V>               iterator;
    typedef std::size_t                                           size_type;

    // 'structors
    array_hash_table(size_type min_capacity=16, double load_factor=0.7);
    array_hash_table(array_hash_table const& table);
    ~array_hash_table();

    // iterators
    iterator begin()                { return iterator(_buckets, _buckets_end); }
    iterator end()                  { return iterator(_buckets_end); }
    const_iterator begin() const    { return cbegin(); }
    const_iterator cbegin() const   { return const_iterator(_buckets, _buckets_end); }
    const_iterator end() const      { return cend(); }
    const_iterator cend() const     { return const_iterator(_buckets_end); }

    // capacity
    bool empty() const              { return 0 == _size; }
    size_type size() const          { return _size; }

    // modify
    void clear();
    void erase(iterator pos);
    iterator erase(K const& key);
    std::pair<iterator,bool> insert(K const& key, V const& value);
    void swap(array_hash_table& table);
    array_hash_table& operator=(array_hash_table const& table);

    // lookup
    V& at(K const& key);
    V const& at(K const& key) const;
    size_type count(K const& key) const;
    iterator find(K const& key);
    const_iterator find(K const& key) const;
    V& operator[](K const& key);
  private:
    size_type hash(K const& key) const;
    void expand(bucket& b);
    void expand();

    template <typename B, typename V>
    V* find_in_overflow(B& bucket, K const& key) const;

    double const  _load_factor;
    size_type     _capacity;
    size_type     _mask;
    size_type     _size;
    size_type     _resize;
    bucket_type*  _buckets;
    bucket_type*  _buckets_end;
    Hash          _hasher;
    Equal         _equator;
  };
}

#include "special_array_hash_table.inl"

#endif /* SIB_SPECIAL_ARRAY_HASH_TABLE_HPP */

