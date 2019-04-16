cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(VHAT
  VERSION 0.0.1.0
)

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17 CACHE STRING
  "Default value for CXX_STANDARD property of targets")

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
  "Default value for CXX_STANDARD_REQUIRED property of targets")

option(VHAT_WITH_TESTS "Build VHAT with tests" ON)

add_subdirectory(third_party)
add_subdirectory(ATE_server/src)

if(VHAT_WITH_TESTS)
  # ATE-server tests directory
  add_subdirectory(ATE_server/test)
endif(VHAT_WITH_TESTS)
