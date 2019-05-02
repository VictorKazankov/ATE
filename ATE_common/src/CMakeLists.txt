cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# TODO: replace INTERFACE with SHARED after add source file
add_library(vhat_common
  SHARED
)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

# TODO: replace INTERFACE with PUBLIC after add source file
target_include_directories(vhat_common
  PUBLIC
    ../include
)

# TODO: remove INTERFACE after add source file
target_link_libraries(vhat_common 
  inihcpp
  spdlog
  jsoncpp_lib
  Boost::asio
)

if(UNIX)
  # TODO: replace INTERFACE with PUBLIC after add source file
  target_compile_options(vhat_common
    PUBLIC
      -pedantic
      -Wall
      -Werror
      -Wextra
  )
endif(UNIX)
