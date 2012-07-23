//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_COMPARE_HPP
#define SIB_COMPARE_HPP

#include <boost/call_traits.hpp>

//#include <string>

namespace sib
{
  typedef int comparison_type;

  template <typename T>
  struct comparator
  {
  private:
    typedef typename boost::call_traits<T>::param_type param_type;
  public:
    inline comparison_type operator()(param_type a, param_type b) const
    {
      return a < b ? -1 : (!(a == b) ? 1 : 0);
    }
  };

  template <>
  struct comparator<std::string>
  {
    inline comparison_type operator()(std::string const& a, std::string const& b) const
    {
      return static_cast<comparison_type>(a.compare(b));
    }
  };
}

#endif /* SIB_COMPARE_HPP */

