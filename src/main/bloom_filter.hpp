#ifndef SIB_BLOOM_FILTER_HPP
#define SIB_BLOOM_FILTER_HPP

#include <functional>
#include <stdint.h>

#include "hash.hpp"

namespace sib
{
  template <typename T, unsigned Count, typename Hash=std::hash<T> >
  class bloom_filter
  {
  public:
    typedef std::size_t size_type;

    bloom_filter(size_type min_byte_count);
    ~bloom_filter();

    void insert(T const& x);
    bool operator[](T const& x) const;
  private:
    size_type const _byte_count;
    size_type const _byte_mask;
    uint8_t* _data;
    Hash _hasher;
  };
}

#include "internal/bloom_filter.inl"

#endif /* SIB_BLOOM_FILTER_HPP */

