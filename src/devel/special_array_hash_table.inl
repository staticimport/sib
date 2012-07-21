//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <cstring>

#include "bits.hpp"

namespace sib
{
  namespace internal_saht {
    template <typename B, typename E>
    inline iterator<B,E>::iterator(B* begin, B* end)
    : _bucket(begin)
    , _bucket_end(end)
    , _entry(NULL)
    , _entry_end(NULL)
    , _overflow_entry(true)
    {
      next();
    }

    template <typename B, typename E>
    inline iterator<B,E>::iterator(B* end)
    ; _bucket(end)
    , _bucket_end(end)
    , _entry(NULL)
    , _entry_end(NULL)
    , _overflow_entry(true)
    {
    }

    template <typename B, typename E>
    inline iterator<B,E>::iterator(B* b, B* b_end, E* e, E* e_end, bool overflow)
    : _bucket(b)
    , _bucket_end(b_end)
    , _entry(e)
    , _entry_end(e_end)
    , _overflow_entry(overflow)
    {
    }

    template <typename B, typename E>
    void iterator<B,E>::next()
    {
      if (_entry != _entry_end) {
        ++_entry;
      } else if (!_overflow_entry && _bucket->_overflow && _bucket->_used > B::Depth) {
        _entry = _bucket->_overflow;
        _entry_end = _bucket->_overflow + B::Depth - _bucket->_used;
        _overflow_entry = true;
      } else {
        while(++_bucket != _bucket_end && !_bucket->_used) ;
        if (_bucket != _bucket_end) {
          _entry = _bucket->_local;
          _entry_end <D-P>= _entry + std::min(_bucket->_used, B::Depth);
        } else 
          _entry = _entry_end = NULL;
        _overflow_entry = false;
      }
    }

    template <typename K, typename B, typename Equals, unsigned D>
    struct local
    {
      static inline B* check(B* bs, K& k, Equals& e)
      {
        return e(k,*bs) ? bs : foo<K,B,Equals,D-1>::check(bs+1,k,e);
      }
    };

    template <typename K, typename B, typename Equals>
    struct foo<K,B,Equals,0>
    {
      static inline B* check(B* bs, K& k, Equals& e) { return NULL; }
    };
  };

  template <typename K, typename V, unsigned D, typename H, typename E>
  array_hash_table<K,V,D,H,E>::array_hash_table(size_type min_capacity,
                                                double load_factor)
  : _load_factor(load_factor)
  , _capacity(power2_above(min_capacity, true))
  , _mask(_capacity - 1)
  , _size(0)
  , _resize(static_cast<size_type>(_capacity * _load_factor)+1)
  , _buckets(static_cast<bucket_type*>(calloc(_capacity, sizeof(bucket_type))))
  , _buckets_end(_buckets + _capacity)
  {
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  array_hash_table::array_hash_table(array_hash_table const& table)
  : _load_factor(table._load_factor)
  , _capacity(power2_above(static_cast<size_type>(table.size() / _load_factor), false))
  , _mask(_capacity - 1)
  , _size(0)
  , _resize(table.size())
  , _buckets(static_cast<bucket_type*>(calloc(_capacity, sizeof(bucket_type))))
  , _buckets_end(_buckets + _capacity)
  {
    for(const_iterator iter = table.begin(); iter != table.end(); ++iter)
      insert(helper<K,V>::get_key(*iter), *iter);
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  array_hash_table<K,V,D,H,E>::~array_hash_table()
  {
    clear();
    for(size_type ii = 0; ii != _capacity; ++ii) {
      if (_buckets[ii]._overflow)
        free(_buckets[ii]._overflow);
    }
    free(_buckets);
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  void array_hash_table::clear()
  {
    for(bucket_type* b = _buckets; b != _buckets_end; ++b) {

    }
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  V& special_array_hash_table<K,V,D,H,E>::operator[](K const& key)
  {
    if (_size >= _resize)
      expand();
    //std::cout << key << " -> index: " << (hash(key) & _mask) << std::endl;
    bucket& b = _buckets[hash(key) & _mask];
    /*V* local_end = b._local + b._used;
    V* overflow_end = b._overflow;
    if (b._used > D) {
      local_end = b._local + D;
      overflow_end = b._overflow + (b._used - D);
    }*/
    V* const local_end = b._local + std::min(b._used, D);
    for(V* v = b._local; v != local_end; ++v) {
      if (_equator(key, *v))
        return *v;
    }
    V* const overflow_end = b._overflow + std::max(b._used, D) - D;
    for(V* v = b._overflow; v != overflow_end; ++v) {
      if (_equator(key, *v))
        return *v;
    }
    if (b._capacity == b._used)
      expand(b);
    V* const new_v = b._used < D ? b._local + b._used : b._overflow + (b._used - D);
    new(new_v) V(key, typename V::second_type());
    ++b._used;
    ++_size;
    return *new_v;
  } 
 
  /*union match {
    uint64_t aggregate;
    struct {
      bool b1;
      bool b2;
    } indiv;
  };*/

  template <typename K, typename V, unsigned D, typename H, typename E>
  V const* special_array_hash_table<K,V,D,H,E>::find(K const& key) const
  {
    bucket const& b = _buckets[hash(key) & _mask];
    /*V* local_end = b._local + b._used;
    V* overflow_end = b._overflow;
    if (b._used > D) {
      local_end = b._local + D;
      overflow_end = b._overflow + (b._used - D);
    }*/
    /*V const* const local_end = b._local + b._used;//std::min(b._used, D);
    for(V const* v = b._local; v != local_end; ++v) {
      if (_equator(key, *v))
        return v;
    }*/
    //bool const b1 = _equator(key, b._local[0]);
    //bool const b2 = _equator(key, b._local[1]);
    //bool const b3 = _equator(key, b._local[2]);
    /*if (b1)
      return b._local;
    else if (b2)
      return b._local + 1;*/
    //return b1 ? b._local : (b2 ? &(b._local[1]) : (b3 ? &(b._local[2]) : NULL));
    /*if (b._used == 1) {
      return _equator(key, *b._local) ? b._local : NULL;
    }*/
    /*V const* const overflow_end = b._overflow + std::max(b._used, D) - D;
    for(V const* v = b._overflow; v != overflow_end; ++v) {
      if (_equator(key, *v))
        return v;
    }
    return NULL;*/

    V const* const result = internal_saht::foo<D,K const,V const,E const>::check(b._local, key, _equator);
    return result ? result : find_in_overflow(b,key);
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  inline std::size_t special_array_hash_table<K,V,D,H,E>::hash(K const& key) const
  {
    return static_cast<size_type>(_hasher(key));
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  void special_array_hash_table<K,V,D,H,E>::expand(bucket& b)
  {
    std::cout << "expanding bucket!\n";
    //sleep(1);
    size_type const new_overflow_capacity = (b._capacity << 1) - D;
    V* const new_overflow = static_cast<V*>(malloc(new_overflow_capacity * sizeof(V)));
    V* const old_overflow_end = b._overflow + (b._used - D);
    //std::cout << "overflow=" << b._overflow << ", used=" << b._used << ", D=" << D << ", end=" << old_overflow_end << std::endl;
    for(V* v = b._overflow; v != old_overflow_end; ++v) {
      //std::cout << v << " to " << old_overflow_end << std::endl;
      //sleep(10);
      new(new_overflow + (v - b._overflow)) V(*v);
      v->~V();
    }
    if (b._overflow)
      free(b._overflow);
    b._capacity = new_overflow_capacity + D;
    b._overflow = new_overflow;
  }
  
  template <typename K, typename V, unsigned D, typename H, typename E>
  void special_array_hash_table<K,V,D,H,E>::expand()
  {
    //std::cout << "expanding!\n";
    //sleep(1);
    bucket* const old_buckets = _buckets;
    bucket* const old_buckets_end = _buckets_end;
    _capacity <<= 1;
    _mask = _capacity - 1;
    _size = 0;
    _resize = static_cast<size_type>(_capacity * _load_factor) + 1;
    _buckets = static_cast<bucket*>(calloc(_capacity, sizeof(bucket)));
    _buckets_end = _buckets + _capacity;
    for(bucket* b = _buckets; b != _buckets_end; ++b)
      b->_capacity = D;
    for(bucket* b = old_buckets; b != old_buckets_end; ++b) {
      V* local_end = b->_local + b->_used;
      V* overflow_end = b->_overflow;
      if (b->_used > D) {
        local_end = b->_local + D;
        overflow_end = b->_overflow + (b->_used - D);
      }
      for(V* v = b->_local; v != local_end; ++v) {
        operator[](internal_saht::helper<K,V>::get_key(*v)).second = v->second;
        v->~V();
      }
      for(V* v = b->_overflow; v != overflow_end; ++v) {
        operator[](internal_saht::helper<K,V>::get_key(*v)).second = v->second;
        v->~V();
      }
      if (b->_overflow)
        free(b->_overflow);
    }
    free(old_buckets);
    //std::cout << "expand complete!\n";
  }
    
  template <typename K, typename V, unsigned D, typename H, typename E>
  V const* 
  special_array_hash_table<K,V,D,H,E>::find_in_overflow(bucket const& b, K const& key) const
  {
    V const* const overflow_end = b._overflow + std::max(b._used, D) - D;
    for(V const* v = b._overflow; v != overflow_end; ++v) {
      if (_equator(key, *v))
        return v;
    }
    return NULL;
  }
};
