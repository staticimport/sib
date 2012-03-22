
template <typename Integer>
inline bool sib::is_power2(Integer x)
{
  return ((x & (x-1)) == 0) && (x > 0);
}

template <typename Integer>
inline Integer sib::power2_above(Integer x, bool inclusive)
{
  if (x > 0) {
    if (!inclusive) --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    return x+1;
  } else {
    return 1;
  }
}

