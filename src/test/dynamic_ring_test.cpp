
#include <queue>
#ifdef _GLIBCXX_HAS_GTHREADS
#include <thread>
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <gtest/gtest.h>

#include "dynamic_ring.hpp"

TEST(dynamic_ring, new_dynamic_ringempty)
{
  sib::dynamic_ring<int> ring(16);
  EXPECT_TRUE(ring.empty() == true);
  EXPECT_TRUE(ring.size() == 0);
}

TEST(dynamic_ring, dynamic_ringpush_one)
{
  sib::dynamic_ring<int> ring(16);
  ring.push(423);
  EXPECT_TRUE(ring.empty() == false);
  EXPECT_TRUE(ring.size() == 1);
}

TEST(dynamic_ring, dynamic_ringpop_one)
{
  sib::dynamic_ring<int> ring(16);
  ring.push(423);
  ring.pop();
  EXPECT_TRUE(ring.empty() == true);
  EXPECT_TRUE(ring.size() == 0);
}

TEST(dynamic_ring, dynamic_ringpop_all)
{
  sib::dynamic_ring<int> ring(1);
  sib::dynamic_ring<int>::size_type count = 200;
  for(std::size_t ii = count; ii; --ii) {
    ring.push(static_cast<int>(ii));
  }
  for(std::size_t ii = count; ii; --ii) {
    ring.pop();
  }
  EXPECT_TRUE(ring.empty() == true);
  EXPECT_TRUE(ring.size() == 0);
}

TEST(dynamic_ring, dynamic_ringpeek_one)
{
  sib::dynamic_ring<int> ring(1);
  ring.push(2543);
  EXPECT_TRUE(ring.front() == 2543);
}

TEST(dynamic_ring, dynamic_ringfifo)
{
  sib::dynamic_ring<int> ring(1);
  boost::random::mt19937 rgen;
  boost::random::uniform_int_distribution<> dist;
  std::queue<int> values;
  for(std::size_t ii = 0; ii != 16; ++ii) {
    int val = dist(rgen);
    values.push(val);
    ring.push(val);
  }
  for(std::size_t ii = 0; ii != 16; ++ii) {
    EXPECT_TRUE(values.front() == ring.front());
    values.pop();
    ring.pop();
  }
}

TEST(dynamic_ring, dynamic_ringsize)
{
  sib::dynamic_ring<int> ring(1);
  for(std::size_t iter = 0; iter != 3; ++iter) {
    for(sib::dynamic_ring<int>::size_type ii = 0; ii != 100; ++ii) {
      ring.push(1);
      EXPECT_TRUE(ring.size() == ii+1);
    }
    for(std::size_t ii = 100; ii; --ii) {
      ring.pop();
      EXPECT_TRUE(ring.size() == ii-1);
    }
  }
}

/*template <typename iterator>
static void dynamic_ringread(sib::dynamic_ring<int,true>* ring, 
                             iterator begin, iterator end)
{
  while (begin != end) {
    while (ring->empty()) ;
    EXPECT_TRUE(ring->front() == *begin);
    ring->pop();
    ++begin;
  }
}

template <typename iterator>
static void dynamic_ringwrite(sib::dynamic_ring<int,true>* ring, iterator begin, iterator end)
{
  while (begin != end) {
    ring->push(*begin);
    ++begin;
  }
}

TEST(dynamic_ring, dynamic_ringconcurrent_safe)
{
  sib::dynamic_ring<int,true> ring(16);
  boost::random::mt19937 rgen;
  boost::random::uniform_int_distribution<> dist;
  std::vector<int> values;
  for(std::size_t ii = 0; ii != 20000; ++ii) {
    values.push_back(dist(rgen));
  }
  std::thread reader(dynamic_ringread<std::vector<int>::const_iterator>,
                     &ring, values.cbegin(), values.cend());
  std::thread writer(dynamic_ringwrite<std::vector<int>::const_iterator>, 
                     &ring, values.cbegin(), values.cend());
  writer.join();
  reader.join();
}*/

