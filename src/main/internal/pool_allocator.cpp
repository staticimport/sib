//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "pool_allocator.hpp"

namespace sib
{
  namespace internal
  {
    simple_memory_pool::simple_memory_pool(std::size_t init_count, double grow_multiplier)
    : _grow_multiplier(grow_multiplier)
    , _next_items_per_block(init_count)
    , _item_size(0)
    , _block_list(NULL)
    , _free_list(NULL)
    {
    }

    simple_memory_pool::~simple_memory_pool()
    {
      block_header* block = _block_list;
      while (block) {
        block_header* next = block->_next_block;
        free(block);
        block = next;
      }
    }

    void* simple_memory_pool::allocate(std::size_t size)
    {
      if (SIB_UNLIKELY(_item_size == 0)) {
        _item_size = std::max(size, sizeof(free_node));
        create_block();
      }
      else if (SIB_UNLIKELY(_item_size < size))
        throw std::runtime_error("Attempted alloc of multiple sizes from same pool!");
      
      if (_free_list) {
        free_node* const free = _free_list;
        _free_list = free->_next;
        return free;
      } else if (_block_list->_num_available || create_block()) {
        return get_block_item(_block_list, --(_block_list->_num_available));
      } else
        throw std::runtime_error("Unable to allocate new block!");
    }

    void simple_memory_pool::deallocate(void* alloced)
    {
      free_node* const free = static_cast<free_node*>(alloced);
      free->_next = _free_list;
      _free_list = free;
    }

    bool simple_memory_pool::create_block()
    {
      std::size_t const size = sizeof(block_header) + _next_items_per_block * _item_size;
      block_header* const block = static_cast<block_header*>(malloc(size));
      if (block)
      {
        block->_next_block = _block_list;
        block->_num_available = _next_items_per_block;
        _next_items_per_block *= _grow_multiplier;
        _block_list = block;
        return true;
      }
      else
        return false;
    }

    void* simple_memory_pool::get_block_item(block_header* block, unsigned index)
    {
      return reinterpret_cast<char*>(block+1) + index * _item_size;
    }
  }
}
