#ifndef SIB_VECTOR_SET_HPP
#define SIB_VECTOR_SET_HPP

#include <vector>

namespace sib {
  template <typename T,
            typename Alloc = std::allocator<T> >
  class vector_set {
  private:
    typedef std::vector<T,Alloc> vector;
  public:
    typedef vector::allocator_type allocator_type;
    typedef vector::const_iterator const_iterator;
    typedef vector::const_reference const_pointer;
    typedef vector::const_reverse_iterator const_reverse_iterator;
    typedef vector::difference_type difference_type;
    typedef vector::iterator iterator;
    typedef vector::value_type key_type;
    typedef vector::pointer pointer;
    typedef vector::reference reference;
    typedef vector::reverse_iterator reverse_iterator;
    typedef vector::size_type size_type;
    typedef vector::value_type value_type;

    // 'structors
    explicit vector_set(Alloc const& a = Alloc());
    template <typename InputIterator>
    vector_set(InputIterator begin, InputIterator end,
               Alloc const& a = Alloc());
    ~vector_set();

    // misc
    vector_set<T,Alloc>& operator=(vector_set<T,Alloc> const& set);
    allocator_type get_allocator() const;

    // iterators
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    // capacity
    bool empty() const;
    size_type max_size() const;
    size_type size() const;

    // modifiers
    void clear();
    std::pair<iterator,bool> insert(value_type const& value);
    std::pair<iterator,bool> insert(value_type&& value);
    template <typename InputIterator>
    void insert(InputIterator begin, InputIterator end);
    void insert(std::initializer_list<value_type> ilist);
    template <class Args...>
    std::pair<iterator,bool> emplace(Args&&... args);
    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(key_type const& key);
    void swap(vector_set& other);

    // lookup
    size_type count(key_type const& key) const;
    iterator find(key_type const& key);
    const_iterator find(key_type const& key) const;
  private:
    vector _vec;
  };
}
   
/**
 * Template Implemenation
 **/
template <typename T, typename A>
sib::vector_set<T,A>::vector_set(A const& a)
: _vec(a)
{
}

template <typename T, typename A>
template <typename I>
sib::vector_set<T,A>::vector_set<T>(I begin, I end, A const& a)
: _vec(a)
{
  for(I iter = begin; iter != end; ++iter) {
    insert(*iter);
  }
}

template <typename T, typename A>
sib::vector_set<T,A>::~vector_set()
{
}

// misc
template <typename T, typename A>
sib::vector_set<T,A>& 
sib::vector_set<T,A>::operator=(sib::vector_set<T,Alloc> const& set)
{
  if (this != &set) {
    _vec = set._vec;
  }
  return *this;
}

template <typename T, typename A>
inline sib::vector_set<T,A>::allocator_type 
sib::vector_set<T,A>::get_allocator() const
{
  return _vec.get_allocator();
}

// iterators
template <typename T, typename A>
inline sib::vector_set<T,A>::iterator 
sib::vector_set<T,A>::begin()
{
  return _vec.begin();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_iterator 
sib::vector_set<T,A>::begin() const
{
  return _vec.begin();
}

template <typename T, typename A>
sib::vector_set<T,A>::const_iterator 
sib::vector_set<T,A>::cbegin() const
{
  return _vec.cbegin;
}

template <typename T, typename A>
inline sib::vector_set<T,A>::iterator 
sib::vector_set<T,A>::end()
{
  return _vec.end();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_iterator 
sib::vector_set<T,A>::end() const
{
  return _vec.end();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_iterator 
sib::vector_set<T,A>::cend() const
{
  return _vec.cend();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::reverse_iterator 
sib::vector_set<T,A>::rbegin()
{
  return _vec.rbegin();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_reverse_iterator 
sib::vector_set<T,A>::rbegin() const
{
  return _vec.rbegin();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_reverse_iterator 
sib::vector_set<T,A>::crbegin() const
{
  return _vec.crbegin();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::reverse_iterator 
sib::vector_set<T,A>::rend()
{
  return _vec.rend();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_reverse_iterator 
sib::vector_set<T,A>::rend() const
{
  return _vec.rend();
}

template <typename T, typename A>
inline sib::vector_set<T,A>::const_reverse_iterator 
sib::vector_set<T,A>::crend() const
{
  return _vec.crend();
}

    // capacity
template <typename T, typename A>
    bool empty() const;
template <typename T, typename A>
    size_type max_size() const;
template <typename T, typename A>
    size_type size() const;

    // modifiers
template <typename T, typename A>
    void clear();
template <typename T, typename A>
    std::pair<iterator,bool> insert(value_type const& value);
template <typename T, typename A>
    std::pair<iterator,bool> insert(value_type&& value);
template <typename T, typename A>
    template <typename InputIterator>
    void insert(InputIterator begin, InputIterator end);
template <typename T, typename A>
    void insert(std::initializer_list<value_type> ilist);
template <typename T, typename A>
    template <class Args...>
    std::pair<iterator,bool> emplace(Args&&... args);
template <typename T, typename A>
    iterator erase(const_iterator position);
template <typename T, typename A>
    iterator erase(const_iterator first, const_iterator last);
template <typename T, typename A>
    size_type erase(key_type const& key);
template <typename T, typename A>
    void swap(vector_set& other);

    // lookup
template <typename T, typename A>
    size_type count(key_type const& key) const;
template <typename T, typename A>
    iterator find(key_type const& key);
template <typename T, typename A>
    const_iterator find(key_type const& key) const;
  private:
    vector _vec;

#endif /* SIB_VECTOR_SET_HPP */

