//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

template <typename T>
bool sib::vector_range_set<T>::contains(T const x) const
{
  for(typename std::vector<std::pair<T,T> >::const_iterator iter = _ranges.begin(); iter != _ranges.end(); ++iter)
  {
    if (iter->first <= x && x <= iter->second)
      return true;
  }
  return false;
}

template <typename T>
void sib::vector_range_set<T>::erase(T const x)
{
  for(typename std::vector<std::pair<T,T> >::iterator iter = _ranges.begin(); iter != _ranges.end(); ++iter)
  {
    std::pair<T,T>& range = *iter;
    if (range.first <= x && x <= range.second)
    {
      if (range.first == x)
        ++range.first;
      else if (range.second == x)
        --range.second;
      else {
        T const old_second = range.second;
        range.second = x-1;
        _ranges.push_back(std::make_pair(x+1, old_second));
        return;
      }
      if (range.first > range.second) {
        std::pair<T,T>& last = *(_ranges.end()-1);
        if (&range != &last)
          range = last;
        _ranges.pop_back();
      }
      return;
    }
  }
}

template <typename T>
void sib::vector_range_set<T>::insert(T begin, T end)
{
  std::size_t index = 0;
  while (index < _ranges.size())
  {
    std::pair<T,T>& range = _ranges[index];
    if (range.first <= begin && begin <= range.second) {
      begin = range.first;
      end = std::max(range.second, end);
      remove(range);
    } else if (range.first <= end && end <= range.second) {
      begin = std::min(range.first, begin);
      end = range.second;
      remove(range);
    } else if (begin < range.first && range.second < end) {
      remove(range);
    } else
      ++index;
  }
  _ranges.push_back(std::make_pair(begin, end));
}

