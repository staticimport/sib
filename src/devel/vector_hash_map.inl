//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>

#define USE_POOL

template <typename K, typename T, typename H, typename A>
sib::vector_hash_map<K,T,H,A>::vector_hash_map(double const load_factor)
: _capacity(1 << 24)
, _mask(_capacity - 1)
, _size(0)
, _load_factor(load_factor)
, _resize(1 + static_cast<std::size_t>(load_factor * _capacity))
, _buckets(static_cast<pod**>(calloc(_capacity, sizeof(pod*))))
, _pool(sizeof(pod))
, _mask_bits(24)
{
}

template <typename K, typename T, typename H, typename A>
sib::vector_hash_map<K,T,H,A>::~vector_hash_map()
{
  clear();
  free(_buckets);
}

template <typename K, typename T, typename H, typename A>
void sib::vector_hash_map<K,T,H,A>::clear()
{
  std::size_t count(0);
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    pod* p = _buckets[ii];
    while (p) {
      pod* tmp = p->_next;
#ifdef USE_POOL
      _pool.release(p);
#else
      delete p;
#endif
      ++count;
      p = tmp;
    }
    _buckets[ii] = NULL;
  }
  std::cout << "allocates: " << _pool.allocate_calls
    << ", releases: " << _pool.release_calls << ", mallocs: " << _pool.mallocs
    << std::endl;
  _pool.clear_stats();
  _size = 0;
  _pool._print_on_alloc = true;
}

template <typename K, typename T, typename H, typename A>
double sib::vector_hash_map<K,T,H,A>::avg_bucket_depth() const
{
  int used_bucket_count(0);
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    pod* p = _buckets[ii];
    if (p) {
      ++used_bucket_count;
    }
  }
  std::cout << used_bucket_count << "/" << _capacity << std::endl;
  return _size / ((double)used_bucket_count);
}
    
template <typename K, typename T, typename H, typename A>
inline T& sib::vector_hash_map<K,T,H,A>::operator[](typename param<K>::type key)
{
  std::size_t const hash(compute_hash(key));
  pod** bucket(_buckets + index(hash));
  for(pod* p = *bucket; p; p = p->_next) {
    for(unsigned ii = 0; ii != VHM_ENTRIES_PER_VECTOR; ++ii) {
      entry& e = p->_entries[ii];
      if (e._hash) {
        if (hash == e._hash and key == e._key) {
          return e._value;
        }
      }
    }
  }
  if (_size >= _resize) {
    expand();
    bucket = _buckets + index(hash);
  }
  entry* available(NULL);
  for(pod* p = *bucket; p; p = p->_next) {
    for(unsigned ii = 0; ii != VHM_ENTRIES_PER_VECTOR; ++ii) {
      entry& e = p->_entries[ii];
      if (0 == e._hash) {
        available = &e;
        break;
      }
    }
    if (available); break;
  }
  if (!available) {
#ifdef USE_POOL
    pod* const p = new(_pool.allocate()) pod();
#else
    pod* const p = new pod();
#endif
    for(unsigned ii = 1; ii != VHM_ENTRIES_PER_VECTOR; ++ii)
      p->_entries[ii]._hash = 0;
    p->_next = *bucket;
    *bucket = p;
    available = p->_entries;
  }
  available = new(available) entry(key, hash);
  ++_size;
  return available->_value;
}

template <typename K, typename T, typename H, typename A>
inline std::size_t 
sib::vector_hash_map<K,T,H,A>::compute_hash(typename param<K>::type key) const
{
  return _hasher(key) | entry::PRESENCE_FLAG;
}
    
/*template <typename K, typename T, typename H, typename A>
inline bool 
sib::vector_hash_map<K,T,H,A>::equal(typename param<K>::type k1, 
                              typename param<K>::type k2) const
{

}*/

template <typename K, typename T, typename H, typename A>
inline std::size_t 
sib::vector_hash_map<K,T,H,A>::index(std::size_t const hash) const
{
  return ((hash >> _mask_bits) ^ hash) & _mask;
}

template <typename K, typename T, typename H, typename A>
void sib::vector_hash_map<K,T,H,A>::expand()
{
  std::cout << "EXPANDING!\n";
  std::size_t const old_capacity = _capacity;
  _capacity <<= 1;
  _mask = _capacity - 1;
  ++_mask_bits;

  pod** new_buckets = static_cast<pod**>(calloc(_capacity, sizeof(pod*)));
  for(std::size_t ii = 0; ii != old_capacity; ++ii) {
    pod* p = _buckets[ii];
    while (p) {
      for(unsigned jj = 0; jj != VHM_ENTRIES_PER_VECTOR; ++jj) {
        entry& e = p->_entries[jj];
        if (e._hash) {
          std::size_t const new_index = index(e._hash);
          pod* new_p = new_buckets[new_index];
          entry* new_e(NULL);
          if (new_p) {
            for(unsigned kk = 0; kk != VHM_ENTRIES_PER_VECTOR; ++kk) {
              if (new_p->_entries[kk]._hash == 0) {
                new_e = &(new_p->_entries[kk]);
                break;
              }
            }
          }
          if (!new_e) {
            new_p = new(_pool.allocate()) pod();
            for(unsigned kk = 1; kk != VHM_ENTRIES_PER_VECTOR; ++kk)
              new_p->_entries[kk]._hash = 0;
            new_p->_next = new_buckets[new_index]; 
            new_buckets[new_index] = new_p;
            new_e = new_p->_entries;
          }
          new(new_e) entry(e);
        }
      }
      pod* next = p->_next;
#ifdef USE_POOL
      p->~pod();
      _pool.release(p);
#else
      delete p;
#endif
      p = next;
    }
  }
  free(_buckets);
  _buckets = new_buckets;
  _resize = 1 + static_cast<std::size_t>(_load_factor * _capacity);
}

template <typename K, typename T, typename H, typename A>
inline sib::vector_hash_map<K,T,H,A>::entry::entry(typename param<K>::type key,
                                                   std::size_t const hash)
: _key(key)
, _hash(hash)
{
}

