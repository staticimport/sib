//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <limits>


template <typename T, typename A>
inline sib::xor_list<T,A>::xor_list()
: _size(0)
{
#ifdef USE_TAIL_SENTINEL
  _tail_sentinel = _alloc.allocate(1);
  _tail_sentinel->_link_xor = 0;
  _head = _tail_sentinel;
#else
  _head = _tail = NULL;
#endif
}

template <typename T, typename A>
inline sib::xor_list<T,A>::~xor_list()
{
  clear();
}

template <typename T, typename A>
inline typename sib::xor_list<T,A>::size_type
sib::xor_list<T,A>::max_size() const
{
  return std::numeric_limits<T>::max() / sizeof(T);
}

template <typename T, typename A>
inline void sib::xor_list<T,A>::clear()
{
  node* prev = NULL;
  node* n = _head;
#if USE_TAIL_SENTINEL
  while (n != _tail_sentinel) {
#else
  while (n) {
#endif
    node* const next = n->next(prev);
    n->~node();
    _alloc.deallocate(n, 1);
    prev = n;
    n = next;
  }
#if USE_TAIL_SENTINEL
  _head = _tail_sentinel;
#else
  _head = _tail = NULL;
#endif
}

/*template <typename T, typename A>
inline typename sib::xor_list<T,A>::iterator
sib::xor_list<T,A>::erase(iterator pos)
{
  node* const n = *pos;
  node* const prev = pos - 1;
  node* const next = pos + 1;
  next->update_link(prev, next->next(n));
  if (prev)
    prev->update_link(prev->previous(n), next);
  else
    _head = next;
  n->~node();
  _alloc.deallocate(n, 1);
  --_size;
}*/

template <typename T, typename A>
inline void sib::xor_list<T,A>::pop_back()
{
#if USE_TAIL_SENTINEL
  node* const n = _tail_sentinel->previous(NULL);
  node* const prev = n->previous(_tail_sentinel);
  _tail_sentinel->update_link(prev, NULL);
  if (prev)
    prev->update_link(prev->previous(n), _tail_sentinel);
  else
    _head = _tail_sentinel;
  n->~node();
  _alloc.deallocate(n, 1);
  --_size;
#endif
}

template <typename T, typename A>
inline void sib::xor_list<T,A>::pop_front()
{
  node* const n = _head;
  _head = n->next(NULL);
  _head->update_link(NULL, _head->next(n));
  n->~node();
  _alloc.deallocate(n, 1);
  --_size;
}

template <typename T, typename A>
inline void sib::xor_list<T,A>::push_back(typename param<T>::type item)
{
#ifdef USE_TAIL_SENTINEL
  node* const prev = _tail_sentinel->previous(NULL);
  node* const n = new(_alloc.allocate(1,prev)) node(item);
  n->update_link(prev, _tail_sentinel);
  _tail_sentinel->update_link(n, NULL);
  if (prev)
    prev->update_link(prev->previous(_tail_sentinel), n);
  else
    _head = n;
  ++_size;
#else
  node* const n = new(_alloc.allocate(1,_tail)) node(item);
  if (SIB_LIKELY(0 != _tail)) {
    n->update_link(_tail, NULL);
    _tail->update_link(_tail->previous(NULL), n);
    _tail = n;
    if (SIB_UNLIKELY(_tail == _head))
      _head = n;
  } else {
    n->_link_xor = 0;
    _head = _tail = n;
  }
  ++_size;
#endif
}

template <typename T, typename A>
inline void sib::xor_list<T,A>::push_front(typename param<T>::type item)
{
  node* const n = new(_alloc.allocate(1,_head)) node(item);
  n->update_link(NULL, _head);
  _head->update_link(n, _head->next(NULL));
  _head = n;
  ++_size;
}

template <typename T, typename A>
template <typename OutStream>
void sib::xor_list<T,A>::print(OutStream& out) const
{
  out << '[';
  node* prev = NULL;
  node* n = _head;
  bool first(true);
#ifdef USE_TAIL_SENTINEL
  while (n != _tail_sentinel) {
#else
  while (n) {
#endif
    if (!first)
      out << ',';
    else
      first = false;
    out << n->_item;
    node* next = n->next(prev);
    prev = n;
    n = next;
  }
  out << ']';
}

template <typename T, typename A>
inline typename sib::xor_list<T,A>::node*
sib::xor_list<T,A>::node::next(node* const prev)
{
  //return reinterpret_cast<node*>(static_cast<difference_type>(prev) ^ _link_xor);
  return (node*)(((difference_type)prev) ^ _link_xor);
}

template <typename T, typename A>
inline typename sib::xor_list<T,A>::node*
sib::xor_list<T,A>::node::previous(node* const next)
{
  //return static_cast<node*>(_link_xor ^ static_cast<difference_type>(next));
  return (node*)(_link_xor ^ ((difference_type)next));
}

template <typename T, typename A>
inline void 
sib::xor_list<T,A>::node::update_link(node* const prev, node* const next)
{
  //_link_xor = static_cast<difference_type>(prev) ^ static_cast<difference_type>(next);
  _link_xor = ((difference_type)prev) ^ ((difference_type)next);
}
