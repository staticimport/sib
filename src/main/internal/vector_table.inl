
#include <algorithm>

template <typename K, typename V, typename E>
template <typename I>
inline sib::vector_table<K,V,E>::vector_table(I begin, I end)
{
  for(I iter = begin; iter != end; ++iter) {
    insert(*iter);
  }
}

/*template <typename K, typename V, typename E>
inline typename sib::vector_table<K,V,E>::reference
sib::vector_table<K,V,E>::operator[](K const& key)
{
  iterator iter = find(key);
  if (iter != end())
    return *iter;
  else {
    _vec.resize(_vec.size()+1);
    iterator iter = end()-1;
    iter->
    return *(end()-1);
  }
}*/

template <typename K, typename V, typename E>
inline std::pair<typename sib::vector_table<K,V,E>::iterator,bool> 
sib::vector_table<K,V,E>::insert(K const& key, V const& value)
{
  iterator iter = find(key);
  if (iter == end()) {
    _vec.push_back(value);
    return std::pair<iterator,bool>(end()-1,true);
  } else {
    return std::pair<iterator,bool>(iter,false);
  }
}

/*template <typename K, typename V, typename E>
template <typename InputIterator>
inline void 
sib::vector_table<K,V,E>::insert(InputIterator begin, InputIterator end)
{
  for(InputIterator iter = begin; iter != end; ++iter) {
    insert(*iter);
  }
}*/

//template <typename T, typename A>
//void insert(std::initializer_list<value_type> ilist);

//template <typename T, typename A>
//template <class Args...>
//std::pair<iterator,bool> emplace(Args&&... args);

template <typename K, typename V, typename E>
inline void
sib::vector_table<K,V,E>::erase(sib::vector_table<K,V,E>::const_iterator position)
{
  const_iterator last = end() - 1;
  if (position < last)
    *(begin() + std::distance(begin(), position)) = *last;
  else if (position != last)
    throw std::runtime_error("attempted to erase invalid iterator position");
  _vec.pop_back();
}

template <typename K, typename V, typename E>
inline typename sib::vector_table<K,V,E>::size_type 
sib::vector_table<K,V,E>::erase(K const& key)
{
  iterator iter = find(key);
  if (iter != end()) {
    erase(iter);
    return 1;
  } else
    return 0;
}

template <typename K, typename V, typename E>
sib::vector_table<K,V,E>& 
sib::vector_table<K,V,E>::operator=(sib::vector_table<K,V,E> const& other)
{
  _vec = other._vec;
  return *this;
}

template <typename K, typename V, typename E>
inline typename sib::vector_table<K,V,E>::size_type 
sib::vector_table<K,V,E>::count(K const& key) const
{
  return find(key) == end() ? 0 : 1;
}

template <typename K, typename V, typename E>
inline typename sib::vector_table<K,V,E>::iterator
sib::vector_table<K,V,E>::find(K const& key)
{
  for(iterator i = begin(); i != end(); ++i) {
    if (_equal(key, *i))
      return i;
  }
  return end();
}

template <typename K, typename V, typename E>
inline typename sib::vector_table<K,V,E>::const_iterator
sib::vector_table<K,V,E>::find(K const& key) const
{
  for(const_iterator i = begin(); i != end(); ++i) {
    if (_equal(key, *i))
      return i;
  }
  return end();
}
