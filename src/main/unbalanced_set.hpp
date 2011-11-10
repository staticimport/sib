#ifndef ZTL_UNBALANCED_SET_HPP
#define ZTL_UNBALANCED_SET_HPP

namespace ztd {
  namespace ns_unbalanced_set {
    template <typename T>
    struct traits {

      class const_iterator {

      };

      class iterator {

      };
    };
  };

  template <typename T,
            typename Compare=std::less<T>,
            typename Allocator = std::allocator<T> >
  class unbalanced_set {
  private:
    typedef ns_unbalanced_set::traits<T> traits;
  public:
    typedef Allocator allocator_type;
    typedef typename traits::const_iterator const_iterator;
    typedef typename Allocator::const_pointer const_pointer;
    typedef typename Allocator::const_reference const_reference;
    typedef typename Allocator::difference_type difference_type;
    typedef typename traits::iterator iterator;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::size_type size_type;
    typedef typename Allocator::value_type value_type;

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

#endif /* ZTL_UNBALANCED_SET_HPP */

