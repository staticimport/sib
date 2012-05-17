//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/foreach.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <gtest/gtest.h>
#include <map>

#include "array_hash_map.hpp"

#define SZT(x) static_cast<std::size_t>(x)

static std::vector<int> generate_ints(std::size_t count)
{
  std::vector<int> v;
  for(std::size_t ii = 0; ii != count; ++ii)
    v.push_back((int)ii);
  return v;
}

static std::vector<std::string> generate_strings(std::size_t count)
{
  std::vector<std::string> v;
  for(std::size_t ii = 0; ii != count; ++ii) {
    std::ostringstream oss;
    oss << ii;
    v.push_back(oss.str());
  }
  return v;
}

template <typename T>
static void verify_equal(sib::array_hash_map<T,T> const& sib_map,
                  std::map<T,T> const& std_map)
{
  EXPECT_EQ(std_map.size(), sib_map.size());
  for(typename std::map<T,T>::const_iterator iter = std_map.begin();
      iter != std_map.end();
      ++iter)
  {
    EXPECT_EQ(iter->second, sib_map.find(iter->first)->second);
  }
}

template <typename T>
static void test_insert(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map;
  std::map<T,T> std_map;
  for(std::size_t ii = 1; ii < data.size(); ++ii) {
    std::pair<T,T> p(data[ii],data[ii]);
    std::pair<typename sib::array_hash_map<T,T>::iterator, bool> r1 = map.insert(p);
    std_map.insert(p);
    EXPECT_TRUE(r1.second);
    EXPECT_TRUE(data[ii] == r1.first->first);
    EXPECT_TRUE(data[ii] == r1.first->second);
    verify_equal<T>(map, std_map);
    std::pair<T,T> p2(data[ii],data[0]);
    std::pair<typename sib::array_hash_map<T,T>::iterator, bool> r2 = map.insert(p2);
    std_map.insert(p2);
    EXPECT_FALSE(r2.second);
    EXPECT_TRUE(data[ii] == r1.first->first);
    EXPECT_TRUE(data[ii] == r2.first->second);
    verify_equal<T>(map, std_map);
    std::pair<typename sib::array_hash_map<T,T>::iterator, bool> r3 = map.insert(p);
    std_map.insert(p);
    EXPECT_FALSE(r3.second);
    EXPECT_TRUE(data[ii] == r1.first->first);
    EXPECT_TRUE(data[ii] == r2.first->second);
    verify_equal<T>(map, std_map);
  }
}

template <typename T>
static void test_clear(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    map.insert(std::pair<T,T>(data[ii],data[ii]));
  map.clear();
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(SZT(0), map.size());
}

template <typename T>
static void test_swap(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map1;
  sib::array_hash_map<T,T> map2;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    map1.insert(std::pair<T,T>(data[ii],data[ii]));
  map1.swap(map2);
  EXPECT_TRUE(map1.empty());
  EXPECT_EQ(data.size(), map2.size());
  map1.swap(map2);
  EXPECT_TRUE(map2.empty());
  EXPECT_EQ(data.size(), map1.size());
}

template <typename T>
static void test_assignment(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map1;
  sib::array_hash_map<T,T> map2;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    map1.insert(std::pair<T,T>(data[ii],data[ii]));
  map2 = const_cast<sib::array_hash_map<T,T> const&>(map1);
  EXPECT_EQ(map1.size(), map2.size());
  typename sib::array_hash_map<T,T>::iterator i1 = map1.begin();
  typename sib::array_hash_map<T,T>::iterator i2 = map2.begin();
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(*(i1++) == *(i2++));
}

template <typename T>
static void test_copy(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map1;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    map1.insert(std::pair<T,T>(data[ii],data[ii]));
  sib::array_hash_map<T,T> map2(const_cast<sib::array_hash_map<T,T> const&>(map1));
  EXPECT_EQ(map1.size(), map2.size());
  typename sib::array_hash_map<T,T>::iterator i1 = map1.begin();
  typename sib::array_hash_map<T,T>::iterator i2 = map2.begin();
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(*(i1++) == *(i2++));
}

template <typename T>
static void test_lookup(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map;
  for(std::size_t ii = 1; ii != data.size(); ++ii)
    map.insert(std::pair<T,T>(data[ii],data[ii]));
  sib::array_hash_map<T,T> const& const_ref = map;
  for(std::size_t ii = 1; ii != data.size(); ++ii) {
    EXPECT_TRUE(data[ii] == map.at(data[ii]));
    EXPECT_TRUE(data[ii] == const_ref.at(data[ii]));
    EXPECT_TRUE(data[ii] == map[data[ii]]);
    EXPECT_TRUE(data[ii] == map.find(data[ii])->first);
    EXPECT_TRUE(data[ii] == map.find(data[ii])->second);
    EXPECT_TRUE(data[ii] == const_ref.find(data[ii])->first);
    EXPECT_TRUE(data[ii] == const_ref.find(data[ii])->second);
    EXPECT_EQ(SZT(1), const_ref.count(data[ii]));
  }
  EXPECT_EQ(SZT(0), const_ref.count(data[0]));
  EXPECT_TRUE(map.end() == map.find(data[0]));
  EXPECT_TRUE(map.cend() == const_ref.find(data[0]));
  EXPECT_THROW(map.at(data[0]), std::out_of_range);
  EXPECT_THROW(const_ref.at(data[0]), std::out_of_range);
}

template <typename T>
static void test_operator_insert(std::vector<T> const& data)
{
  sib::array_hash_map<T,T> map;
  map[data[0]] = data[1];
  map[data[0]] = data[0];
  for(std::size_t ii = 1; ii != data.size(); ++ii) {
    map[data[ii]] = data[0];
    map[data[ii]] = data[ii];
  }
  EXPECT_EQ(data.size(), map.size());
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(data[ii] == map.at(data[ii]));
}

TEST(array_hash_map, new_array_hash_map_empty)
{
  sib::array_hash_map<int,int> map;
  EXPECT_TRUE(map.empty());
  EXPECT_EQ(SZT(0), map.size());
}

TEST(array_hash_map, insert)
{
  test_insert<int>(generate_ints(100));
  test_insert<std::string>(generate_strings(100));
}

TEST(array_hash_map, clear)
{
  test_clear<int>(generate_ints(100));
  test_clear<std::string>(generate_strings(100));
}

TEST(array_hash_map, swap)
{
  test_swap<int>(generate_ints(100));
  test_swap<std::string>(generate_strings(100));
}

/*TEST(array_hash_map, assignment)
{
  test_assignment<int>(generate_ints(100));
  test_assignment<std::string>(generate_strings(100));
}*/

/*TEST(array_hash_map, copy_constructor)
{
  test_copy<int>(generate_ints(100));
  test_copy<std::string>(generate_strings(100));
}*/

TEST(array_hash_map, lookup)
{
  test_lookup<int>(generate_ints(100));
  test_lookup<std::string>(generate_strings(100));
}

TEST(array_hash_map, operator_insert)
{
  test_operator_insert<int>(generate_ints(100));
  test_operator_insert<std::string>(generate_strings(100));
}

