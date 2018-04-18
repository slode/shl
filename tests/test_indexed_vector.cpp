#include "test/test.h"
#include "container/indexed_vector.h"

using namespace shl::test;
using namespace shl::container;

SHL_TEST_SUITE(IndexedVector)

SHL_TEST_CASE(InsertTest) {
  IndexedVector<int, int> vec;
  vec.insert(3, 4);
  vec.insert(1, 9);
  vec.insert(0, 2);

  SHL_TEST_EQUAL(vec.get(3), 4);
  vec.insert(3, 2);
  SHL_TEST_EQUAL(vec.get(3), 2);
  SHL_TEST_EQUAL(vec.get(1), 9);
  SHL_TEST_EQUAL(vec.get(0), 2);

  vec.insert(9, 0);
  SHL_TEST_EQUAL(vec.size(), 4);
  SHL_TEST_EQUAL(vec.expanse(), 10);
  SHL_TEST_EQUAL(vec.get(100), 0);
  SHL_TEST_EQUAL(vec.size(), 5);
  SHL_TEST_EQUAL(vec.expanse(), 101);

  SHL_TEST_EQUAL(vec.get(2), 0);
}
SHL_TEST_SUITE_END
