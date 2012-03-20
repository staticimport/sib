#ifndef SIB_vector_HASH_MAP_HPP
#define SIB_vector_HASH_MAP_HPP

#include <cstddef>
#include <stdint.h>

#include "common.hpp"
#include "hash.hpp"
#include "pool_allocator.hpp"

namespace sib
{
#define VHM_ENTRIES_PER_VECTOR 3

  template <typename K, typename T>
  class vector_hash_map_entry
  {
  private:
    //char _data[sizeof(K) + sizeof(T) + sizeof(std::size_t) + sizeof(void*)];
    char _data[sizeof(void*) + VHM_ENTRIES_PER_VECTOR * (sizeof(K)+sizeof(T)+sizeof(std::size_t))];
  };

  template <typename K, 
            typename T, 
            typename Hash=std::hash<K>,
            typename Alloc=std::allocator<vector_hash_map_entry<K,T> > >
  class vector_hash_map
  {
  public:
    vector_hash_map(double const load_factor = 0.6);
    ~vector_hash_map();

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
      static std::size_t const PRESENCE_FLAG = 0x8000000000000000;

      entry(typename param<K>::type key, std::size_t const hash);

      K _key;
      T _value;
      std::size_t _hash;
    };

    struct pod
    {
      pod() : _entries(reinterpret_cast<entry*>(_raw)), _next(NULL) {}

      char _raw[VHM_ENTRIES_PER_VECTOR * sizeof(entry)];
      entry* _entries;
      pod* _next;
    };

    std::size_t compute_hash(typename param<K>::type key) const;
    std::size_t index(std::size_t const hash_value) const;
    
    void expand();

    std::size_t _capacity;
    std::size_t _mask;
    std::size_t _size;
    double const _load_factor;
    std::size_t _resize;
    pod** _buckets;
    mutable Hash _hasher;
    simple_pool_allocator _pool;
    uint8_t _mask_bits;
  };
}

#include "vector_hash_map.inl"

#endif /* SIB_vector_HASH_MAP_HPP */

