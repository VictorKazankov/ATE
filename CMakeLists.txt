cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(VHAT
  VERSION 0.0.1.0
)

set(CMAKE_CXX_STANDARD 14 CACHE STRING
  "Default value for CXX_STANDARD property of targets")

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
  "Default value for CXX_STANDARD_REQUIRED property of targets")

# BUILD_SHARED_LIBS=ON necessary for building shared gtest library
option(BUILD_SHARED_LIBS "Build shared libs by default" ON)

option(VHAT_BUILD_CLIENT "Build VHAT client" ON)
option(VHAT_BUILD_SERVER "Build VHAT server" ON)
option(VHAT_WITH_TESTS "Build VHAT with tests" ON)

set(VHAT_COMMON_DATA_PATH "${CMAKE_BINARY_DIR}/ATE_common/data" CACHE PATH "Path to VHAT common data")
set(VHAT_SERVER_DATA_PATH "${CMAKE_BINARY_DIR}/ATE_server/data" CACHE PATH "Path to VHAT server data")
set(VHAT_CLIENT_DATA_PATH "${CMAKE_BINARY_DIR}/ATE_client/data" CACHE PATH "Path to VHAT client data")

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION})
message(STATUS "VHAT_BUILD_CLIENT: " ${VHAT_BUILD_CLIENT})
message(STATUS "VHAT_BUILD_SERVER: " ${VHAT_BUILD_SERVER})
message(STATUS "VHAT_WITH_TESTS: " ${VHAT_WITH_TESTS})
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})

add_subdirectory(third_party)
add_subdirectory(ATE_common/src)

if(VHAT_BUILD_SERVER)
  add_subdirectory(ATE_server/src)
endif(VHAT_BUILD_SERVER)

if(VHAT_BUILD_CLIENT)
  add_subdirectory(ATE_client/src)
endif(VHAT_BUILD_CLIENT)

if(VHAT_WITH_TESTS)
  # Enable testing via CTest
  # https://cmake.org/cmake/help/latest/manual/ctest.1.html
  include(CTest)
  enable_testing()

  # ATE tests directories
  add_subdirectory(ATE_common/test)

  if(VHAT_BUILD_SERVER)
    add_subdirectory(ATE_server/test)
  endif(VHAT_BUILD_SERVER)
endif(VHAT_WITH_TESTS)

# Copy 'data' directories contents into the target 'data' folder
# Trailing slash is required or 'data' folder itself will be copied inside target 'data'
file(COPY
  ATE_common/data/
  DESTINATION "${VHAT_COMMON_DATA_PATH}"
)

if(VHAT_BUILD_SERVER)
  file(COPY
    ATE_server/data/
    DESTINATION "${VHAT_SERVER_DATA_PATH}"
  )
endif(VHAT_BUILD_SERVER)

if(VHAT_BUILD_CLIENT)
  file(COPY
    ATE_client/data/
    DESTINATION "${VHAT_CLIENT_DATA_PATH}"
  )
endif(VHAT_BUILD_CLIENT)
