#ifndef SPARSE_SET_H
#define SPARSE_SET_H
#pragma once

#include <algorithm>
#include <vector>

namespace shl { namespace container {

template<typename T, typename S = std::size_t>
class IndexedVector {
public:

  typedef T value_type;
  typedef S size_type;
  typedef std::vector<T> value_vector;
	typedef std::vector<S> index_vector;

  IndexedVector() noexcept = default;
  IndexedVector(IndexedVector &&) = default;
  IndexedVector& operator=(const IndexedVector &) = delete;
  IndexedVector& operator=(IndexedVector &&) = default;
  ~IndexedVector(){}


  void swap(IndexedVector &set) {
    set.data_.swap(data_);
    set.index_.swap(index_);
  }

  void insert(const size_type pos, const value_type& value)
  {
    if (pos >= index_.size())
      index_.resize(pos + 1);

    // replaces existing value
    if (index_[pos] & used) {
      const size_type data_pos = index_[pos] >> 1;
      data_[data_pos] = std::move(value);
      return;
    }

    const size_type data_pos = data_.size() << 1 | used;
    index_[pos] = data_pos;
    data_.emplace_back(std::move(value));
  };

  value_type& get(const size_type pos) {
    if (pos >= index_.size() || !(index_[pos] & used))
      insert(pos, value_type());

    const size_type data_pos = index_[pos] >> 1;
    return data_[data_pos];
  }

  // TODO
  void erase(size_type pos) {
    // Element is not present
    if (pos >= index_.size() || !(index_[pos] & used)) {
      return;
    }
    
    size_type data_end = data_.size() - 1;
    size_type data_pos = index_[pos] >> 1;
    size_type last_item = index_.size()-1;
    index_[last_item] = data_pos;
    std::swap(data_[data_pos], data_[last_item]);

    index_[pos] = 0;

    data_.pop_back();
  }

  void clear() {
    data_.clear();
    index_.clear();
  }
  
  size_type size() {
    return data_.size();
  };

  size_type expanse() {
    return index_.size();
  };

  class IndexIterator {
    IndexedVector &m_indexedVector;
    size_type m_pos;

    void scan() {
      while (m_pos < m_indexedVector.expanse() && !(m_indexedVector.index_[m_pos] & m_indexedVector.used)) {
        m_pos++;
      }
    }
  public:
    IndexIterator(IndexedVector &ivec, size_type pos)
      : m_indexedVector(ivec), m_pos(pos) { scan(); }

    value_type& operator*() {

      size_type data_pos = m_indexedVector.index_[m_pos] >> 1;
      return m_indexedVector.data_[data_pos];
    }

    IndexIterator& operator++() {
      m_pos++;
      scan();
      return *this;
    }

    IndexIterator operator++(int) {
      IndexIterator clone(*this);
      m_pos++;
      scan();
      return clone;
    }
    
    bool operator==(const IndexIterator &other) {
      return &m_indexedVector == &other.m_indexedVector 
             && m_pos == other.m_pos;
    }
    
    bool operator!=(const IndexIterator &other) {
      return !(*this == other);
    }
  };

  IndexIterator ordered_begin() { return IndexIterator(*this, 0); } 
  IndexIterator ordered_end() { return IndexIterator(*this, index_.size()); } 

  typename value_vector::iterator begin() { return data_.begin(); }
  typename value_vector::iterator end() { return data_.end(); }

private:
  value_vector data_;
	index_vector index_;
  static const size_type used = 0x1;
};
}}

#endif
