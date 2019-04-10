cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(VHAT
  VERSION 0.0.1.0
)

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(third_party)
add_subdirectory(ATE_server/src)
