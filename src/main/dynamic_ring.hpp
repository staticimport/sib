#ifndef SIB_DYNAMIC_RING_HPP
#define SIB_DYNAMIC_RING_HPP

#include "ring.hpp"

namespace sib {
  namespace ns_dynamic_ring {
    template <typename T, bool ConcurrentPushPop, typename Allocator>
    struct traits {
      typedef ring<T,ConcurrentPushPop,Allocator> sub_ring;
      typedef ring<T,ConcurrentPushPop,Allocator>* volatile sub_ring_pointer;
    };

    template <typename T, typename Allocator>
    struct traits<T,false,Allocator> {
      typedef ring<T,false,Allocator> sub_ring;
      typedef ring<T,false,Allocator>* sub_ring_pointer;
    };
  };

  template <typename T,
            bool ConcurrentPushPop = false,
            typename Allocator = std::allocator<T> >
  class dynamic_ring {
  private:
    typedef ns_dynamic_ring::traits<T,ConcurrentPushPop,Allocator> traits;
  public:
    typedef Allocator allocator_type;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::difference_type difference_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::size_type size_type;
    typedef typename Allocator::value_type value_type;

    explicit dynamic_ring(size_type const min_capacity,
                          Allocator const& allocator = Allocator());
    ~dynamic_ring();

    // Const
    bool empty() const;
    //bool full() const; // Only implement this if a max_capacity is supported
    const_reference front() const;
    size_type size(bool reader_context=true) const;

    // Non-Const
    void finish_push();
    reference front();
    void pop();
    void push(T const& x);
    reference start_push();
  private:
    void advance_read_ring();
    void advance_write_ring();

    typename Allocator::template rebind<typename traits::sub_ring>::other _ring_allocator;
    ring<typename traits::sub_ring_pointer,ConcurrentPushPop,Allocator> _expand_ring;
    typename traits::sub_ring_pointer _read_ring;
    typename traits::sub_ring_pointer _write_ring;
  };
}

/**
 * Template Implemenation
 **/
template <typename T, bool C, typename A>
sib::dynamic_ring<T,C,A>::dynamic_ring(
    sib::dynamic_ring<T,C,A>::size_type const min_capacity,
    A const& allocator)
: _ring_allocator(allocator),
  _expand_ring(sizeof(void*) * 8),
  _read_ring(new(_ring_allocator.allocate(1)) 
             typename traits::sub_ring(min_capacity)), // TODO: pass allocator? ring_allocator?
  _write_ring(_read_ring)
{
}

template <typename T, bool C, typename A>
sib::dynamic_ring<T,C,A>::~dynamic_ring()
{
  while (!empty()) {
    pop();
  }
  _ring_allocator.deallocate(_read_ring, 1);
}

template <typename T, bool C, typename A>
inline bool
sib::dynamic_ring<T,C,A>::empty() const
{
  return _write_ring->empty();
}

template <typename T, bool C, typename A>
inline typename sib::dynamic_ring<T,C,A>::const_reference
sib::dynamic_ring<T,C,A>::front() const
{
  if (_read_ring->empty()) advance_read_ring();
  return _read_ring.front();
}

template <typename T, bool C, typename A>
typename sib::dynamic_ring<T,C,A>::size_type
sib::dynamic_ring<T,C,A>::size(bool reader_context) const
{
  size_type total(0);
  if (!C) {
    total += _read_ring->size();
    auto expand_ring_end(_expand_ring.cend());
    for(auto iter = _expand_ring.cbegin(); iter != expand_ring_end; ++iter) {
      total += (*iter)->size();
    }
  } else if (reader_context) {
    auto read_ring(_read_ring);
    size_type const expand_count(_expand_ring.size());
    total += read_ring.size();
    for(size_type ii = 1; ii < expand_count; ++ii) {
      total += read_ring.capacity() << ii;
    }
  } else {
    auto write_ring(_write_ring);
    size_type const expand_count(_expand_ring.size());
    total += write_ring.size();
    for(size_type ii = 1; ii < expand_count; ++ii) {
      total += write_ring.capacity() >> ii;
    }
  }
  return total;
}

template <typename T, bool C, typename A>
inline void
sib::dynamic_ring<T,C,A>::finish_push()
{
  _write_ring->finish_push();
  if (_write_ring->size() == 1 and _read_ring != _write_ring) {
    _expand_ring.push(_write_ring);
  }
}

template <typename T, bool C, typename A>
inline typename sib::dynamic_ring<T,C,A>::reference
sib::dynamic_ring<T,C,A>::front()
{
  if (_read_ring->empty()) advance_read_ring();
  return _read_ring.front();
}

template <typename T, bool C, typename A>
inline void
sib::dynamic_ring<T,C,A>::pop()
{
  _read_ring->pop();
}

template <typename T, bool C, typename A>
inline void
sib::dynamic_ring<T,C,A>::push(T const& x)
{
  if (_write_ring->full()) advance_write_ring();
  _write_ring->push(x);
  if (_write_ring->size() == 1 and _read_ring != _write_ring) {
    _expand_ring.push(_write_ring);
  }
}

template <typename T, bool C, typename A>
inline typename sib::dynamic_ring<T,C,A>::reference
sib::dynamic_ring<T,C,A>::start_push()
{
  if (_write_ring->full()) advance_write_ring();
  return _write_ring->start_push();
}

template <typename T, bool C, typename A>
void
sib::dynamic_ring<T,C,A>::advance_read_ring()
{
  _read_ring->~ring();
  _ring_allocator.deallocate(_read_ring, 1);
  _read_ring = _expand_ring.peek();
  _expand_ring.pop();
}

template <typename T, bool C, typename A>
void
sib::dynamic_ring<T,C,A>::advance_write_ring()
{
  size_type const next_capacity = _write_ring->capacity() * 2;
  _write_ring = new(_ring_allocator.allocate(1)) 
                typename traits::sub_ring(next_capacity);
  _expand_ring.push(_write_ring);
}

#endif /* SIB_DYNAMIC_RING_HPP */
