#ifndef SIB_POOL_ALLOCATOR_H
#define SIB_POOL_ALLOCATOR_H

#include <deque>
#include <cstdlib>
#include <iostream>

namespace sib
{
  class simple_pool_allocator
  {
  public:
    simple_pool_allocator(std::size_t const size)
      : _entry_size(size), _free_list(NULL), _print_on_alloc(false)
    {
      clear_stats();
    }

    ~simple_pool_allocator()
    {
      for(std::deque<void*>::iterator iter = _alloced.begin(); iter != _alloced.end(); ++iter) {
        free(*iter);
      }
    }

    void* allocate()
    {
      ++allocate_calls;
      if (!_free_list) {
        ++mallocs;
        if (_print_on_alloc)
          std::cout << "ALLOC!\n";
        entry* chunk = static_cast<entry*>(malloc(64 * _entry_size));
        _alloced.push_back(chunk);
        for(std::size_t ii = 0; ii != 64; ++ii) {
          chunk->_next = _free_list;
          _free_list = chunk;
          chunk = reinterpret_cast<entry*>(reinterpret_cast<char*>(chunk) + _entry_size);
        }
      }
      entry* e = _free_list;
      _free_list = _free_list->_next;
      return e;
    }

    void release(void* x)
    {
      ++release_calls;
      entry* e = static_cast<entry*>(x);
      e->_next = _free_list;
      _free_list = e;
    }
  //private:
    struct entry {
      entry* _next;
    };

    void clear_stats()
    {
      allocate_calls = release_calls = mallocs = 0;
    }


    std::size_t const _entry_size; 
    entry* _free_list;
    std::deque<void*> _alloced;
    bool _print_on_alloc;

    // stats
    std::size_t allocate_calls;
    std::size_t release_calls;
    std::size_t mallocs;
  };
}

#endif /* SIB_POOL_ALLOCATOR_H */

