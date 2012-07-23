//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_TREAP_TABLE_HPP
#define SIB_TREAP_TABLE_HPP

#include <cstddef>
#include <stdint.h>

#include <boost/call_traits.hpp>
#include <boost/type_traits.hpp>

namespace sib
{
  namespace treap_internal
  {
    typedef uint32_t heap_key_type;

    /*template <typename X, bool Copy=true>
    struct property
    {
      typedef boost::call_traits<X const>::param_type const_param_type;
      typedef boost::call_traits<X>::param_type param_type;

      template <typename Allocator>
      property(Allocator& alloc);

      const_param_type value() const;
      param_type value(); 

      X _value;
    };

    template <typename X>
    struct property<X,false>
    {
      typedef boost::call_traits<X const>::param_type const_param_type;
      typedef boost::call_traits<X>::param_type param_type;

      template <typename Allocator>
      property(Allocator& alloc);
      
      const_param_type value() const;
      param_type value(); 

      X* _value;
    };*/

    template <typename K, typename V>
    struct helper
    {
      static bool const copy_key = 
        boost::is_fundamental<K>::value || boost::is_pointer<K>::value;
      static bool const copy_value = copy_key &&
        (boost::is_fundamental<typename V::second_type>::value ||
        boost::is_pointer<typename V::second_type>::value);

      typedef typename boost::call_traits<K>::param_type key_param_type;
      typedef typename boost::call_traits<V>::param_type value_param_type;

      static inline key_param_type key(value_param_type v) { return v.first; }

      static inline void update(V& dest, value_param_type v) { dest.second = v.second; }
    };

    template <typename K>
    struct helper<K,K>
    {
      static bool const copy_key = 
        boost::is_fundamental<K>::value || boost::is_pointer<K>::value;
      static bool const copy_value = copy_key;

      typedef typename boost::call_traits<K>::param_type key_param_type;
      typedef typename boost::call_traits<K>::param_type value_param_type;

      static inline key_param_type key(key_param_type k) { return k; }

      static inline void update(K& dest, key_param_type from) { dest = from; }
    };

    /*template <typename K, typename V, bool CopyK=true, bool CopyV=true>
    struct entry
    {
      typedef boost::call_traits<K>::param_type key_param_type;
      typedef boost::call_traits<V>::param_type value_param_type;

      template <typename Allocator>
      entry(heap_key_type heap_key, value_param_type v) 
      : _heap_key(heap_key), _data(v) {}

      template <typename Allocator>
      void release(Allocator& alloc) {}

      key_param_type key() const        { return helper<K,V>::key(_data); }
      value_param_type value() const    { return _data; }
      V& value()                        { return _data; }

      heap_key_type const _heap_key;
      V _data;
    };

    template <typename K, typename V, bool CopyV>
    struct entry<K,T,false,CopyV>
    {
      typedef boost::call_traits<K>::param_type key_param_type;
      typedef boost::call_traits<V>::param_type value_param_type;

      template <typename Allocator>
      entry(heap_key_type heap_key, value_param_type v, Allocator& alloc)
      : _heap_key(heap_key), _data(new(alloc.allocate(1)) V(v)) {}
      
      template <typename Allocator>
      void release(Allocator& alloc)    { _data->~V(); alloc.deallocate(_data, 1); }

      key_param_type key() const        { return helper<K,V>::key(*_data); }
      value_param_type value() const    { return *_data; }
      V& value()                        { return *_data; }

      heap_key_type _heap_key;
      V* _data;
    };

    template <typename K, typename T, typename V>
    struct entry<K,T,true,false>
    {
      typedef boost::call_traits<K>::param_type key_param_type;
      typedef boost::call_traits<V>::param_type value_param_type;

      template <typename Allocator>
      entry(heap_key_type heap_key, value_param_type v, Allocator& alloc)
      : _heap_key(heap_key), _key(v.first), _data(new(alloc.allocate(1)) V(v)) {}
      
      template <typename Allocator>
      void release(Allocator& alloc) { _data->~V(); alloc.deallocate(_data, 1); }

      key_param_type key() const { return _key; }
      value_param_type value() const { return *_data; }
      V& value() { return *_data; }

      heap_key_type const _heap_key;
      K const _key;
      V* _data;
    };*/

    template <typename V>
    struct node
    {
      typedef typename boost::call_traits<V>::param_type param_type;

      node(param_type data);

      void rotate_left();
      void rotate_right();

      V _data;
      heap_key_type _heap_key;
      node* _parent;
      node* _left;
      node* _right;
    };
  };

  template <typename K, typename V,
            typename Compare, typename Rand,
            typename Allocator>
  class treap_table
  {
  private:
    typedef treap_internal::node<V> node_type;
    typedef treap_internal::helper<K,V> helper;
    typedef typename helper::key_param_type key_param_type;
    typedef typename helper::value_param_type value_param_type;
  public:
    typedef std::size_t size_type;

    // 'structors
    treap_table();
    ~treap_table();

    // iterators
    V const* end() const          { return NULL; }

    // capacity
    bool empty() const            { return 0 == _size; }
    size_type size() const        { return _size; }

    // modify
    void clear();
    void insert(key_param_type key, value_param_type value);

    // lookup
    V const* find(key_param_type key) const;
  private:
    void delete_sub_tree(node_type* node);
    void push_up(node_type* node);

    node_type*    _root;
    size_type     _size;
    Compare       _compare;
    Rand          _rand;
    typename Allocator::template rebind<node_type>::other     _alloc;
  };
}

#include "treap_table.inl"

#endif /* SIB_TREAP_TABLE_HPP */

