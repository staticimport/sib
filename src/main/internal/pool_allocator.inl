//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <iostream>

namespace sib
{
  class single_size_memory_pool
  {
  public:
    single_size_memory_pool(std::size_t init_count, double grow_multiplier);
    ~single_size_memory_pool();

    void* allocate(std::size_t const size, std::size_t const count, 
                   void const* const hint=NULL);
    void deallocate(void* const pointer, std::size_t count);
  private:
    struct free_node
    {
      free_node() : _next(NULL) {}
      free_node* _next;
    };

    struct block
    {
      bool contains(void const* const p) const;

      char* _begin;
      char* _end;
      char* _next_unused;
      free_node* _free;
    };

    block* expand(std::size_t count=0);
    void init(std::size_t const item_size);

    std::size_t const _init_count;
    double const _grow_multiplier;
    std::size_t _item_size;
    sib::vector<block,true> _blocks;
    typename sib::vector<block,true>::reverse_iterator _blocks_rend;
  };
};

inline bool sib::single_size_memory_pool::block::contains(void const* const p) const
{
  return p >= _begin && p < _end;
}

template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator()
: _pool(new single_size_memory_pool(I, G/10.0))
, _ref_count(new unsigned(1))
{
}

template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator(pool_allocator const& other)
: _pool(other._pool)
, _ref_count(other._ref_count)
{
  ++(*_ref_count);
}

template <typename T, unsigned I, unsigned G>
template <typename U>
inline sib::pool_allocator<T,I,G>::pool_allocator(pool_allocator<U,I,G> const& other)
: _pool(other._pool)
, _ref_count(other._ref_count)
{
  ++(*_ref_count);
}

template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::~pool_allocator()
{
  if (--(*_ref_count) == 0) {
    delete _pool;
    delete _ref_count;
  }
}

template <typename T, unsigned I, unsigned G>
inline typename sib::pool_allocator<T,I,G>::pointer
sib::pool_allocator<T,I,G>::allocate(size_type count, const_pointer hint)
{
  return static_cast<T*>(_pool->allocate(sizeof(T), count, hint));
}

template <typename T, unsigned I, unsigned G>
inline void sib::pool_allocator<T,I,G>::deallocate(pointer p, size_type count)
{
  _pool->deallocate(p, count);
}


