#ifndef SIB_HASH_MAP_HPP
#define SIB_HASH_MAP_HPP

#include <cstddef>

#include "common.hpp"
#include "hash.hpp"

namespace sib
{
  template <typename K,
            typename T,
            typename Hash=sib::hash<K> >
  class hash_map
  {
  public:
    hash_map(double const load_factor = 0.6);

    // Const
    bool empty() const        { return 0 == _size; }
    std::size_t size() const  { return _size; }

    // Non-Const
    T& operator[](typename param<K>::type key);
  private:
    struct entry
    {
      entry(typename param<K>::type key, uint32_t const hash);
      entry(typename param<K>::type key, typename param<T>::type value, 
            uint32_t const hash);

      void clear()            { _hash = 0; _next = NULL; }
      bool is_present() const { return _hash & 0x80000000; }

      std::pair<K,T> _pair;
      entry* _next;
      uint32_t _hash;
    };

    uint32_t compute_hash(typename param<K>::type key) const;
    bool find(typename param<K>::type key, uint32_t const hash,
              entry*& e, entry*& prev);
    void expand();

    std::size_t _capacity;
    std::size_t _mask;
    std::size_t _size;
    double const _load_factor;
    std::size_t _resize;
    entry* _entries;
    mutable Hash _hasher;
  };
}

#include "hash_map.inl"


#endif /* SIB_HASH_MAP_HPP */

