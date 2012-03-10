
#include <iostream>
#include <set>
#include <vector>

namespace sib
{
  namespace sib_internal {
    template <typename K>
    static int comp_vec_ptr_max_size(const void* a, const void* b)
    {
      std::vector<K> const* v1 = static_cast<std::vector<K> const*>(a);
      std::vector<K> const* v2 = static_cast<std::vector<K> const*>(b);
      return v2->size() - v1->size();
    }

    template <typename K>
    struct ptr_max_size_compare
    {
      bool operator()(K const* a, K const* b)
      {
        return a->size() > b->size();
      }
    };
  };

#define SIB_PERFECT_HASH_INVALID_SEED 0x7FFFFFFF
  template <typename K, typename V, typename H>
  template <typename input_iterator>
  perfect_hash_map<K,V,H>::perfect_hash_map(input_iterator begin,
                                            input_iterator end)
  {
    // Get vector (and number) of possible keys
    std::vector<K> keys;
    while (begin != end) {
      keys.push_back(*(begin++));
    }
    std::size_t const count(keys.size());

    std::vector<std::vector<K*>*> buckets;
    for(std::size_t ii = 0; ii != count; ++ii) {
      buckets.push_back(new std::vector<K*>());
    }
    for(typename std::vector<K>::iterator iter = keys.begin(); 
        iter != keys.end(); 
        ++iter)
    {
      std::size_t index = _seed_hash(*iter,0) % count;
      std::vector<K*>* bucket = buckets[index];
      K& key = *iter;
      bucket->push_back(&key);
    }
    //std::qsort(buckets.data(), count, sizeof(void*),
    //           &sib_internal::comp_vec_ptr_max_size<K>);
    std::sort(buckets.begin(), buckets.end(), sib_internal::ptr_max_size_compare<std::vector<K*> >());
    _seeds = new int[count];
    _begin = new std::pair<K,V>[count];
    bool* used_entries = new bool[count];
    for(std::size_t ii = 0; ii != count; ++ii) {
      _seeds[ii] = SIB_PERFECT_HASH_INVALID_SEED;
      used_entries[ii] = false;
    }
    for(std::size_t bucket_index = 0; bucket_index != buckets.size(); ++bucket_index)
    {
      std::vector<K*>* bucket = buckets[bucket_index];
      //std::cout << "bucket_index=" << bucket_index << ", count=" 
      //  << bucket->size() << std::endl;
      if (bucket->empty()) continue;
      std::size_t index = 0;
      int seed = 1;
      std::set<std::size_t> used_new_slots;
      while (index != bucket->size()) {
        std::size_t slot = _seed_hash(*((*bucket)[index]),seed) % count;
        //if (_seeds[slot] != SIB_PERFECT_HASH_INVALID_SEED or
        //    used_new_slots.count(slot) != 0)
        if (used_entries[slot] or used_new_slots.count(slot) != 0)
        {
          index = 0;
          used_new_slots.clear();
          ++seed;
        } else {
          used_new_slots.insert(slot);
          ++index;
        }
      }
      std::size_t seed_index = _seed_hash(*((*bucket)[0]),0) % count;
      _seeds[seed_index] = seed;
      //std::cout << "seeds[" << seed_index << "] = " << seed << std::endl;
      for(std::size_t jj = 0; jj != bucket->size(); ++jj) {
        K const& key = *((*bucket)[jj]);
        std::size_t entry_index = _seed_hash(key,seed) % count;
        //std::cout << key << " -> " << entry_index << " (with seed="
        //  << seed << ")\n";
        _begin[entry_index].first = key;
        used_entries[entry_index] = true;
      }
    }
    _key_count = count;
  }
  
  template <typename K, typename V, typename H>
  perfect_hash_map<K,V,H>::perfect_hash_map(perfect_hash_map<K,V,H> const& map)
  {
    _key_count = map._key_count;
    _seeds = new int[_key_count];
    _begin = new std::pair<K,V>[_key_count];
    for(std::size_t ii = 0; ii != _key_count; ++ii) {
      _seeds[ii] = map._seeds[ii];
      _begin[ii] = map._begin[ii];
    }
  }

  template <typename K, typename V, typename H>
  perfect_hash_map<K,V,H>::~perfect_hash_map()
  {
    delete [] _seeds;
    delete [] _begin;
  }
  
  template <typename K, typename V, typename H>
  inline V const& perfect_hash_map<K,V,H>::operator[](K const& key) const
  {
    std::size_t const seed_index = _seed_hash(key,0) % _key_count;
    return _begin[_seed_hash(key,_seeds[seed_index]) % _key_count].second; 
  }
 
  template <typename K, typename V, typename H>
  inline std::pair<typename perfect_hash_map<K,V,H>::iterator,bool>
  perfect_hash_map<K,V,H>::insert(perfect_hash_map<K,V,H>::value_type const& x)
  {
    std::size_t const seed_index = _seed_hash(x.first,0) % _key_count;
    std::pair<K,V>* entry = _begin + (_seed_hash(x.first, _seeds[seed_index]));
    entry->second = x.second;
    return std::pair<iterator,bool>(entry,true);
  }
  
  template <typename K, typename V, typename H>
  inline V& perfect_hash_map<K,V,H>::operator[](K const& key)
  {
    std::size_t const seed_index = _seed_hash(key,0) % _key_count;
    return _begin[_seed_hash(key,_seeds[seed_index]) % _key_count].second; 
  }

}
#undef SIB_PERFECT_HASH_INVALID_SEED

