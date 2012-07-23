#include <iostream>

template <typename K, typename T, typename H>
inline sib::hash_map<K,T,H>::hash_map()
: _capacity(8)
, _mask(_capacity - 1)
, _size(0)
, _load_factor(load_factor)
, _resize(1 + static_cast<std::size_t>(_capacity * _load_factory))
, _entries(static_cast<entry*>(malloc(_capacity * sizeof(entry))))
{
  for(std::size_t ii = 0; ii != _capacity; ++ii) {
    _entries[ii].clear();
  }
}

template <typename K, typename T, typename H>
inline T& sib::hash_map<K,T,H>::operator[](typename param<K>::type key)
{
  uint32_t const hash(compute_hash(key));
  std::cout << (hash & _mask) << std::endl;
  entry *e, *prev;
  if (!find(key, hash, e, prev)) {
    e = (e ? new(e) entry(key, hash) : new entry(key, hash));
    if (prev) prev->_next = e;
    ++_size;
  }
  return e->_pair.second;
}

template <typename K, typename T, typename H>
inline uint32_t 
sib::hash_map<K,T,H>::compute_hash(typename param<K>::type key) const
{
  return static_cast<uint32_t>(_hasher(key) & 0x7fffffff) | 0x80000000;
}

template <typename K, typename T, typename H>
inline bool sib::hash_map<K,T,H>::find(
    typename param<K>::type key, uint32_t hash, entry*& e, entry*& prev)
{
  entry* first_empty(NULL);
  e = _entries + (hash & _mask);
  prev = NULL;
  do {
    if (e->is_present()) {
      if (hash == e->_hash and key == e->_pair.first) 
        return true;
    } else if (!first_empty)
      first_empty = e;
    e = (prev = e)->_next;
  } while(e);
  return false;
}
                                    

template <typename K, typename T, typename H>
inline sib::hash_map<K,T,H>::entry::entry(typename param<K>::type key,
                                          uint32_t const hash)
: _pair(key,T())
, _next(NULL)
, _hash(hash)
{
  _hash |= 0x80000000;
}

template <typename K, typename T, typename H>
inline sib::hash_map<K,T,H>::entry::entry(typename param<K>::type key,
                                          typename param<T>::type value,
                                          uint32_t const hash)
: _pair(key,value)
, _next(NULL)
, _hash(hash)
{
  _hash |= 0x80000000;
}

