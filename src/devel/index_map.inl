//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "bits.hpp"

#include <cstdlib>
#include <cstring>

namespace sib
{

template <typename K, typename T, K M, typename O>
index_map<K,T,M,O>::index_map(K const empty_key, size_type min_init_capacity)
: _empty_key(empty_key)
, _indexed_size(0)
{
  size_type const capacity = power2_above(min_init_capacity);
  _begin = static_cast<value_type*>(malloc(capacity * sizeof(value_type)));
  _end = _begin + capacity;
  clear();
}

template <typename K, typename T, K M, typename O>
index_map<K,T,M,O>::~index_map()
{
  clear();
  free(_begin);
}

template <typename K, typename T, K M, typename O>
void index_map<K,T,M,O>::clear()
{
  for(value_type* v = _begin; v != _end; ++v) {
    v->~value_type();
    v->first = _empty_key;
  }
  _overflow.clear();
  _indexed_size = 0;
}

template <typename K, typename T, K M, typename O>
T& index_map<K,T,M,O>::operator[](K const key)
{
  std::cout << "begin key: " << _begin_key << ", end_key: " << _end_key << std::endl;
  value_type* v = NULL;
  if (_indexed_size) {
    if (key >= _begin_key) {
      if (key <= _end_key) {
        v = _begin + (key - _begin_key);
        v->first = key;
      }
      else if (key <= _begin_key + M) {
        reserve_end(key);
        _end_key = key;
        v = _begin + (key - _begin_key);
        new(v) value_type(key, T());
        ++_indexed_size;
      }
    } else if (key <= _end_key && key >= _end_key - M) {
      reserve_begin(key);
      _begin_key = key;
      v = _begin + (key - _begin_key);
      new(v) value_type(key, T());
      ++_indexed_size;
    }
  } else {
    _begin_key = _end_key = key;
    v = _begin + (key - _begin_key);
    new(v) value_type(key, T());
    ++_indexed_size;
  }

  if (v) {
    return v->second;
  } else
    return _overflow[key];
}

template <typename K, typename T, K M, typename O>
T* index_map<K,T,M,O>::find(K const key)
{
  if (key >= _begin_key && key <= _end_key) {
    value_type* const v = _begin + (key - _begin_key);
    return v->first == _empty_key ? NULL : &(v->second);
  } else {
    typename O::iterator iter = _overflow.find(key);
    return (iter == _overflow.end()) ? NULL : &(iter->second);
  }
}

template <typename K, typename T, K M, typename O>
T const* index_map<K,T,M,O>::find(K const key) const
{
  if (key >= _begin_key && key <= _end_key) {
    std::cout << "in range!\n";
    value_type const* const v = _begin + (key - _begin_key);
    return v->first == _empty_key ? NULL : &(v->second);
  } else {
    typename O::const_iterator iter = _overflow.find(key);
    return (iter == _overflow.end()) ? NULL : &(iter->second);
  }
}

template <typename K, typename T, K M, typename O>
void index_map<K,T,M,O>::reserve_begin(K const min)
{
  if (_end - (_end_key - min) < _begin) {
    std::cout << "expand front!\n";
    size_type const old_capacity = _end - _begin;
    size_type new_capacity = old_capacity << 1;
    while (_end_key - min >= new_capacity)
      new_capacity <<= 1;
    value_type* const new_begin = 
      static_cast<value_type*>(malloc(new_capacity * sizeof(value_type)));
    value_type* const new_end = new_begin + new_capacity;
    memcpy(new_end - old_capacity, _begin, old_capacity * sizeof(value_type));
    for(value_type* v = new_end - old_capacity - 1; v >= new_begin; --v)
      v->first = _empty_key;
    free(_begin);
    _begin = new_begin;
    _end = new_end;
  }
}

template <typename K, typename T, K M, typename O>
void index_map<K,T,M,O>::reserve_end(K const max)
{
  if (_begin + (max - _begin_key) >= _end) {
    std::cout << "expand back!\n";
    size_type const old_capacity = _end - _begin;
    size_type new_capacity = old_capacity << 1;
    while (max - _begin_key >= new_capacity)
      new_capacity <<= 1;
    _begin = static_cast<value_type*>(
      realloc(_begin, new_capacity * sizeof(value_type)));
    _end = _begin + new_capacity;
    for(value_type* v = _begin + old_capacity; v != _end; ++v)
      v->first = _empty_key;
  }
}

}
