//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

template <typename T>
inline typename sib::ns_ring::iterator<T>
operator+(long x, sib::ns_ring::iterator<T> const& i)
{
  return i + x;
}

template <typename T>
inline sib::ns_ring::iterator<T>::iterator(
    T* array, std::size_t mask, std::size_t begin, std::size_t end)
: _array(array), _mask(mask), _index(begin), _end(end)
{
}

template <typename T>
inline sib::ns_ring::iterator<T>&
sib::ns_ring::iterator<T>::operator+=(long x)
{
  _index += x;
  return *this;
}

template <typename T>
inline sib::ns_ring::iterator<T>&
sib::ns_ring::iterator<T>::operator-=(long x)
{
  _index -= x;
  return *this;
}

template <typename T>
inline sib::ns_ring::iterator<T>&
sib::ns_ring::iterator<T>::operator++()
{
  ++_index;
  return *this;
}

template <typename T>
inline sib::ns_ring::iterator<T>
sib::ns_ring::iterator<T>::operator++(int)
{
  iterator<T> iter(*this);
  ++_index;
  return iter;
}

template <typename T>
inline sib::ns_ring::iterator<T>&
sib::ns_ring::iterator<T>::operator--()
{
  --_index;
  return *this;
}

template <typename T>
inline sib::ns_ring::iterator<T>
sib::ns_ring::iterator<T>::operator--(int)
{
  iterator<T> iter(*this);
  --_index;
  return iter;
}

template <typename T>
inline sib::ns_ring::iterator<T>
sib::ns_ring::iterator<T>::operator+(long x) const
{
  return iterator(_array, _mask, _index + x, _end);
}

template <typename T>
inline sib::ns_ring::iterator<T>
sib::ns_ring::iterator<T>::operator-(long x) const
{
  return iterator(_array, _mask, _index - x, _end);
}

template <typename T>
inline typename sib::ns_ring::iterator<T>::difference_type
sib::ns_ring::iterator<T>::operator-(iterator const& i) const
{
  return (_index - i._index) * sizeof(T);
}

template <typename T>
inline typename sib::ns_ring::iterator<T>::reference_type
sib::ns_ring::iterator<T>::operator*() const
{
  return _array[_index & _mask];
}

template <typename T>
inline typename sib::ns_ring::iterator<T>::pointer_type
sib::ns_ring::iterator<T>::operator->() const
{
  return &(operator*());
}

template <typename T>
inline typename sib::ns_ring::iterator<T>::reference_type
sib::ns_ring::iterator<T>::operator[](std::size_t x) const
{
  return _array[(_index + x) & _mask];
}

template <typename T>
inline bool
sib::ns_ring::iterator<T>::operator==(iterator const& i) const
{
  return _index == i._index && _array == i._array;
}

template <typename T>
inline bool
sib::ns_ring::iterator<T>::operator!=(iterator const& i) const
{
  return _index != i._index || _array != i._array;
}

template <typename T>
inline bool
sib::ns_ring::iterator<T>::operator<(iterator const& i) const
{
  return _index < i._index;
}

template <typename T>
inline bool
sib::ns_ring::iterator<T>::operator<=(iterator const& i) const
{
  return _index <= i._index;
}

template <typename T>
inline bool
sib::ns_ring::iterator<T>::operator>(iterator const& i) const
{
  return _index > i._index;
}

template <typename T>
inline bool
sib::ns_ring::iterator<T>::operator>=(iterator const& i) const
{
  return _index >= i._index;
}

template <typename T, bool C, typename A>
sib::ring<T,C,A>::ring(sib::ring<T,C,A>::size_type const min_capacity,
                       A const& allocator)
: _array_allocator(allocator),
  _capacity(power2_above<size_type>(min_capacity)),
  _mask(_capacity - 1),
  _array(_array_allocator.allocate(_capacity)),
  _pop_count(0),
  _push_count(0)
{
}

template <typename T, bool C, typename A>
sib::ring<T,C,A>::~ring()
{
  while (!empty()) {
    pop();
  }
  _array_allocator.deallocate(_array, _capacity);
}
    
template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::const_iterator
sib::ring<T,C,A>::begin() const
{
  return cbegin();
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::const_iterator
sib::ring<T,C,A>::cbegin() const
{
  return const_iterator(_array, _mask, _pop_count, _push_count);
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::size_type
sib::ring<T,C,A>::capacity() const
{
  return _capacity;
}

template <typename T, bool C, typename A>
inline bool
sib::ring<T,C,A>::empty() const
{
  return _pop_count == _push_count;
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::const_iterator
sib::ring<T,C,A>::end() const
{
  return cend();
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::const_iterator
sib::ring<T,C,A>::cend() const
{
  std::size_t const push_count = _push_count;
  return const_iterator(_array, _mask, push_count, push_count);
}

template <typename T, bool C, typename A>
inline bool
sib::ring<T,C,A>::full() const
{
  return _pop_count + _capacity == _push_count;
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::const_reference
sib::ring<T,C,A>::front() const
{
  return _array[_pop_count & _mask];
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::size_type
sib::ring<T,C,A>::size() const
{
  return _push_count - _pop_count;
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::iterator
sib::ring<T,C,A>::begin()
{
  return iterator(_array, _mask, _pop_count, _push_count);
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::iterator
sib::ring<T,C,A>::end()
{
  std::size_t const push_count = _push_count;
  return iterator(_array, _mask, push_count, push_count);
}

template <typename T, bool C, typename A>
inline void
sib::ring<T,C,A>::finish_push()
{
  ++_push_count;
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::reference
sib::ring<T,C,A>::front()
{
  return _array[_pop_count & _mask];
}

template <typename T, bool C, typename A>
inline void
sib::ring<T,C,A>::pop()
{
  _array[_pop_count & _mask].~T();
  ++_pop_count;
}

template <typename T, bool C, typename A>
inline void
sib::ring<T,C,A>::push(T const& x)
{
  new(_array + (_push_count & _mask)) T(x);
  ++_push_count;
}

template <typename T, bool C, typename A>
inline typename sib::ring<T,C,A>::reference
sib::ring<T,C,A>::start_push()
{
  return *(new(_array[_push_count & _mask]) T());
}


