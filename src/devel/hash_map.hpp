//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_PROBE_HASH_MAP_HPP
#define SIB_PROBE_HASH_MAP_HPP

#include <cstddef>
#include <functional>
#include <stdint.h>

#include "common.hpp"
#include "hash.hpp"
#include "vector.hpp"

namespace sib
{
  template <typename K, typename T, typename Hash=std::hash<K> >
  class probe_hash_map
  {
  public:
    probe_hash_map(double const load_factor = 0.5);
    ~probe_hash_map();

    // Const
    bool empty() const       { return 0 == size(); }
    std::size_t size() const { return _data.size() - _free.size(); }

    // Non-Const
    void clear();
    T& operator[](typename param<K>::type key);
  private:
    struct bucket
    {
      static uint32_t const INVALID_OFFSET = 0xffffffff;
      bucket() : _lower_hash(0), _data_offset(INVALID_OFFSET) { }
      inline bool valid() const { return _data_offset != INVALID_OFFSET; }

      uint32_t _lower_hash;
      uint32_t _data_offset;
    };

    void expand(std::size_t const new_capacity);
    bucket* lookup(char* const buckets, uint32_t const hash, 
                   uint32_t const increment, std::size_t const mask);

    double const _load_factor;
    std::size_t _capacity;
    std::size_t _mask;
    std::size_t _resize;
    bucket* _buckets;
    char* _char_buckets;
    sib::vector<T> _data;
    sib::vector<T*> _free;
    Hash _hash;
  };
}

#include "probe_hash_map.inl"

#endif /* SIB_PROBE_HASH_MAP_HPP */

