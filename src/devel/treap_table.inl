//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "compare.hpp"

namespace sib
{
  namespace treap_internal
  {
    template <typename V>
    inline node<V>::node(param_type data)
    : _data(data)
    , _left(NULL)
    , _right(NULL)
    {}

    template <typename V>
    inline void node<V>::rotate_left()
    {
      node* const new_top = _right;
      _right = new_top->_left;
      if (_right)
        _right->_parent = this;
      new_top->_left = this;
      new_top->_parent = _parent;
      if (_parent)
        (_parent->_left == this ? _parent->_left : _parent->_right) = new_top;
      _parent = new_top;
    }

    template <typename V>
    inline void node<V>::rotate_right()
    {
      node* const new_top = _left;
      _left = new_top->_right;
      if (_left)
        _left->_parent = this;
      new_top->_right = this;
      new_top->_parent = _parent;
      if (_parent)
        (_parent->_left == this ? _parent->_left : _parent->_right) = new_top;
      _parent = new_top;
    }
  };

  template <typename K, typename V, typename C, typename R, typename A>
  inline treap_table<K,V,C,R,A>::treap_table()
  : _root(NULL)
  , _size(0)
  {
  };

  template <typename K, typename V, typename C, typename R, typename A>
  inline treap_table<K,V,C,R,A>::~treap_table()
  {
    clear();
  }
  
  template <typename K, typename V, typename C, typename R, typename A>
  inline void treap_table<K,V,C,R,A>::clear()
  {
    delete_sub_tree(_root);
    _root = NULL;
  }

  template <typename K, typename V, typename C, typename R, typename A>
  void treap_table<K,V,C,R,A>::insert(key_param_type key, value_param_type value)
  {
    node_type* node = _root;
    comparison_type comparison;
    if (node) {
      do {
        comparison = _compare(key, helper::key(node->_data));
        if (comparison < 0) {
          if (node->_left)
            node = node->_left;
          else
            break;
        } else if (comparison) {
          if (node->_right)
            node = node->_right;
          else
            break;
        } else {
          helper::update(node->_data, value);
          return;
        }
      } while(true);
    }
    node_type* const new_node = new(_alloc.allocate(1,node)) node_type(value);
    new_node->_heap_key = static_cast<treap_internal::heap_key_type>(_rand());
    new_node->_parent = node;
    if (node) {
      (comparison < 0 ? node->_left : node->_right) = new_node;
      push_up(new_node);
    }
    if (new_node->_parent == NULL)
      _root = new_node;
    ++_size;
  }

  template <typename K, typename V, typename C, typename R, typename A>
  inline V const* treap_table<K,V,C,R,A>::find(key_param_type key) const
  {
    node_type const* node = _root;
    while (node) {
      comparison_type const comparison = _compare(key, helper::key(node->_data));
      if (comparison < 0)
        node = node->_left;
      else if (comparison)
        node = node->_right;
      else
        return &(node->_data);
    }
  }
  
  template <typename K, typename V, typename C, typename R, typename A>
  void treap_table<K,V,C,R,A>::delete_sub_tree(node_type* node)
  {
    if (NULL == node)
      return;
    delete_sub_tree(node->_left);
    delete_sub_tree(node->_right);
    _alloc.deallocate(node,1);
  }
  
  template <typename K, typename V, typename C, typename R, typename A>
  void treap_table<K,V,C,R,A>::push_up(node_type* node)
  {
    node_type* parent = node->_parent;
    while (parent && node->_heap_key > parent->_heap_key)
    {
      if (parent->_left == node)
        parent->rotate_right();
      else
        parent->rotate_left();
      parent = node->_parent;
    }
  }
};

