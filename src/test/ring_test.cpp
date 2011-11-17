
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

