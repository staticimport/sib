//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <stdexcept>
#include <utility>

namespace sib { namespace internal
{
  template <typename T, bool IsPod=false>
  struct vector_helper
  {
    static inline T* alloc(std::size_t const capacity)
    {
      return static_cast<T*>(malloc(capacity * sizeof(T)));
    }

    static inline void destruct(T* x) { x->~T(); }

    static inline void destruct(T* begin, T* end)
    {
      for(T* i = begin; i < end; ++i)
        i->~T();
    }

    static T* expand(T* alloced, std::size_t size, std::size_t new_cap)
    {
      T* const new_alloced = static_cast<T*>(malloc(new_cap * sizeof(T)));
      T* const new_end = new_alloced + size;
      T* from = alloced;
      T* to = new_alloced;
      while (to != new_end) {
#ifdef SIB_CXX11
        new(to++) T(std::move(*from));
#else
        new(to++) T(*from);
#endif
        destruct(from++);
      }
      release(alloced);
      return new_alloced;
    }

    static inline void move_left(T* src, T* dest, std::size_t count)
    {
      T* next = dest;
      T* const end = src + count;
      for(T* i = src; i != end; ++i) {
        destruct(next);
#ifdef SIB_CXX11
        new(next++) T(std::move(*i));
#else
        new(next++) T(*i);
#endif
      }
      for(; next != end; ++next)
        destruct(next);
    }

    static inline void move_right(T* src, T* dest, std::size_t count)
    {
      T* to = dest + count - 1;
      for(T* i = src + count - 1; i >= src; --i) {
#ifdef SIB_CXX11
        new(to--) T(std::move(*i));
#else
        new(to--) T(*i);
#endif
        destruct(i);
      }
    }
    
    static inline void release(T* alloced) { free(alloced); }
  };

  template <typename T>
  struct vector_helper<T,true>
  {
    static inline T* alloc(std::size_t const capacity)
    {
      return static_cast<T*>(malloc(capacity * sizeof(T)));
    }

    static inline void destruct(T* x) { }

    static inline void destruct(T* begin, T* end) { }

    static inline T* expand(T* alloced, std::size_t size, std::size_t new_cap)
    {
      return static_cast<T*>(realloc(alloced, new_cap * sizeof(T)));
    }

    static inline void move_left(T* src, T* dest, std::size_t count)
    {
      memmove(dest, src, count * sizeof(T));
    }

    static inline void move_right(T* src, T* dest, std::size_t count)
    {
      memmove(dest, src, count * sizeof(T));
    }

    static inline void release(T* alloced) { free(alloced); }
  };
}}

template <typename T, bool P>
sib::vector<T,P>::vector(size_type const reserve_capacity)
: _begin(internal::vector_helper<T,P>::alloc(reserve_capacity))
, _end(_begin)
, _capacity_end(_begin + reserve_capacity)
{
}

template <typename T, bool P>
sib::vector<T,P>::vector(vector const& other)
: _begin(internal::vector_helper<T,P>::alloc(other.size()))
, _end(_begin + other.size())
, _capacity_end(_end)
{
  T* to = _begin;
  T* from = other._begin;
  while (to != _end)
    new(to++) T(*(from++));
}

template <typename T, bool P>
sib::vector<T,P>::~vector()
{
  internal::vector_helper<T,P>::destruct(_begin, _end);
  internal::vector_helper<T,P>::release(_begin);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::const_reference
sib::vector<T,P>::at(size_type pos) const
{
  T const* const x = _begin + pos;
  if (x < _end)
    return *x;
  else
    throw std::runtime_error("vector.at index beyond end of vector");
}

template <typename T, bool P>
inline typename sib::vector<T,P>::reference
sib::vector<T,P>::at(size_type pos)
{
  T* const x = _begin + pos;
  if (x < _end)
    return *x;
  else
    throw std::runtime_error("vector.at index beyond end of vector");
}

template <typename T, bool P>
inline typename sib::vector<T,P>::reverse_iterator
sib::vector<T,P>::rbegin()
{
  return reverse_iterator(_end);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::const_reverse_iterator
sib::vector<T,P>::rbegin() const
{
  return reverse_iterator(_end);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::const_reverse_iterator
sib::vector<T,P>::crbegin() const
{
  return reverse_iterator(_end);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::reverse_iterator
sib::vector<T,P>::rend()
{
  return reverse_iterator(_begin);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::const_reverse_iterator
sib::vector<T,P>::rend() const
{
  return reverse_iterator(_begin);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::const_reverse_iterator
sib::vector<T,P>::crend() const
{
  return reverse_iterator(_begin);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::size_type
sib::vector<T,P>::max_size() const
{
  return (static_cast<std::size_t>(1) << ((sizeof(size_type) << 3)-1)) / sizeof(T);
}

template <typename T, bool P>
inline void sib::vector<T,P>::reserve(size_type count)
{
  if (capacity() < count) {
    size_type const the_size = size();
    _begin = internal::vector_helper<T,P>::expand(_begin, the_size, count);
    _end = _begin + the_size;
    _capacity_end = _begin + count;
  }
}

template <typename T, bool P>
inline void sib::vector<T,P>::clear()
{
  internal::vector_helper<T,P>::destruct(_begin,_end);
  _end = _begin;
}

template <typename T, bool P>
inline typename sib::vector<T,P>::iterator
sib::vector<T,P>::erase(iterator pos)
{
  internal::vector_helper<T,P>::move_left(pos+1,pos,_end-pos-1);
  --_end;
  return pos;
}

template <typename T, bool P>
inline typename sib::vector<T,P>::iterator
sib::vector<T,P>::insert(iterator pos, param_type value)
{
  if (_end != _capacity_end) 
    internal::vector_helper<T,P>::move_right(pos,pos+1,_end-pos);
  else {
    ptrdiff_t const diff = pos - _begin;
    reserve(size() << 1);
    pos = _begin + diff;
    internal::vector_helper<T,P>::move_right(pos,pos+1,_end-pos);
  }
  ++_end;
  new(pos) T(value);
  return pos;
}

template <typename T, bool P>
inline void sib::vector<T,P>::pop_back()
{
  internal::vector_helper<T,P>::destruct(--_end);
}

template <typename T, bool P>
inline void sib::vector<T,P>::push_back(param_type x)
{
  if (_end != _capacity_end)
    new(_end++) T(x);
  else {
    reserve(capacity() << 1);
    new(_end++) T(x);
  }
}

template <typename T, bool P>
inline void sib::vector<T,P>::resize(size_type count)
{
  size_type const old_size = size();
  if (count > old_size) {
    reserve(count);
    _end = _begin + count;
    for(T* t = _begin + old_size; t != _end; ++t)
      new(t) T();
  } else {
    T* const old_end = _end;
    _end = _begin + count;
    internal::vector_helper<T,P>::destruct(_end, old_end);
  }
}

template <typename T, bool P>
inline void sib::vector<T,P>::swap(vector& other)
{
  std::swap(_begin, other._begin);
  std::swap(_end, other._end);
  std::swap(_capacity_end, other._capacity_end);
}

template <typename T, bool P>
inline sib::vector<T,P>&
sib::vector<T,P>::operator=(vector const& other)
{
  if (this != &other) {
    vector copy(other);
    swap(copy);
  }
  return *this;
}

