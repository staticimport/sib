//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_XOR_LIST_HPP
#define SIB_XOR_LIST_HPP

#include <cstddef>

#include "common.hpp"

//#define USE_TAIL_SENTINEL

namespace sib
{
  template <typename T, typename Allocator=std::allocator<T> >
  class xor_list
  {
  public:
    typedef Allocator                           allocator_type;
    typedef typename Allocator::const_pointer   const_pointer;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::difference_type difference_type;
    typedef typename Allocator::pointer         pointer;
    typedef typename Allocator::reference       reference;
    typedef typename Allocator::size_type       size_type;
    typedef T                                   value_type;

    xor_list();
    ~xor_list();

    // element access
#ifdef USE_TAIL_SENTINEL
    reference back()               { return _tail_sentinel->previous(NULL)->_item; }
    const_reference back() const   { return _tail_sentinel->previous(NULL)->_item; }
#endif
    reference front()              { return _head->_item; }
    const_reference front() const  { return _head->_item; }

    // capacity
    bool empty() const             { return 0 == _size; }
    size_type max_size() const;
    size_type size() const         { return _size; }

    // modifiers
    void clear();
    //iterator erase(iterator pos);
    //iterator erase(iterator begin, iterator end);
    //iterator insert(iterator pos, typename param<T>::type item);
    void pop_back();
    void pop_front();
    void push_back(typename param<T>::type item);
    void push_front(typename param<T>::type item);
    //void resize(size_type count);
    //void swap(xor_list& other);
    //xor_list& operator=(xor_list const& other);

    template <typename OutStream>
    void print(OutStream& out) const;
  private:
    struct node
    {
      node(typename param<T>::type item) : _item(item) { }

      node* next(node* const prev);
      node* previous(node* const next);
      void update_link(node* const prev, node* const next);

      difference_type _link_xor;
      T _item;
    };

    //node* _tail_sentinel;
    node* _head;
    node* _tail;
    std::size_t _size;
    typename Allocator::template rebind<node>::other _alloc;
  };
}

//#include "internal/xor_list.inl"
#include "xor_list.inl"

#endif /* SIB_XOR_LIST_HPP */

