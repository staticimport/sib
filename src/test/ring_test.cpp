
#include <queue>
#include <thread>

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
static void ring_read(sib::ring<int,true>* ring, iterator begin, iterator end)
{
  while (begin != end) {
    while (ring->empty()) ;
    BOOST_CHECK(ring->front() == *begin);
    ring->pop();
    ++begin;
  }
}

template <typename iterator>
static void ring_write(sib::ring<int,true>* ring, iterator begin, iterator end)
{
  while (begin != end) {
    while (ring->full()) ;
    ring->push(*begin);
    ++begin;
  }
}

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

