//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_TREAP_SET_HPP
#define SIB_TREAP_SET_HPP

#include "compare.hpp"
#include "random.hpp"
#include "treap_table.hpp"

#include <functional>

namespace sib
{
  template <typename T,
            typename Compare=sib::comparator<T>,
            typename RandGen=sib::standard_random_generator,
            typename Allocator=std::allocator<T> >
  class treap_set
  {
  private:
    typedef typename boost::call_traits<T>::param_type param_type;
    typedef treap_table<T,T,Compare,RandGen,Allocator> table_type;
  public:
    typedef typename table_type::size_type        size_type;

    // 'structors
    treap_set() {}

    // iterators
    T const* end() const { return NULL; }

    // modify
    void clear()              { _table.clear(); }
    void insert(param_type x) { _table.insert(x,x); }

    // find
    T const* find(param_type x) const { return _table.find(x); }
  private:
    table_type _table;
  };
}

#endif /* SIB_TREAP_SET_HPP */
