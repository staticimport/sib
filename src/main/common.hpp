//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_COMMON_HPP
#define SIB_COMMON_HPP

#ifdef HAS_BOOST
#define SIB_BOOST
#include <boost/call_traits.hpp>
#endif

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#define SIB_CXX11
//#define SIB_NOEXCEPT noexcept
#define SIB_NOEXCEPT 
#else
#define SIB_NOEXCEPT 
#endif

#define SIB_LIKELY(x) __builtin_expect((x),1)
#define SIB_UNLIKELY(x) __builtin_expect((x),0)

namespace sib
{
  template <typename T>
  struct param
  {
#ifdef HAS_BOOST
    typedef boost::call_traits<T>::param_type type;
#else
    typedef T const& type;
#endif
  };
}

#endif /* SIB_COMMON_HPP */

