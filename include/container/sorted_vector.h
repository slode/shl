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
  typedef T value_type;

  struct TypeWrapper {
    size_t index;
    T value;
    bool operator<(const size_t &other) const
    {
      return index < other;
    }
  };

  SortedVector()
    :buffer(){};
  ~SortedVector() {};

  void insert(size_t index, const T &value) {
    size_t found = binary_search(buffer, index);
    TypeWrapper wrap{index, value};
    buffer.insert(buffer.begin() + found, wrap);
  }
  
  T& at(size_t index) {
    size_t found = binary_search(buffer, index);
    TypeWrapper& result = buffer.at(found);
    if (result.index != index) {
      throw std::out_of_range("index not found.");
    }
    return result.value;
  }

  void remove(size_t index) {
    size_t found = binary_search(buffer, index);
    TypeWrapper& result = buffer.at(found);
    if (result.index == index) {
      buffer.erase(buffer.begin() + found)
    }
  }

  size_t size() {
    return buffer.size();
  }

private:
  std::vector<TypeWrapper> buffer;
};

}}
#endif /* SORTED_VECTOR_H */
