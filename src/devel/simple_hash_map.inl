//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>

//#define USE_POOL

template <typename K, typename T, typename H, typename A>
sib::simple_hash_map<K,T,H,A>::simple_hash_map(double const load_factor)
: _capacity(16)
, _mask(_capacity - 1)
, _size(0)
, _load_factor(load_factor)
, _resize(1 + static_cast<std::size_t>(load_factor * _capacity))
, _buckets(static_cast<entry**>(calloc(_capacity, sizeof(entry*))))
, _pool(sizeof(entry))
, _mask_bits(4)
{
}

template <typename K, typename T, typename H, typename A>
sib::simple_hash_map<K,T,H,A>::~simple_hash_map()
{
  clear();
  free(_buckets);
}

template <typename K, typename T, typename H, typename A>
void sib::simple_hash_map<K,T,H,A>::clear()
{
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    entry* e = _buckets[ii];
    while (e) {
      entry* tmp = e->_next;
#ifndef USE_POOL
      delete e;
#else
      _pool.release(e);
#endif
      e = tmp;
    }
    _buckets[ii] = NULL;
  }
  _size = 0;
}

template <typename K, typename T, typename H, typename A>
double sib::simple_hash_map<K,T,H,A>::avg_bucket_depth() const
{
  int used_bucket_count(0);
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    entry* e = _buckets[ii];
    if (e) {
      ++used_bucket_count;
    }
  }
  std::cout << used_bucket_count << "/" << _capacity << std::endl;
  return _size / ((double)used_bucket_count);
}
    
template <typename K, typename T, typename H, typename A>
inline T& sib::simple_hash_map<K,T,H,A>::operator[](typename param<K>::type key)
{
  std::size_t const hash(compute_hash(key));
  entry** bucket(_buckets + index(hash));
  for(entry* e = *bucket; e; e = e->_next) {
    //if (equal(key, hash, e)) {
    if (hash == e->_hash and key == e->_key) {
      return e->_value;
    }
  }
  if (_size >= _resize) {
    expand();
    bucket = _buckets + index(hash);
  }
#ifndef USE_POOL
  entry* const e = new entry(key, hash, *bucket);
#else
  entry* const e = new(_pool.allocate()) entry(key, hash, *bucket);
#endif
  *bucket = e;
  ++_size;
  return e->_value;
}

template <typename K, typename T, typename H, typename A>
inline std::size_t 
sib::simple_hash_map<K,T,H,A>::compute_hash(typename param<K>::type key) const
{
  return _hasher(key);
}
    
/*template <typename K, typename T, typename H, typename A>
inline bool 
sib::simple_hash_map<K,T,H,A>::equal(typename param<K>::type k1, 
                              typename param<K>::type k2) const
{

}*/

template <typename K, typename T, typename H, typename A>
inline std::size_t 
sib::simple_hash_map<K,T,H,A>::index(std::size_t const hash) const
{
  return ((hash >> _mask_bits) ^ hash) & _mask;
}

template <typename K, typename T, typename H, typename A>
void sib::simple_hash_map<K,T,H,A>::expand()
{
  std::size_t const old_capacity = _capacity;
  _capacity <<= 1;
  _mask = _capacity - 1;
  ++_mask_bits;

  entry** new_buckets = static_cast<entry**>(calloc(_capacity, sizeof(entry*)));
  for(std::size_t ii = 0; ii != old_capacity; ++ii) {
    entry* e = _buckets[ii];
    while (e) {
      entry* const next = e->_next;
      std::size_t const new_index = index(e->_hash);
      e->_next = new_buckets[new_index];
      new_buckets[new_index] = e;
      e = next;
    }
  }
  free(_buckets);
  _buckets = new_buckets;
  _resize = 1 + static_cast<std::size_t>(_load_factor * _capacity);
}

template <typename K, typename T, typename H, typename A>
inline sib::simple_hash_map<K,T,H,A>::entry::entry(typename param<K>::type key,
                                            std::size_t const hash,
                                            typename sib::simple_hash_map<K,T,H,A>::entry* next)
: _key(key)
, _hash(hash)
, _next(next)
{
}

