//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/foreach.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <gtest/gtest.h>
#include <set>

#include "vector_set.hpp"

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
static void verify_equal(sib::vector_set<T> const& sib_set,
                  std::set<T> const& std_set)
{
  EXPECT_EQ(std_set.size(), sib_set.size());
  for(typename std::set<T>::const_iterator iter = std_set.begin();
      iter != std_set.end();
      ++iter)
  {
    EXPECT_EQ(SZT(1), sib_set.count(*iter));
  }
}

template <typename T>
static void test_insert(std::vector<T> const& data)
{
  sib::vector_set<T> set;
  std::set<T> std_set;
  for(std::size_t ii = 1; ii < data.size(); ++ii) {
    std::pair<typename sib::vector_set<T>::iterator, bool> r1 = set.insert(data[ii]);
    std_set.insert(data[ii]);
    EXPECT_TRUE(r1.second);
    EXPECT_TRUE(data[ii] == *r1.first);
    verify_equal<T>(set, std_set);
    std::pair<typename sib::vector_set<T>::iterator, bool> r2 = set.insert(data[ii]);
    std_set.insert(data[ii]);
    EXPECT_FALSE(r2.second);
    EXPECT_TRUE(data[ii] == *r1.first);
    verify_equal<T>(set, std_set);
  }
}

template <typename T>
static void test_clear(std::vector<T> const& data)
{
  sib::vector_set<T> set;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    set.insert(data[ii]);
  set.clear();
  EXPECT_TRUE(set.empty());
  EXPECT_EQ(SZT(0), set.size());
}

template <typename T>
static void test_swap(std::vector<T> const& data)
{
  sib::vector_set<T> set1;
  sib::vector_set<T> set2;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    set1.insert(data[ii]);
  set1.swap(set2);
  EXPECT_TRUE(set1.empty());
  EXPECT_EQ(data.size(), set2.size());
  set1.swap(set2);
  EXPECT_TRUE(set2.empty());
  EXPECT_EQ(data.size(), set1.size());
}

template <typename T>
static void test_assignment(std::vector<T> const& data)
{
  sib::vector_set<T> set1;
  sib::vector_set<T> set2;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    set1.insert(data[ii]);
  set2 = const_cast<sib::vector_set<T> const&>(set1);
  EXPECT_EQ(set1.size(), set2.size());
  typename sib::vector_set<T>::const_iterator i1 = set1.begin();
  typename sib::vector_set<T>::const_iterator i2 = set2.begin();
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(*(i1++) == *(i2++));
}

template <typename T>
static void test_copy(std::vector<T> const& data)
{
  sib::vector_set<T> set1;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    set1.insert(data[ii]);
  sib::vector_set<T> set2(const_cast<sib::vector_set<T> const&>(set1));
  EXPECT_EQ(set1.size(), set2.size());
  typename sib::vector_set<T>::const_iterator i1 = set1.begin();
  typename sib::vector_set<T>::const_iterator i2 = set2.begin();
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(*(i1++) == *(i2++));
}

template <typename T>
static void test_lookup(std::vector<T> const& data)
{
  sib::vector_set<T> set;
  for(std::size_t ii = 1; ii != data.size(); ++ii)
    set.insert(data[ii]);
  sib::vector_set<T> const& const_ref = set;
  for(std::size_t ii = 1; ii != data.size(); ++ii) {
    EXPECT_TRUE(data[ii] == *set.find(data[ii]));
    EXPECT_TRUE(data[ii] == *const_ref.find(data[ii]));
    EXPECT_EQ(SZT(1), const_ref.count(data[ii]));
  }
  EXPECT_EQ(SZT(0), const_ref.count(data[0]));
  EXPECT_TRUE(set.end() == set.find(data[0]));
  EXPECT_TRUE(set.cend() == const_ref.find(data[0]));
}

TEST(vector_set, new_vector_set_empty)
{
  sib::vector_set<int> set;
  EXPECT_TRUE(set.empty());
  EXPECT_EQ(SZT(0), set.size());
}

TEST(vector_set, insert)
{
  test_insert<int>(generate_ints(100));
  test_insert<std::string>(generate_strings(100));
}

TEST(vector_set, clear)
{
  test_clear<int>(generate_ints(100));
  test_clear<std::string>(generate_strings(100));
}

TEST(vector_set, swap)
{
  test_swap<int>(generate_ints(100));
  test_swap<std::string>(generate_strings(100));
}

TEST(vector_set, assignment)
{
  test_assignment<int>(generate_ints(100));
  test_assignment<std::string>(generate_strings(100));
}

TEST(vector_set, copy_constructor)
{
  test_copy<int>(generate_ints(100));
  test_copy<std::string>(generate_strings(100));
}

TEST(vector_set, lookup)
{
  test_lookup<int>(generate_ints(100));
  test_lookup<std::string>(generate_strings(100));
}

