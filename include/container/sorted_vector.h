#ifndef SORTED_VECTOR_H
#define SORTED_VECTOR_H

#include <vector>
#include "algorithm/binary_search.h"

namespace shl { namespace container {

using namespace shl::algorithm;

template <typename T>
class SortedVector
{
public:

  class TypeWrapper;
  class SortedVectorIterator;
  class CSortedVectorIterator;

  typedef T value_type;
  typedef T* pointer;
  typedef T& reference;
  typedef size_t size_type;
  typedef std::vector<TypeWrapper> ValueVector;
  typedef SortedVectorIterator iterator;
  typedef CSortedVectorIterator const_iterator;

  struct TypeWrapper {
    size_type id;
    value_type value;
    bool operator<(const size_type &other) const
    {
      return id < other;
    }
  };

  class SortedVectorIterator {
    SortedVector &m_sortedVector;
    size_type m_index;

  public:
    SortedVectorIterator(SortedVector &svec, size_type index)
      : m_sortedVector(svec), m_index(index) {}

    reference operator*() {
      return m_sortedVector.m_buffer[m_index].value;
    }

    iterator& operator++() {
      ++m_index;
      return *this;
    }

    iterator operator++(int) {
      iterator clone(*this);
      ++m_index;
      return clone;
    }
    
    bool operator==(const iterator &other) {
      return &m_sortedVector == &other.m_sortedVector 
             && m_index == other.m_index;
    }
    
    bool operator!=(const iterator &other) {
      return !(*this == other);
    }
  };

  class CSortedVectorIterator {
    SortedVector &m_sortedVector;
    size_type m_index;

  public:
    CSortedVectorIterator(SortedVector &svec, size_type index)
      : m_sortedVector(svec), m_index(index) {}

    const reference operator*() const {
      return m_sortedVector.m_buffer[m_index].value;
    }

    const_iterator& operator++() {
      ++m_index;
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator clone(*this);
      ++m_index;
      return clone;
    }
    
    bool operator==(const const_iterator &other) const {
      return m_sortedVector == other.m_sortedVector 
             && m_index == other.m_index;
    }
    
    bool operator!=(const const_iterator &other) const {
      return !(*this == other);
    }
  };

  SortedVector() : m_buffer() {};
  SortedVector(size_type size) : m_buffer() {
      m_buffer.reserve(size);
  };
  ~SortedVector() {};

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, m_buffer.size()); }

  const_iterator begin() const { return const_iterator(*this, 0); }
  const_iterator end() const { return const_iterator(*this, m_buffer.size()); }

  void insert(size_t id, const T &value) {
    size_t found = binary_search(m_buffer, id);
    TypeWrapper wrap{id, value};
    m_buffer.insert(m_buffer.begin() + found, wrap);
  }
  
  T& at(size_t id) {
    size_t found = binary_search(m_buffer, id);
    TypeWrapper& result = m_buffer.at(found);
    if (result.id != id) {
      throw std::out_of_range("");
    }
    return result.value;
  }

  void erase(size_t id) {
    size_t found = binary_search(m_buffer, id);
    TypeWrapper& result = m_buffer.at(found);
    if (result.id == id) {
      m_buffer.erase(m_buffer.begin() + found);
    }
  }

  size_t size() {
    return m_buffer.size();
  }

  reference front() {
    return m_buffer.front();
  }

  const reference front() const {
    return m_buffer.front();
  }

  reference back() {
    return m_buffer.back();
  }

  const reference back() const {
    return m_buffer.back();
  }

private:
  ValueVector m_buffer;
};

}}
#endif /* SORTED_VECTOR_H */
