#ifndef SIB_PERFECT_HASH_MAP_HPP
#define SIB_PERFECT_HASH_MAP_HPP

#include <algorithm>
#include <cstddef>

namespace sib
{
  template <typename T>
  struct seed_hash
  {
    std::hash<T> _std_hash;

    inline std::size_t operator()(T const& x, int seed)
  {
    return static_cast<std::size_t>(seed) ^ _std_hash(x);
  }
  };

  template <typename K, 
            typename V, 
            typename seed_hash=sib::seed_hash<K> >
  class perfect_hash_map
  {
  public:
    typedef std::pair<K,V> const*   const_iterator;
    typedef std::pair<K,V>*         iterator;
    typedef std::pair<K,V>*         value_type;
    
    template <typename input_iterator>
    perfect_hash_map(input_iterator begin, input_iterator end);
    perfect_hash_map(perfect_hash_map const& map);
    ~perfect_hash_map();

    // Const
    const_iterator begin() const  { return _begin; }
    bool empty() const            { return false; }
    const_iterator end() const    { return _end; }
    std::size_t size() const      { return _key_count; }
    V const& operator[](K const& key) const;

    // Non-Const
    iterator begin()              { return _begin; }
    iterator end()                { return _end; }
    std::pair<iterator,bool> insert(value_type const& entry);
    V& operator[](K const& key);
  private:
    seed_hash _seed_hash;
    std::size_t _key_count;
    int* _seeds;
    std::pair<K,V>* _begin;
    std::pair<K,V>* _end;
  };

};

#include "perfect_hash_map.inl"

#endif /* SIB_PERFECT_HASH_MAP_HPP */

