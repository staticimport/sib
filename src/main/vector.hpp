#ifndef SIB_VECTOR_HPP
#define SIB_VECTOR_HPP

namespace sib
{
  template <typename T, bool IsPod=boost::is_pod<T> >
  class vector
  {
  private:
    typedef typename param<T>::type param_type;
  public:
    typedef Allocator                             allocator_type;
    typedef typename Allocator::const_pointer     const_iterator;
    typedef typename Allocator::const_pointer     const_pointer;
    typedef typename Allocator::const_reference   const_reference;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef typename Allocator::difference_type   difference_type;
    typedef typename Allocator::pointer           iterator;
    typedef typename Allocator::pointer           pointer;
    typedef typename Allocator::reference         reference;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef typename Allocator::size_type         size_type;
    typedef typename Allocator::value_type        value_type;

    // Init/Uninit
    explicit vector(size_type const reserve_capacity=8);
    /*template <typename input_iterator>
    vector(input_iterator first, input_iterator last,
               Allocator const& alloc = Allocator());
    vector(vector const& vector);*/
    ~vector();

    // element access
    const_reference at(size_type pos) const;
    reference at(size_type pos);
    const_reference back() const                    { return *(_end-1); }
    reference back()                                { return *(_end-1); }
    T const* data() const                           { return _begin; }
    T* data()                                       { return _data; }
    const_reference front() const                   { return *_begin; }
    reference front()                               { return *_begin; }
    const_reference operator[](size_type pos) const { return _begin[pos]; }
    reference operator[](size_type pos)             { return _begin[pos]; }

    // iterators
    SIB_NOEXCEPT iterator begin()               { return _begin; }
    SIB_NOEXCEPT const_iterator begin() const   { return _begin; }
    SIB_NOEXCEPT const_iterator cbegin() const  { return _begin; }
    SIB_NOEXCEPT iterator end()                 { return _end; }
    SIB_NOEXCEPT const_iterator end() const     { return _end; }
    SIB_NOEXCEPT const_iterator cend() const    { return _end; }
    SIB_NOEXCEPT reverse_iterator rbegin();
    //SIB_NOEXCEPT const_reverse_iterator rbegin() const;
    //SIB_NOEXCEPT const_reverse_iterator crbegin() const;
    SIB_NOEXCEPT reverse_iterator rend();
    //SIB_NOEXCEPT const_reverse_iterator rend() const;
    //SIB_NOEXCEPT const_reverse_iterator crend() const;

    // capacity
    SIB_NOEXCEPT size_type capacity() const   { return _capacity_end - _begin; }
    SIB_NOEXCEPT bool empty() const           { return _begin == _end; }
    SIB_NOEXCEPT size_type max_size() const;
    void reserve(size_type capacity);
    SIB_NOEXCEPT size_type size() const       { return _end - _begin; }

    // modifiers
    SIB_NOEXCEPT void clear();
    iterator erase(iterator pos);
    iterator insert(iterator pos, param_type value);
    void pop_back();
    void push_back(param_type x);
    void reserve(size_type size);
    void swap(vector& vector);
    vector& operator=(vector const& vector);

#ifdef SIB_CXX11
    /*vector(size_type count, T const& value,
               Allocator const& alloc = Allocator());
    explicit vector(size_type count);
    vector(vector const& vector, Allocator const& alloc);
    vector(vector&& vector);
    vector(vector&& vector, Allocator const& alloc);
    vector(std::initializer_list<T> init, 
               Allocator const& alloc = Allocator());
    SIB_NOEXCEPT const_iterator cbegin() const     { return _begin; }
    SIB_NOEXCEPT const_iterator cend() const       { return _end; }
    SIB_NOEXCEPT const_iterator crbegin() const;
    SIB_NOEXCEPT const_iterator crend() const;
    template <typename... args>
    iterator emplace(const_iterator pos, args&&... args);
    template <typename... args>
    iterator emplace_back(args&&... args);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    iterator insert(const_iterator pos, T const& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, T const& value);
    template <typename input_iterator>
    iterator insert(const_iterator pos, input_iterator first, input_iterator last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    void push_back(T&& value);
    void resize(size_type count);
    void resize(size_type count, param_type value);
    void shrink_to_fit();*/
#else
    /*explicit vector(size_type count, T const& value = T(),
                        Allocator const& alloc = Allocator());
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator pos, T const& value);
    void insert(iterator pos, T const& value);
    template <typename input_iterator>
    void insert(iterator pos, input_iterator first, input_iterator last);
    void resize(size_type count, T value = T());*/
#endif
  private:
    friend class vector_test;

    T* _begin;
    T* _end;
    T* _capacity_end;
  };
}

/*template <typename T, typename Alloc>
bool operator==(vector<T,Alloc> const& lhs, vector<T,Alloc> const& rhs);
template <typename T, typename Alloc>
bool operator!=(vector<T,Alloc> const& lhs, vector<T,Alloc> const& rhs);
template <typename T, typename Alloc>
bool operator<(vector<T,Alloc> const& lhs, vector<T,Alloc> const& rhs);
template <typename T, typename Alloc>
bool operator<=(vector<T,Alloc> const& lhs, vector<T,Alloc> const& rhs);
template <typename T, typename Alloc>
bool operator>(vector<T,Alloc> const& lhs, vector<T,Alloc> const& rhs);
template <typename T, typename Alloc>
bool operator>=(vector<T,Alloc> const& lhs, vector<T,Alloc> const& rhs);*/

#include "internal/vector.inl"

#endif /* SIB_VECTOR_HPP */

