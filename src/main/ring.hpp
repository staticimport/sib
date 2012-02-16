#ifndef SIB_RING_HPP
#define SIB_RING_HPP

#include <cstddef>
#include <iterator>
#include <memory>

namespace sib {
  namespace ns_ring {
    template <bool Volatile>
    struct counter {
      typedef std::size_t volatile type;
    };

    template <>
    struct counter<false> {
      typedef std::size_t type;
    };

    template <bool NonEmpty>
    struct padding {
      //typedef char type[0];
    };

    template <>
    struct padding<false> {
      typedef char type[64]; // TODO: don't hardcode 64
    };

    template <typename T, bool ConcurrentPushPop>
    struct traits {
      typedef typename counter<ConcurrentPushPop>::type counter_type;
      typedef padding<ConcurrentPushPop> padding_type;
    };

    template <typename T>
    class iterator {
    public:
      typedef std::ptrdiff_t                    difference_type;
      typedef std::random_access_iterator_tag   iterator_category;
      typedef T*                                pointer_type;
      typedef T&                                reference_type;
      typedef iterator                          self_type;
      typedef T                                 value_type;

      iterator(T* array, std::size_t mask, std::size_t begin, std::size_t end);

      iterator& operator+=(long);
      iterator& operator-=(long);
      iterator& operator++();
      iterator operator++(int);
      iterator& operator--();
      iterator operator--(int);

      iterator operator+(long) const;
      iterator operator-(long) const;
      difference_type operator-(iterator const&) const;
      reference_type operator*() const;
      pointer_type operator->() const;
      reference_type operator[](std::size_t) const;
      bool operator==(iterator const&) const;
      bool operator!=(iterator const&) const;
      bool operator<(iterator const&) const;
      bool operator<=(iterator const&) const;
      bool operator>(iterator const&) const;
      bool operator>=(iterator const&) const;
    private:
      T* _array;
      std::size_t _mask;
      std::size_t _index;
      std::size_t _end;
    };
  }

  template <typename T,
            bool ConcurrentPushPop=false,
            typename Allocator = std::allocator<T> >
  class ring {
  private:
    typedef ns_ring::traits<T,ConcurrentPushPop> traits;
  public:
    typedef Allocator                             allocator_type;
    typedef typename ns_ring::iterator<T const>   const_iterator;
    typedef typename Allocator::const_pointer     const_pointer;
    typedef typename Allocator::const_reference   const_reference;
    typedef typename Allocator::difference_type   difference_type;
    typedef typename ns_ring::iterator<T>         iterator;
    typedef typename Allocator::pointer           pointer;
    typedef typename Allocator::reference         reference;
    typedef typename Allocator::size_type         size_type;
    typedef typename Allocator::value_type        value_type;

    // Init/Uninit
    explicit ring(size_type const min_capacity,
                  Allocator const& allocator = Allocator());
    ~ring();

    // Const
    const_iterator begin() const;
    const_iterator cbegin() const;
    size_type capacity() const;
    bool empty() const;
    const_iterator end() const;
    const_iterator cend() const;
    bool full() const;
    const_reference front() const;
    size_type size() const;

    // Non-Const
    iterator begin();
    iterator end();
    void finish_push();
    reference front();
    void pop();
    void push(T const& x);
    reference start_push();
  private:
    //typename Allocator::template rebind<T*>::other _array_allocator;
    Allocator _array_allocator;
    size_type const _capacity;
    size_type const _mask;
    T* _array;
    typename traits::padding_type pad1;
    typename traits::counter_type _pop_count;
    typename traits::padding_type pad2;
    typename traits::counter_type _push_count;
  };
}

template <typename T>
typename sib::ns_ring::iterator<T>
operator+(long, sib::ns_ring::iterator<T> const& i);

/**
 * Implementation
 **/
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
  auto iter(*this);
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
  auto iter(*this);
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
  _capacity(min_capacity), // TODO: power of 2
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
  auto const push_count = _push_count;
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
  auto const push_count = _push_count;
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
  return _array[_push_count & _mask];
}

#endif /* SIB_UNBALANCED_SET_HPP */

