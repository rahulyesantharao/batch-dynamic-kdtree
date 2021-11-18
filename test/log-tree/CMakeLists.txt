cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS *.cpp)
include(GoogleTest)
add_executable(test_log_tree ${SRCS})
target_link_libraries(test_log_tree PRIVATE
  kdtree
  ${GTest_LIBRARIES})

gtest_discover_tests(test_log_tree)
#add_test(NAME test_log_tree)
