
#include <queue>
#include <thread>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "dynamic_ring.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ring
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(new_dynamic_ringempty)
{
  sib::dynamic_ring<int> ring(16);
  BOOST_CHECK(ring.empty() == true);
  BOOST_CHECK(ring.size() == 0);
}

BOOST_AUTO_TEST_CASE(dynamic_ringpush_one)
{
  sib::dynamic_ring<int> ring(16);
  ring.push(423);
  BOOST_CHECK(ring.empty() == false);
  BOOST_CHECK(ring.size() == 1);
}

BOOST_AUTO_TEST_CASE(dynamic_ringpop_one)
{
  sib::dynamic_ring<int> ring(16);
  ring.push(423);
  ring.pop();
  BOOST_CHECK(ring.empty() == true);
  BOOST_CHECK(ring.size() == 0);
}

BOOST_AUTO_TEST_CASE(dynamic_ringpop_all)
{
  sib::dynamic_ring<int> ring(1);
  sib::dynamic_ring<int>::size_type count = 100;
  for(auto ii = count; ii; --ii) {
    ring.push(static_cast<int>(ii));
  }
  for(auto ii = count; ii; --ii) {
    ring.pop();
  }
  BOOST_CHECK(ring.empty() == true);
  BOOST_CHECK(ring.size() == 0);
}

BOOST_AUTO_TEST_CASE(dynamic_ringpeek_one)
{
  sib::dynamic_ring<int> ring(1);
  ring.push(2543);
  BOOST_CHECK(ring.front() == 2543);
}

BOOST_AUTO_TEST_CASE(dynamic_ringfifo)
{
  sib::dynamic_ring<int> ring(1);
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

BOOST_AUTO_TEST_CASE(dynamic_ringsize)
{
  sib::dynamic_ring<int> ring(1);
  for(std::size_t iter = 0; iter != 3; ++iter) {
    for(sib::dynamic_ring<int>::size_type ii = 0; ii != 100; ++ii) {
      ring.push(1);
      BOOST_CHECK(ring.size() == ii+1);
    }
    for(std::size_t ii = 100; ii; --ii) {
      ring.pop();
      BOOST_CHECK(ring.size() == ii-1);
    }
  }
}

template <typename iterator>
static void dynamic_ringread(sib::dynamic_ring<int,true>* ring, 
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
static void dynamic_ringwrite(sib::dynamic_ring<int,true>* ring, iterator begin, iterator end)
{
  while (begin != end) {
    ring->push(*begin);
    ++begin;
  }
}

BOOST_AUTO_TEST_CASE(dynamic_ringconcurrent_safe)
{
  sib::dynamic_ring<int,true> ring(16);
  boost::random::mt19937 rgen;
  boost::random::uniform_int_distribution<> dist;
  std::vector<int> values;
  for(auto ii = 0; ii != 20000; ++ii) {
    values.push_back(dist(rgen));
  }
  std::thread reader(dynamic_ringread<std::vector<int>::const_iterator>,
                     &ring, values.cbegin(), values.cend());
  std::thread writer(dynamic_ringwrite<std::vector<int>::const_iterator>, 
                     &ring, values.cbegin(), values.cend());
  writer.join();
  reader.join();
}

