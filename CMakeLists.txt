cmake_minimum_required(VERSION 3.5)
project(libshl VERSION 0.0.1 LANGUAGES CXX)

add_library(shl INTERFACE)

target_include_directories(shl INTERFACE include/)
target_compile_features(shl INTERFACE cxx_std_11)

enable_testing()
add_subdirectory(tests)
