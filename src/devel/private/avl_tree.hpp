#ifndef SIB_AVL_TREE_HPP
#define SIB_AVL_TREE_HPP

#include "private/tree.hpp"

namespace sib {
  namespace tree {
    template <typename T>
    struct avl_node {
      typedef unsigned height_type;

      avl_node* parent;
      avl_node* left;
      avl_node* right;
      height_type height;
      T value;
    };

    template <typename T,
              typename Compare = std::less<T>,
              typename Allocator = std::allocator<T> >
    class avl_tree {
    public:
    private:
      Compare _compare;
      Allocator _allocator;
      avl_node<T>* _root;
      avl_node<T>* _begin;
      avl_node<T>* _end;
    };
  };
}

#endif /* SIB_AVL_TREE_HPP */

