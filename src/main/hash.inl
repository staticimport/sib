

inline std::size_t sib::fnv_hash(char const* begin, char const* const end,
                                 std::size_t const seed)
{
  std::size_t hash(14695981039346656037ULL + seed);
  while (begin != end) {
    hash ^= *(begin++);
    hash *= 1099511628211;
  }
  return hash;
}

