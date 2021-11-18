cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS *.cpp)

# tree time
add_executable(treeTime treeTime.cpp)
target_link_libraries(treeTime PRIVATE
  kdtree
  external)

# test parlay functions
add_executable(parlayTest parlayTest.cpp)
target_link_libraries(parlayTest PRIVATE
  kdtree
  external)
