#ifndef SHL_TEST_TEST_REPORT_H
#define SHL_TEST_TEST_REPORT_H

#include <string>
#include <iostream>

namespace shl { namespace test {
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
public:
  virtual void test_suite_init(const std::string &suite_name) {
    std::cout << "Test suite [" << suite_name << "]" << std::endl;
  };
  virtual void test_case_init(const std::string &test_name) {
    std::cout << "Testing " << test_name << " ";
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
    std::cout << std::endl;
  };
  virtual void test_suite_end() {
    std::cout << std::endl;
  };
};

}}

#endif // SHL_TEST_TEST_REPORT_H
