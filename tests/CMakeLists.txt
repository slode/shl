cmake_minimum_required(VERSION 3.5)

file(GLOB test_files "test_*.cpp")
foreach(file ${test_files})

  get_filename_component(TEST_TARGET_NAME ${file} NAME_WE)

  add_executable(${TEST_TARGET_NAME} ${file})

  target_link_libraries(${TEST_TARGET_NAME} shl)

  # Add a test to the project to be run by ctest.
  # See https://cmake.org/cmake/help/latest/command/add_test.html
  # See https://cmake.org/cmake/help/latest/manual/ctest.1.html
  # COMMAND tag specifies the test command-line. If it is an executable target
  # created by add_executable(), it will automatically be replaced by the location
  # of the executable created at build time.
  add_test(NAME ${TEST_TARGET_NAME}
           COMMAND ${TEST_TARGET_NAME}
           WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)

endforeach()
