
#include "pool_allocator.hpp"

sib::single_size_memory_pool::single_size_memory_pool(std::size_t init_count,
                                                      double grow_multiplier)
: _init_count(init_count)
, _grow_multiplier(grow_multiplier)
, _item_size(0)
{
}

sib::single_size_memory_pool::~single_size_memory_pool()
{
  for(typename sib::vector<block,true>::iterator iter = _blocks.begin();
      iter != _blocks.end();
      ++iter)
  {
    free(iter->_begin);
  }
}

void*
sib::single_size_memory_pool::allocate(std::size_t const size,
                                       std::size_t const count,
                                       void const* const hint)
{
  if (SIB_UNLIKELY(_item_size == 0)) init(size);
  if (SIB_UNLIKELY(_item_size < size || count != 1))
    throw std::runtime_error("Invalid size or count");
  block* b(NULL);
  for(typename sib::vector<block,true>::reverse_iterator iter = _blocks.rbegin();
      iter != _blocks_rend;
      ++iter)
  {
    if (iter->_free || iter->_next_unused != iter->_end) {
      b = &(*iter);
      if (!hint || b->contains(hint))
        break;
    }
  }
  if (!b)
    b = expand();
  if (b->_free) {
    free_node* const node = b->_free;
    b->_free = node->_next;
    return node;
  } else {
    void* const node = b->_next_unused;
    b->_next_unused += _item_size;
    return node;
  }
}

void
sib::single_size_memory_pool::deallocate(void* const pointer,
                                         std::size_t count)
{
  char* p = static_cast<char*>(pointer);
  for(typename sib::vector<block,true>::reverse_iterator iter = _blocks.rbegin();
      iter != _blocks_rend;
      ++iter)
  {
    if (iter->contains(pointer)) {
      while (count--) {
        free_node* const node = reinterpret_cast<free_node*>(p);
        node->_next = iter->_free;
        iter->_free = node;
        p += _item_size;
      }
      break;
    }
  }
}

sib::single_size_memory_pool::block*
sib::single_size_memory_pool::expand(std::size_t count)
{
  if (count == 0) {
    block const& last = _blocks.back();
    count = std::max<std::size_t>(static_cast<std::size_t>(_grow_multiplier * (last._end - last._begin) / _item_size), 1);
  }
  _blocks.resize(_blocks.size()+1);
  _blocks_rend = _blocks.rend();
  block& new_block = _blocks.back();
  std::size_t const alloc_size = count * _item_size;
  new_block._begin = static_cast<char*>(malloc(alloc_size));
  new_block._end = new_block._begin + alloc_size;
  new_block._next_unused = new_block._begin;
  new_block._free = NULL;
  return &new_block;
}

void
sib::single_size_memory_pool::init(std::size_t const item_size)
{
  _item_size = std::max(item_size, sizeof(void*));
  expand(_init_count);
}

