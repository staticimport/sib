//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "../bits.hpp"

#include <cstdlib>

namespace sib
{
  namespace internal_aht
  {
    template <typename B, typename V>
    inline iterator<B,V>::iterator(B* begin, B* end)
    : _bucket(begin-1)
    , _bucket_end(end)
    , _data(NULL)
    , _data_end(NULL)
    {
      next();
    }

    template <typename B, typename V>
    inline iterator<B,V>::iterator(B* end)
    : _bucket(end)
    , _bucket_end(end)
    , _data(NULL)
    , _data_end(NULL)
    {
    }

    template <typename B, typename V>
    inline iterator<B,V>::iterator(B* b, B* end, V* v, V* v_end)
    : _bucket(b)
    , _bucket_end(end)
    , _data(v)
    , _data_end(v_end)
    {
    }
    
    template <typename B, typename V>
    inline bool iterator<B,V>::operator==(iterator const& other) const
    {
      return _bucket == other._bucket && _data == other._data;
    }

    template <typename B, typename V>
    inline bool iterator<B,V>::operator!=(iterator const& other) const
    {
      return _bucket != other._bucket || _data != other._data;
    }
    
    template <typename B, typename V>
    void iterator<B,V>::next()
    {
      if (_data != _data_end) {
        ++_data;
        return;
      }
      while (_bucket != _bucket_end) {
        ++_bucket;
        if (_bucket->_used) {
          _data = _bucket->_data;
          _data_end = _bucket->_data + _bucket->_used;
          return;
        }
      }
    }

    template <typename K, typename V>
    struct helper
    {
      static K const& get_key(V const& v)
      {
        return v.first;
      }
    };

    template <typename K>
    struct helper<K,K>
    {
      static K const& get_key(K const& k)
      {
        return k;
      }
    };
  };

  template <typename K, typename V, typename H, typename E, typename A>
  array_hash_table<K,V,H,E,A>::array_hash_table(size_type min_capacity,
                                                double load_factor)
  : _load_factor(load_factor)
  , _capacity(power2_above(min_capacity, true))
  , _mask(_capacity - 1)
  , _size(0)
  , _resize(static_cast<size_type>(_capacity * _load_factor)+1)
  , _buckets(static_cast<bucket*>(calloc(_capacity, sizeof(bucket))))
  , _buckets_end(_buckets + _capacity)
  {
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  array_hash_table<K,V,H,E,A>::~array_hash_table()
  {
    clear();
    for(size_type ii = 0; ii != _capacity; ++ii) {
      if (_buckets[ii]._data)
        free(_buckets[ii]._data);
    }
    free(_buckets);
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  void array_hash_table<K,V,H,E,A>::clear()
  {
    for(bucket* b = _buckets; b != _buckets_end; ++b) {
      V* const v_end = b->_data + b->_used;
      for(V* v = b->_data; v != v_end; ++v) {
        v->~V();
      }
      b->_used = 0;
    }
    _size = 0;
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  typename array_hash_table<K,V,H,E,A>::iterator
  array_hash_table<K,V,H,E,A>::erase(const_iterator pos)
  {
    bucket& b = _buckets[hash(pos->first) & _mask];
    V* const v_last = b._data + (--b._used);
    V* const v = b._data + (&(*pos) - b._data);
    v->~V();
    if (v_last != v) {
      new(v) V(*v_last);
      v_last->~V();
    }
    --_size;
    return iterator(&b, _buckets_end, v, v_last);
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  typename array_hash_table<K,V,H,E,A>::size_type
  array_hash_table<K,V,H,E,A>::erase(K const& key)
  {
    bucket& b = _buckets[hash(key) & _mask];
    V* const v_end = b._data + b._used;
    for(V* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v)) {
        v->~V();
        V* const v_last = v_end - 1;
        if (v != v_last) {
          new(v) V(*v_last);
          v_last->~V();
        }
        --b._used;
        --_size;
        return 1;
      }
    }
    return 0;
  }

  template <typename K, typename V, typename H, typename E, typename A>
  std::pair<typename array_hash_table<K,V,H,E,A>::iterator,bool> 
  array_hash_table<K,V,H,E,A>::insert(K const& key, V const& value)
  {
    if (_size >= _resize)
      expand();
    bucket& b = _buckets[hash(key) & _mask];
    V* const v_end = b._data + b._used;
    for(V* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return std::make_pair(iterator(&b, _buckets_end, v, v_end), false);
    }
    if (b._capacity == b._used)
      expand(b);
    V* const new_v = b._data + b._used;
    new(new_v) V(value);
    ++b._used;
    ++_size;
    return std::make_pair(iterator(&b, _buckets_end, new_v, new_v), true);
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  void array_hash_table<K,V,H,E,A>::swap(array_hash_table& table)
  {
    std::swap(_capacity, table._capacity);
    std::swap(_mask, table._mask);
    std::swap(_size, table._size);
    std::swap(_resize, table._resize);
    std::swap(_buckets, table._buckets);
    std::swap(_buckets_end, table._buckets_end);
  }

  template <typename K, typename V, typename H, typename E, typename A>
  array_hash_table<K,V,H,E,A>& 
  array_hash_table<K,V,H,E,A>::operator=(array_hash_table const& table)
  {
    if (this != &table) {
      clear();
      for(const_iterator iter = table.cbegin(); iter != table.cend(); ++iter)
        insert(internal_aht::helper<K,V>::get_key(*iter), *iter);
    }
    return *this;
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  inline V& array_hash_table<K,V,H,E,A>::at(K const& key)
  {
    bucket& b = _buckets[hash(key) & _mask];
    V* const v_end = b._data + b._used;
    for(V* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return *v;
    }
    throw std::out_of_range("invalid key");
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  inline V const& array_hash_table<K,V,H,E,A>::at(K const& key) const
  {
    bucket const& b = _buckets[hash(key) & _mask];
    V const* const v_end = b._data + b._used;
    for(V const* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return *v;
    }
    throw std::out_of_range("invalid key");
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  inline typename array_hash_table<K,V,H,E,A>::size_type
  array_hash_table<K,V,H,E,A>::count(K const& key) const
  {
    bucket const& b = _buckets[hash(key) & _mask];
    V const* const v_end = b._data + b._used;
    for(V const* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return 1;
    }
    return 0;
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  inline typename array_hash_table<K,V,H,E,A>::const_iterator 
  array_hash_table<K,V,H,E,A>::find(K const& key) const
  {
    bucket& b = _buckets[hash(key) & _mask];
    V* const v_end = b._data + b._used;
    for(V* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return const_iterator(&b, _buckets_end, v, v_end);
    }
    return cend();
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  inline typename array_hash_table<K,V,H,E,A>::iterator 
  array_hash_table<K,V,H,E,A>::find(K const& key)
  {
    bucket& b = _buckets[hash(key) & _mask];
    V* const v_end = b._data + b._used;
    for(V* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return iterator(&b, _buckets_end, v, v_end);
    }
    return end();
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  inline V& array_hash_table<K,V,H,E,A>::operator[](K const& key)
  {
    if (_size >= _resize)
      expand();
    bucket& b = _buckets[hash(key) & _mask];
    V* const v_end = b._data + b._used;
    for(V* v = b._data; v != v_end; ++v) {
      if (_equator(key, *v))
        return *v;
    }
    if (b._capacity == b._used)
      expand(b);
    V* const new_v = b._data + b._used;
    new(new_v) V(key, typename V::second_type());
    ++b._used;
    ++_size;
    return *new_v;
  }

  template <typename K, typename V, typename H, typename E, typename A>
  inline std::size_t array_hash_table<K,V,H,E,A>::hash(K const& key) const
  {
    return static_cast<size_type>(_hasher(key));
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  void array_hash_table<K,V,H,E,A>::expand(bucket& b)
  {
    size_type const new_capacity = std::max<size_type>(b._capacity << 1, 2);
    V* const new_data = static_cast<V*>(malloc(new_capacity * sizeof(V)));
    V* const old_data_end = b._data + b._used;
    for(V* v = b._data; v != old_data_end; ++v) {
      new(new_data + (v - b._data)) V(*v);
      v->~V();
    }
    if (b._data)
      free(b._data);
    b._capacity = new_capacity;
    b._data = new_data;
  }
  
  template <typename K, typename V, typename H, typename E, typename A>
  void array_hash_table<K,V,H,E,A>::expand()
  {
    bucket* const old_buckets = _buckets;
    bucket* const old_buckets_end = _buckets_end;
    _capacity <<= 1;
    _mask = _capacity - 1;
    _size = 0;
    _resize = static_cast<size_type>(_capacity * _load_factor) + 1;
    _buckets = static_cast<bucket*>(calloc(_capacity, sizeof(bucket)));
    _buckets_end = _buckets + _capacity;
    for(bucket* b = old_buckets; b != old_buckets_end; ++b) {
      V* const v_end = b->_data + b->_used;
      for(V* v = b->_data; v != v_end; ++v) {
        insert(internal_aht::helper<K,V>::get_key(*v), *v);
        v->~V();
      }
      free(b->_data);
    }
    free(old_buckets);
  }
}

