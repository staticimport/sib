//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_bucket_HASH_MAP_HPP
#define SIB_bucket_HASH_MAP_HPP

#include <cstddef>
#include <stdint.h>

#include "common.hpp"
#include "hash.hpp"
#include "pool_allocator.hpp"

namespace sib
{
  template <typename K, typename T>
  class bucket_hash_map_entry
  {
  private:
    char _data[sizeof(K) + sizeof(T) + sizeof(std::size_t) + sizeof(void*)];
  };

  template <typename K, 
            typename T, 
            typename Hash=std::hash<K>,
            typename Alloc=std::allocator<bucket_hash_map_entry<K,T> > >
  class bucket_hash_map
  {
  public:
    bucket_hash_map(double const load_factor = 0.7);
    ~bucket_hash_map();

    // Const
    bool empty() const          { return 0 == _size; }
    double avg_bucket_depth() const;
    std::size_t size() const    { return _size; }

    // Non-Const
    void clear();
    T& operator[](typename param<K>::type key);
  private:
    struct entry
    {
      static const std::size_t PRESENCE_FLAG = 0x8000000000000000;

      entry(typename param<K>::type key, std::size_t const hash, entry* next=NULL);

      K _key;
      T _value;
      std::size_t _hash;
      entry* _next;
    };

    std::size_t compute_hash(typename param<K>::type key) const;
    std::size_t index(std::size_t const hash_value) const;
    
    void expand();

    std::size_t _capacity;
    std::size_t _mask;
    std::size_t _size;
    double const _load_factor;
    std::size_t _resize;
    entry* _buckets;
    mutable Hash _hasher;
    simple_pool_allocator _pool;
    uint8_t _mask_bits;
  };
}

#include "bucket_hash_map.inl"

#endif /* SIB_bucket_HASH_MAP_HPP */

