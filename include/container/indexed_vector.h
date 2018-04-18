#ifndef SPARSE_SET_H
#define SPARSE_SET_H
#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>
#include <iostream>

namespace shl { namespace container {

template<typename T, typename S = std::size_t>
class IndexedVector {
public:

  typedef T value_type;
  typedef S size_type;

  IndexedVector() noexcept = default;
  IndexedVector(IndexedVector &&) = default;
  IndexedVector& operator=(const IndexedVector &) = delete;
  IndexedVector& operator=(IndexedVector &&) = default;
  ~IndexedVector(){}


  void swap(IndexedVector &set) {
    set.data_.swap(data_);
    set.index_.swap(index_);
  }

  void insert(const size_type pos, value_type value)
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
    if (pos > index_.size() || !(index_[pos] & used))
      insert(pos, value_type());

    const size_type data_pos = index_[pos] >> 1;
    return data_[data_pos];
  }

  // TODO
  void erase(size_type pos) {
    // Element is not present
    if (pos > index_.size() || !(index_[pos] & used)) {
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

  bool test(value_type val);


private:
  std::vector<value_type> data_;
	std::vector<size_type> index_;
  static const size_type used = 0x1;
};
}}

#endif
