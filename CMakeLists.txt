cmake_minimum_required(VERSION 3.12)
project(kdtree)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_ENABLE_EXPORTS true)

# Issue with clang-tidy - it picks up errors from googletest
#set(CMAKE_CXX_CLANG_TIDY
  #clang-tidy-9;
  #-header-filter=include/kdtree/.;
  #-checks=*;
  #-warnings-as-errors=*;)

# Build Types
set(available_build_types Debug Release RelWithDebInfo)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type ${available_build_types}" FORCE)
  message(STATUS "CMAKE_BUILD_TYPE is not set. Using default")
  message(STATUS "Available build types are: ${available_build_types}")
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -mcx16 -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")

# ------ Build Options -----
set(OMPFLAGS "-DPARLAY_OPENMP -fopenmp")
set(CILKFLAGS "-DPARLAY_CILK -fcilkplus")
set(PBBFLAGS "-DHOMEGROWN -pthread")
if(PARALLEL_BUILD_TYPE EQUAL "OPENMP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OMPFLAGS}")
elseif(PARALLEL_BUILD_TYPE EQUAL "CILK")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CILKFLAGS}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PBBFLAGS}")
endif()

# ------ kdtree options -----
if(DEFINED DUAL_KNN_MODE)
  if(DUAL_KNN_MODE STREQUAL "ATOMIC_LEAF")
    set(DUAL_KNN_MODE 0)
  elseif(DUAL_KNN_MODE STREQUAL "NONATOMIC_LEAF")
    set(DUAL_KNN_MODE 1)
  elseif(DUAL_KNN_MODE STREQUAL "ARRAY")
    set(DUAL_KNN_MODE 2)
  else()
    message(FATAL_ERROR "Invalid DUAL_KNN_MODE=${DUAL_KNN_MODE}")
  endif()
  add_compile_definitions(DUAL_KNN_MODE=${DUAL_KNN_MODE})
endif()

if(DEFINED PARTITION_TYPE)
  if(PARTITION_TYPE STREQUAL "PARTITION_OBJECT_MEDIAN")
    set(PARTITION_TYPE 0)
  elseif(PARTITION_TYPE STREQUAL "PARTITION_SPATIAL_MEDIAN")
    set(PARTITION_TYPE 1)
  else()
    message(FATAL_ERROR "Invalid PARTITION_TYPE=${PARTITION_TYPE}")
  endif()
  add_compile_definitions(PARTITION_TYPE=${PARTITION_TYPE})
endif()

if(DEFINED CLUSTER_SIZE)
  add_compile_definitions(CLUSTER_SIZE=${CLUSTER_SIZE})
endif()
if(DEFINED ERASE_BASE_CASE)
  add_compile_definitions(ERASE_BASE_CASE=${ERASE_BASE_CASE})
endif()
if(DEFINED RANGEQUERY_BASE_CASE)
  add_compile_definitions(RANGEQUERY_BASE_CASE=${RANGEQUERY_BASE_CASE})
endif()
if(DEFINED BOUNDINGBOX_BASE_CASE)
  add_compile_definitions(BOUNDINGBOX_BASE_CASE=${BOUNDINGBOX_BASE_CASE})
endif()
if(DEFINED DUALKNN_BASE_CASE)
  add_compile_definitions(DUALKNN_BASE_CASE=${DUALKNN_BASE_CASE})
endif()
if(DEFINED CO_TOP_BUILD_BASE_CASE)
  add_compile_definitions(CO_TOP_BUILD_BASE_CASE=${CO_TOP_BUILD_BASE_CASE})
endif()
if(DEFINED CO_BOTTOM_BUILD_BASE_CASE)
  add_compile_definitions(CO_BOTTOM_BUILD_BASE_CASE=${CO_BOTTOM_BUILD_BASE_CASE})
endif()
if(DEFINED BHL_BUILD_BASE_CASE)
  add_compile_definitions(BHL_BUILD_BASE_CASE=${BHL_BUILD_BASE_CASE})
endif()

OPTION(ALL_USE_BLOOM "all use bloom" OFF)
if(ALL_USE_BLOOM)
  add_compile_definitions(ALL_USE_BLOOM)
endif()

OPTION(LOGTREE_USE_BLOOM "logtree use bloom" OFF)
if(LOGTREE_USE_BLOOM)
  add_compile_definitions(LOGTREE_USE_BLOOM)
endif()

OPTION(BLOOM_FILTER_BUILD_COPY "logtree filter copy" OFF)
if(BLOOM_FILTER_BUILD_COPY)
  add_compile_definitions(BLOOM_FILTER_BUILD_COPY)
endif()

message(STATUS "--------------- General configuration -------------")
message(STATUS "CMake Generator:                ${CMAKE_GENERATOR}")
message(STATUS "Compiler:                       ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build type:                     ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_FLAGS:                ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG:          ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE:        ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS          ${CMAKE_CXX_LINKER_FLAGS}")
message(STATUS "CMAKE_INSTALL_PREFIX:           ${CMAKE_INSTALL_PREFIX}" )

# ------ Library ------
add_library(kdtree INTERFACE)
set(KDTREE_INCLUDE_DIR "include/")
target_include_directories(kdtree INTERFACE ${KDTREE_INCLUDE_DIR})
target_link_libraries(kdtree INTERFACE external)
target_compile_features(kdtree INTERFACE cxx_std_17)

# ------ External ------
add_subdirectory(utils)

# ------ Targets ------
enable_testing()
add_subdirectory(test)
add_subdirectory(executable)
add_subdirectory(benchmark)
