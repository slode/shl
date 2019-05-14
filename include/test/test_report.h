#ifndef SHL_TEST_TEST_REPORT_H
#define SHL_TEST_TEST_REPORT_H

#include <chrono>
#include <iostream>
#include <string>

namespace shl { namespace test {
class TestReportIface {
public:
  virtual void test_suite_init(const std::string &suite_name) = 0;
  virtual void test_case_init(const std::string &test_name) = 0;
  virtual void success(const std::string &status) = 0;
  virtual void failure(const std::string &status) = 0;
  virtual void error(const std::string &status) = 0;
  virtual void test_case_end() = 0;
  virtual void test_suite_end() = 0;
};

class ConsoleReport: public TestReportIface {
  using clock = std::chrono::high_resolution_clock;

  typedef clock::time_point clock_type;
  typedef std::chrono::duration<double, std::milli> dur;
  
  clock_type test_suite_time;
  clock_type test_case_time;
public:
  virtual void test_suite_init(const std::string &suite_name) {
    test_suite_time = clock::now();
    std::cout << "Test suite [" << suite_name << "]" << std::endl;
  };
  virtual void test_case_init(const std::string &test_name) {
    test_case_time = clock::now();
    std::cout << " * Test case [" << test_name << "] ";
  };
  virtual void success(const std::string &status) {
    std::cout << "OK! " << status;
  };
  virtual void failure(const std::string &status) {
    std::cout << "Failed! " << status;
  };
  virtual void error(const std::string &status) {
    std::cout << "Error! " << status;
  };
  virtual void test_case_end() {
    std::cout << "in " << dur(clock::now() - test_case_time).count() << " ms" << std::endl;
  };
  virtual void test_suite_end() {
    std::cout << "in " << dur(clock::now() - test_suite_time).count() << " ms" << std::endl;
  };
};

}}

#endif // SHL_TEST_TEST_REPORT_H
