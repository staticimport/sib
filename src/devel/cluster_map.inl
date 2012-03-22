
namespace sib
{
  template <typename K, typename T, typename C, typename E>
  inline cluster_map<K,T,C,E>::cluster_map(typename param<K>::type max_cluster_gap)
  : _max_cluster_gap(max_cluster_gap)
  , _size(0)
  {
  }

  template <typename K, typename T, typename C, typename E>
  cluster_map<K,T,C,E>::~cluster_map()
  {
  }

  template <typename K, typename T, typename C, typename E>
  inline T& cluster_map<K,T,C,E>::operator[](typename param<K>::type key)
  {
    // Find cluster
    std::size_t dist = std::numeric_limits<std::size_t>::max();
    cluster* c(NULL);
    for(std::vector<cluster>::iterator iter = _clusters.begin;
        iter != _clusters.end();
        ++iter)
    {
      std::size_t const d = iter->distance(key);
      if (d < _max_cluster_gap && dist < d) {
        dist = d;
        c = &(*iter);
        if (dist == 0) break;
      }
    }
    if (c)
      return c[key];
    else {
      _clusters.resize(_clusters.size()+1);
      return _clusters[_clusters.size()-1].initialize(key);
    }
  }

  template <typename K, typename T, typename C, typename E>
  inline cluster_map<K,T,C,E>::cluster::cluster()
  : _first(NULL)
  , _last(NULL)
  , _entries(new std::vector<entry>())
  {
  }

  template <typename K, typename T, typename C, typename E>
  inline T& cluster_map<K,T,C,E>::initialize(typename param<K>::type key)
  {
    _entries->resize(1);
    entry& e = (*_entries)[0];
    e._key = key;
    return e._value;
  }

  template <typename K, typename T, typename C, typename E>
  inline T& cluster_map<K,T,C,E>::operator[](typename param<K>::type key)
  {
    std::size_t const index = static_cast<std::size_t>(key - (*_entries)[0]._key);
    if (index >= _entries->size())
      _entries->resize(index+1);
    return (*_entries)[index];
  }

  template <typename K, typename T, typename C, typename E>
  inline std::size_t cluster_map<K,T,C,E>::distance(typename param<K>::type key)
  {

  }
  template <typename K, typename T, typename C, typename E>
  cluster_map<K,T,C,E>::
  template <typename K, typename T, typename C, typename E>
  cluster_map<K,T,C,E>::
  template <typename K, typename T, typename C, typename E>
  cluster_map<K,T,C,E>::
}

