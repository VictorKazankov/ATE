cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_library(vhat_common
  SHARED
)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

target_include_directories(vhat_common
  PUBLIC
    ../include
)

target_link_libraries(vhat_common 
  inihcpp
  spdlog
  jsoncpp_lib
  Boost::asio
)

if(UNIX)
  target_compile_options(vhat_common
    PUBLIC
      -pedantic
      -Wall
      -Werror
      -Wextra
  )
endif(UNIX)
