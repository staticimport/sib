//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_RANDOM_HPP
#define SIB_RANDOM_HPP

#include <cstdlib>

namespace sib
{
  typedef int rand_type;

  struct standard_random_generator
  {
    inline rand_type operator()() const
    {
      return rand();
    }
  };
};

#endif /* SIB_RANDOM_HPP */

