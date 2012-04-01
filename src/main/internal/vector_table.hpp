#ifndef SIB_VECTOR_TABLE_HPP
#define SIB_VECTOR_TABLE_HPP

//#include <vector>
#include "../vector.hpp"

namespace sib {
  template <typename K, typename V, typename Equal>
  class vector_table {
  private:
    //typedef std::vector<V> vector;
    typedef sib::vector<V> vector;
  public:
    //typedef typename vector::allocator_type allocator_type;
    typedef typename vector::const_iterator const_iterator;
    typedef typename vector::const_reference const_reference;
    typedef typename vector::const_reverse_iterator const_reverse_iterator;
    typedef typename vector::difference_type difference_type;
    typedef typename vector::iterator iterator;
    typedef typename vector::pointer pointer;
    typedef typename vector::reference reference;
    typedef typename vector::reverse_iterator reverse_iterator;
    typedef typename vector::size_type size_type;

    // 'structors
    vector_table() {}
    template <typename InputIterator>
    vector_table(InputIterator begin, InputIterator end);
    ~vector_table() {}

    // iterators
    iterator begin()                          { return _vec.begin(); }
    const_iterator begin() const              { return _vec.begin(); }
    const_iterator cbegin() const             { return _vec.begin(); }
    iterator end()                            { return _vec.end(); }
    const_iterator end() const                { return _vec.end(); }
    const_iterator cend() const               { return _vec.end(); }
    reverse_iterator rbegin()                 { return _vec.rbegin(); }
    const_reverse_iterator rbegin() const     { return _vec.rbegin(); }
    const_reverse_iterator crbegin() const    { return _vec.rbegin(); }
    reverse_iterator rend()                   { return _vec.rend(); }
    const_reverse_iterator rend() const       { return _vec.rend(); }
    const_reverse_iterator crend() const      { return _vec.rend(); }

    // capacity
    size_type capacity() const                { return _vec.capacity(); }
    bool empty() const                        { return _vec.empty(); }
    size_type max_size() const                { return _vec.max_size(); }
    void reserve(size_type size)              { _vec.reserve(size); }
    size_type size() const                    { return _vec.size(); }

    // element access
    //const_reference at(size_type index) const         { return _vec.at(index); }
    const_reference back() const                      { return _vec.back(); }
    reference back()                                  { return _vec.back(); }
    const_reference front() const                     { return _vec.front(); }
    reference front() const                           { return _vec.front(); }
    //const_reference operator[](size_type index) const { return _vec[index]; }
    //reference operator[](K const& key);

    // modifiers
    void clear()                              { _vec.clear(); }
    std::pair<iterator,bool> insert(K const& key, V const& value);
    /*template <typename InputIterator>
    void insert(InputIterator begin, InputIterator end);*/
    void erase(const_iterator position);
    size_type erase(K const& key);
    void push_back(V const& x)                  { _vec.push_back(x); }
    void resize(size_type size)                 { _vec.resize(size); }
    void swap(vector_table& other)              { _vec.swap(other._vec); }
    vector_table& operator=(vector_table const& other);

    // lookup
    size_type count(K const& key) const;
    iterator find(K const& key);
    const_iterator find(K const& key) const;
  private:
    vector _vec;
    Equal _equal;
  };
}

#include "internal/vector_table.inl"
   
#endif /* SIB_VECTOR_TABLE_HPP */

