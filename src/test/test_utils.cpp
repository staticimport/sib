//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include "ring.hpp"
#include "test_utils.hpp"

void unit_test(std::string const& name, std::string const& result)
{
  std::cout << name << ": ";
  if (result.length() == 0) {
    std::cout << "pass\n";
  } else {
    std::cout << "FAIL --> " << result << std::endl;
  }
}


