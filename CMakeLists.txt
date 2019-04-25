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
set(VHAT_DATA_PATH "${CMAKE_BINARY_DIR}/ATE_server/data" CACHE PATH "Path to VHAT data")

add_subdirectory(third_party)
add_subdirectory(ATE_server/src)

if(VHAT_WITH_TESTS)
  # ATE-server tests directory
  add_subdirectory(ATE_server/test)
endif(VHAT_WITH_TESTS)

# Copy 'data' directory contents into the target 'data' folder
# Trailing slash is required or 'data' folder itself will be copied inside target 'data'
file(COPY
  ATE_server/data/
  DESTINATION ${VHAT_DATA_PATH}
)
