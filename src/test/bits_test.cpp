
#include <gtest/gtest.h>
#include <limits>
#include <stdint.h>

#include "bits.hpp"

template <typename T, bool Signed>
static inline void check()
{
  EXPECT_EQ((T)1, sib::power2_above<T>(0,true));
  EXPECT_EQ((T)1, sib::power2_above<T>(0,false));
  EXPECT_EQ((T)1, sib::power2_above<T>(1,true));
  EXPECT_EQ((T)2, sib::power2_above<T>(1,false));
  EXPECT_EQ((T)2, sib::power2_above<T>(2,true));
  EXPECT_EQ((T)4, sib::power2_above<T>(2,false));

  std::size_t bits = sizeof(T) * 8;
  if (Signed) --bits;
  for(std::size_t ii = 2; ii != bits; ++ii) {
    T const pow2 = ((T)1) << ii;
    EXPECT_EQ((T)pow2, sib::power2_above<T>(pow2/2,false));
    EXPECT_EQ((T)pow2, sib::power2_above<T>(pow2-1,true));
    EXPECT_EQ((T)pow2, sib::power2_above<T>(pow2-1,false));
    EXPECT_EQ((T)pow2, sib::power2_above<T>(pow2,true));
  }

  if (Signed) {
    for(std::size_t ii = 0; ii != bits; ++ii) {
      T const value = (((T)1) << ii) * -1;
      EXPECT_EQ((T)1, sib::power2_above<T>(value-1,false));
      EXPECT_EQ((T)1, sib::power2_above<T>(value-1,true));
      EXPECT_EQ((T)1, sib::power2_above<T>(value,false));
      EXPECT_EQ((T)1, sib::power2_above<T>(value,true));
    }
  }
}

template <typename T, bool Signed>
static void check_is_power2()
{
  EXPECT_TRUE(sib::is_power2<T>(1));
  EXPECT_TRUE(sib::is_power2<T>(2));
  std::size_t bits = sizeof(T) * 8;
  if (Signed) --bits;
  for(std::size_t ii = 2; ii != bits; ++ii) {
    T const pow2 = ((T)1) << ii;
    EXPECT_TRUE(sib::is_power2<T>(pow2));
    EXPECT_FALSE(sib::is_power2<T>(pow2-1));
    EXPECT_FALSE(sib::is_power2<T>(pow2+1));
  }
  if (Signed)
  {
    for(std::size_t ii = 0; ii != bits; ++ii) {
      T const value = (((T)1) << ii) * -1;
      EXPECT_FALSE(sib::is_power2<T>(value));
      EXPECT_FALSE(sib::is_power2<T>(value+1));
    }
  }
}

TEST(bits, is_power2_u8)
{
  check_is_power2<uint8_t,false>();
}

TEST(bits, is_power2_i8)
{
  check_is_power2<int8_t,true>();
}

TEST(bits, is_power2_u16)
{
  check_is_power2<uint16_t,false>();
}

TEST(bits, is_power2_i16)
{
  check_is_power2<int16_t,true>();
}

TEST(bits, is_power2_u32)
{
  check_is_power2<uint32_t,false>();
}

TEST(bits, is_power2_i32)
{
  check_is_power2<int32_t,true>();
}

TEST(bits, is_power2_u64)
{
  check_is_power2<uint64_t,false>();
}

TEST(bits, is_power2_i64)
{
  check_is_power2<int64_t,true>();
}

TEST(bits, power2_above_u8)
{
  check<uint8_t,false>();
}

TEST(bits, power2_above_i8)
{
  check<int8_t,true>();
}

TEST(bits, power2_above_u16)
{
  check<uint16_t,false>();
}

TEST(bits, power2_above_i16)
{
  check<int16_t,true>();
}
TEST(bits, power2_above_u32)
{
  check<uint32_t,false>();
}

TEST(bits, power2_above_i32)
{
  check<int32_t,true>();
}

TEST(bits, power2_above_u64)
{
  check<uint64_t,false>();
}

TEST(bits, power2_above_i64)
{
  check<int64_t,true>();
}

