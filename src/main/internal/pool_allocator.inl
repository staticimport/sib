
template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator()
: _grow_multiplier(G / 10.0)
{
  grow();
}

template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::~pool_allocator()
{
  for(sib::vector<block>::iterator iter = _blocks.begin();
      iter != _blocks.end();
      ++iter)
  {
    free(iter->_begin);
  }
}

template <typename T, unsigned I, unsigned G>
inline typename sib::pool_allocator<T,I,G>::pointer
sib::pool_allocator<T,I,G>::allocate(size_type count, const_pointer hint)
{
}

template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator()
template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator()
template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator()
template <typename T, unsigned I, unsigned G>
inline sib::pool_allocator<T,I,G>::pool_allocator()
template <typename T, typename I, typename G>
sib::pool_allocator<T,I,G>::
template <typename T, typename I, typename G>
sib::pool_allocator<T,I,G>::
template <typename T, typename I, typename G>
sib::pool_allocator<T,I,G>::
template <typename T, typename I, typename G>
sib::pool_allocator<T,I,G>::
