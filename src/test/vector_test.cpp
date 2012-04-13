//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <utility>

#include "vector.hpp"

#define SZT(x) static_cast<std::size_t>(x)

static std::vector<int> generate_ints(std::size_t const count)
{
  std::vector<int> v;
  for(std::size_t ii = 0; ii != count; ++ii)
    v.push_back(ii);
  return v;
}

static std::vector<std::string> generate_strings(std::size_t const count)
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
static void test_new()
{
  sib::vector<T> v1;
  EXPECT_TRUE(v1.empty());
  EXPECT_EQ(SZT(0), v1.size());
  EXPECT_TRUE(v1.capacity() > 0);
  EXPECT_TRUE(v1.max_size() > 100000000);
  sib::vector<T> v2(1);
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(SZT(1), v2.capacity());
}

template <typename T>
static void test_push_back()
{
  sib::vector<T> v;
  for(std::size_t ii = 1; ii != 100; ++ii) {
    v.push_back(T());
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(ii, v.size());
  }
}

template <typename T>
static void test_iterators(std::vector<T> const& data)
{
  sib::vector<T> v;
  for(std::size_t ii = 0; ii != data.size(); ++ii) {
    v.push_back(data[ii]);
    typename sib::vector<T>::iterator iter = v.begin();
    for(std::size_t jj = 0; jj <= ii; ++jj) {
      EXPECT_FALSE(iter == v.end());
      EXPECT_EQ(data[jj], *(iter++));
    }
    EXPECT_TRUE(iter == v.end());
    typename sib::vector<T>::const_iterator citer = v.begin();
    for(std::size_t jj = 0; jj <= ii; ++jj) {
      EXPECT_FALSE(citer == v.end());
      EXPECT_EQ(data[jj], *(citer++));
    }
    EXPECT_TRUE(citer == v.end());
    typename sib::vector<T>::reverse_iterator riter = v.rbegin();
    for(int jj = ii; jj >= 0; --jj) {
      EXPECT_FALSE(riter == v.rend());
      EXPECT_EQ(data[jj], *(riter++));
    }
    EXPECT_TRUE(riter == v.rend());
    typename sib::vector<T>::const_reverse_iterator criter = v.rbegin();
    for(int jj = ii; jj >= 0; --jj) {
      EXPECT_FALSE(criter == v.rend());
      EXPECT_EQ(data[jj], *(criter++));
    }
    EXPECT_TRUE(criter == v.rend());
  }
}

template <typename T>
void verify_equal(sib::vector<T>& v1, std::vector<T>& v2)
{
  EXPECT_EQ(v2.size(), v1.size());
  typename sib::vector<T>::iterator i1 = v1.begin();
  typename std::vector<T>::iterator i2 = v2.begin();
  while (i1 != v1.end()) 
    EXPECT_EQ(*(i2++), *(i1++));
  EXPECT_TRUE(i2 == v2.end());
}

template <typename T>
void test_insert(std::vector<T> const& data)
{
  sib::vector<T> sib_v;
  std::vector<T> std_v;
  for(std::size_t ii = 0; ii != data.size(); ++ii) {
    for(std::size_t jj = 0; jj != ii; ++jj) {
      for(std::size_t kk = 0; kk != ii; ++kk) {
        sib_v.push_back(data[kk]);
        std_v.push_back(data[kk]);
      }
      typename sib::vector<T>::iterator sib_i = sib_v.insert(sib_v.begin()+jj,T());
      typename std::vector<T>::iterator std_i = std_v.insert(std_v.begin()+jj,T());
      verify_equal<T>(sib_v, std_v);
      EXPECT_EQ(std_i - std_v.begin(), sib_i - sib_v.begin());
      sib_v.clear();
      std_v.clear();
    }
  }
}

template <typename T>
void test_pop_back(std::vector<T> const& data)
{
  sib::vector<T> sib_v;
  std::vector<T> std_v;
  for(std::size_t ii = 0; ii != data.size(); ++ii) {
    for(std::size_t jj = 0; jj != ii; ++jj) {
      sib_v.push_back(data[jj]);
      std_v.push_back(data[jj]);
    }
    for(std::size_t jj = 0; jj != ii; ++jj) {
      sib_v.pop_back();
      std_v.pop_back();
      verify_equal<T>(sib_v, std_v);
    }
  }
}

template <typename T>
void test_resize(std::vector<T> const& data)
{
  sib::vector<T> sib_v;
  std::vector<T> std_v;
  for(std::size_t ii = 0; ii != data.size(); ++ii) {
    sib_v.push_back(data[ii]);
    std_v.push_back(data[ii]);
  }
  for(std::size_t ii = 2*data.size(); ii; --ii) {
    sib_v.resize(ii);
    std_v.resize(ii);
    verify_equal<T>(sib_v, std_v);
  }
}

template <typename T>
void test_lookup(std::vector<T> const& data)
{
  sib::vector<T> sib_v;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    sib_v.push_back(data[ii]);
  EXPECT_EQ(data.back(), sib_v.back());
  EXPECT_EQ(data.front(), sib_v.front());
  for(std::size_t ii = 0; ii != data.size(); ++ii) {
    EXPECT_EQ(data.at(ii), sib_v.at(ii));
    EXPECT_EQ(data[ii], sib_v[ii]);
  }
  EXPECT_THROW(sib_v.at(sib_v.size()), std::runtime_error);
}

template <typename T>
void test_swap(std::vector<T> const& data)
{
  sib::vector<T> v1;
  sib::vector<T> v2;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    v1.push_back(data[ii]);
  v1.swap(v2);
  EXPECT_TRUE(v1.empty());
  EXPECT_EQ(data.size(), v2.size());
  v1.swap(v2);
  EXPECT_TRUE(v2.empty());
  EXPECT_EQ(data.size(), v1.size());
}

template <typename T>
void test_assignment(std::vector<T> const& data)
{
  sib::vector<T> v1;
  sib::vector<T> v2;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    v1.push_back(data[ii]);
  v2 = const_cast<sib::vector<T> const&>(v1);
  EXPECT_EQ(v1.size(), v2.size());
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(v1.at(ii) == v2.at(ii));
}

template <typename T>
void test_copy(std::vector<T> const& data)
{
  sib::vector<T> v1;
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    v1.push_back(data[ii]);
  sib::vector<T> v2(const_cast<sib::vector<T> const&>(v1));
  EXPECT_EQ(v1.size(), v2.size());
  for(std::size_t ii = 0; ii != data.size(); ++ii)
    EXPECT_TRUE(v1.at(ii) == v2.at(ii));
}

TEST(vector, pod_new)
{
  test_new<int>();
  test_new<char>();
  test_new<double>();
}

TEST(vector, class_new)
{
  test_new<std::string>();
  test_new<std::pair<const int,int> >();
}

TEST(vector, pod_push_back)
{
  test_push_back<int>();
  test_push_back<char>();
  test_push_back<double>();
}

TEST(vector, class_push_back)
{
  test_push_back<std::string>();
  test_push_back<std::pair<const int, float> >();
}

TEST(vector, pod_iterators)
{
  std::vector<int> data;
  for(int ii = 0; ii != 30; ++ii)
    data.push_back(ii);
  test_iterators<int>(data);
}

TEST(vector, class_iterators)
{
  std::vector<std::string> data;
  for(int ii = 0; ii != 30; ++ii)
    data.push_back("" + ii);
  test_iterators<std::string>(data);
}

TEST(vector, pod_clear)
{
  sib::vector<int> sib_v;
  sib_v.clear();
  EXPECT_TRUE(sib_v.empty());
  EXPECT_EQ(SZT(0), sib_v.size());
  for(int ii = 0; ii != 100; ++ii)
    sib_v.push_back(ii);
  sib_v.clear();
  EXPECT_TRUE(sib_v.empty());
  EXPECT_EQ(SZT(0), sib_v.size());
}

TEST(vector, class_clear)
{
  sib::vector<std::string> sib_v;
  sib_v.clear();
  EXPECT_TRUE(sib_v.empty());
  EXPECT_EQ(SZT(0), sib_v.size());
  for(int ii = 0; ii != 100; ++ii)
    sib_v.push_back("");
  sib_v.clear();
  EXPECT_TRUE(sib_v.empty());
  EXPECT_EQ(SZT(0), sib_v.size());
}

TEST(vector, pod_erase)
{
  sib::vector<int> sib_v;
  std::vector<int> std_v;
  for(std::size_t ii = 0; ii != 25; ++ii) {
    for(std::size_t jj = 0; jj != 25; ++jj) {
      sib_v.push_back(jj);
      std_v.push_back(jj);
    }
    typename sib::vector<int>::iterator sib_e = sib_v.erase(sib_v.begin()+ii);
    typename std::vector<int>::iterator std_e = std_v.erase(std_v.begin()+ii);
    verify_equal<int>(sib_v, std_v);
    EXPECT_TRUE(sib_e - sib_v.begin() == std_e - std_v.begin());
    sib_v.clear();
    std_v.clear();
  }
}

TEST(vector, class_erase)
{
  sib::vector<std::string> sib_v;
  std::vector<std::string> std_v;
  for(std::size_t ii = 0; ii != 25; ++ii) {
    for(std::size_t jj = 0; jj != 25; ++jj) {
      sib_v.push_back(""+jj);
      std_v.push_back(""+jj);
    }
    typename sib::vector<std::string>::iterator sib_e = sib_v.erase(sib_v.begin()+ii);
    typename std::vector<std::string>::iterator std_e = std_v.erase(std_v.begin()+ii);
    verify_equal<std::string>(sib_v, std_v);
    EXPECT_TRUE(sib_e - sib_v.begin() == std_e - std_v.begin());
    sib_v.clear();
    std_v.clear();
  }
}

TEST(vector, pod_insert)
{
  test_insert<int>(generate_ints(20));
}

TEST(vector, class_insert)
{
  test_insert<std::string>(generate_strings(20));
}

TEST(vector, pod_pop_back)
{
  test_pop_back<int>(generate_ints(20));
}

TEST(vector, class_pop_back)
{
  test_pop_back<std::string>(generate_strings(20));
}

TEST(vector, pod_resize)
{
  test_resize<int>(generate_ints(20));
}

TEST(vector, class_resize)
{
  test_resize<std::string>(generate_strings(20));
}

TEST(vector, pod_lookup)
{
  test_lookup<int>(generate_ints(20));
}

TEST(vector, class_lookup)
{
  test_lookup<std::string>(generate_strings(20));
}

TEST(vector, pod_swap)
{
  test_swap<int>(generate_ints(20));
}

TEST(vector, class_swap)
{
  test_swap<std::string>(generate_strings(20));
}

TEST(vector, pod_assignment)
{
  test_assignment<int>(generate_ints(20));
}

TEST(vector, class_assignment)
{
  test_assignment<std::string>(generate_strings(20));
}

TEST(vector, pod_copy_constructor)
{
  test_copy<int>(generate_ints(20));
}

TEST(vector, class_copy_constructor)
{
  test_copy<std::string>(generate_strings(20));
}

