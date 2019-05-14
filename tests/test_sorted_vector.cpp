#include "test/test.h"
#include "container/sorted_vector.h"

using namespace shl::test;
using namespace shl::algorithm;
using namespace shl::container;

struct Position {
  float x;
  float y;
};

SHL_TEST_SUITE(SortedVector)

SHL_TEST_CASE(Insert) {
  SortedVector<Position> vec;
  vec.insert(0, Position{0.0, 3.0});
  vec.insert(1, Position{1.0, 1.0});
  vec.insert(2, Position{2.0, 1.0});
  vec.insert(3, Position{3.0, 1.0});
  vec.insert(4, Position{4.0, 1.0});
  vec.insert(5, Position{5.0, 1.0});
  vec.insert(6, Position{6.0, 1.0});
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(8, Position{8.0, 1.0});

  SHL_TEST_EQUAL(vec.at(7).x, 7.0);
}

SHL_TEST_CASE(InsertReverse) {
  SortedVector<Position> vec;
  vec.insert(8, Position{8.0, 1.0});
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(6, Position{6.0, 1.0});
  vec.insert(5, Position{5.0, 1.0});
  vec.insert(4, Position{4.0, 1.0});
  vec.insert(3, Position{3.0, 1.0});
  vec.insert(2, Position{2.0, 1.0});
  vec.insert(1, Position{1.0, 1.0});
  vec.insert(0, Position{0.0, 3.0});

  SHL_TEST_EQUAL(vec.at(7).x, 7.0);
}

SHL_TEST_CASE(Get) {
  SortedVector<Position> vec;
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(8, Position{8.0, 1.0});

  SHL_TEST_EQUAL(vec.at(7).x, 7.0);
  SHL_TEST_EQUAL(vec.at(8).x, 8.0);
}

SHL_TEST_CASE(OutOfRange) {
  SortedVector<Position> vec;
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(8, Position{8.0, 1.0});

  SHL_TEST_RAISE(vec.at(9).x, std::out_of_range);
}

SHL_TEST_CASE(Delete) {
  SortedVector<Position> vec;
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(8, Position{8.0, 1.0});

  SHL_TEST_EQUAL(vec.size(), 2);
  vec.erase(7);
  SHL_TEST_EQUAL(vec.size(), 1);
}

SHL_TEST_CASE(Iterator) {
  SortedVector<Position> vec;
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(8, Position{8.0, 1.0});

  auto it = vec.begin();

  SHL_TEST_EQUAL((*it++).x, 7.0);
  SHL_TEST_EQUAL((*it++).x, 8.0);
  SHL_TEST_TRUE(it == vec.end());
}

SHL_TEST_CASE(IteratorLoop) {
  SortedVector<Position> vec;
  vec.insert(7, Position{7.0, 1.0});
  vec.insert(8, Position{8.0, 1.0});

  for (auto p : vec) {
    SHL_TEST_TRUE(p.x > 0);
  }
}

SHL_TEST_CASE(Benchmark) {

  SortedVector<Position> vec;

  size_t N = 1000000;
  for (size_t i = 0; i < N; i++) {
    vec.insert(i, Position{float(i), float(i)});
    SHL_TEST_EQUAL(vec.at(i).x, float(i));
  }

}

SHL_TEST_CASE(BenchmarkReserve) {
  size_t N = 1000000;
  SortedVector<Position> vec(N);

  for (size_t i = 0; i < N; i++) {
    vec.insert(i, Position{float(i), float(i)});
    SHL_TEST_EQUAL(vec.at(i).x, float(i));
  }

}

SHL_TEST_CASE(BenchmarkReverse) {
  size_t N = 5000;
  SortedVector<Position> vec;
  for (size_t i = N; i > 0; i--) {
    vec.insert(i, Position{float(i), float(i)});
    SHL_TEST_EQUAL(vec.at(i).x, float(i));
  }

}
SHL_TEST_SUITE_END
