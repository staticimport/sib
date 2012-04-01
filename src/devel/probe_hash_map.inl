
template <typename K, typename T, typename H>
sib::probe_hash_map<K,T,H>::probe_hash_map(double const load_factor)
: _load_factor(load_factor)
, _capacity(32)
, _mask(_capacity-1)
, _resize(static_cast<std::size_t>(_load_factor * _capacity)+1)
, _buckets(new bucket[_capacity])
, _char_buckets(reinterpret_cast<char*>(_buckets))
, _data(16)
{
}


template <typename K, typename T, typename H>
sib::probe_hash_map<K,T,H>::~probe_hash_map()
{
  delete [] _buckets;
}

template <typename K, typename T, typename H>
inline void sib::probe_hash_map<K,T,H>::clear()
{
  _data.clear();
  _free.clear();
  memset(_buckets, 0xff, _capacity << 3);
}

template <typename K, typename T, typename H>
T& sib::probe_hash_map<K,T,H>::operator[](typename param<K>::type key)
{
  uint32_t const hash = static_cast<uint32_t>(_hash(key));
  uint32_t increment(0);
  while(true) {
    bucket* b = lookup(_char_buckets, hash, increment, _mask);
    if (b->_lower_hash == hash && b->valid()) {
      return _data[b->_data_offset];
    } else if (!b->valid()) {
      //std::cout << increment << std::endl;
      if (size() >= _resize) {
        expand(_capacity << 1);
        increment = 0;
        while(true) {
          b = lookup(_char_buckets, hash, increment, _mask);
          if (!b->valid()) break;
          else ++increment;
        }
      }
      b->_lower_hash = hash;
      if (_free.empty()) {
        std::size_t const offset = _data.size();
        _data.resize(offset+1);
        b->_data_offset = static_cast<uint32_t>(offset);
        return _data.back();
      } else {
        T* const entry = _free.back();
        _free.pop_back();
        b->_data_offset = static_cast<uint32_t>(entry - _data.data());
        return *entry;
      }
    } else
      ++increment;
  }
}

template <typename K, typename T, typename H>
inline typename sib::probe_hash_map<K,T,H>::bucket*
sib::probe_hash_map<K,T,H>::lookup(char* const char_buckets, uint32_t const hash, 
                                   uint32_t const increment, std::size_t const mask)
{
  return reinterpret_cast<bucket*>(char_buckets + (((hash + increment*increment) & mask) << 3));
}

template <typename K, typename T, typename H>
void sib::probe_hash_map<K,T,H>::expand(std::size_t const new_capacity)
{
  std::size_t const new_mask = new_capacity - 1;
  bucket* const new_buckets = new bucket[new_capacity];
  char* const new_char_buckets = reinterpret_cast<char*>(new_buckets);
  bucket* const old_end = _buckets + _capacity;
  for(bucket* b = _buckets; b != old_end; ++b) {
    if (!b->valid()) continue;
    uint32_t const hash = b->_lower_hash;
    uint32_t increment = 0;
    while(true) {
      bucket* new_b = lookup(new_char_buckets, hash, increment, new_mask);
      if (!new_b->valid()) {
        new_b->_lower_hash = hash;
        new_b->_data_offset = b->_data_offset;
        break;
      } else
        ++increment;
    }
  }
  delete [] _buckets;
  _capacity = new_capacity;
  _mask = new_mask;
  _buckets = new_buckets;
  _char_buckets = new_char_buckets;
  _resize = static_cast<std::size_t>(_load_factor * _capacity) + 1;
}

