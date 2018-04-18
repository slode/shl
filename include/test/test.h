#ifndef SHL_TEST_TEST_H
#define SHL_TEST_TEST_H

#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "test_report.h"

namespace shl { namespace test {

class TestException {
  std::string msg_;
public:
  TestException(const std::string &msg): msg_(msg) {}
  const std::string& what() const { return msg_; }
};

class Test {
  std::string name_;
public:
  Test(const std::string &name): name_(name) {}
  const std::string& get_name() { return name_; }
  void run() { return do_run(); }
  virtual void do_run() = 0;
};

class TestSuite {
  typedef std::vector<Test*> TestVector;
  TestVector tests_;
  std::string name_;
  std::unique_ptr<TestReportIface> reporter;

  static TestSuite *instance_;
  TestSuite():reporter(new ConsoleReport()) {}
  TestSuite(const TestSuite&);
  TestSuite& operator=(const TestSuite&);
 
public:
  void set_name(const std::string& name) { name_ = name; }
  void register_test(Test *test) { tests_.push_back(test); }

  static TestSuite* get() {
    if (!instance_) {
      instance_ = new TestSuite();
    }
    return instance_;
  }

  int run_tests() {
    int errors = 0;
    reporter->test_suite_init(name_);
    for (TestVector::iterator it = tests_.begin();
        it != tests_.end();
        it++) {
      reporter->test_case_init((*it)->get_name());
      try {
        (*it)->run();
        reporter->success("");
      } catch(const TestException &e) {
        reporter->failure(e.what());
        errors++;
      } catch(...) {
        reporter->error("Unknown exception thrown.");
        errors++;
      }
      reporter->test_case_end();
    }
    reporter->test_suite_end();
    return errors > 0;
  }
};

template<typename T, typename S>
bool compare(T a, S b) {return a == b;}

template<typename T>
bool is_true(T a) {return a ? true : false;}

}} // ends namespace shl::test

#define SHL_TEST_SUITE(suite_name)               \
using namespace shl::test;                      \
TestSuite* TestSuite::instance_ = 0;      \
void test_suite_initialize() {                  \
  TestSuite::get()->set_name(#suite_name);   \
}                                               \
namespace { /* Isolates the test-case */        \


#define SHL_TEST_SUITE_END                       \
}                                               \
int main(int argc, char **argv) {               \
  (void)argc;                                   \
  (void)argv;                                   \
  test_suite_initialize();                      \
  return TestSuite::get()->run_tests();      \
}                                               \

#define SHL_TEST_CASE(test_name)                 \
class test_name : public Test {              \
public:                                         \
  test_name(): Test(#test_name)              \
  { TestSuite::get()->register_test(         \
      reinterpret_cast<Test*>(this)); }      \
                                                \
  virtual void do_run();                        \
} test_name ## instance;                        \
void test_name::do_run()                        \


#define SHL_TEST_EQUAL(a, b)                     \
do {                                            \
  auto da = (a);                                \
  auto db = (b);                                \
  if (!compare(da, db)) {                       \
      std::ostringstream oss;                   \
      oss << " [" << #a << " != " << #b << "] ";\
      oss << " [" << da << " != " << db << "] ";\
      oss << __FILE__ << ": " << __LINE__;      \
      throw TestException(oss.str());        \
  }                                             \
} while(0);                                     \

#define SHL_TEST_TRUE(a)                        \
do {                                            \
  auto da = (a);                                \
  if (!is_true(da)) {                           \
      std::ostringstream oss;                   \
      oss << " [" << #a << " != true] ";        \
      oss << " [" << da << " != true] ";        \
      oss << __FILE__ << ": " << __LINE__;      \
      throw TestException(oss.str());        \
  }                                             \
} while(0);                                     \

#endif // SHL_TEST_TEST_H
