#include "test/test.h"
#include "algorithm/binary_search.h"

using namespace shl::test;
using namespace shl::algorithm;

SHL_TEST_SUITE(BinarySearch)

SHL_TEST_CASE(Search) {

  std::vector<int> vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
    vec.push_back(i);
    vec.push_back(i);
  }
  SHL_TEST_EQUAL(binary_search(vec, -1), 0);
  SHL_TEST_EQUAL(binary_search(vec, 0), 0);
  SHL_TEST_EQUAL(binary_search(vec, 3), 9);
  SHL_TEST_EQUAL(binary_search(vec, 7), 21);
  SHL_TEST_EQUAL(binary_search(vec, 10), 30);
}

SHL_TEST_CASE(SearchNegative) {

  std::vector<int> vec;
  for (int i = -9; i < 0; i++) {
    vec.push_back(i);
  }
  SHL_TEST_EQUAL(binary_search(vec, -10), 0);
  SHL_TEST_EQUAL(binary_search(vec, -9), 0);
  SHL_TEST_EQUAL(binary_search(vec, -8), 1);
  SHL_TEST_EQUAL(binary_search(vec, 0), vec.size());
  SHL_TEST_EQUAL(binary_search(vec, 1), vec.size());
  SHL_TEST_EQUAL(binary_search(vec, 3), vec.size());
}

SHL_TEST_CASE(One) {

  std::vector<int> vec;
  vec.push_back(4);
  SHL_TEST_EQUAL(binary_search(vec, 4), 0);
  SHL_TEST_EQUAL(binary_search(vec, 5), 1);
  SHL_TEST_EQUAL(binary_search(vec, 3), 0);
}

SHL_TEST_CASE(None) {

  std::vector<int> vec;
  SHL_TEST_EQUAL(binary_search(vec, 4), 0);
  SHL_TEST_EQUAL(binary_search(vec, 5), 0);
  SHL_TEST_EQUAL(binary_search(vec, 3), 0);
}

SHL_TEST_CASE(TwoSimilar) {

  std::vector<int> vec;
  vec.push_back(4);
  vec.push_back(4);
  SHL_TEST_EQUAL(binary_search(vec, 4), 0);
  SHL_TEST_EQUAL(binary_search(vec, 5), 2);
  SHL_TEST_EQUAL(binary_search(vec, 3), 0);
}
SHL_TEST_SUITE_END
