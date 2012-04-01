
template <typename K, typename T, typename E>
inline T& sib::vector_map<K,T,E>::at(K const& key)
{
  iterator iter = find(key);
  if (iter != end())
    return iter->second;
 else
    throw std::runtime_error("vector_map key not found");
}

template <typename K, typename T, typename E>
inline T const& sib::vector_map<K,T,E>::at(K const& key) const
{
  const_iterator iter = find(key);
  if (iter != end())
    return iter->second;
 else
    throw std::runtime_error("vector_map key not found");
}

template <typename K, typename T, typename E>
inline T& sib::vector_map<K,T,E>::operator[](K const& key)
{
  iterator iter = find(key);
  if (iter != end())
    return iter->second;
  else {
    _table.push_back(std::make_pair(key,T()));
    return _table.back().second;
  }
}

template <typename K, typename T, typename E>
inline std::pair<typename sib::vector_map<K,T,E>::iterator,bool>
sib::vector_map<K,T,E>::insert(value_type const& x)
{
  return _table.insert(x.first, x);
}

template <typename K, typename T, typename E>
inline sib::vector_map<K,T,E>&
sib::vector_map<K,T,E>::operator=(vector_map const& map)
{
  _table = map._table;
  return *this;
}



