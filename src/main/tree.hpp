#ifndef ZTD_TREE_HPP
#define ZTD_TREE_HPP

namespace ztd {
  namespace tree {
    template <typename N, typename T>
    class iterator {
    public:
      typedef std::bidirectional_iterator_tag iterator_category;

      typedef T* pointer;
      typedef T& reference;
      typedef T value_type;
      
      iterator();
      explicit iterator(N* const node);
      
      reference operator*() const;
      pointer operator->() const;
      iterator& operator++();
      iterator& operator++(int);
      iterator& operator--();
      iterator& operator--(int);
      bool operator==(iterator const& iter) const;
      bool operator!=(iterator const& iter) const;
    private:
      N* _node;
    };

    template <typename T,
              typename Compare,
              typename Allocator,
              typename Node>
    class tree {
    protected:
      
      Compare _compare;
      Allocator _allocator;
      Node* _root;
      Node* _begin;
      Node* _end;
    };

    template <typename N>
    static N* maximum(N* node);

    template <typename N>
    static N* minimum(N* node);

    template <typename N>
    static N* rotate_left(N* node);

    template <typename N>
    static N* rotate_right(N* node);
  }
}

/**
 * Template Implemenation
 **/
template <typename N, typename T>
inline ztd::tree::iterator<N,T>::iterator()
: _node(0)
{
}

template <typename N, typename T>
inline ztd::tree::iterator<N,T>::iterator(N* const node)
: _node(node)
{
}

template <typename N, typename T>
inline ztd::tree::iterator<N,T>::reference
ztd::tree::iterator<N,T>::operator*() const
{
  return _node->value;
}

template <typename N, typename T>
inline ztd::tree::iterator<N,T>::pointer
ztd::tree::iterator<N,T>::operator->() const
{
  return &(_node->value);
}

template <typename N, typename T>
ztd::tree::iterator<N,T>&
ztd::tree::iterator<N,T>::operator++() const
{
  if (_node->right) {
    _node = minimum(_node->right);
  } else {
    while (_node->parent) {
      if (_node->parent->left == _node) {
        _node = _node->parent;
        break;
      } else {
        _node = _node->parent;
      }
    }
  }
  return _node;
}

template <typename N, typename T>
inline ztd::tree::iterator<N,T>&
ztd::tree::iterator<N,T>::operator++(int)
{
  N* const old(_node);
  operator++();
  return *old;
}

template <typename N, typename T>
ztd::tree::iterator<N,T>&
ztd::tree::iterator<N,T>::operator--()
{
  if (_node->left) {
    _node = maximum(_node->left);
  } else {
    while (_node->parent) {
      if (_node->parent->right == _node) {
        _node = _node->parent;
        break;
      } else {
        _node = _node->parent;
      }
    }
  }
  return _node;
}

template <typename N, typename T>
inline ztd::tree::iterator<N,T>&
ztd::tree::iterator<N,T>::operator--(int)
{
  N* const old(_node);
  operator--();
  return *old;
}

template <typename N, typename T>
inline bool
ztd::tree::iterator<N,T>::operator==(ztd::tree::iterator<N,T> const& iter) const
{
  return _node == iter._node;
}

template <typename N, typename T>
inline bool
ztd::tree::iterator<N,T>::operator!=(ztd::tree::iterator<N,T> const& iter) const
{
  return _node != iter._node;
}

template <typename N>
inline N*
ztd::tree::maximum<N>(N* node)
{
  while (node->right) node = node->right;
  return node;
}

template <typename N>
inline N*
ztd::tree::minimum<N>(N* node)
{
  while (node->left) node = node->left;
  return node;
}

template <typename N>
N*
ztd::tree::rotate_left(N* node)
{
  // Update child pointers
  N* top = node->right;
  node->right = top->left;
  top->left = node;

  // Update parent pointers
  if ((top->parent = node->parent))  {
    if (node->parent->left == node) {
      node->parent->left = top;
    } else {
      node->parent->right = top;
    }
  }
  if (node->right) node->right->parent = node;
  node->parent = top;
  return top;
}

template <typename N>
N*
ztd::tree::rotate_right(N* node)
{
  // Update child pointers
  N* top = node->left;
  node->left = top->right;
  top->right = node;

  // Update parent pointers
  if ((top->parent = node->parent)) {
    if (node->parent->left == node) {
      node->parent->left = top;
    } else {
      node->parent->right = top;
    }
  }
  if (node->left) node->left->parent = node;
  node->parent = top;
  return top;
}

#endif /* ZTD_TREE_HPP */

