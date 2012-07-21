//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_UTILITY_HPP
#define SIB_UTILITY_HPP

#include <cstddef>
#include <functional>

#include "common.hpp"

namespace sib
{
  template <typename T, unsigned Count, typename Equals=std::equal_to<T> >
  struct fixed_count
  {
    // Array
    static T* find(T* begin, T const& item);
    static T* find(T* begin, T const& item, Equals const& equals);

    // Non-Array, e.g. iterator
    template <typename I>
    static I find(I begin, I npos, T const& item);
    template <typename I>
    static I find(I begin, I npos, T const& item, Equals const& equals);
  };
}

#include "internal/utility.inl"

#endif /* SIB_UTILITY_HPP */

