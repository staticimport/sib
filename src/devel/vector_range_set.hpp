//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_VECTOR_RANGE_SET_HPP
#define SIB_VECTOR_RANGE_SET_HPP

#include <cstddef>
#include <utility>
#include <vector>

namespace sib
{
  template <typename T>
  class vector_range_set
  {
  public:
    bool contains(T const x) const;
    bool empty() const              { return _ranges.empty(); }
    void erase(T const x);
    void insert(T begin, T end);
    std::pair<T,T> const& operator[](unsigned index) const { return _ranges[index]; }
    std::size_t size() const { return _ranges.size(); }
  private:
    void remove(std::pair<T,T>& range);

    std::vector<std::pair<T,T> > _ranges;
  };
}

#include "vector_range_set.inl"

#endif /* SIB_VECTOR_RANGE_SET_HPP */

