#ifndef SIB_MEMORY_HPP
#define SIB_MEMORY_HPP

namespace sib
{
  template <typename T, bool POD=false>
  struct alloc
  {
    static void act(std::size_t capacity, bool default_init=true);
  };

  template <typename T, bool POD=false>
  struct destruct
  {
    static void act(T* x);
    static void act(T* begin, T* end);
  };

  template <typename T, bool POD=false>
  struct memcpy
  {
    static void act(T* dest, T* source, std::size_t count);
  };

  template <typename T, bool POD=false>
  struct memmove
  {
    static void act(T* dest, T* source, std::size_t count);
  };

  template <typename T, bool POD=false>
  struct realloc
  {
    static void act(T* begin, T* end, std::size_t old_capacity, std::size_t new_capacity);
  };
}

#endif /* SIB_MEMORY_HPP */

