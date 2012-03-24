
#include <gtest/gtest.h>

#include "hash_map.hpp"

#define SIZET(x) static_cast<std::size_t>(x)

TEST(hash_map, new_map_empty)
{
  sib::hash_map<int,int> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(SIZET(0), map.size());
}

TEST(hash_map, operator_insert)
{
  sib::hash_map<int,int> map;
  map[0] = 1;
  EXPECT_FALSE(map.empty());
  EXPECT_EQ(SIZET(1), map.size());
  map[2] = 3;
  EXPECT_EQ(SIZET(2), map.size());
  map[-1] = -2;
  EXPECT_EQ(SIZET(3), map.size());
}
