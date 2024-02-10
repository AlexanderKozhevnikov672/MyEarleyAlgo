project(EarleyAlgo)

cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../bin)

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
  "${PROJECT_SOURCE_DIR}/include/*.h"
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Testing
  tests/EarleyTesting.cpp
  ${all_SRCS}
)

target_link_libraries(Testing ${GTEST_LIBRARIES} Threads::Threads)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Manual
  manual/Manual.cpp
  ${all_SRCS}
)
