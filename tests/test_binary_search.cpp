#include "test/test.h"
#include "algorithm/binary_search.h"

using namespace shl::test;
using namespace shl::algorithm;

SHL_TEST_SUITE(BinarySearch)

SHL_TEST_CASE(InsertTest) {

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
SHL_TEST_SUITE_END
