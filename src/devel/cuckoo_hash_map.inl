//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

template <typename K, typename T, typename H1, typename H2, typename E>
sib::cuckoo_hash_map<K,T,H1,H2,E>::cuckoo_hash_map()
: _load_factor(0.5)
, _capacity(1 << 24)
, _mask(_capacity-1)
, _resize(static_cast<std::size_t>(_load_factor * _capacity)+1)
//, _buckets(static_cast<bucket*>(malloc(_capacity << bucket::SIZE_SHIFT)))
, _buckets(new bucket[_capacity])
, _char_buckets(reinterpret_cast<char*>(_buckets))
, _data(_capacity / (2 * bucket::ENTRIES_PER_BUCKET))
{
  //if (sizeof(entry) != 16 || sizeof(bucket) != 32)
  if (sizeof(entry) != 16 || sizeof(bucket) != 64)
    abort();
}

template <typename K, typename T, typename H1, typename H2, typename E>
sib::cuckoo_hash_map<K,T,H1,H2,E>::~cuckoo_hash_map()
{
  delete [] _buckets;
}

template <typename K, typename T, typename H1, typename H2, typename E>
inline T& sib::cuckoo_hash_map<K,T,H1,H2,E>::operator[](typename param<K>::type key)
{
  //if (_size >= _resize) expand();
  //std::cout << _size++ << std::endl;

  // Check with hash1
  uint32_t const hash1 = static_cast<uint32_t>(_hasher1(key));
  bucket* const b1 = lookup(_char_buckets, hash1, _mask);
  for(unsigned ii = 0; ii != bucket::ENTRIES_PER_BUCKET; ++ii) {
    entry& e = b1->_entries[ii];
    if (e.is_empty())
      return init(e, key, hash1, static_cast<uint32_t>(_hasher2(key)))->second;
    else if (e._hash1 == hash1) {
      std::pair<K,T>& item = _data[e._data_index];
      if (_equal(item.first, key))
        return item.second;
    }
  }

  // Check with hash2
  uint32_t const hash2 = static_cast<uint32_t>(_hasher2(key));
  bucket* const b2 = lookup(_char_buckets, hash2, _mask);
  for(unsigned ii = 0; ii != bucket::ENTRIES_PER_BUCKET; ++ii) {
    entry& e = b2->_entries[ii];
    if (e.is_empty())
      return init(e, key, hash1, hash2)->second;
    else if (e._hash1 == hash1 && e._hash2 == hash2) {
      std::pair<K,T>& item = _data[e._data_index];
      if (_equal(item.first, key))
        return item.second;
    }
  }

  entry& e = b1->_entries[0];
  move_with_hash2(e, 20);
  return init(e, key, hash1, hash2)->second;
}

template <typename K, typename T, typename H1, typename H2, typename E>
inline std::pair<K,T>*
sib::cuckoo_hash_map<K,T,H1,H2,E>::init(entry& e, typename param<K>::type key,
                                        uint32_t hash1, uint32_t hash2)
{
  e._hash1 = hash1;
  e._hash2 = hash2;
  if (_free.empty()) {
    e._data_index = _data.size();
    _data.push_back(std::pair<K,T>(key, T()));
  } else {
    std::pair<K,T>* const x = _free.back();
    _free.pop_back();
    e._data_index = static_cast<uint32_t>(x - _data.data());
  }
  return _data.data() + e._data_index;
}

template <typename K, typename T, typename H1, typename H2, typename E>
inline void
sib::cuckoo_hash_map<K,T,H1,H2,E>::init(entry& e, entry const& old_e)
{
  e._data_index = old_e._data_index;
  e._hash1 = old_e._hash1;
  e._hash2 = old_e._hash2;
}

template <typename K, typename T, typename H1, typename H2, typename E>
inline typename sib::cuckoo_hash_map<K,T,H1,H2,E>::bucket*
sib::cuckoo_hash_map<K,T,H1,H2,E>::lookup(char* buckets, uint32_t hash, std::size_t mask)
{
  return reinterpret_cast<bucket*>(buckets + ((hash & mask) << bucket::SIZE_SHIFT));
}

template <typename K, typename T, typename H1, typename H2, typename E>
void sib::cuckoo_hash_map<K,T,H1,H2,E>::move_with_hash1(entry& e, unsigned left)
{
  if (left == 0)
    abort();

  bucket* const b = lookup(_char_buckets, e._hash1, _mask);
  for(unsigned ii = 0; ii != bucket::ENTRIES_PER_BUCKET; ++ii) {
    entry& new_e = b->_entries[ii];
    if (new_e.is_empty()) {
      init(new_e, e);
      return;
    }
  }

  entry& new_e = b->_entries[0];
  move_with_hash2(new_e, left-1);
  init(new_e, e);
}

template <typename K, typename T, typename H1, typename H2, typename E>
inline void sib::cuckoo_hash_map<K,T,H1,H2,E>::move_with_hash2(entry& e, unsigned left)
{
  if (left == 0)
    abort();

  bucket* const b = lookup(_char_buckets, e._hash2, _mask);
  for(unsigned ii = 0; ii != bucket::ENTRIES_PER_BUCKET; ++ii) {
    entry& new_e = b->_entries[ii];
    if (new_e.is_empty()) {
      init(new_e, e);
      return;
    }
  }

  entry& new_e = b->_entries[0];
  move_with_hash1(new_e, left-1);
  init(new_e, e);
}

