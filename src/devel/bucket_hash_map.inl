
#include <cstdlib>

//#define USE_POOL

template <typename K, typename T, typename H, typename A>
sib::bucket_hash_map<K,T,H,A>::bucket_hash_map(double const load_factor)
: _capacity(1 << 24)
, _mask(_capacity - 1)
, _size(0)
, _load_factor(load_factor)
, _resize(1 + static_cast<std::size_t>(load_factor * _capacity))
, _buckets(static_cast<entry*>(calloc(_capacity, sizeof(entry))))
, _pool(sizeof(entry))
, _mask_bits(24)
{
}

template <typename K, typename T, typename H, typename A>
sib::bucket_hash_map<K,T,H,A>::~bucket_hash_map()
{
  clear();
  free(_buckets);
}

template <typename K, typename T, typename H, typename A>
void sib::bucket_hash_map<K,T,H,A>::clear()
{
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    entry* e = _buckets[ii]._next;
    while (e) {
      entry* tmp = e->_next;
#ifndef USE_POOL
      delete e;
#else
      _pool.release(e);
#endif
      e = tmp;
    }
    _buckets[ii]._next = NULL;
    _buckets[ii]._hash = 0;
  }
  _size = 0;
}

template <typename K, typename T, typename H, typename A>
double sib::bucket_hash_map<K,T,H,A>::avg_bucket_depth() const
{
  int used_bucket_count(0);
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    entry& e = _buckets[ii];
    if (e._hash || e._next) {
      ++used_bucket_count;
    }
  }
  std::cout << used_bucket_count << "/" << _capacity << std::endl;
  return _size / ((double)used_bucket_count);
}
    
template <typename K, typename T, typename H, typename A>
inline T& sib::bucket_hash_map<K,T,H,A>::operator[](typename param<K>::type key)
{
  std::size_t const hash(compute_hash(key));
  entry* bucket(_buckets + index(hash));
  for(entry* e = bucket; e; e = e->_next) {
    if (hash == e->_hash and key == e->_key) {
      return e->_value;
    }
  }
  if (_size >= _resize) {
    expand();
    bucket = _buckets + index(hash);
  }
  entry* e;
  if (0 == bucket->_hash) {
    e = new(bucket) entry(key, hash, bucket->_next);
  } else {
#ifndef USE_POOL
    e = new entry(key, hash, bucket->_next);
#else
    e = new(_pool.allocate()) entry(key, hash, bucket->_next);
#endif
    bucket->_next = e;
  }
  ++_size;
  return e->_value;
}

template <typename K, typename T, typename H, typename A>
inline std::size_t 
sib::bucket_hash_map<K,T,H,A>::compute_hash(typename param<K>::type key) const
{
  return _hasher(key) | entry::PRESENCE_FLAG;
}
    
/*template <typename K, typename T, typename H, typename A>
inline bool 
sib::bucket_hash_map<K,T,H,A>::equal(typename param<K>::type k1, 
                              typename param<K>::type k2) const
{

}*/

template <typename K, typename T, typename H, typename A>
inline std::size_t 
sib::bucket_hash_map<K,T,H,A>::index(std::size_t const hash) const
{
  return ((hash >> _mask_bits) ^ hash) & _mask;
}

template <typename K, typename T, typename H, typename A>
void sib::bucket_hash_map<K,T,H,A>::expand()
{
  std::size_t const old_capacity = _capacity;
  _capacity <<= 1;
  _mask = _capacity - 1;
  ++_mask_bits;

  entry* new_buckets = static_cast<entry*>(calloc(_capacity, sizeof(entry*)));
  for(std::size_t ii = 0; ii != old_capacity; ++ii) {
    bool first(true);
    entry* e = &(_buckets[ii]);
    while (e) {
      entry* const next = e->_next;
      std::size_t const new_index = index(e->_hash);
      entry& new_bucket = new_buckets[new_index];
      if (0 == new_bucket._hash) {
        new(&new_bucket) entry(*e);
        if (!first) {
#ifndef USE_POOL
          delete e;
#else
          e->~entry();
          _pool.release(e);
#endif
        }
      } else if (first) {
#ifndef USE_POOL
        entry* const new_e = new entry(*e);
#else
        entry* const new_e = new(_pool.allocate()) entry(*e);
#endif
        new_e->_next = new_bucket._next;
        new_bucket._next = new_e;
      } else {
        e->_next = new_bucket._next;
        new_bucket._next = e;
      }
      e = next;
      first = false;
    }
  }
  free(_buckets);
  _buckets = new_buckets;
  _resize = 1 + static_cast<std::size_t>(_load_factor * _capacity);
}

template <typename K, typename T, typename H, typename A>
inline sib::bucket_hash_map<K,T,H,A>::entry::entry(typename param<K>::type key,
                                            std::size_t const hash,
                                            typename sib::bucket_hash_map<K,T,H,A>::entry* next)
: _key(key)
, _hash(hash)
, _next(next)
{
}

