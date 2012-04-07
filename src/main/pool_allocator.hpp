#ifndef SIB_POOL_ALLOCATOR_HPP
#define SIB_POOL_ALLOCATOR_HPP

#include <cstddef>

#include "common.hpp"
#include "vector.hpp"

namespace sib
{
  template <typename T, 
            unsigned InitSize=8, 
            unsigned GrowMultiplierTenths=20> // 2x default
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
      typedef pool_allocator<U> other;
    };

    pool_allocator();
    ~pool_allocator();

    pointer address(reference x) const              { return &x; }
    const_pointer address(const_reference x) const  { return &x; }
    pointer allocate(size_type count, const_pointer hint=NULL);
    void deallocate(pointer p, size_type count);
    size_type max_size() const;
    
    void construct(pointer p, const_reference val)  { new(p) T(val); }
    void destroy(pointer p)                         { p->~T(); }
  private:
    pool_allocator(pool_allocator const& other); // not supported

    struct free_node
    {
      free_node() : _next(NULL) { }
      free_node* _next;
    };

    struct block
    {
      T* _begin;
      T* _end;
    };

    sib::vector<block,true> _blocks;
    double const _grow_multiplier;
  };
}

#endif /* SIB_POOL_ALLOCATOR_HPP */

