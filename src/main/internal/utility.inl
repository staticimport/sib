//          Copyright Craig Bowles 2012
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace sib
{
  template <typename T, typename Equals>
  struct fixed_count<T,0,Equals>
  {
    // Array
    static inline T* find(T*, T const&)                 { return NULL; }
    static inline T* find(T*, T const&, Equals const&)  { return NULL; }

    // Non-Array, e.g. iterator
    template <typename I>
    static I find(I, I npos, T const&)                  { return npos; }
    template <typename I>
    static I find(I, I npos, T const&, Equals const&)   { return npos; }
  };

  template <typename T, unsigned Count, typename Equals>
  inline T* fixed_count<T,Count,Equals>::find(T* begin, T const& item)
  {
    return find(begin, item, Equals());
  }

  template <typename T, unsigned Count, typename Equals>
  inline T* fixed_count<T,Count,Equals>::find(T* begin, T const& item, Equals const& equals)
  {
    return equals(*begin, item) ? 
      begin : fixed_count<T,Count-1,Equals>::find(begin+1, item, equals);
  }

  template <typename T, unsigned Count, typename Equals>
  template <typename I>
  inline I fixed_count<T,Count,Equals>::find(I begin, I npos, T const& item)
  {
    return find<I>(begin, npos, item, Equals());
  }

  template <typename T, unsigned Count, typename Equals>
  template <typename I>
  inline I fixed_count<T,Count,Equals>::find(I begin, I npos, T const& item, Equals const& equals)
  {
    return equals(*begin, item) ?
      begin : fixed_count<T,Count-1,Equals>::find(++begin, npos, item, equals);
  }
}

