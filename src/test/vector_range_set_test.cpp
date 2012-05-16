//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>

#include "vector_range_set.hpp"

TEST(vector_range_set, init)
{
  sib::vector_range_set<int> set;
  EXPECT_TRUE(set.empty());
}

TEST(vector_range_set, insert_and_erase_small)
{
  sib::vector_range_set<int> set;
  set.insert(20,24);
  set.insert(10,10);
  EXPECT_FALSE(set.empty());
  for(int ii = 0; ii != 100; ++ii) {
    if (ii == 10 || (20 <= ii && ii <= 24))
      EXPECT_TRUE(set.contains(ii));
    else
      EXPECT_FALSE(set.contains(ii));
  }
  set.erase(10);
  for(int ii = 0; ii != 100; ++ii) {
    if (20 <= ii && ii <= 24)
      EXPECT_TRUE(set.contains(ii));
    else
      EXPECT_FALSE(set.contains(ii));
  }
  set.erase(21);
  for(int ii = 0; ii != 100; ++ii) {
    if (20 <= ii && ii <= 24 && ii != 21)
      EXPECT_TRUE(set.contains(ii));
    else
      EXPECT_FALSE(set.contains(ii));
  }
  set.erase(20);
  for(int ii = 0; ii != 100; ++ii) {
    if (22 <= ii && ii <= 24)
      EXPECT_TRUE(set.contains(ii));
    else
      EXPECT_FALSE(set.contains(ii));
  }
  set.erase(23);
  set.erase(24);
  EXPECT_FALSE(set.empty());
  set.erase(22);
  EXPECT_TRUE(set.empty());
  for(int ii = 0; ii != 100; ++ii)
    EXPECT_FALSE(set.contains(ii));
}

TEST(vector_range_set, insert_and_erase)
{
  int max = 1000;
  sib::vector_range_set<int> range_set;
  std::set<int> set;
  for(unsigned ii = 0; ii != 100; ++ii)
  {
    // insert
    int begin = rand() % max;
    int end = std::min(max, begin + (rand() % 100));
    range_set.insert(begin, end);
    for(int jj = begin; jj <= end; ++jj)
      set.insert(jj);

    // erase
    for(unsigned jj = 0; jj != (end-begin); ++jj) {
      int to_remove = rand() % max;
      range_set.erase(to_remove);
      EXPECT_FALSE(range_set.contains(to_remove));
      set.erase(to_remove);
    }
    for(int kk = 0; kk != max; ++kk)
      EXPECT_EQ(set.count(kk) != 0, range_set.contains(kk));
  }
}
