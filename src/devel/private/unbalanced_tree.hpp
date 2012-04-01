#ifndef SIB_UNBALANCED_SET_HPP
#define SIB_UNBALANCED_SET_HPP

#include "private/tree.hpp"

namespace sib {
  namespace ns_unbalanced_tree {
    template <typename T>
    struct traits {

      class const_iterator {

      };

      class iterator {

      };
    };
  };

  template <typename K,
            typename T,
            typename Node,
            typename Compare=std::less<K>,
            typename Allocator = std::allocator<T> >
  class unbalanced_tree : 
  private tree<K,T,Node,Compare,Allocator> {
  private:
    typedef ns_unbalanced_set::traits<T> traits;
  public:
    // Base types
    typedef Allocator allocator_type;
    typedef typename tree::iterator<Node const, T const> const_iterator;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::difference_type difference_type;
    typedef typename tree::iterator<Node,T> iterator;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::size_type size_type;
    typedef typename Allocator::value_type value_type;

    // Init/Uninit
    explicit unbalanced_tree(Compare const& compare = Compare(),
                             Allocator const& allocator = Allocator());
    //template <typename InputIterator>
    //unbalanced_set(InputIterator first, InputIterator last,
    //               Compare const& compare = Compare(),
    //               Allocator const& allocator = Allocator());
    //unbalanced_set(unbalanced_set<T,Compare,Allocator> const& set);
    //unbalanced_set<T,Compare,Allocator>&
    //operator=(unbalanced_set<T,Compare,Allocator> const& set);



  private:
  };
}

/**
 * Implementation
 **/


#endif /* SIB_UNBALANCED_SET_HPP */

