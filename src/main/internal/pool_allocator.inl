//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <iostream>

namespace sib
{
  namespace internal
  {
    class simple_memory_pool
    {
    public:
      simple_memory_pool(std::size_t init_count, double grow_multiplier);
      ~simple_memory_pool();

      void* allocate(std::size_t size);
      void deallocate(void* alloced);
    private:
      struct free_node
      {
        free_node() : _next(NULL) {}
        free_node* _next;
      };

      struct block_header
      {
        block_header* _next_block;
        unsigned _num_available;
      };

      bool create_block();
      void* get_block_item(block_header* block, unsigned index);

      double const _grow_multiplier;
      std::size_t _next_items_per_block;
      std::size_t _item_size;
      block_header* _block_list;
      free_node* _free_list; 
    };
  };

  template <typename T, unsigned I, unsigned E>
  inline pool_allocator<T,I,E>::pool_allocator()
  : _pool(new internal::simple_memory_pool(I, E/10.0))
  , _ref_count(new unsigned(1))
  {
  }

  template <typename T, unsigned I, unsigned E>
  inline pool_allocator<T,I,E>::pool_allocator(pool_allocator const& other)
  : _pool(other._pool)
  , _ref_count(other._ref_count)
  {
    ++(*_ref_count);
  }

  template <typename T, unsigned I, unsigned E>
  template <typename U>
  inline pool_allocator<T,I,E>::pool_allocator(pool_allocator<U,I,E> const& other)
  : _pool(other._pool)
  , _ref_count(other._ref_count)
  {
    ++(*_ref_count);
  }

  template <typename T, unsigned I, unsigned E>
  inline pool_allocator<T,I,E>::~pool_allocator()
  {
    if (0 == --(*_ref_count)) {
      delete _pool;
      delete _ref_count;
    }
  }

  template <typename T, unsigned I, unsigned E>
  inline typename pool_allocator<T,I,E>::pointer
  pool_allocator<T,I,E>::allocate(size_type count, const_pointer hint)
  {
    if (SIB_LIKELY(1 == count))
      return static_cast<T*>(_pool->allocate(sizeof(T)));
    else
      throw std::runtime_error("pool_allocator only supports single allocations");
  }

  template <typename T, unsigned I, unsigned E>
  inline void pool_allocator<T,I,E>::deallocate(pointer p, size_type count)
  {
    if (SIB_LIKELY(1 == count))
      _pool->deallocate(p);
    else
      throw std::runtime_error("pool_allocator only supports single allocations");
  }

  template <typename T, unsigned I, unsigned E>
  inline bool pool_allocator<T,I,E>::operator==(pool_allocator const& other) const
  {
    return this == &other;
  }
}
