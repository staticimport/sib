//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_RING_HPP
#define SIB_RING_HPP

#include <cstddef>
#include <iterator>
#include <memory>

#include "bits.hpp"

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

#include "internal/ring.inl"

#endif /* SIB_RING_HPP */

