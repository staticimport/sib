//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_PERFECT_HASH_MAP_HPP
#define SIB_PERFECT_HASH_MAP_HPP

#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cstddef>

#include "hash.hpp"

namespace sib
{
  template <typename T>
  struct seed_hash
  {
    boost::hash<T> _std_hash;

    inline std::size_t operator()(T const& x, int seed)
  {
    /*std::size_t const hash(_std_hash(x));
    std::size_t result(hash);
    do {
      result ^= (hash >> (seed & 63));
      seed >>= 6;
    } while (seed);
    return result;*/
    //return hash ^ (hash >> (seed & 63)) ^ (hash >> ((seed >> 6) & 63)) ^
    //  (hash >> ((seed >> 12) & 63)) ^ (hash >> ((seed >> 18) & 63)) ^
    //  (hash >> ((seed >> 24) & 63));

    //std::size_t hash(seed);
    //boost::hash_combine(hash, x);
    //std::size_t const hash = _std_hash(x);
    //std::cout << "hash(" << x << ") = " << hash << std::endl;
    //std::cout << "hash >> 56 = " << (hash >> 56) << std::endl;
    //std::size_t result = hash ^ (seed ^ (hash >> 56));// ^ (hash >> seed);
    //std::cout << "result = " << result << " (& 31 = " << (result & 31) << std::endl;
    //return hash ^ (((hash >> (seed & 15)) & seed)) - (((hash >> (seed & 31)) & seed));
    //return hash ^ (((hash >> (seed & 31)) & seed));

    /*std::size_t hash(_std_hash(x));
    char const* hash_bytes = reinterpret_cast<char const*>(&hash);
    std::size_t result(seed);
    for(unsigned ii = 0; ii != sizeof(std::size_t); ++ii) {
      result ^= hash_bytes[ii];
      result *= 1099511628211;
    }
    return result;*/

    std::size_t hash(_std_hash(x));
    if (seed) hash ^= (hash >> 8) & seed;
    if (seed < 64)
      return hash >> seed;
    else {
      return hash ^ ((hash >> 32) & seed);
    }
    //std::size_t hash(_std_hash(x));
    //return (hash >> (seed & 63)) ^ seed;

    /*std::size_t hash(_std_hash(x));
    std::size_t result = hash ^((hash >> (seed & 63)) & seed);
    if (seed < 64)
      return result;
    else {
    char const* hash_bytes = reinterpret_cast<char const*>(&hash);
    for(unsigned ii = 0; ii != sizeof(std::size_t); ++ii) {
      result *= 1099511628211;
      result ^= hash_bytes[ii];
    }
    return result;
    }*/

    //std::size_t hash(seed);
    //boost::hash_combine(hash, x);
    /*std::size_t const seed_szt(seed);
    std::size_t const hash(_std_hash(x));
    std::size_t const hash1(hash >> (seed & 63));
    std::size_t const hash2((hash >> 5) & seed_szt);
    std::size_t const hash3((hash >> 11) & seed_szt);
    std::size_t const hash4((hash >> 17) & seed_szt);
    std::size_t const hash5((hash >> 23) & seed_szt);
    std::size_t const hash6((hash >> 47) & seed_szt);
    return hash ^ hash1 ^ hash2 ^ hash3 ^ hash4 ^ hash5 ^ hash6;*/
    //return hash ^ ((hash >> 32) & seed) ^ ((hash >> 48) & seed) ^ ((hash >> 16) & seed);
    //return hash ^ (hash >> 32);
  }
  };

  template <typename K, 
            typename V, 
            typename seed_hash=sib::hash<K> >//sib::seed_hash<K> >
  class perfect_hash_map
  {
  public:
    typedef std::pair<K,V> const*   const_iterator;
    typedef std::pair<K,V>*         iterator;
    typedef std::pair<K,V>          value_type;
    
    template <typename input_iterator>
    perfect_hash_map(input_iterator begin, input_iterator end);
    perfect_hash_map(perfect_hash_map const& map);
    ~perfect_hash_map();

    // Const
    const_iterator begin() const  { return _begin; }
    bool empty() const            { return false; }
    const_iterator end() const    { return _end; }
    const_iterator find(K const& key) const;
    std::size_t size() const      { return _key_count; }
    V const& operator[](K const& key) const;

    // Non-Const
    iterator begin()              { return _begin; }
    void clear()                  { /* TODO */ }
    iterator end()                { return _end; }
    iterator find(K const& key);
    std::pair<iterator,bool> insert(value_type const& entry);
    V& operator[](K const& key);
  private:
    seed_hash _seed_hash;
    std::size_t _key_count;
    std::size_t _mask;
    int _init_seed;
    int* _seeds;
    std::pair<K,V>* _begin;
    std::pair<K,V>* _end;
  };

};

#include "perfect_hash_map.inl"

#endif /* SIB_PERFECT_HASH_MAP_HPP */

