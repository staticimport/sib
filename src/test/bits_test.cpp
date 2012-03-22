
#include <gtest/gtest.h>

#include "bits.hpp"

#define NUM_MAX(type) std::numeric_limits< type >::max()

#define CHECK_POWER2_ABOVE_POSITIVE(type) \
  EXPECT_EQ(1, power2_above< type >(0,true));\
  EXPECT_EQ(1, power2_above< type >(0,false));\
  EXPECT_EQ(1, power2_above< type >(1,true));\
  EXPECT_EQ(2, power2_above< type >(1,false));\
  EXPECT_EQ(2, power2_above< type >(2,true));\
  EXPECT_EQ(4, power2_above< type >(2,false));\
  EXPECT_EQ(4, power2_above< type >(3,true));\
  EXPECT_EQ(4, power2_above< type >(3,false));\
  EXPECT_EQ(16, power2_above< type >(15,true));\
  EXPECT_EQ(16, power2_above< type >(15,false));\
  EXPECT_EQ(16, power2_above< type >(16,true));\
  EXPECT_EQ(32, power2_above< type >(16,false));\
  EXPECT_EQ(NUM_MAX(type)/2, power2_above< type >(NUM_MAX(type)/2-1,true));\
  EXPECT_EQ(NUM_MAX(type)/2, power2_above< type >(NUM_MAX(type)/2-1,false));\
  EXPECT_EQ(NUM_MAX(type)/2, power2_above< type >(NUM_MAX(type)/2,true));

#define CHECK_POWER2_ABOVE_NEGATIVE(type) \
  EXPECT_EQ(1, power2_above< type >(-1,true);\
  EXPECT_EQ(1, power2_above< type >(-1,false);\
  EXPECT_EQ(1, power2_above< type >(-2,true);\
  EXPECT_EQ(1, power2_above< type >(-2,false);\
  EXPECT_EQ(1, power2_above< type >(-10,true);\
  EXPECT_EQ(1, power2_above< type >(-100,true);

TEST(bits, power2_above_u8)
{
  COMMON_POWER2_ABOVE_POSITIVE(uint8_t)
}

TEST(bits, power2_above_i8)
{
  CHECK_POWER2_ABOVE_POSITIVE(int8_t)
  CHECK_POWER2_ABOVE_NEGATIVE(int8_t)
}

TEST(bits, power2_above_u16)
{
  COMMON_POWER2_ABOVE_POSITIVE(uint16_t)
}

TEST(bits, power2_above_i16)
{
  CHECK_POWER2_ABOVE_POSITIVE(int16_t)
  CHECK_POWER2_ABOVE_NEGATIVE(int16_t)
}
TEST(bits, power2_above_u32)
{
  COMMON_POWER2_ABOVE_POSITIVE(uint32_t)
}

TEST(bits, power2_above_i32)
{
  CHECK_POWER2_ABOVE_POSITIVE(int32_t)
  CHECK_POWER2_ABOVE_NEGATIVE(int32_t)
}

TEST(bits, power2_above_u64)
{
  COMMON_POWER2_ABOVE_POSITIVE(uint64_t)
}

TEST(bits, power2_above_i64)
{
  CHECK_POWER2_ABOVE_POSITIVE(int64_t)
  CHECK_POWER2_ABOVE_NEGATIVE(int64_t)
}

