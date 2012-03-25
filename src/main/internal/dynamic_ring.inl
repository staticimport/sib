
template <typename T, bool C, typename A>
sib::dynamic_ring<T,C,A>::dynamic_ring(
    sib::dynamic_ring<T,C,A>::size_type const min_capacity,
    A const& allocator)
: _ring_allocator(allocator),
  _expand_ring(sizeof(void*) * 8),
  _read_ring(new(_ring_allocator.allocate(1)) 
             typename traits::sub_ring(min_capacity)), // TODO: pass allocator? ring_allocator?
  _write_ring(_read_ring)
{
}

template <typename T, bool C, typename A>
sib::dynamic_ring<T,C,A>::~dynamic_ring()
{
  while (!empty()) {
    pop();
  }
  _ring_allocator.deallocate(_read_ring, 1);
}

template <typename T, bool C, typename A>
inline bool
sib::dynamic_ring<T,C,A>::empty() const
{
  return _write_ring->empty();
}

template <typename T, bool C, typename A>
inline typename sib::dynamic_ring<T,C,A>::const_reference
sib::dynamic_ring<T,C,A>::front() const
{
  if (_read_ring->empty()) advance_read_ring();
  return _read_ring.front();
}

template <typename T, bool C, typename A>
typename sib::dynamic_ring<T,C,A>::size_type
sib::dynamic_ring<T,C,A>::size(bool reader_context) const
{
  size_type total(0);
  if (!C) {
    total += _read_ring->size();
    expand_ring_citer end = _expand_ring.cend();
    for(expand_ring_citer iter = _expand_ring.cbegin(); iter != end; ++iter) {
      total += (*iter)->size();
    }
  } else if (reader_context) {
    typename traits::sub_ring_pointer read_ring(_read_ring);
    size_type const expand_count(_expand_ring.size());
    total += read_ring->size();
    for(size_type ii = 1; ii < expand_count; ++ii) {
      total += read_ring->capacity() << ii;
    }
  } else {
    typename traits::sub_ring_pointer write_ring(_write_ring);
    size_type const expand_count(_expand_ring.size());
    total += write_ring->size();
    for(size_type ii = 1; ii < expand_count; ++ii) {
      total += write_ring->capacity() >> ii;
    }
  }
  return total;
}

template <typename T, bool C, typename A>
inline void
sib::dynamic_ring<T,C,A>::finish_push()
{
  _write_ring->finish_push();
  if (_write_ring->size() == 1 and _read_ring != _write_ring) {
    _expand_ring.push(_write_ring);
  }
}

template <typename T, bool C, typename A>
inline typename sib::dynamic_ring<T,C,A>::reference
sib::dynamic_ring<T,C,A>::front()
{
  if (_read_ring->empty()) advance_read_ring();
  return _read_ring->front();
}

template <typename T, bool C, typename A>
inline void
sib::dynamic_ring<T,C,A>::pop()
{
  if (!_read_ring->empty()) {
    _read_ring->pop();
  } else if (!_expand_ring.empty()) {
    advance_read_ring();
    _read_ring->pop();
  } else
    throw std::runtime_error("attempted pop of empy dynamic_ring.");
}

template <typename T, bool C, typename A>
inline void
sib::dynamic_ring<T,C,A>::push(T const& x)
{
  bool advanced(false);
  if (_write_ring->full()) { advanced = true; advance_write_ring(); }
  _write_ring->push(x);
  if (advanced) {
    _expand_ring.push(_write_ring);
  }
}

template <typename T, bool C, typename A>
inline typename sib::dynamic_ring<T,C,A>::reference
sib::dynamic_ring<T,C,A>::start_push()
{
  if (_write_ring->full()) advance_write_ring();
  return _write_ring->start_push();
}

template <typename T, bool C, typename A>
void
sib::dynamic_ring<T,C,A>::advance_read_ring()
{
  _read_ring->~ring();
  _ring_allocator.deallocate(_read_ring, 1);
  _read_ring = _expand_ring.front();
  _expand_ring.pop();
}

template <typename T, bool C, typename A>
void
sib::dynamic_ring<T,C,A>::advance_write_ring()
{
  size_type const next_capacity = _write_ring->capacity() * 2;
  _write_ring = new(_ring_allocator.allocate(1)) 
                typename traits::sub_ring(next_capacity);
  //_expand_ring.push(_write_ring);
}

