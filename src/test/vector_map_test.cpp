
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <gtest/gtest.h>

#include "vector_map.hpp"

#define SZT(x) static_cast<std::size_t>(x)

TEST(vector_map, new_vector_map_empty)
{
  sib::vector_map<int,int> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(SZT(0), map.size());
}

TEST(vector_map, insert_one)
{
  sib::vector_map<int,int> map;
  std::pair<int,int> x(1,2);
  map.insert(x);
  EXPECT_FALSE(map.empty());
  EXPECT_EQ(SZT(1), map.size());
  EXPECT_EQ(123, map.at(0));
  EXPECT_EQ(123, map[0]);
}

