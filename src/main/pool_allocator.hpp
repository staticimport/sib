//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_POOL_ALLOCATOR_HPP
#define SIB_POOL_ALLOCATOR_HPP

#include <cstddef>

#include "common.hpp"
#include "vector.hpp"

namespace sib
{
  namespace internal
  {
    class simple_memory_pool;
  };

  template <typename T, 
            unsigned InitCount=8,
            unsigned ExpandMultiplierInTenths=15> // 15 == 1.5x multiplier
  class pool_allocator
  {
  public:
    typedef std::ptrdiff_t  difference_type;
    typedef T const*        const_pointer;
    typedef T const&        const_reference;
    typedef T*              pointer;
    typedef T&              reference;
    typedef std::size_t     size_type;
    typedef T               value_type;
    template <typename U> struct rebind
    {
      typedef pool_allocator<U,InitCount,ExpandMultiplierInTenths> other;
    };

    pool_allocator();
    pool_allocator(pool_allocator const& other);
    template <typename U>
    pool_allocator(pool_allocator<U,InitCount,ExpandMultiplierInTenths> const& other);
    ~pool_allocator();

    pointer address(reference x) const              { return &x; }
    const_pointer address(const_reference x) const  { return &x; }
    pointer allocate(size_type count, const_pointer hint=NULL);
    void deallocate(pointer p, size_type count);
    size_type max_size() const;
    
    void construct(pointer p, const_reference val)  { new(p) T(val); }
    void destroy(pointer p)                         { p->~T(); }
  //private:
    internal::simple_memory_pool* _pool;
    unsigned* _ref_count;
  };
}

#include "internal/pool_allocator.inl"

#endif /* SIB_POOL_ALLOCATOR_HPP */

