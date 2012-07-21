
namespace sib
{
  namespace unrolled_hash_table_internal
  {
    /*template <typename K, typename V, typename E, unsigned TotalDepth, unsigned DepthLeft>
    struct entry_finder
    {
      typedef boost::call_traits<K>::param_type key_param_type;

      static inline entry<V>* 
      find_matching_entry(bucket<V,TotalDepth>* bucket, entry<V>* entry, key_param_type key,
                          E const& equals)
      {
        return equals(key, entry->_value) ? entry : 
          entry_finder<K,V,E,TotalDepth,DepthLeft-1>::find_matching_entry(
              bucket, entry+1, key, equals);
      }

      static inline entry<V>*
      find_empty_entry(bucket<V,TotalDepth>* bucket, entry<V>* entry)
      {
        return (entry->_hash == 0) ? entry :
          entry_finder<K,V,E,TotalDepth,DepthLeft-1>::find_empty_entry(bucket, entry+1);
      }
    };

    template <typename K, typename V, typename E, unsigned TotalDepth>
    struct entry_finder<K,V,E,TotalDepth,0>
    {
      static inline entry<V>* 
      find_matching_entry(bucket<V,TotalDepth>* bucket, entry<V>*, key_param_type key, 
                          E const& equals)
      {
        return (bucket->_next == NULL) ? NULL :
          entry_finder<K,V,E,TotalDepth,TotalDepth>::find_matching_entry(
            bucket->_next, bucket->_next->_entries, key, equals);
      }

      static inline entry<V>*
      find_empty_entry(bucket<V,TotalDepth>* bucket, entry<V>*)
      {
        return (bucket->_next == NULL) ? NULL :
          entry_finder<K,V,E,TotalDepth,TotalDepth>::find_empty_entry(
            bucket->_next, bucket->_next->_entries);
      }
    }*/

    template <typename K, typename V>
    struct entry<K,V,true>
    {
      template <typename H>
      inline hash_type get_hash(H const& hasher) const
      {
        return _hash;
      }

      template <typename E>
      inline bool equals(boost::call_traits<K>::param_type key, hash_type const hash,
                         E const& equator)
      {
        return _hash == hash && equator(key, _value);
      }

      hash_type _hash;
      V _value;
    };

    template <typename K, typename V>
    struct entry<K,V,false>
    {
      template <typename H>
      inline hash_type get_hash(H const& hasher) const
      {
        return hasher(_value);
      }

      template <typename E>
      inline bool equals(boost::call_traits<K>::param_type key, hash_type const hash,
                         E const& equator)
      {
        return equator(key, _value);
      }

      V _value;
    };
  };

  template <typename K, typename V, unsigned D, typename H, typename E, typename A>
  unrolled_hash_table<K,V,D,H,E,A>::unrolled_hash_table(size_type min_init_capacity,
                                                        double load_factor)
    : _load_factor(std::max(load_factor,0.1))
    , _capacity(power2_above(min_init_capacity, true))
    , _mask(_capacity-1)
    , _size(0)
    , _resize(1 + static_cast<size_type>(_load_factor * _capacity * D))
    , _buckets(NULL)
    , _buckets_end(NULL)
  {
  }

  template <typename K, typename V, unsigned D, typename H, typename E, typename A>
  unrolled_hash_table<K,V,D,H,E,A>::~unrolled_hash_table()
  {
    clear();
    if (_buckets)
      free(_buckets);
  }

  template <typename K, typename V, unsigned D, typename H, typename E, typename A>
  void unrolled_hash_table<K,V,D,H,E,A>::clear()
  {
    // TODO
  }

  template <typename K, typename V, unsigned D, typename H, typename E, typename A>
  void unrolled_hash_table<K,V,D,H,E,A>::insert(key_param_type key,
                                                value_param_type value)
  {
    hash_type const hash = get_hash(key);
    bucket_type* bucket = _buckets + (hash & _mask);
    entry_type* empty_entry = NULL;
    do
    {
      entry_type* const existing_entry = find_matching_entry<V,E,D>(*bucket, key);
      if (existing_entry) {
        return; // TODO: return (iterator,false)
      } else if (NULL == empty_entry)
        empty_entry = find_empty_entry<V,E,D>(*bucket);
      bucket = bucket->_next;
    } while (bucket);
    if (NULL == empty_entry)

  }

  //template <typename K, typename V, unsigned D, typename H, typename E, typename A>
  //unrolled_hash_table<K,V,D,H,E,A>::
  //template <typename K, typename V, unsigned D, typename H, typename E, typename A>
  //unrolled_hash_table<K,V,D,H,E,A>::

}
