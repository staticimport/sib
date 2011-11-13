#ifndef SIB_UNBALANCED_SET_HPP
#define SIB_UNBALANCED_SET_HPP

namespace sib {
  namespace ns_unbalanced_set {
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
            typename Compare=std::less<K>,
            typename Allocator = 
              std::allocator<std::pair<K,T> > >
  class unbalanced_tree {
  private:
    typedef ns_unbalanced_set::traits<T> traits;
    typedef tree::node<T> node;
  public:
    // Base types
    typedef Allocator allocator_type;
    typedef typename tree::const_iterator< const_iterator;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::difference_type difference_type;
    typedef typename traits::iterator iterator;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::size_type size_type;
    typedef typename Allocator::value_type value_type;

    // Iterator types
    typedef typename tree::iterator<node const, value_type const> const_iterator;
    typedef typename tree::iterator<node,value_type> iterator;

    // Init/Uninit
    explicit unbalanced_set(Compare const& compare = Compare(),
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

#endif /* SIB_UNBALANCED_SET_HPP */

