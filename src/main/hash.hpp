#ifndef SIB_HASH_HPP
#define SIB_HASH_HPP

#include <cstddef>
#include <string>

namespace sib
{
  inline std::size_t fnv_hash(char const* begin, char const* const end,
                              std::size_t const seed=0);

  template <typename T>
  inline std::size_t hash_value(T const& x, std::size_t const seed=0)
  {
    char const* begin = reinterpret_cast<char const*>(&x);
    return fnv_hash(begin, begin+sizeof(T), seed);
  }

  template <>
  inline std::size_t hash_value<std::string>(std::string const& x,
                                             std::size_t const seed)
  {
    return fnv_hash(x.c_str(), x.c_str()+x.length(), seed);
  }

  template <typename T>
  struct hash
  {
    inline std::size_t operator()(T const& x, std::size_t const seed=0)
    {
      return hash_value<T>(x,seed);
    }
  };
}

#include "internal/hash.inl"

#endif /* SIB_HASH_HPP */

