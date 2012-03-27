
namespace sib { namespace internal
{
  template <typename T, bool IsPod=false>
  struct vector_helper
  {
    static inline T* alloc(std::size_t const capacity)
    {
      return malloc(capacity * sizeof(T));
    }

    static inline void destruct(T* x) { x->~T(); }

    static inline void destruct(T* begin, T* end)
    {
      for(T* i = begin; i < end; ++i)
        i->~T();
    }

    static inline T* expand(T* alloced, std::size_t size, std::size_t new_cap)
    {
      T* const new_alloced = static_cast<T*>(malloc(new_cap * sizeof(T)));
      T* const end = alloced + size;
      for(T* i = alloced; i != end; ++i) {
        new(new_alloced[i]) T(*i);
        destruct(i);
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
        new(next++) T(*i);
      }
      for(; next != end; ++next)
        destruct(next);
    }

    static inline void move_right(T* src, T* dest, std::size_t count)
    {
      T* to = dest + count;
      for(T* i = src + count; i != src; ++i) {
        new(to--) T(*i);
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
      return malloc(capacity * sizeof(T));
    }

    static inline void destruct(T* x) { }

    static inline void destruct(T* begin, T* end) { }

    static inline T* expand(T* alloced, std::size_t size, std::size_t new_cap)
    {
      return realloc(alloced, new_cap * sizeof(T));
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
: _begin(vector_helper<T,P>::alloc(reserve_capacity))
, _end(_begin)
, _capacity_end(_begin + reserve_capacity)
{
}

template <typename T, bool P>
sib::vector<T,P>::~vector()
{
  vector_helper<T,P>::destruct(_begin, _end);
  vector_helper<T,P>::release(_begin);
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
  return std::reverse_iterator(_end);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::reverse_iterator
sib::vector<T,P>::rend()
{
  return std::reverse_iterator(_begin);
}

template <typename T, bool P>
inline typename sib::vector<T,P>::max_size
sib::vector<T,P>::max_size()
{
  return ((sizeof(size_type)) << 8) / sizeof(T);
}

template <typename T, bool P>
inline void sib::vector<T,P>::reserve(size_type capacity)
{
  if (capacity() < capacity) {
    size_type const the_size = size();
    _begin = vector_helper<T,P>::expand(_begin, the_size, capacity);
    _end = _begin + the_size;
    _capacity_end = _begin + capacity;
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
  internal::vector_helper<T,P>::move_left(pos,pos-1,_end-pos);
  --_end;
  return pos-1;
}

template <typename T, bool P>
inline typename sib::vector<T,P>::iterator
sib::vector<T,P>::insert(iterator pos, param_type value)
{
  internal::vector_helper<T,P>::move_right(pos,pos+1,_end-pos);
  ++_end;
  new(pos) T(value);
  return pos;
}

template <typename T, bool P>
inline void sib::vector<T,P>::pop_back()
{
  destruct(--_end);
}

template <typename T, bool P>
inline void sib::vector<T,P>::push_back(param_type x)
{
  if (_end == _capacity_end)
    reserve(capacity() << 1);
  new(_end++) T(x);
}

// TODO: reserve, swap, =
