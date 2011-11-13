
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


