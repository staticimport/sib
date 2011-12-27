
#include <queue>

#ifdef _GLIBCXX_HAS_GTHREADS
#include <thread>
#endif

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "ring.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ring
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(new_ring_empty)
{
  sib::ring<int> ring(16);
  BOOST_CHECK(ring.capacity() == 16);
  BOOST_CHECK(ring.empty() == true);
  BOOST_CHECK(ring.full() == false);
  BOOST_CHECK(ring.size() == 0);
}

BOOST_AUTO_TEST_CASE(ring_push_one)
{
  sib::ring<int> ring(16);
  ring.push(423);
  BOOST_CHECK(ring.empty() == false);
  BOOST_CHECK(ring.full() == false);
  BOOST_CHECK(ring.size() == 1);
}

BOOST_AUTO_TEST_CASE(ring_push_until_full)
{
  sib::ring<int> ring(16);
  for(auto ii = ring.capacity(); ii; --ii) {
    ring.push(static_cast<int>(ii));
  }
  BOOST_CHECK(ring.empty() == false);
  BOOST_CHECK(ring.full() == true);
  BOOST_CHECK(ring.size() == ring.capacity());
}

BOOST_AUTO_TEST_CASE(ring_pop_one)
{
  sib::ring<int> ring(16);
  ring.push(423);
  ring.pop();
  BOOST_CHECK(ring.empty() == true);
  BOOST_CHECK(ring.full() == false);
  BOOST_CHECK(ring.size() == 0);
}

BOOST_AUTO_TEST_CASE(ring_pop_all)
{
  sib::ring<int> ring(16);
  for(auto ii = ring.capacity(); ii; --ii) {
    ring.push(static_cast<int>(ii));
  }
  for(auto ii = ring.capacity(); ii; --ii) {
    ring.pop();
  }
  BOOST_CHECK(ring.empty() == true);
  BOOST_CHECK(ring.full() == false);
  BOOST_CHECK(ring.size() == 0);
}

BOOST_AUTO_TEST_CASE(ring_peek_one)
{
  sib::ring<int> ring(16);
  ring.push(2543);
  BOOST_CHECK(ring.front() == 2543);
}

BOOST_AUTO_TEST_CASE(ring_fifo)
{
  sib::ring<int> ring(16);
  boost::random::mt19937 rgen;
  boost::random::uniform_int_distribution<> dist;
  std::queue<int> values;
  for(auto ii = 0; ii != 16; ++ii) {
    auto val = dist(rgen);
    values.push(val);
    ring.push(val);
  }
  for(auto ii = 0; ii != 16; ++ii) {
    BOOST_CHECK(values.front() == ring.front());
    values.pop();
    ring.pop();
  }
}

BOOST_AUTO_TEST_CASE(ring_size)
{
  sib::ring<int> ring(16);
  for(auto iter = 0; iter != 3; ++iter) {
    for(sib::ring<int>::size_type ii = 0; ii != ring.capacity(); ++ii) {
      ring.push(1);
      BOOST_CHECK(ring.size() == ii+1);
    }
    for(auto ii = ring.capacity(); ii; --ii) {
      ring.pop();
      BOOST_CHECK(ring.size() == ii-1);
    }
  }
}

template <typename iterator>
static void ring_read(sib::ring<int,true>* ring, 
                      iterator begin, iterator end)
{
  while (begin != end) {
    while (ring->empty()) ;
    BOOST_CHECK(ring->front() == *begin);
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
BOOST_AUTO_TEST_CASE(ring_concurrent_safe)
{
  sib::ring<int,true> ring(16);
  boost::random::mt19937 rgen;
  boost::random::uniform_int_distribution<> dist;
  std::vector<int> values;
  for(auto ii = 0; ii != 20000; ++ii) {
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

BOOST_AUTO_TEST_CASE(ring_const_iterator_begin_end)
{
  sib::ring<int,true> ring(16);
  BOOST_CHECK(ring.cbegin() == ring.cend());
  ring.push(1);
  sib::ring<int,true>::const_iterator begin = ring.cbegin();
  BOOST_CHECK(*begin == 1);
  BOOST_CHECK(begin == begin);
  BOOST_CHECK(begin != ring.cend());
  BOOST_CHECK((begin + 1) == ring.cend());
  BOOST_CHECK((1 + begin) == ring.cend());
  BOOST_CHECK(begin == (ring.cend() - 1));
  BOOST_CHECK((ring.cend() - begin) == sizeof(int));
}

BOOST_AUTO_TEST_CASE(ring_const_iterate)
{
  sib::ring<int,true> ring(16);
  for(int ii = 0; ii != 16; ++ii) {
    ring.push(ii);
    auto iter = ring.cbegin();
    for(int jj = 0; jj <= ii; ++jj) {
      BOOST_CHECK(iter[jj] == jj);
    }
    for(int jj = 0; jj <= ii; ++jj) {
      BOOST_CHECK(*iter == jj);
      ++iter;
    }
    BOOST_CHECK(iter == ring.cend());
    iter = ring.cbegin();
    for(int jj = 0; jj <= ii; ++jj) {
      BOOST_CHECK(*iter == jj);
      auto copy = iter++;
      BOOST_CHECK(*copy == jj);
    }
  }
}

BOOST_AUTO_TEST_CASE(ring_iterator_begin_end)
{
  sib::ring<int,true> ring(16);
  BOOST_CHECK(ring.begin() == ring.end());
  ring.push(1);
  sib::ring<int,true>::iterator begin = ring.begin();
  BOOST_CHECK(*begin == 1);
  BOOST_CHECK(begin == begin);
  BOOST_CHECK(begin != ring.end());
  BOOST_CHECK((begin + 1) == ring.end());
  BOOST_CHECK((1 + begin) == ring.end());
  BOOST_CHECK(begin == (ring.end() - 1));
  BOOST_CHECK((ring.end() - begin) == sizeof(int));
}

BOOST_AUTO_TEST_CASE(ring_iterate)
{
  sib::ring<int,true> ring(16);
  for(int ii = 0; ii != 16; ++ii) {
    ring.push(ii);
    sib::ring<int,true>::iterator iter = ring.begin();
    for(int jj = 0; jj <= ii; ++jj) {
      BOOST_CHECK(iter[jj] == jj);
    }
    for(int jj = 0; jj <= ii; ++jj) {
      BOOST_CHECK(*iter == jj);
      ++iter;
    }
    BOOST_CHECK(iter == ring.end());
    iter = ring.begin();
    for(int jj = 0; jj <= ii; ++jj) {
      BOOST_CHECK(*iter == jj);
      auto copy = iter++;
      BOOST_CHECK(*copy == jj);
    }
  }
}
