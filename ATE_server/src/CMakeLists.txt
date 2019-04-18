cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# A separate static library is necessary to support unit tests
add_library(vhat_server_lib
  STATIC
    ate.cpp
)

target_include_directories(vhat_server_lib
  PUBLIC
    ../include
)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

target_link_libraries(vhat_server_lib
  inihcpp
  jsoncpp_lib
  Boost::asio
)

add_executable(vhat_server
  ate_server_main.cpp
)

target_compile_options(vhat_server_lib
  PUBLIC -Wall -Wextra -pedantic -Werror)

target_link_libraries(vhat_server
  PRIVATE
    vhat_server_lib
)
