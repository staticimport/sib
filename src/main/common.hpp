#ifndef SIB_COMMON_HPP
#define SIB_COMMON_HPP

#ifdef HAS_BOOST
#define SIB_BOOST
#include <boost/call_traits.hpp>
#endif

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#define SIB_CXX11
#define SIB_NOEXCEPT noexcept
#else
#define SIB_NOEXCEPT 
#endif

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

