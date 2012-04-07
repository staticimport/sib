#ifndef SIB_CUCKOO_HASH_MAP_HPP
#define SIB_CUCKOO_HASH_MAP_HPP


#include <cstddef>
#include <functional>
#include <stdint.h>
#include <utility>

#include "common.hpp"
#include "hash.hpp"
#include "vector.hpp"

namespace sib
{
  template <typename K, typename T, 
            typename Hash1=std::hash<T>,
            typename Hash2=sib::hash<T>,
            typename Equal=std::equal_to<T> >
  class cuckoo_hash_map
  {
  public:
    cuckoo_hash_map();
    ~cuckoo_hash_map();

    T& operator[](typename param<K>::type key);
  private:
    struct entry
    {
      entry() : _data_index(0xffffffff) { }

      inline bool is_empty() const { return _data_index == 0xffffffff; }

      std::size_t _data_index;
      uint32_t _hash1;
      uint32_t _hash2;
//#if sizeof(std::size_t) == 4
//      char pad[4];
//#endif
    };

    struct bucket
    {
      static unsigned const ENTRIES_PER_BUCKET = 4;
      static unsigned const SIZE_SHIFT = 6;
      
      entry _entries[ENTRIES_PER_BUCKET];
    };

    //void expand(std::size_t const new_capacity);
    std::pair<K,T>* init(entry& e, typename param<K>::type key, 
                         uint32_t hash1, uint32_t hash2);
    void init(entry& e, entry const& old);
    bucket* lookup(char* const buckets, uint32_t const hash, std::size_t const mask);
    void move_with_hash1(entry& e, unsigned moves_left);
    void move_with_hash2(entry& e, unsigned moves_left);

    double const _load_factor;
    std::size_t _capacity;
    std::size_t _mask;
    std::size_t _size;
    std::size_t _resize;
    bucket* _buckets;
    char* _char_buckets;
    sib::vector<std::pair<K,T> > _data;
    sib::vector<std::pair<K,T>*> _free;
    Hash1 _hasher1;
    Hash2 _hasher2;
    Equal _equal;
  };
}

#include "cuckoo_hash_map.inl"

#endif /* SIB_CUCKOO_HASH_MAP_HPP */

