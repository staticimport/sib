
#include <gtest/gtest.h>

#include "pool_allocator.hpp"

using namespace sib;

TEST(pool_allocator, alloc)
{
  pool_allocator<unsigned,1> pool;
  std::vector<unsigned*> alloced;
  for(unsigned k = 0; k != 10; ++k) {
    for(unsigned ii = 0; ii != 100; ++ii) {
      alloced.push_back(new(pool.allocate(1)) unsigned(ii));
    }
    for(unsigned ii = 0; ii != 100; ++ii)
      EXPECT_EQ(ii, *(alloced[ii]));
    alloced.clear();
  }
}

TEST(pool_allocator, dealloc)
{
  pool_allocator<unsigned,1> pool;
  std::vector<unsigned*> alloced;
  for(unsigned k = 0; k != 10; ++k) {
    for(unsigned ii = 0; ii != 100; ++ii) {
      alloced.push_back(new(pool.allocate(1)) unsigned(ii));
    }
    for(unsigned ii = 0; ii != 100; ++ii) {
      EXPECT_EQ(ii, *(alloced[ii]));
      pool.deallocate(alloced[ii], 1);
    }
    alloced.clear();
  }
}

TEST(pool_allocator, alloc_with_hint)
{
  pool_allocator<unsigned,1> pool;
  std::vector<unsigned*> alloced;
  for(unsigned k = 0; k != 10; ++k) {
    for(unsigned ii = 0; ii != 100; ++ii) {
      void* hint = alloced.empty() ? (void*)&pool : (void*)alloced[alloced.size()/2];
      alloced.push_back(new(pool.allocate(1,reinterpret_cast<unsigned*>(hint))) unsigned(ii));
    }
    for(unsigned ii = 0; ii != 100; ++ii)
      EXPECT_EQ(ii, *(alloced[ii]));
    alloced.clear();
  }
}

TEST(pool_allocator, alloc_many)
{
  pool_allocator<unsigned> pool;
  EXPECT_THROW(pool.allocate(2), std::runtime_error);
}

