//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIB_SIMD_INT_HPP
#define SIB_SIMD_INT_HPP

#include <immintrin.h>

namespace sib
{
  template <typename T, unsigned X=16>
  struct aligned
  {
    typedef T __attribute__((aligned(X))) type;
  };

  template <typename T>
  class simd_int128
  {
  public:
    simd_int128(__m128i const& x);
    simd_int128(T x);
    
    /*void add8(simd_int128 const& x) const;
    void add16(simd_int128 const& x) const;
    void add32(simd_int128 const& x) const;
    void add64(simd_int128 const& x) const;

    static simd_int128  add8(simd_int128 const& a, simd_int128 const& b);
    static void         add8(simd_int128& out, simd_int128 const& a, simd_int128 const& b);
    static simd_int128  add16(simd_int128 const& a, simd_int128 const& b);
    static void         add16(simd_int128& out, simd_int128 const& a, simd_int128 const& b);
    static simd_int128  add32(simd_int128 const& a, simd_int128 const& b);
    static void         add32(simd_int128& out, simd_int128 const& a, simd_int128 const& b);
    static simd_int128  add32(simd_int128 const& a, simd_int128 const& b);
    static void         add32(simd_int128& out, simd_int128 const& a, simd_int128 const& b);
    */

    /*
    // SSE 128-bit value
    __m128i m128 __attribute__((aligned));

    // 1 byte ints
    int8_t i8[16];
    uint8_t u8[16];
    char c[16];
    unsigned char uc[16];

    // 2 byte ints
    int16_t i16[8];
    uint16_t u16[8];
    
    // 4 byte ints
    int32_t i32[4];
    uint32_t u32[4];

    // 8 byte ints
    int64_t i64[2];
    uint64_t u64[2];*/
  };
}

#include "internal/simd_int.inl"

#endif /* SIB_SIMD_INT_HPP */

