cmake_minimum_required(VERSION 3.0)

project("echo-numeric_array")

enable_testing()

include(Echo/Test)
include(Echo/Benchmark)

find_package(EchoExecutionContext REQUIRED)

include_directories(${TBB_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB headers
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/echo/*.h"
)

file(GLOB headers-numeric_array
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/echo/numeric_array/*.h"
)

file(GLOB unittests
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/unittest/*_test.cpp"
)

file(GLOB benchmarks
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_SOURCE_DIR}/benchmark/*_benchmark.cpp"
)

install(FILES ${headers} DESTINATION include/echo)
install(FILES ${headers-numeric_array} DESTINATION include/echo/numeric_array)

add_catch_test(numeric_array-test ${unittests})
add_benchmark(benchmark ${benchmarks})
target_link_libraries(benchmark ${TBB_LIBRARIES})

target_link_libraries(numeric_array-test tbb)

# add_subdirectory(unittest)
