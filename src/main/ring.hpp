#ifndef ZTL_RING_HPP
#define ZTL_RING_HPP

namespace ztd {
  namespace ns_ring {
    template <bool Volatile>
    struct counter {
      typedef std::size_t volatile type;
    };

    template <>
    struct counter<false> {
      typedef std::size_t type;
    };

    typedef <bool NonEmpty>
    struct padding {
      typedef char[0] type;
    };

    typedef <>
    struct padding<false> {
      typedef char[64] type; // TODO: don't hardcode 64
    };

    template <typename T, bool ConcurrentPushPop>
    struct traits {
      typedef typename counter<ConcurrentPushPop>::type counter_type;
      typedef typename padding<ConcurrentPushPop>::type padding_type;

      class const_iterator {

      };

      class iterator {

      };
    };
  };

  template <typename T,
            bool ConcurrentPushPop=false,
            typename Allocator = std::allocator<T> >
  class ring {
  private:
    typedef ns_ring::traits<T,ConcurrentPushPop> traits;
  public:
    typedef Allocator allocator_type;
    typedef typename traits::const_iterator const_iterator;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::difference_type difference_type;
    typedef typename traits::iterator iterator;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::size_type size_type;
    typedef typename Allocator::value_type value_type;

    // Init/Uninit
    explicit ring(size_type const min_capacity,
                  Allocator const& allocator = Allocator());

    // Const
    size_type capacity() const;
    bool empty() const;
    bool full() const;
    const_reference front() const;
    size_type size() const;

    // Non-Const
    void finish_push();
    value_type& front();
    void pop();
    void push(T const& x);
    T& start_push();
  private:
    typename Allocator::rebind<T*>::other _array_allocator;
    size_type const _capacity;
    size_type const _mask;
    T* _array;
    typedef traits::padding_type pad1;
    typename traits::counter_type _pop_count;
    typedef traits::padding_type pad2;
    typename traits::counter_type _push_count;
  };
}

/**
 * Implementation
 **/
template <typename T, bool C, typename A>
ztd::ring<T,C,A>::ring(ztd::ring<T,C,A>::size_type const min_capacity,
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
ztd::ring<T,C,A>::~ring()
{
  while (!empty()) {
    pop();
  }
  _array_allocator.deallocate(_array, _capacity)
}

template <typename T, bool C, typename A>
inline bool
ztd::ring<T,C,A>::empty() const
{
  return _pop_count == _push_count;
}

template <typename T, bool C, typename A>
inline bool
ztd::ring<T,C,A>::full() const
{
  return _pop_count + _capacity > _push_count;
}

template <typename T, bool C, typename A>
inline ztd::ring<T,C,A>::const_reference
ztd::ring<T,C,A>::front() const
{
  return _array[_pop_count & _mask];
}

template <typename T, bool C, typename A>
inline ztd::ring<T,C,A>::size_type
ztd::ring<T,C,A>::size() const
{
  return _push_count - _pop_count;
}

template <typename T, bool C, typename A>
inline void
ztd::ring<T,C,A>::finish_push()
{
  ++_push_count;
}

template <typename T, bool C, typename A>
inline ztd::ring<T,C,A>::reference
ztd::ring<T,C,A>::front()
{
  return _array[_pop_count & _mask];
}

template <typename T, bool C, typename A>
inline void
ztd::ring<T,C,A>::pop()
{
  _array[_pop_count & _mask].~T();
  ++_pop_count;
}

template <typename T, bool C, typename A>
inline void
ztd::ring<T,C,A>::push(T const& x)
{
  new(_array + (_push_count & _mask)) T(x);
  ++_push_count;
}

template <typename T, bool C, typename A>
inline T&
ztd::ring<T,C,A>::start_push()
{
  return _array[_push_count & _mask];
}

#endif /* ZTL_UNBALANCED_SET_HPP */

