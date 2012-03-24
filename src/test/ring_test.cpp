
#include <queue>

#ifdef _GLIBCXX_HAS_GTHREADS
#include <thread>
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <gtest/gtest.h>

#include "ring.hpp"

TEST(ring, new_ring_empty)
{
  sib::ring<int> ring(16);
  EXPECT_TRUE(ring.capacity() == 16);
  EXPECT_TRUE(ring.empty() == true);
  EXPECT_TRUE(ring.full() == false);
  EXPECT_TRUE(ring.size() == 0);
}

TEST(ring, ring_push_one)
{
  sib::ring<int> ring(16);
  ring.push(423);
  EXPECT_TRUE(ring.empty() == false);
  EXPECT_TRUE(ring.full() == false);
  EXPECT_TRUE(ring.size() == 1);
}

TEST(ring, ring_push_until_full)
{
  sib::ring<int> ring(16);
  for(std::size_t ii = ring.capacity(); ii; --ii) {
    ring.push(static_cast<int>(ii));
  }
  EXPECT_TRUE(ring.empty() == false);
  EXPECT_TRUE(ring.full() == true);
  EXPECT_TRUE(ring.size() == ring.capacity());
}

TEST(ring, ring_pop_one)
{
  sib::ring<int> ring(16);
  ring.push(423);
  ring.pop();
  EXPECT_TRUE(ring.empty() == true);
  EXPECT_TRUE(ring.full() == false);
  EXPECT_TRUE(ring.size() == 0);
}

TEST(ring, ring_pop_all)
{
  sib::ring<int> ring(16);
  for(std::size_t ii = ring.capacity(); ii; --ii) {
    ring.push(static_cast<int>(ii));
  }
  for(std::size_t ii = ring.capacity(); ii; --ii) {
    ring.pop();
  }
  EXPECT_TRUE(ring.empty() == true);
  EXPECT_TRUE(ring.full() == false);
  EXPECT_TRUE(ring.size() == 0);
}

TEST(ring, ring_peek_one)
{
  sib::ring<int> ring(16);
  ring.push(2543);
  EXPECT_TRUE(ring.front() == 2543);
}

TEST(ring, ring_fifo)
{
  sib::ring<int> ring(16);
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

TEST(ring, ring_size)
{
  sib::ring<int> ring(16);
  for(std::size_t iter = 0; iter != 3; ++iter) {
    for(sib::ring<int>::size_type ii = 0; ii != ring.capacity(); ++ii) {
      ring.push(1);
      EXPECT_TRUE(ring.size() == ii+1);
    }
    for(std::size_t ii = ring.capacity(); ii; --ii) {
      ring.pop();
      EXPECT_TRUE(ring.size() == ii-1);
    }
  }
}

template <typename iterator>
static void ring_read(sib::ring<int,true>* ring, 
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
static void ring_write(sib::ring<int,true>* ring, 
                       iterator begin, iterator end)
{
  while (begin != end) {
    while (ring->full()) ;
    ring->push(*begin);
    ++begin;
  }
}

#ifdef _GLIBCXX_HAS_GTHREADS
TEST(ring, ring_concurrent_safe)
{
  sib::ring<int,true> ring(16);
  boost::random::mt19937 rgen;
  boost::random::uniform_int_distribution<> dist;
  std::vector<int> values;
  for(std::size_t ii = 0; ii != 20000; ++ii) {
    values.push_back(dist(rgen));
  }
  std::thread reader(ring_read<std::vector<int>::const_iterator>,
                     &ring, values.cbegin(), values.cend());
  std::thread writer(ring_write<std::vector<int>::const_iterator>, 
                     &ring, values.cbegin(), values.cend());
  writer.join();
  reader.join();
}
#endif

TEST(ring, ring_const_iterator_begin_end)
{
  sib::ring<int,true> ring(16);
  EXPECT_TRUE(ring.cbegin() == ring.cend());
  ring.push(1);
  sib::ring<int,true>::const_iterator begin = ring.cbegin();
  EXPECT_TRUE(*begin == 1);
  EXPECT_TRUE(begin == begin);
  EXPECT_TRUE(begin != ring.cend());
  EXPECT_TRUE((begin + 1) == ring.cend());
  EXPECT_TRUE((1 + begin) == ring.cend());
  EXPECT_TRUE(begin == (ring.cend() - 1));
  EXPECT_TRUE((ring.cend() - begin) == sizeof(int));
}

TEST(ring, ring_const_iterate)
{
  sib::ring<int,true> ring(16);
  for(int ii = 0; ii != 16; ++ii) {
    ring.push(ii);
    sib::ring<int,true>::const_iterator iter = ring.cbegin();
    for(int jj = 0; jj <= ii; ++jj) {
      EXPECT_TRUE(iter[jj] == jj);
    }
    for(int jj = 0; jj <= ii; ++jj) {
      EXPECT_TRUE(*iter == jj);
      ++iter;
    }
    EXPECT_TRUE(iter == ring.cend());
    iter = ring.cbegin();
    for(int jj = 0; jj <= ii; ++jj) {
      EXPECT_TRUE(*iter == jj);
      sib::ring<int,true>::const_iterator copy = iter++;
      EXPECT_TRUE(*copy == jj);
    }
  }
}

TEST(ring, ring_iterator_begin_end)
{
  sib::ring<int,true> ring(16);
  EXPECT_TRUE(ring.begin() == ring.end());
  ring.push(1);
  sib::ring<int,true>::iterator begin = ring.begin();
  EXPECT_TRUE(*begin == 1);
  EXPECT_TRUE(begin == begin);
  EXPECT_TRUE(begin != ring.end());
  EXPECT_TRUE((begin + 1) == ring.end());
  EXPECT_TRUE((1 + begin) == ring.end());
  EXPECT_TRUE(begin == (ring.end() - 1));
  EXPECT_TRUE((ring.end() - begin) == sizeof(int));
}

TEST(ring, ring_iterate)
{
  sib::ring<int,true> ring(16);
  for(int ii = 0; ii != 16; ++ii) {
    ring.push(ii);
    sib::ring<int,true>::iterator iter = ring.begin();
    for(int jj = 0; jj <= ii; ++jj) {
      EXPECT_TRUE(iter[jj] == jj);
    }
    for(int jj = 0; jj <= ii; ++jj) {
      EXPECT_TRUE(*iter == jj);
      ++iter;
    }
    EXPECT_TRUE(iter == ring.end());
    iter = ring.begin();
    for(int jj = 0; jj <= ii; ++jj) {
      EXPECT_TRUE(*iter == jj);
      sib::ring<int,true>::iterator copy = iter++;
      EXPECT_TRUE(*copy == jj);
    }
  }
}
