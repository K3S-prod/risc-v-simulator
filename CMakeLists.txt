
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(RVSIM_BINARY_DIR ${CMAKE_BINARY_DIR})

include_directories(${RVSIM_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})
# Header-only libs:
include_directories(${CMAKE_SOURCE_DIR}/third-party/ELFIO)

project(risc_v_simulator)

add_subdirectory(simulator)
add_subdirectory(tests)
