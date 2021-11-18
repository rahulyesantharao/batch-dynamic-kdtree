cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

# ------ Library ------
add_library(external INTERFACE)
set(XXHASH_INCLUDE_DIR "external/")
#set(PARLAY_INCLUDE_DIR "external/parlay/include/")
#set(PARGEO_INCLUDE_DIR "external/pargeo/include/")
set(PARGEO_INCLUDE_DIR "external/old_pargeo/include/")
target_include_directories(external INTERFACE ${PARLAY_INCLUDE_DIR} ${XXHASH_INCLUDE_DIR} ${PARGEO_INCLUDE_DIR})
target_compile_features(external INTERFACE cxx_std_17)
