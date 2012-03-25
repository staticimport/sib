#ifndef SIB_DYNAMIC_RING_HPP
#define SIB_DYNAMIC_RING_HPP

#include <exception>

#include "ring.hpp"

namespace sib {
  namespace ns_dynamic_ring {
    template <typename T, bool ConcurrentPushPop, typename Allocator>
    struct traits { // TODO: fix these to actually use the allocator!
      //typedef ring<T,ConcurrentPushPop,Allocator> sub_ring;
      //typedef ring<T,ConcurrentPushPop,Allocator>* volatile sub_ring_pointer;
      typedef ring<T,ConcurrentPushPop> sub_ring;
      typedef ring<T,ConcurrentPushPop>* volatile sub_ring_pointer;
    };

    template <typename T, typename Allocator>
    struct traits<T,false,Allocator> {
      //typedef ring<T,false,Allocator> sub_ring;
      //typedef ring<T,false,Allocator>* sub_ring_pointer;
      typedef ring<T,false> sub_ring;
      typedef ring<T,false>* sub_ring_pointer;
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

    typedef typename ring<typename traits::sub_ring_pointer>::const_iterator expand_ring_citer;

    typename Allocator::template rebind<typename traits::sub_ring>::other _ring_allocator;
    //ring<typename traits::sub_ring_pointer,ConcurrentPushPop,Allocator> _expand_ring;
    ring<typename traits::sub_ring_pointer> _expand_ring;
    typename traits::sub_ring_pointer _read_ring;
    typename traits::sub_ring_pointer _write_ring;
  };
}

#include "internal/dynamic_ring.inl"

#endif /* SIB_DYNAMIC_RING_HPP */

