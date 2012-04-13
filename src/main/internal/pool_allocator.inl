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
    if (iter->_free) {
      b = &(*iter);
      if (!hint || b->contains(hint))
        break;
    }
  }
  if (!b)
    b = expand();
  free_node* const node = b->_free;
  b->_free = node->_next;
  return node;
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
  new_block._free = NULL;
  for(char* entry = new_block._end - _item_size; 
      entry >= new_block._begin;
      entry -= _item_size) 
  {
    free_node* const node = reinterpret_cast<free_node*>(entry);
    node->_next = new_block._free;
    new_block._free = node;
  }
  return &new_block;
}

void
sib::single_size_memory_pool::init(std::size_t const item_size)
{
  _item_size = std::max(item_size, sizeof(void*));
  expand(_init_count);
}

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


