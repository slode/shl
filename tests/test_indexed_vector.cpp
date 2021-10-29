#include "test/test.h"
#include "container/indexed_vector.h"

using namespace shl::test;
using namespace shl::container;

SHL_TEST_SUITE(IndexedVector)

SHL_TEST_CASE(InsertTest) {
  IndexedVector<int> vec;
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

SHL_TEST_CASE(GetTest) {
  IndexedVector<int> vec;
  vec.insert(3, 4);
  vec.insert(1, 9);
  vec.insert(0, 2);

  SHL_TEST_EQUAL(vec.get(3), 4);
  SHL_TEST_EQUAL(vec.get(0), 2);
  SHL_TEST_EQUAL(vec.get(1), 9);
  SHL_TEST_EQUAL(vec.get(3), 4);
}

SHL_TEST_CASE(EraseTest) {
  IndexedVector<int> vec;
  vec.insert(3, 4);
  vec.insert(1, 9);
  vec.insert(0, 2);

  SHL_TEST_EQUAL(vec.get(3), 4);
  SHL_TEST_EQUAL(vec.size(), 3);
  vec.erase(2);
//  SHL_TEST_EQUAL(vec.size(), 2);

}

SHL_TEST_CASE(StringTest) {
  IndexedVector<std::string> vec;
  vec.insert(4, "four");
  vec.insert(3, "three");
  vec.insert(2, "two");
  vec.insert(1, "one");

  SHL_TEST_EQUAL(vec.get(4), "four");
  SHL_TEST_EQUAL(vec.get(1), "one");
  SHL_TEST_EQUAL(vec.size(), 4);
  SHL_TEST_EQUAL(vec.get(1), "one");

  vec.insert(6, "six");
  SHL_TEST_EQUAL(vec.size(), 5);
  SHL_TEST_EQUAL(vec.expanse(), 7);

  auto it = vec.ordered_begin();
  SHL_TEST_EQUAL(*it, "one");
  SHL_TEST_EQUAL(*++it, "two");
  SHL_TEST_EQUAL(*++it, "three");
  SHL_TEST_EQUAL(*++it, "four");
  SHL_TEST_EQUAL(*++it, "six");
  SHL_TEST_TRUE(++it == vec.ordered_end());

  auto it2 = vec.ordered_begin();
  SHL_TEST_EQUAL(*it2++, "one");
  SHL_TEST_EQUAL(*it2++, "two");
  SHL_TEST_EQUAL(*it2++, "three");
  SHL_TEST_EQUAL(*it2++, "four");
  SHL_TEST_EQUAL(*it2++, "six");
  SHL_TEST_TRUE(it2++ == vec.ordered_end());
}

SHL_TEST_SUITE_END
