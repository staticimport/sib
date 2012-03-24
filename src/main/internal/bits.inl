
namespace sib_internal
{
  template <typename Integer, unsigned Bytes> 
  struct bit_fill
  {
    static Integer act(Integer x);
  };

  template <typename Integer>
  struct bit_fill<Integer,1>
  {
    static inline Integer act(Integer x)
    {
      x |= x >> 1;
      x |= x >> 2;
      x |= x >> 4;
      return x;
    }
  };

  template <typename Integer>
  struct bit_fill<Integer,2>
  {
    static inline Integer act(Integer x)
    {
      x |= x >> 1;
      x |= x >> 2;
      x |= x >> 4;
      x |= x >> 8;
      return x;
    }
  };

  template <typename Integer>
  struct bit_fill<Integer,4>
  {
    static inline Integer act(Integer x)
    {
      x |= x >> 1;
      x |= x >> 2;
      x |= x >> 4;
      x |= x >> 8;
      x |= x >> 16;
      return x;
    }
  };

  template <typename Integer>
  struct bit_fill<Integer,8>
  {
    static inline Integer act(Integer x)
    {
      x |= x >> 1;
      x |= x >> 2;
      x |= x >> 4;
      x |= x >> 8;
      x |= x >> 16;
      x |= x >> 32;
      return x;
    }
  };
};

template <typename Integer>
inline bool sib::is_power2(Integer x)
{
  return ((x & (x-1)) == 0) && (x > 0);
}

template <typename Integer>
inline Integer sib::power2_above(Integer x, bool inclusive)
{
  if (x > 0) {
    if (inclusive) --x;
    return sib_internal::bit_fill<Integer,sizeof(Integer)>::act(x) + 1;
  } else {
    return 1;
  }
}

