//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

template <typename T, typename E>
inline std::pair<typename sib::vector_set<T,E>::iterator,bool>
sib::vector_set<T,E>::insert(value_type const& x)
{
  return _table.insert(x,x);
}

template <typename T, typename E>
inline sib::vector_set<T,E>&
sib::vector_set<T,E>::operator=(vector_set const& set)
{
  _table = set._table;
  return *this;
}
