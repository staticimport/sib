//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "bits.hpp"

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::cuckoo_hash_table(size_type min_capacity)
: _bucket_set1(power2_above<size_type>(min_capacity, true) / 2)
, _bucket_set2(_bucket_set1._end - _bucket_set1._begin)
, _load_factor(0.7)
, _size(0)
, _resize(static_cast<size_type>(_load_factor * (2 * (_bucket_set1._end - _bucket_set1._begin))))
, _expand_set1_next(true)
{
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::~cuckoo_hash_table()
{
  clear();
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
inline void sib::cuckoo_hash_table<K,V,H1,H2,E,A>::reserve(size_type size)
{
  if (_resize < size)
    expand(size);
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
void sib::cuckoo_hash_table<K,V,H1,H2,E,A>::clear()
{
  std::pair<bucket*,bucket*> bucket_ranges[2];
  bucket_ranges[0] = std::make_pair(_bucket_set1._begin, _bucket_set1._end);
  bucket_ranges[1] = std::make_pair(_bucket_set2._begin, _bucket_set2._end);
  for(unsigned set_index = 0; set_index != 2; ++set_index) {
    for(bucket* bucket = bucket_ranges[set_index].first; 
        bucket != bucket_ranges[set_index].second; 
        ++bucket)
    {
      for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
        if (!bucket->_entries[ii].is_empty())
          _alloc.deallocate(bucket->_entries[ii]._data, 1);
      }
    }
  }
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
std::pair<typename sib::cuckoo_hash_table<K,V,H1,H2,E,A>::iterator,bool> 
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::insert(K const& key, V const& value)
{
  // Check bucket set 1
  uint32_t const hash1 = static_cast<uint32_t>(_bucket_set1._hasher(key));
  bucket* const b1 = lookup(_bucket_set1, hash1);
  for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
    entry& e = b1->_entries[ii];
    if (e.is_empty())
      return init(e, key, hash1, static_cast<uint32_t>(_bucket_set2._hasher(key)));
    else if (e._hash1 == hash1 && _equal(value, *e._data))
      return iterator(e._data);
  }

  // Check bucket set 2;
  uint32_t const hash2 = static_cast<uint32_t>(_bucket_set2._hasher(key));
  bucket* const b2 = lookup(_bucket_set2, hash2);
  for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
    entry& e = b2->_entries[ii];
    if (e.is_empty())
      return init(e, key, hash1, hash2);
    else if (e._hash1 == hash1 && e._hash2 == hash2 && _equal(value, *e._data))
      return iterator(e._data);
  }

  if (_expand_set1_next) {
    entry& e = b1->_entries[0];
    move_to_set2(e, &e, 10);
    init(e, key, hash1, hash2);
    return iterator(e._data);
  } else {
    entry& e = b2->_entries[0];
    move_to_set1(e, &e, 10);
    init(e, key, hash1, hash2);
    return iterator(e._data);
  }
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
inline typename sib::cuckoo_hash_table<K,V,H1,H2,E,A>::size_type 
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::count(K const& key) const
{
  return find(key) == NULL ? 0 : 1;
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
typename sib::cuckoo_hash_table<K,V,H1,H2,E,A>::iterator 
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::find(K const& key)
{
  // Check bucket set 1
  uint32_t const hash1 = static_cast<uint32_t>(_bucket_set1._hasher(key));
  bucket* const b1 = lookup(_bucket_set1, hash1);
  for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
    entry& e = b1->_entries[ii];
    if (!e.is_empty() && e._hash1 == hash1 && _equal(key, *e._data))
      return iterator(e._data);
  }

  // Check bucket set 2;
  uint32_t const hash2 = static_cast<uint32_t>(_bucket_set2._hasher(key));
  bucket* const b2 = lookup(_bucket_set2, hash2);
  for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
    entry& e = b2->_entries[ii];
    if (!e.is_empty() && e._hash1 == hash1 && e._hash2 && _equal(key, *e._data))
      return iterator(e._data);
  }

  return iterator(NULL);
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
typename sib::cuckoo_hash_table<K,V,H1,H2,E,A>::const_iterator
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::find(K const& key) const
{
  // Check bucket set 1
  uint32_t const hash1 = static_cast<uint32_t>(_bucket_set1._hasher(key));
  bucket const* const b1 = lookup(_bucket_set1, hash1);
  for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
    entry const& e = b1->_entries[ii];
    if (!e.is_empty() && e._hash1 == hash1 && _equal(key, *e._data))
      return const_iterator(e._data);
  }

  // Check bucket set 2;
  uint32_t const hash2 = static_cast<uint32_t>(_bucket_set2._hasher(key));
  bucket const* const b2 = lookup(_bucket_set2, hash2);
  for(unsigned ii = 0; ii != ENTRIES_PER_BUCKET; ++ii) {
    entry const& e = b2->_entries[ii];
    if (!e.is_empty() && e._hash1 == hash1 && e._hash2 && _equal(key, *e._data))
      return const_iterator(e._data);
  }

  // Check stash
  for(typename sib::vector<V*>::const_iterator iter = _stash.begin(); iter != _stash.end(); ++iter) {
    if (_equal(key, *iter))
      return const_iterator(*iter);
  }

  return const_iterator(NULL);
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
template <typename H>
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::bucket_set<H>::bucket_set(size_type capacity)
: _mask(capacity - 1)
, _begin(new bucket[capacity])
, _end(_begin + capacity)
{
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
template <typename H>
void sib::cuckoo_hash_table<K,V,H1,H2,E,A>::bucket_set<H>::expand(size_type min_cap)
{
  size_type new_capacity = (_end - _begin);
  do {
    new_capacity <<= 1;
  } while (new_capacity < min_cap);
  bucket* const new_buckets = new bucket[new_capacity];
  size_type const new_mask = new_capacity - 1;
  for(bucket* b = _begin; b != _end; ++b) {
    for(unsigned old_ii = 0; old_ii != ENTRIES_PER_BUCKET; ++old_ii) {
      entry& e = b->_entries[old_ii];
      if (e.is_empty()) continue;
      bucket* const new_b = lookup(_begin, e._hash2, _mask);
      for(unsigned new_ii = 0; new_ii != ENTRIES_PER_BUCKET; ++new_ii) {
        entry& new_e = new_b->_entries[old_ii];
        if (new_e.is_empty()) {
          new_e = e;
          break;
        }
      }
    }
  }
  delete[] _begin;
  _mask = new_mask;
  _begin = new_buckets;
  _end = new_buckets + new_capacity;
}


template <typename K, typename V, typename H1, typename H2, typename E, typename A>
template <typename H>
inline typename sib::cuckoo_hash_table<K,V,H1,H2,E,A>::bucket*
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::lookup(bucket_set<H>& set, uint32_t hash)
{
  return reinterpret_cast<bucket*>(reinterpret_cast<char*>(set._buckets) + 
    ((hash & set._mask) << SIZE_SHIFT));
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
template <typename H>
inline typename sib::cuckoo_hash_table<K,V,H1,H2,E,A>::bucket const*
sib::cuckoo_hash_table<K,V,H1,H2,E,A>::lookup(bucket_set<H>& set, uint32_t hash) const
{
  return reinterpret_cast<bucket*>(reinterpret_cast<char*>(set._buckets) + 
    ((hash & set._mask) << SIZE_SHIFT));
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
void sib::cuckoo_hash_table<K,V,H1,H2,E,A>::move_with_hash1(entry& e, unsigned moves_left)
{
  bucket& b = *lookup(_bucket_set1, e._hash1);
  for(unsigned ii = 0; ii != bucket::ENTRIES_PER_BUCKET; ++ii) {
    entry& new_e = b._entries[ii];
    if (new_e.is_empty()) {
      init(new_e, e);
      return;
    }
  }

  if (--moves_left) {
    entry& new_e = b._entries[0];
    move_with_hash2(new_e, moves_left-1);
    init(new_e, e);
  } else {
    _stash.push_back(e._data);
  }
}

template <typename K, typename V, typename H1, typename H2, typename E, typename A>
void sib::cuckoo_hash_table<K,V,H1,H2,E,A>::move_with_hash2(entry& e, unsigned moves_left)
{
  bucket& b = *lookup(_bucket_set2, e._hash2);
  for(unsigned ii = 0; ii != bucket::ENTRIES_PER_BUCKET; ++ii) {
    entry& new_e = b._entries[ii];
    if (new_e.is_empty()) {
      init(new_e, e);
      return;
    }
  }

  if (--moves_left) {
    entry& new_e = b._entries[0];
    move_with_hash1(new_e, moves_left-1);
    init(new_e, e);
  } else {
    _stash.push_back(e._data);
  }
}

