cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(VHAT
  VERSION 0.0.1.0
)

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 14 CACHE STRING
  "Default value for CXX_STANDARD property of targets")

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
  "Default value for CXX_STANDARD_REQUIRED property of targets")

# BUILD_SHARED_LIBS=ON necessary for building shared gtest library
option(BUILD_SHARED_LIBS "Build shared libs by default" ON)

option(VHAT_WITH_TESTS "Build VHAT with tests" ON)

set(VHAT_COMMON_DATA_PATH "${CMAKE_BINARY_DIR}/ATE_common/data" CACHE PATH "Path to VHAT common data")
set(VHAT_SERVER_DATA_PATH "${CMAKE_BINARY_DIR}/ATE_server/data" CACHE PATH "Path to VHAT server data")

add_subdirectory(third_party)
add_subdirectory(ATE_common/src)
add_subdirectory(ATE_server/src)
add_subdirectory(ATE_client/src)

if(VHAT_WITH_TESTS)
  # Enable testing via CTest
  # https://cmake.org/cmake/help/latest/manual/ctest.1.html
  include(CTest)
  enable_testing()

  # ATE tests directories
  add_subdirectory(ATE_common/test)
  add_subdirectory(ATE_server/test)
endif(VHAT_WITH_TESTS)

# Copy 'data' directories contents into the target 'data' folder
# Trailing slash is required or 'data' folder itself will be copied inside target 'data'
file(COPY
  ATE_common/data/
  DESTINATION "${VHAT_COMMON_DATA_PATH}"
)

file(COPY
  ATE_server/data/
  DESTINATION "${VHAT_SERVER_DATA_PATH}"
)
