#ifndef SIB_TREE_HPP
#define SIB_TREE_HPP

namespace sib {
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

    template <typename K, typename T>
    class map_node {
    public:
      typedef K key_type;
      typedef T mapped_type;
      typedef std::pair<K const, T> value_type;

      key_type const& key() const;
      mapped_type& mapped();
      mapped_type const& mapped() const;
      value_type& value()&
      value_type const& value() const;
    protected:
      value_type _value;
    };

    template <typename T>
    class set_node {
    public:
      typedef T key_type;
      typedef T value_type;

      key_type const& key() const;
      value_type const& value() const;
    protected:
      V _value;
    };

    template <typename N>
    static N* find(N* node);

    template <typename N>
    static std::size_t height(N* node);

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
inline sib::tree::iterator<N,T>::iterator()
: _node(0)
{
}

template <typename N, typename T>
inline sib::tree::iterator<N,T>::iterator(N* const node)
: _node(node)
{
}

template <typename N, typename T>
inline sib::tree::iterator<N,T>::reference
sib::tree::iterator<N,T>::operator*() const
{
  return _node->value;
}

template <typename N, typename T>
inline sib::tree::iterator<N,T>::pointer
sib::tree::iterator<N,T>::operator->() const
{
  return &(_node->value);
}

template <typename N, typename T>
sib::tree::iterator<N,T>&
sib::tree::iterator<N,T>::operator++() const
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
inline sib::tree::iterator<N,T>&
sib::tree::iterator<N,T>::operator++(int)
{
  N* const old(_node);
  operator++();
  return *old;
}

template <typename N, typename T>
sib::tree::iterator<N,T>&
sib::tree::iterator<N,T>::operator--()
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
inline sib::tree::iterator<N,T>&
sib::tree::iterator<N,T>::operator--(int)
{
  N* const old(_node);
  operator--();
  return *old;
}

template <typename N, typename T>
inline bool
sib::tree::iterator<N,T>::operator==(sib::tree::iterator<N,T> const& iter) const
{
  return _node == iter._node;
}

template <typename N, typename T>
inline bool
sib::tree::iterator<N,T>::operator!=(sib::tree::iterator<N,T> const& iter) const
{
  return _node != iter._node;
}

template <typename K, typename T>
inline sib::tree::map_node<K,T>::key_type const&
sib::tree::map_node<K,T>::key() const
{
  return _value.first();
}

template <typename K, typename T>
inline sib::tree::map_node<K,T>::mapped_type&
sib::tree::map_node<K,T>::mapped()
{
  return _value.second();
}

template <typename K, typename T>
inline sib::tree::map_node<K,T>::mapped_type const&
sib::tree::map_node<K,T>::mapped() const
{
  return _value.second();
}

template <typename K, typename T>
inline sib::tree::map_node<K,T>::value_type&
sib::tree::map_node<K,T>::value()
{
  return _value;
}

template <typename K, typename T>
inline sib::tree::map_node<K,T>::value_type const&
sib::tree::map_node<K,T>::value() const
{
  return _value;
}

template <typename T>
inline sib::tree::set_node<T>::key_type const&
sib::tree::set_node<T>::key() const
{
  return _value;
}

template <typename T>
inline sib::tree::set_node<T>::value_type const&
sib::tree::set_node<T>::value() const
{
  return _value;
}

template <typename N, typename Compare, typename K>
inline N*
sib::tree::find(N* const node, K const& key)
{
  while(node) {
    if (Compare(key, node->key())) {
      node = node->left;
    } else if (Compare(node->key(), key)) {
      node = node->right;
    } else {
      break;
    }
  }
  return node;
}
    
template <typename N>
static std::size_t height(N* node)
{
  if (node) {
    return 1 + std::max(height(node->left, node->right));
  } else {
    return 0;
  }
}

template <typename N>
inline N*
sib::tree::maximum<N>(N* node)
{
  while (node->right) node = node->right;
  return node;
}

template <typename N>
inline N*
sib::tree::minimum<N>(N* node)
{
  while (node->left) node = node->left;
  return node;
}

template <typename N>
N*
sib::tree::rotate_left(N* node)
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
sib::tree::rotate_right(N* node)
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

#endif /* SIB_TREE_HPP */

